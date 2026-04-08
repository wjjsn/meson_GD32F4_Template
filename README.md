这个工程为裸机模板，RTOS的模板请[切换分支](https://github.com/wjjsn/Cmake_GD32F4_Template/tree/RTOS)
1. 安装cmake并添加到环境变量
2. 在`gcc-arm-none-eabi.cmake`的第10行设置你的交叉编译器路径
3. 使用下面的命令来配置和编译
```shell
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE="./gcc-arm-none-eabi.cmake" -S . -B./build/Debug -G Ninja
cmake --build ./build/Debug --
```
