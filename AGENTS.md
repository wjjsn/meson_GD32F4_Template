用这个命令来setup：`meson setup build -DGD32F4xx_SPL:chip=GD32F470_xE --cross-file meson_cross_file.txt`

用这个命令调试真实的设备：`gdb-multiarch -x init.gdb <path-to-elf> -ex <command> {-ex <command>...} -batch`