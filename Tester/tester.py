import os
import argparse
import subprocess

# Colors for output
RESET = "\033[0m"
BLUE = "\033[34m"
RED = "\033[91m"
GREEN = "\033[92m"
YELLOW = "\033[93m"

# Color function
def colorize(text, color):
    return f"{color}{text}{RESET}"

# Test function
def run_tests(compiler_path, test_folder):
    # Get a list of files with test inputs and sort them
    test_inputs = sorted([file for file in os.listdir(test_folder) if file.endswith(".in")])

    # Test colors
    test_passed = colorize("PASSED", GREEN)
    test_failed = colorize("FAILED", RED)
    test_error = colorize("ERROR", BLUE)

    # Variables
    passed_tests = 0
    failed_tests = 0
    error_tests = 0

    for test_input in test_inputs:
        input_path = os.path.join(test_folder, test_input)
        expected_output_path = os.path.join(
            test_folder, test_input.replace(".in", ".out")
        )

        # Test name without .in extension
        test_name = colorize(os.path.splitext(test_input)[0], YELLOW)

        # Run compiler with the current test input and get a CompletedProcess object
        try:

            try:
                result = subprocess.run(
                    [compiler_path], stdin=open(input_path, "r"), stdout=subprocess.PIPE,
                    stderr=subprocess.DEVNULL, text=True
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
                    print(f"{test_name} {test_passed}")

                    passed_tests += 1
                else:

                    # Print expected program output and return code in case of failure
                    print(f"{test_name} {test_failed}")
                    for line in expected_output_lines:
                        print("\t", line, end="")

                    # Print actual program output and return code in case of failure
                    print(f"\n\t Your Output:\n\t {result.stdout}")
                    print(f"\t Your Return Code: {result.returncode}")

                    failed_tests += 1

            # Error handling
            except subprocess.TimeoutExpired:
                print(f"{test_name} {test_error}")
                print(f"\t Info: Test execution timed out (possible infinite loop)")

                # Print additional information for debugging
                print("\t Subprocess return code:", result.returncode)
                if result.stdout is not None:
                    print("\t Subprocess stdout:", result.stdout)
                if result.stderr is not None:
                    print("\t Subprocess stderr:", result.stderr)

                error_tests += 1
            except subprocess.CalledProcessError as e:
                print(f"{test_name} {test_error}")
                print(f"\t Info: {e.output}")

                # Print additional information for debugging
                print("\t Subprocess return code:", e.returncode)
                if e.stdout is not None:
                    print("\t Subprocess stdout:", e.stdout)
                if e.stderr is not None:
                    print("\t Subprocess stderr:", e.stderr)

                error_tests += 1

        except KeyboardInterrupt:
            print(f"\n{test_name} {test_error}")
            print(f"\t Info: Testing interrupted by user (skipping to the next test)")

            error_tests += 1

    # Output the summary of passed and failed tests
    print(f"==========\n")
    print(f"{passed_tests} {colorize('PASSED', GREEN)}")
    print(f"{failed_tests} {colorize('FAILED', RED)}")
    print(f"{error_tests} {colorize('ERROR', BLUE)}")
    print(f"\n==========")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Tests the compiler using test data.")
    parser.add_argument("compiler_path", help="Path to the compiler")
    parser.add_argument("test_folder", help="Folder with test inputs and expected outputs")
    args = parser.parse_args()

    # Run test script
    run_tests(args.compiler_path, args.test_folder)
