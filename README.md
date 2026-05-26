1. [安装dev插件](https://vscode.js.cn/docs/devcontainers/tutorial)
2. 在vscode中启动开发容器
3. setup meson，例如：`meson setup build/ --cross-file ./meson_cross_file.txt  --reconfigure -DGD32F4xx_SPL:chip=GD32F470_xE`
4. 编译代码`meson compile -C build/`