import os
import argparse
import subprocess

# Colors for output
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"
RESET = "\033[0m"

# Color function
def colorize(text, color):
    return f"{color}{text}{RESET}"

# Test function
def run_tests(compiler_path, test_folder):
    # Get a list of files with test inputs and sort them
    test_inputs = sorted([file for file in os.listdir(test_folder) if file.endswith(".in")])

    for test_input in test_inputs:
        input_path = os.path.join(test_folder, test_input)
        expected_output_path = os.path.join(
            test_folder, test_input.replace(".in", ".out")
        )

        # Test name without .in extension
        test_name = colorize(os.path.splitext(test_input)[0], YELLOW)

        # Run compiler with the current test input and get a CompletedProcess object
        try:
            result = subprocess.run(
                [compiler_path], stdin=open(input_path, "r"), stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT, text=True
            )

            # Read expected output and expected return code from .out file
            with open(expected_output_path, "r") as expected_output_file:
                expected_output_lines = expected_output_file.readlines()

            expected_output = ""
            expected_return_code = None

            for line in expected_output_lines:
                if line.startswith("Expected Output:"):
                    expected_output = ""
                elif line.startswith("Expected Return Code:"):
                    expected_return_code = int(line.split(":")[1].strip())
                else:
                    expected_output += line

            # Compare output and return code with expected values
            if result.returncode == expected_return_code and result.stdout.strip() == expected_output.strip():
                test_result = colorize("PASSED", GREEN)
                print(f"{test_name} {test_result}")
            else:

                # Print expected program output and return code in case of failure
                test_result = colorize("FAILED", RED)
                print(f"{test_name} {test_result}")
                for line in expected_output_lines:
                    print("\t", line, end="")

                # Print actual program output and return code in case of failure
                print(f"\n\t Your Output:\n\t {result.stdout}")
                print(f"\t Your Return Code: {result.returncode}")

        except subprocess.CalledProcessError as e:
            print(f"{test_name} ERROR: {e.output}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Tests the compiler using test data.")
    parser.add_argument("compiler_path", help="Path to the compiler")
    parser.add_argument("test_folder", help="Folder with test inputs and expected outputs")
    args = parser.parse_args()

    # Run test script
    run_tests(args.compiler_path, args.test_folder)
