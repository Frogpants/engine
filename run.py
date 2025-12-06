import subprocess
import sys

# Commands to run
compile_cmd = [
    "g++", "-std=c++17", "-Wall", "-Wextra",
    "mini_engine/src/essentials.cpp",
    "-o", "mini_engine/src/essentials.out",
    "-lglfw", "-lGLEW", "-lGL"
]

run_cmd = ["./mini_engine/src/essentials.out"]

print("Compiling essentials.cpp...")
result = subprocess.run(compile_cmd)

if result.returncode != 0:
    print("Compilation failed.")
    sys.exit(1)

print("Running essentials.out...")
subprocess.run(run_cmd)

# python3 run.py
