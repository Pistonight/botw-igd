"""Common Util"""
from os.path import isfile
import csv
import toml

WORKSPACE_TOML = "workspace.toml"
WORKSPACE_TEMPLATE_TOML = "workspace.template.toml"
def read_config():
    """Read config from workspace.toml or workspace.template.toml"""
    if isfile(WORKSPACE_TOML):
        with open(WORKSPACE_TOML, "r", encoding="utf-8") as config_file:
            return toml.load(config_file)
    elif isfile(WORKSPACE_TEMPLATE_TOML):
        with open(WORKSPACE_TEMPLATE_TOML, "r", encoding="utf-8") as config_file:
            return toml.load(config_file)

    print("Cannot read workspace config. \
Make sure you either create your own {WORKSPACE_TOML} \
or have the default {WORKSPACE_TEMPLATE_TOML}")
    return None

def read_linker_config():
    """Read linker config"""
    return read_sub_config("linker")

def read_ips_config():
    """Read ips config"""
    return read_sub_config("ips")

def read_ftp_config():
    """Read ftp config"""
    return read_sub_config("ftp")

def read_sub_config(sub):
    """Read a sub member of config"""
    config = read_config()
    if config is not None:
        return config[sub]
    return None

def read_uking_data_symbols(file, addr_set, symbol_map):
    """Read uking data symbols into map from data_symbols.csv"""
    total_count = 0
    print("Reading", file)
    with open(file, "r", encoding="utf-8") as csv_file:
        reader = csv.reader(csv_file)
        for row in reader:
            # Skip invalid rows
            if len(row) < 2:
                continue
            raw_addr = row[0]
            data_name = row[1]
            if len(data_name) > 0:
                addr_str = parse_address(raw_addr)
                if addr_str is None:
                    print("Error: Invalid Address: ", raw_addr)
                    continue
                if addr_str in addr_set:
                    print("Error: Duplicate Address: ", raw_addr)
                    continue
                addr_set.add(addr_str)
                symbol_map[data_name] = addr_str
                total_count+=1
    print(f"Loaded {total_count} uking data symbols.")

def read_uking_func_symbols(file, addr_set, symbol_map):
    """Read uking func symbols into map from uking_functions.csv"""
    total_count = 0
    with open(file, "r", encoding="utf-8") as csv_file:
        reader = csv.reader(csv_file)
        for row in reader:
            # Skip invalid rows
            if len(row) < 4:
                continue
            raw_addr = row[0]
            # Skip the headers
            if raw_addr == "Address":
                continue
            func_name = row[3]
            if len(func_name) > 0:
                addr_str = parse_address(raw_addr)
                if addr_str is None:
                    print("Error: Invalid Address: ", raw_addr)
                    continue
                if addr_str in addr_set:
                    print("Error: Duplicate Address: ", raw_addr)
                    continue
                addr_set.add(addr_str)
                symbol_map[func_name] = addr_str
                total_count+=1

    print(f"Loaded {total_count} uking func symbols.")

# Address Prefix to strip (and check) in botw csv files
ADDR_PREFIX = "0x00000071"
def parse_address(raw_addr):
    """Strip the 0x00000071"""
    if not raw_addr.startswith(ADDR_PREFIX):
        return None
    return "0x" + raw_addr[len(ADDR_PREFIX):]
