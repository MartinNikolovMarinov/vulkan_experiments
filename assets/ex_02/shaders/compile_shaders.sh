#!/bin/bash

function check_exit_code() {
    if [ $? -ne 0 ]; then
        echo -e "\n\033[0;31m[ERROR]\033[0m - Failed to compile shaders!\n"
        exit 1
    fi
}

exec_quiet() {
    eval $@ > /dev/null 2> /dev/null
    check_exit_code
}

# exec_quiet glslc 01_hardcoded.frag -o 01_hardcoded.frag.spv
# exec_quiet glslc 01_hardcoded.vert -o 01_hardcoded.vert.spv

echo "Shaders Compiled!"

