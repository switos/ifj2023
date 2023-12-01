import os
import argparse

parser = argparse.ArgumentParser(description="Splits text from .txt into files with custom names")
parser.add_argument("--name", default="test", help="File name for tests (without extension)")
parser.add_argument("--code", type=int, default=0, help="Test return code")
args = parser.parse_args()

if not os.path.exists("Swift"):
    os.mkdir("Swift")

with open("Split/swift.txt", "r") as gen_file:
    current_test_number = 0
    current_test_content = []
    is_new_test = True

    for line in gen_file:
        line = line.rstrip("\n")

        if line == "# END OF TEST #":
            if current_test_content:
                swift_filename = f"Swift/{args.name}_{args.code}_{current_test_number:02d}.swift"
                with open(swift_filename, "w") as swift_file:
                    swift_file.write("\n".join(current_test_content))

                current_test_number += 1
                current_test_content = []
                is_new_test = True
        else:
            if not is_new_test:
                current_test_content.append(line)
            else:
                current_test_content = [line]
                is_new_test = False

    if current_test_content:
        swift_filename = f"Swift/{args.name}_{args.code}_{current_test_number:02d}.swift"
        with open(swift_filename, "w") as swift_file:
            swift_file.write("\n".join(current_test_content))
