default: lint build

# Install pip modules
pipinstall: 
    python3 -m pip install toml pylint pylint-quotes keystone

# Lint .cpp, .hpp and .py files. Pass --verbose to see more output
lint VERBOSE="":
    @just lintpy {{VERBOSE}}
    @just lintc {{VERBOSE}}

lintpy VERBOSE="":
    python3 scripts/lint.py {{VERBOSE}}
    pylint scripts

lintc VERBOSE="":
    clang-format-12 -n -Werror {{VERBOSE}} $(find src -name *.hpp)
    clang-format-12 -n -Werror {{VERBOSE}} $(find src -name *.cpp) 

# Run clang-format on source files
format VERBOSE="":
    @echo "Checking .hpp"
    clang-format-12 -i {{VERBOSE}} $(find src -name *.hpp)
    @echo "Checking .cpp"
    clang-format-12 -i {{VERBOSE}} $(find src -name *.cpp) 

# Generate linker script
ld:
    mkdir -p build
    python3 scripts/gen_linker_script.py

# Build botwigd, app.npdm, and the ips patches
build: 
    mkdir -p build
    make -C build -f ../build.mk
    objdump -T build/botwigd.elf > build/botwigd.syms
    python3 scripts/check_unlinked_symbols.py build/botwigd.syms
    python3 scripts/gen_ips_patch.py

# Regenerate the linker script and rebuild
relink:
    rm -f build/botwigd.nso build/botwigd.elf
    @just ld
    @just build

clean:
    rm -rf build

lib FLAGS:
    python3 scripts/libtool.py {{FLAGS}}

ftp FTP_OPTION:
    python3 scripts/ftp.py {{FTP_OPTION}}
