#!/usr/bin/env python

from pathlib import Path
from subprocess import run
from shutil import rmtree

for f in Path('.').glob("**/Makefile"):
    run(["make", "-C", f.parent, "clean"])

try:
    rmtree(Path(".") / "exercises" / "7_gpu_compressors" / "build", ignore_errors=True)
except FileNotFoundError:
    pass

run(["docker", "build", ".", "-t", "ghcr.io/robertu94/libpressio_tutorial"])
