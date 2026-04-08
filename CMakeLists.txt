cmake_minimum_required(VERSION 3.22)
set(CMAKE_PROJECT_NAME GD32F4xx_Template)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(${CMAKE_PROJECT_NAME})
enable_language(C CXX ASM)

add_executable(${CMAKE_PROJECT_NAME})
add_subdirectory(Driver)
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    "Driver/CMSIS/GD/GD32F4xx/Source/GCC/startup_gd32f4xx.s"
    "Core/Src/main.c"
    "Core/Src/syscalls.c"
    "Core/Src/gpio.c"
    "Core/Src/led.c"
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    "Core/Inc"
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    GD32F4xx_SPL
)

# 创建自动配置的 launch.json
file(WRITE ${CMAKE_SOURCE_DIR}/.vscode/launch.json.in
[=[
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug",
            "cwd": "${workspaceFolder}",
            "executable": "@PROGRAM_PATH@",
            "request": "launch",
            "type": "cortex-debug",
            "runToEntryPoint": "main",
            "servertype": "jlink",
            "device": "@device@",
            "liveWatch": {
                "enabled": true,
                "samplesPerSecond": 4
            },
            "preLaunchTask": "build",
            "serverArgs": [
                "-speed",
                "1000"
            ]
        },
        {
            "name": "Run",
            "cwd": "${workspaceFolder}",
            "executable": "@PROGRAM_PATH@",
            "request": "launch",
            "type": "cortex-debug",
            "runToEntryPoint": "",
            "servertype": "jlink",
            "device": "@device@",
            "liveWatch": {
                "enabled": true,
                "samplesPerSecond": 4
            },
            "preLaunchTask": "build",
            "serverArgs": [
                "-speed",
                "1000"
            ]
        }
    ]
}
]=])

# 替换占位符
set(PROGRAM_PATH ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf)
set(device "GD32F470VET6")

configure_file(
    ${CMAKE_SOURCE_DIR}/.vscode/launch.json.in
    ${CMAKE_SOURCE_DIR}/.vscode/launch.json
    @ONLY
)