# DPF Webview + Vite Template

- DPF: [DISTRHO/DPF](httos://github.com/DISTRHO/DPF)
- pnpm: [pnpm](https://pnpm.io/)
- Vite: [Vite](https://vitejs.dev/)

## Commands

```bash
# Install Dependencies
git submodule update --init --recursive

# Debug
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build

# Release
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

## Todo

- Support Mac (I don't have a Mac, contributions are welcome)
- Support Linux (I'm simply not familiar with Linux, contributions are welcome)

## License

MIT License.
