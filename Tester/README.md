# My tester for IFJ23 project

The tester runs the compiler program, inputs `.in` files from the tests folder to `stdin`, and then compares `stdout` with the expected output in the form of `.out` files, including the return code.

0. Check `Tests` folder

1. Make sure your compiler has been made using the `make` command

2. Run the test script as `python tester.py /path/to/compiler /path/to/tests/folder` or alternatively as `python3 tester.py /path/to/compiler /path/to/tests/folder`

For example: `python3 tester.py ../compiler ./Tests`
