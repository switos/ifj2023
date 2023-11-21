import os
import shutil

def extract_return_code(file_name):
    parts = file_name.split("_")
    if len(parts) >= 2:
        try:
            return int(parts[1])
        except ValueError:
            pass
    return None

swift_folder = "Swift"
tests_folder = "Tests"

if not os.path.exists(swift_folder):
    print(f"ERROR: No swift folder.")
else:
    if not os.path.exists(tests_folder):
        os.mkdir(tests_folder)

    swift_files = [f for f in os.listdir(swift_folder) if f.endswith(".swift")]

    for swift_file in swift_files:
        swift_file_path = os.path.join(swift_folder, swift_file)

        test_name = os.path.splitext(swift_file)[0]
        test_file_name = test_name + ".in"
        test_file_path = os.path.join(tests_folder, test_file_name)
        out_file_name = test_name + ".out"
        out_file_path = os.path.join(tests_folder, out_file_name)

        shutil.copy(swift_file_path, test_file_path)

        return_code = extract_return_code(test_name)

        if return_code is not None:
            with open(out_file_path, "w") as out_file:
                out_file.write(f"Expected Output:\n\nExpected Return Code: {return_code}\n")
