# OPESY-MP2
Group Members:
- Chua, Edric Jarvis
- Cruz, Renz Ezekiel
- Li, Wai Kei
- Yu, Cedric Leopold

## Dependencies
### Required
- [gcc](https://gcc.gnu.org/install/download.html)
- [GNU Make](https://www.gnu.org/software/make/#download)

## Build
1. Install the required dependencies `gcc` and `make`.
2. `cd` into project root.
3. Run `make` to compile executable.
4. Executable is located at `build/a.out`.
5. After usage, run `make clean` to remove executable and build artifacts.

## Usage
- Inputs are passed in as command line arguments
- Running the executable without arguments will output the following:
```
Expected 6 arguments.
Usage: build/a.out <num_instances> <num_tanks> <num_healers> <num_dps> <t1> <t2>
```

## Extras
- Other compilation targets: `debug`