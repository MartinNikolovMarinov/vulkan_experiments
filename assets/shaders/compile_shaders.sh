function check_exit_code() {
    if [ $? -ne 0 ]; then
        echo "[FAILED]"
        exit 1
    fi
}

exec_quiet() {
    eval $@ > /dev/null 2> /dev/null
    check_exit_code
}

exec_quiet glslc 01_hardcoded_triangle.frag -o 01_hardcoded_triangle.frag.spv
exec_quiet glslc 01_hardcoded_triangle.vert -o 01_hardcoded_triangle.vert.spv

exec_quiet glslc 02_triangle_with_vertex_buffer.frag -o 02_triangle_with_vertex_buffer.frag.spv
exec_quiet glslc 02_triangle_with_vertex_buffer.vert -o 02_triangle_with_vertex_buffer.vert.spv

echo "Shaders Compiled!"
