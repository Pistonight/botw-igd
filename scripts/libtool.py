"""Tool for diffing and syncing library"""
# Options
# [d]iff    Show list of changed files
# [s]ync    Copy the changed files
# [f]orward Direction is from external to this repo
# [r]everse Direction is from this repo to external

# Example: libtool.py -fd Will scan this repo and see
#   if any files are changed when compared to external source
# This does not handle file deletion. You would need to manually delete files
from os import listdir
from os.path import exists, join, isdir, isfile
import filecmp
import sys
import shutil
# pylint: disable-next=import-error
from cu import read_config


def process_path(base, target, path, do_copy):
    """Compare path as if copying from base to target"""
    base_path = join(base, path)
    target_path = join(target, path)
    if not exists(target_path):
        if isfile(base_path):
            if do_copy:
                shutil.copyfile(base_path, target_path)
        elif isdir(base_path):
            if do_copy:
                shutil.copytree(base_path, target_path)
        print("+ "+path)
        return 0
    if isfile(base_path):
        if isfile(target_path):
            if not filecmp.cmp(base_path, target_path):
                if do_copy:
                    shutil.copyfile(base_path, target_path)
                print("C "+path)
                return 1

        print("Ignoring type difference, base is file, target is not")
        return 0
    if isdir(base_path):
        count = 0
        if isdir(target_path):
            for sub_path in listdir(base_path):
                count += process_path(base, target, join(path, sub_path), do_copy)
            return count

        print("Ignoring type difference, base is dir, target is not")
        return 0
    return 0

def read_lib_config():
    """Read config[lib]"""
    config = read_config()
    if config is not None:
        return config["libs"]
    sys.exit(-1)

def is_sync_mode(flags):
    """Scan the flags for s. """
    # In sync mode, the files are actually copied
    for char in flags:
        if char == "s":
            return True
    return False

def is_forward_mode(flags):
    """Scan the falgs for f. """
    # In forward mode, the files are compared as if copying from external to this project
    for char in flags:
        if char == "f":
            return True
    return False

def run():
    """Main"""
    if len(sys.argv) < 2:
        print("No flags provided")
        sys.exit(-1)
    flags = sys.argv[1]
    config = read_lib_config()

    if config is None:
        sys.exit(-1)
    forward_mode = is_forward_mode(flags)
    do_copy = is_sync_mode(flags)
    for lib_name in config:
        lib = config[lib_name]
        if forward_mode:
            base_path = lib["path"]
            target_path = lib["target"]
        else:
            target_path = lib["path"]
            base_path = lib["target"]

        for directory in lib["dirs"]:
            print(f"{base_path} -> {target_path} [{directory}]")
            process_path(base_path, target_path, directory, do_copy)
        #print(f"Processed {count} entries")

if __name__ == "__main__":
    run()
