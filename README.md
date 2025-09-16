<div align="center">
   <img src="assets/ascii-text-banner.gif" alt="ascii-text-banner preview" width="600">
</div>

# ascii-text-banner
A lightweight C++ command-line tool for generating ASCII text banners.

## Features
- Render text into ASCII art
- A scrolling banner effect
- Support for multiple fonts. See [here](fonts/README.md) for more details.

## Dependencies
- A C++17 (or newer) compiler
- [CMake](https://cmake.org/) >= 3.16
- [CLI11](https://github.com/CLIUtils/CLI11) – used to parse command-line arguments, included in `libs/`
- [tomlplusplus](https://github.com/marzer/tomlplusplus) – used to parse `.toml` font files, included in `libs/`

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

### 4. Build the project;
```bash
cmake --build build
```

### 5. Run the program, some examples below:
```bash
./build/main -i "Hello World!"
```
```bash
./build/main -i "A new font?" -f bubble -o ascii/hello.txt
```
```bash
./build/main "Some veryyyyyyy long text......" --scroll 8
```

> [!NOTE]
> Run with `-h` or `--help` for more details.

## Contributing
Feel free to create pull requests and suggest features or changes. Contributions of custom fonts are especially welcome!

## License
This project is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html), see [LICENSE](LICENSE).  
[CLI11](https://github.com/CLIUtils/CLI11) is under a [custom license](https://github.com/CLIUtils/CLI11/blob/main/LICENSE), also included [here](libs/CLI11/LICENSE).  
[tomlplusplus](https://github.com/marzer/tomlplusplus) is licensed under the [MIT License](https://opensource.org/license/mit) and can also be found [here](libs/tomlplusplus/LICENSE).

