import os
import argparse
import subprocess

def run_tests(compiler_path, test_folder):

    # Get a list of files with test inputs
    test_inputs = [file for file in os.listdir(test_folder) if file.endswith(".in")]

    for test_input in test_inputs:
        input_path = os.path.join(test_folder, test_input)
        expected_output_path = os.path.join(
            test_folder, test_input.replace(".in", ".out")
        )

    # Run the compiler with the current test input and get the output
    try:

        # Using subprocess to run the compiler
        output = subprocess.check_output(
                [compiler_path], stdin=open(input_path, "r"), stderr=subprocess.STDOUT, text=True
            )

        # Read expected output
        expected_output = open(expected_output_path, "r").read()

        # Compare output with expected output
        if output.strip() == expected_output.strip():
            print(f"Test {test_input} passed")
        else:
            print(f"Test {test_input} failed")

    except subprocess.CalledProcessError as e:
            print(f"Error while running test {test_input}: {e.output}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Tests the compiler using test data.")
    parser.add_argument("compiler_path", help="Path to the compiler")
    parser.add_argument("test_folder", help="Folder with test inputs and expected outputs")
    args = parser.parse_args()

    # Run test script
    run_tests(args.compiler_path, args.test_folder)
