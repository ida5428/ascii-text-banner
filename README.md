<div align="center">
   <img src="assets/ascii-text-banner.gif" alt="ascii-text-banner preview">
</div>

# ascii-text-banner
A lightweight C++ command-line tool for generating ASCII text banners.

## Features
- Render text into ASCII art
- Optional scrolling effect
- More features such as outputting files and multiple fonts coming soon!
- Upcoming features: file output and support for multiple built-in and custom fonts!

## Dependencies
- A C++17 (or newer) compiler
- [CMake](https://cmake.org/) >= 3.16
- [CLI11](https://github.com/CLIUtils/CLI11) (included in `libs/`)

## Build & Run Instructions
### 1. Clone this repository;
```bash
git clone https://github.com/ida5428/ascii-text-banner.git
```

### 2. Navigate into the project directory:
```bash
cd ascii-text-banner
```

### 3. Generate the build system:
```bash
cmake -S . -B build
```

### 4. Build the project:
```bash
cmake --build build
```

### 5. Run the program:
```bash
./build/main --input "Hello World!" -o
```
```bash
./build/main -i "Some veryyyyyyy long text......" --scroll -v 10
```

> [!NOTE]
> The `-o/--output` flag is not yet implemented and currently has no effect.<br>
> Run with `-h` or `--help` for more details.

## License
This project is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html), and its file can be found [here](LICENSE).
[CLI11](https://github.com/CLIUtils/CLI11) is distributed under a [custom license](https://github.com/CLIUtils/CLI11/blob/main/LICENSE), which is also included [here](libs/CLI11/LICENSE).

