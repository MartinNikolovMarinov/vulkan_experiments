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

exec_quiet glslc 01_hardcoded.frag -o 01_hardcoded.frag.spv
exec_quiet glslc 01_hardcoded.vert -o 01_hardcoded.vert.spv

exec_quiet glslc 02_with_buffers.frag -o 02_with_buffers.frag.spv
exec_quiet glslc 02_with_buffers.vert -o 02_with_buffers.vert.spv

exec_quiet glslc 03_with_ubo.frag -o 03_with_ubo.frag.spv
exec_quiet glslc 03_with_ubo.vert -o 03_with_ubo.vert.spv

exec_quiet glslc 04_with_texture.vert -o 04_with_texture.vert.spv
exec_quiet glslc 04_with_texture.frag -o 04_with_texture.frag.spv

echo "Shaders Compiled!"
