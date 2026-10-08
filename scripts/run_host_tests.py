"""Compile, link, and run the portable terminal core tests."""

import argparse
from pathlib import Path
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[1]
PRACTICE = ROOT / "projects" / "13_环境与时钟信息终端" / "practice"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", choices=("gcc", "clang"), default="gcc")
    parser.add_argument("--sanitize", action="store_true")
    args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="terminal-host-test-") as output_dir:
        executable = Path(output_dir) / ("terminal-tests.exe" if sys.platform == "win32" else "terminal-tests")
        command = [
            args.compiler, "-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic",
            str(PRACTICE / "core" / "terminal_app.c"),
            str(PRACTICE / "tests" / "test_terminal_app.c"),
            "-I", str(PRACTICE / "include"), "-o", str(executable),
        ]
        if args.sanitize:
            command.extend(("-fsanitize=address,undefined", "-fno-omit-frame-pointer"))
        subprocess.run(command, cwd=ROOT, check=True)
        subprocess.run([str(executable)], cwd=ROOT, check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
