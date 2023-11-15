#!/bin/bash
make
echo -e "to run on testFile: push 1\nto run on puthon3 tester:push 2"
read input
if [ "$input" == "1" ]; then
    ./compiler < testFile
else
    python3 ./Tester/tester.py ./compiler ./Tester/Tests
fi
