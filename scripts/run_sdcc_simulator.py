"""Execute the linked MCS-51 image in SDCC uCsim with bounded instructions.

This checks software diagnostic transitions in a generic C52 simulator. It does
not model STC89C52RC package wiring, on-chip AUX-RAM selection or peripherals.
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build", required=True, type=Path)
    parser.add_argument("--simulator", default="s51")
    args = parser.parse_args()
    simulator = shutil.which(args.simulator)
    if not simulator:
        parser.error("s51 simulator not found")
    build = args.build.resolve()
    report = json.loads((build / "build-report.json").read_text(encoding="utf-8"))
    image = build / "stc89_terminal_foundation.ihx"
    if not image.is_file():
        parser.error("linked IHX not found")
    address = report.get("diagnostics_address")
    if address is None:
        parser.error("diagnostic symbol absent from build report")
    commands = (f'file "{image.name}"\nstep 500000\nstate\n'
                f'dx 0x{address:04x} 0x{address + 9:04x}\nquit\n')
    result = subprocess.run([simulator, "-q", "-c", "-", "-t", "C52"],
                            cwd=build, input=commands, text=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                            timeout=30, check=False)
    (build / "simulator.log").write_text(result.stdout, encoding="utf-8")
    if result.returncode or "no loadable file found" in result.stdout:
        raise RuntimeError("uCsim could not load/run linked image")
    instruction_match = re.search(r"\bInst=\s*(\d+)", result.stdout)
    stack_match = re.search(r"(?:Most|Max) value of stack pointer=\s*0x([0-9a-f]+)",
                            result.stdout, re.I)
    xram: dict[int, int] = {}
    for line in result.stdout.splitlines():
        match = re.match(r"^0x([0-9a-f]{4})((?:\s+[0-9a-f]{2})+)(?:\s|$)", line, re.I)
        if match:
            base = int(match[1], 16)
            for offset, value in enumerate(match[2].split()):
                xram[base + offset] = int(value, 16)
    if not instruction_match or not stack_match or any(address + i not in xram for i in range(10)):
        raise ValueError("uCsim state/XRAM evidence incomplete:\n" + result.stdout[-6000:])
    instructions = int(instruction_match[1])
    stack_high = int(stack_match[1], 16)
    values = bytes(xram[address + i] for i in range(10))
    loop_ticks = int.from_bytes(values[:4], "little")
    events, displays, page, rtc_unavailable, temperature_unavailable, save_seen = values[4:]
    if (instructions != 500000 or loop_ticks == 0 or events < 4 or displays < 4 or
            page > 2 or rtc_unavailable != 1 or temperature_unavailable != 1 or
            save_seen != 1 or stack_high >= 256):
        raise ValueError("uCsim did not reach expected software state")
    summary = {
        "simulator": Path(simulator).name,
        "cpu_model": "generic C52 (not STC89C52RC peripheral model)",
        "instructions": instructions,
        "loop_ticks": loop_ticks,
        "synthetic_events": events,
        "diagnostic_display_calls": displays,
        "last_page": page,
        "rtc_unavailable": rtc_unavailable,
        "temperature_unavailable": temperature_unavailable,
        "save_unavailable_seen": save_seen,
        "stack_pointer_high_water": stack_high,
    }
    (build / "simulator-report.json").write_text(json.dumps(summary, indent=2) + "\n",
                                                   encoding="utf-8")
    print(json.dumps(summary, indent=2))


if __name__ == "__main__":
    main()
