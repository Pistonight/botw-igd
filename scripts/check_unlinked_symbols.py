"""Check botwigd.elf after build for unlinked symbols"""
import sys
# pylint: disable-next=import-error
from cu import read_linker_config, read_uking_data_symbols, read_uking_func_symbols


EXEMPTED = set([

])

SKYLINE_EXPORTED = set([
    "__custom_init",
    "__custom_fini",
    "skyline_tcp_send_raw",
    "getRegionAddress",
    "A64HookFunction",
    "A64InlineHook",
    "sky_memcpy",
    "get_program_id",
    "get_plugin_addresses",
])
FALSE_POSITIVES = set([
    ".text",
    ".data",
    "deadbeef",
    "__register_frame_info",
    "__deregister_frame_info"
])

def run(botwigd_syms_path):
    """Main"""
    print("Checking for unlinked symbols")
    warn_exempted()

    print(f"Ignoring {len(SKYLINE_EXPORTED) + len(FALSE_POSITIVES)} skyline symbols")
    botw_symbols = set()
    read_symbols_to_set("data/symbols/main.syms", botw_symbols)
    read_symbols_to_set("data/symbols/rtld.syms", botw_symbols)
    read_symbols_to_set("data/symbols/sdk.syms", botw_symbols)
    read_symbols_to_set("data/symbols/subsdk0.syms", botw_symbols)
    print(f"Loaded {len(botw_symbols)} symbols from botw")

    botwigd_symbols = set()
    read_symbols_to_set(botwigd_syms_path, botwigd_symbols)
    print(f"Loaded {len(botwigd_symbols)} symbols from botwigd")

    difference = botwigd_symbols - botw_symbols - SKYLINE_EXPORTED - FALSE_POSITIVES - EXEMPTED
    count = len(difference)
    if count > 0:
        print(f"The following {count} symbols are supposed to be statically linked:")
        for sym in difference:
            print(f"\t{sym}")
        print("Searching for potential matched symbols in uking data...")
        found_symbols = set()
        uking_data_symbol_map = {}
        config = read_linker_config()
        read_uking_data_symbols(config["data_symbols"], set(), uking_data_symbol_map)
        for sym in difference:
            if sym in uking_data_symbol_map:
                print("Potential symbol matched:")
                print()
                print("[[data]]")
                print(f"sym = \"{sym}\"")
                print()
                found_symbols.add(sym)

        if len(found_symbols) < count:
            print("Searching for potential matched symbols in uking functions...")
            uking_func_symbol_map = {}
            read_uking_func_symbols(config["uking_functions"], set(), uking_func_symbol_map)
            for sym in difference - found_symbols:
                if sym in uking_func_symbol_map:
                    print("Potential symbol matched:")
                    print()
                    print("[[func]]")
                    print(f"sym = \"{sym}\"")
                    print()
                    found_symbols.add(sym)
        if len(found_symbols) < count:
            print("The following symbols cannot be automatically matched:")
            for sym in difference - found_symbols:
                print(f"\t{sym}")

        print()
        print("To fix the issue:")
        print("1) Add the missing symbols to a .link.toml file")
        print("2) Run \"just relink\" to regenerate the linker script and link")
    else:
        print("\033[1;33mbotwigd.nso looks good to me\033[0m")
    return count

def warn_exempted():
    """Print warning if any symbol is in EXEMPTED"""
    if len(EXEMPTED) > 0:
        print(f"Warning: {len(EXEMPTED)} symbols exempted from the check")
def read_symbols_to_set(path, symbol_set):
    """Read the symbols from file and add to set"""
    with open(path, "r", encoding="utf-8") as file:
        for i,line  in enumerate(file):
            if i<4:
                continue # skip the header stuff
            if len(line.strip())==0:
                continue
            symbol = get_symbol_from_line(line)
            symbol_set.add(symbol)

def get_symbol_from_line(line):
    """Get the symbol from a line in objdump"""
    stripped = line[25:] # strips first 25 characters, that will leave us nicer formatting
    # print(stripped.split(" "))
    return stripped.split(" ")[1].strip()

if __name__ == "__main__":
    sys.exit(run(sys.argv[1]))
