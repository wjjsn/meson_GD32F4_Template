pwd是`/workspace/6.5`

用这个命令来setup：`meson setup build --cross-file meson_cross_file.ini`

用这个命令来编译：`meson compile -C build`

用这个命令调试真实的设备,GDBServer已启动好：`gdb-multiarch -x init.gdb <path-to-elf> -ex <command> {-ex <command>...} -batch`