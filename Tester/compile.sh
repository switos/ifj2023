#!/bin/bash

cd Swift || exit

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

for file in *.swift; do
    if [ -f "$file" ]; then
        echo "Compilation of file : $file"
        if swiftc "$file"; then
            echo -e "${GREEN}Compilation successful !!!${NC}"
            compiled_file="${file%.swift}"
            rm "$compiled_file"
        else
            echo -e "${RED}Compilation failed ...${NC}"
        fi
    fi
done
