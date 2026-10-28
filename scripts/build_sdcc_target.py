"""Build the STC89C52RC software-only Terminal Core foundation with SDCC.

All artifacts go to a fresh directory outside this repository. A successful
build proves MCS-51 code generation, linkage and HEX integrity, not execution.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PRACTICE = Path("projects/13_环境与时钟信息终端/practice")
SOURCES = (
    PRACTICE / "core/terminal_app.c",
    Path("target/sdcc/stc89_platform.c"),
    Path("target/sdcc/main.c"),
)
CODE_LIMIT = 8192
IRAM_LIMIT = 256
ON_CHIP_MOVX_LIMIT = 256
MIN_STACK = 64


def run(command: list[str], log: list[str]) -> str:
    result = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    log.extend(("COMMAND: " + " ".join(command), result.stdout,
                f"EXIT: {result.returncode}"))
    if result.returncode:
        raise RuntimeError(f"Command failed ({result.returncode}): {command[0]}")
    return result.stdout


def read_hex(path: Path) -> dict[int, int]:
    memory: dict[int, int] = {}
    eof = False
    for number, line in enumerate(path.read_text(encoding="ascii").splitlines(), 1):
        if eof or not line.startswith(":"):
            raise ValueError(f"Invalid Intel HEX record at line {number}")
        try:
            record = bytes.fromhex(line[1:])
        except ValueError as exc:
            raise ValueError(f"Non-hex Intel HEX record at line {number}") from exc
        if len(record) < 5 or len(record) != record[0] + 5 or sum(record) % 256:
            raise ValueError(f"Invalid Intel HEX length/checksum at line {number}")
        address = (record[1] << 8) | record[2]
        kind = record[3]
        if kind == 0:
            for offset, value in enumerate(record[4:-1]):
                position = address + offset
                if position >= CODE_LIMIT or position in memory:
                    raise ValueError(f"Overlapping/out-of-range code at {position:#x}")
                memory[position] = value
        elif kind == 1 and record[0] == 0 and address == 0:
            eof = True
        else:
            raise ValueError(f"Unexpected Intel HEX record type {kind} at line {number}")
    if not eof or not memory or 0 not in memory:
        raise ValueError("Intel HEX lacks data, reset vector, or EOF")
    return memory


def linked_symbol(map_text: str, symbol: str, space: str) -> int:
    match = re.search(rf"(?m)^{space}:\s+([0-9A-Fa-f]{{8}})\s+_{re.escape(symbol)}\b",
                      map_text)
    if not match:
        raise ValueError(f"Linked {space} symbol missing: {symbol}")
    return int(match[1], 16)


def memory_report(path: Path) -> dict[str, int]:
    content = path.read_text(encoding="ascii")
    stack = re.search(r"Stack starts at: 0x([0-9a-f]+).*?with (\d+) bytes available", content)
    xdata = re.search(r"^\s*EXTERNAL RAM\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+(\d+)\s+(\d+)", content, re.M)
    code = re.search(r"^\s*ROM/EPROM/FLASH\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+(\d+)\s+(\d+)", content, re.M)
    if not stack or not xdata or not code:
        raise ValueError("Could not parse SDCC memory report")
    layout: dict[int, str] = {}
    for line in content.splitlines():
        match = re.match(r"^0x([0-9a-f]{2}):\|([^|](?:\|[^|]){15})\|$", line)
        if match:
            start = int(match[1], 16)
            for offset, cell in enumerate(match[2].split("|")):
                layout[start + offset] = cell
    if len(layout) != IRAM_LIMIT:
        raise ValueError("Incomplete internal RAM layout")
    data_bytes = sum(1 for addr, cell in layout.items()
                     if addr < 128 and cell.islower())
    idata_bytes = sum(1 for cell in layout.values() if cell == "I")
    stack_bytes = sum(1 for cell in layout.values() if cell == "S")
    values = {
        "data_bytes": data_bytes,
        "idata_allocated_bytes": idata_bytes,
        "stack_reserved_bytes": stack_bytes,
        "stack_start": int(stack[1], 16),
        "stack_reported_available_bytes": int(stack[2]),
        "on_chip_movx_bytes": int(xdata[1]),
        "on_chip_movx_limit_bytes": int(xdata[2]),
        "code_bytes": int(code[1]),
        "code_limit_bytes": int(code[2]),
    }
    if (values["on_chip_movx_limit_bytes"] != ON_CHIP_MOVX_LIMIT or
            values["code_limit_bytes"] != CODE_LIMIT or
            values["on_chip_movx_bytes"] > ON_CHIP_MOVX_LIMIT or
            values["code_bytes"] > CODE_LIMIT or
            values["stack_reported_available_bytes"] < MIN_STACK or
            values["stack_start"] + MIN_STACK > IRAM_LIMIT):
        raise ValueError("Memory report violates STC89C52RC limits")
    return values


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sdcc", default="sdcc", help="SDCC executable path")
    parser.add_argument("--output", required=True, type=Path,
                        help="new output directory outside this repository")
    args = parser.parse_args()
    executable = shutil.which(args.sdcc)
    if not executable:
        parser.error("SDCC executable not found")
    sdcc = Path(executable).resolve()
    packihx = sdcc.with_name("packihx.exe" if sdcc.suffix.lower() == ".exe" else "packihx")
    if not packihx.is_file():
        found = shutil.which("packihx")
        if not found:
            parser.error("packihx executable not found")
        packihx = Path(found).resolve()
    output = args.output.resolve()
    if output == ROOT or ROOT in output.parents:
        parser.error("Output must be outside the repository")
    if output.exists():
        parser.error("Output directory already exists; use a fresh directory")
    output.mkdir(parents=True)
    log: list[str] = []
    try:
        version = run([str(sdcc), "-v"], log).strip()
        if "mcs51" not in version.lower():
            raise RuntimeError("SDCC lacks the MCS-51 backend")
        objects: list[str] = []
        for source in SOURCES:
            obj = output / (source.stem + ".rel")
            run([str(sdcc), "-mmcs51", "--std-c11", "--model-small", "--stack-auto",
                 "--Werror", "-I" + str(PRACTICE / "include"), "-Itarget/sdcc",
                 "-c", str(source), "-o", str(obj)], log)
            objects.append(str(obj))
        ihx = output / "stc89_terminal_foundation.ihx"
        run([str(sdcc), "-mmcs51", "--model-small", "--stack-auto",
             "--iram-size", str(IRAM_LIMIT), "--xram-size", str(ON_CHIP_MOVX_LIMIT),
             "--code-size", str(CODE_LIMIT), "--stack-size", str(MIN_STACK),
             "--out-fmt-ihx", "-o", str(ihx), *objects], log)
        packed = subprocess.run([str(packihx), str(ihx)], cwd=ROOT,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                check=False)
        log.extend(("COMMAND: " + str(packihx) + " " + str(ihx),
                    packed.stderr.decode("utf-8", errors="replace"),
                    f"EXIT: {packed.returncode}"))
        if packed.returncode:
            raise RuntimeError("packihx failed")
        hex_path = output / "stc89_terminal_foundation.hex"
        hex_path.write_bytes(packed.stdout)
        ihx_memory = read_hex(ihx)
        hex_memory = read_hex(hex_path)
        if ihx_memory != hex_memory:
            raise ValueError("Packed HEX differs from linked IHX")
        map_text = (output / "stc89_terminal_foundation.map").read_text(encoding="ascii")
        symbols = {name: linked_symbol(map_text, name, "C") for name in
                   ("main", "terminal_app_init", "terminal_app_tick",
                    "terminal_app_handle_key", "stc89_platform_init",
                    "__sdcc_external_startup", "stc89_abi_sizes")}
        main_asm = (output / "main.asm").read_text(encoding="ascii")
        if "_target_app:" not in main_asm or "_target_platform:" not in main_asm:
            raise ValueError("Static target application/platform objects missing")
        diagnostics_address = linked_symbol(map_text, "stc89_diagnostics", "D")
        abi_address = symbols["stc89_abi_sizes"]
        abi_values = [hex_memory[abi_address + offset] for offset in range(5)]
        if abi_values[0] + abi_values[1] + abi_values[4] > ON_CHIP_MOVX_LIMIT:
            raise ValueError("Static target objects exceed on-chip auxiliary RAM")
        if abi_values[2] == 0 or abi_values[3] == 0:
            raise ValueError("Invalid SDCC pointer ABI sizes")
        diagnostics = "\n".join(log)
        if re.search(r"(?im)^.*\b(?:warning|error)\s*[:0-9]", diagnostics):
            raise ValueError("Compiler/linker diagnostic found; inspect build.log")
        report = {
            "target": "STC89C52RC SDCC MCS-51 Terminal Core foundation",
            "compiler": version,
            "sources": [str(source) for source in SOURCES],
            "startup_runtime": "SDCC-provided; original external startup clears AUXR.EXTRAM before data init",
            "memory_model": "small + stack-auto; 256 IRAM, 256 on-chip MOVX, 8192 CODE",
            "memory": memory_report(output / "stc89_terminal_foundation.mem"),
            "abi_sizes": dict(zip(("TerminalApp", "TerminalPlatform", "function_pointer",
                                    "generic_void_pointer", "TargetDiagnostics"), abi_values)),
            "linked_symbols": symbols,
            "diagnostics_address": diagnostics_address,
            "hex_size_bytes": hex_path.stat().st_size,
            "hex_sha256": hashlib.sha256(hex_path.read_bytes()).hexdigest(),
            "compile_link_warnings": 0,
            "compile_link_errors": 0,
            "board_configuration_verified": False,
            "instruction_runtime_verified": False,
        }
        (output / "build-report.json").write_text(json.dumps(report, indent=2) + "\n",
                                                  encoding="utf-8")
        print(json.dumps(report, indent=2))
    finally:
        (output / "build.log").write_text("\n".join(log) + "\n", encoding="utf-8")


if __name__ == "__main__":
    main()
