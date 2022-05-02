default: check build

# Install pip modules
pipinstall: 
    python3 -m pip install toml pylint pylint-quotes

# Check everything is good
check:
    @just lint
    echo "elf symbol check not ran"

lint: 
    python3 scripts/lint.py
    pylint scripts
    echo "clang-format not ran"

build:
    echo "TBD"
