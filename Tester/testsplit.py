import os
import argparse

parser = argparse.ArgumentParser(description="Splits text from gen.txt into files with custom names")
parser.add_argument("--name", default="test", help="File name for tests (without extension)")
parser.add_argument("--code", type=int, default=0, help="Test return code")
args = parser.parse_args()

if not os.path.exists("Gen"):
    os.mkdir("Gen")

with open("gen.txt", "r") as gen_file:
    current_test_number = 0
    current_test_content = []

    for line in gen_file:
        line = line.strip()
        if not line:
            if current_test_content:
                in_filename = f"Gen/{args.name}_{args.code}_{current_test_number:02d}.in"
                with open(in_filename, "w") as in_file:
                    in_file.write("\n".join(current_test_content))

                out_filename = f"Gen/{args.name}_{args.code}_{current_test_number:02d}.out"
                with open(out_filename, "w") as out_file:
                    out_file.write(f"Expected Output:\n\nExpected Return Code: {args.code}")

                current_test_number += 1
                current_test_content = []
        else:
            current_test_content.append(line)

    if current_test_content:
        in_filename = f"Gen/{args.name}_{args.code}_{current_test_number:02d}.in"
        with open(in_filename, "w") as in_file:
            in_file.write("\n".join(current_test_content))

        out_filename = f"Gen/{args.name}_{args.code}_{current_test_number:02d}.out"
        with open(out_filename, "w") as out_file:
            out_file.write(f"Expected Output:\n\nExpected Return Code: {args.code}")
