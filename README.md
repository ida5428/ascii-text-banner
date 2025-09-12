# ascii-text-banner
A simple C++ command-line program to generate ASCII text banners.

## Build & Run Instructions
### Prerequisites
- [CMake](https://cmake.org/) >= 3.16
- A C++17 (or later) Compiler

1. Clone this repository:
``git clone https://github.com/ida5428/ascii-text-banner.git``

2. Navigate into the project directory: 
``cd ascii-text-banner``

3. Generate the build system:
``cmake -S . -B build``

4. Build the project:
``cmake --build build``

5. Run the program, for example:
> [!NOTE]
> Single quotes, double quotes, backticks, and tildes are not yet supported. They appear as placeholders.
> The ``--scroll/-s`` and ``--output/-o`` flags have not yet been implemented, they will have no affect.

``./build/main --input "Hello World!" -o``
``./build/main -i "Some veryyyyyyy long text......" --scroll -v 2``
> Run with the `-h` or `--help` flag for more details.

## License
This project is licensed under the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).  
[CLI11](https://github.com/CLIUtils/CLI11) uses a custom license which can be found [here](https://github.com/ida5428/ascii-text-banner/libs/CLI11/LICENSE).
