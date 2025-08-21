#!/usr/bin/env python3

import os
import shutil
from pathlib import Path

SRC_DIR = "src"
INCLUDE_DIR = "include"
TARGET_DIR = "arduino_flat"

# Extensions à copier depuis src/
SRC_EXTS = [".ino", ".cpp", ".c"]
# Extensions à copier depuis include/
INCLUDE_EXTS = [".h"]

def ensure_clean_dir(path):
    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)

def copy_files_with_ext(src_folder, extensions, target_folder):
    for root, _, files in os.walk(src_folder):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                src_file = os.path.join(root, file)
                dst_file = os.path.join(target_folder, file)
                shutil.copy2(src_file, dst_file)

def main():
    ensure_clean_dir(TARGET_DIR)
    copy_files_with_ext(SRC_DIR, SRC_EXTS, TARGET_DIR)
    copy_files_with_ext(INCLUDE_DIR, INCLUDE_EXTS, TARGET_DIR)

    # Copier ui.h et ui.c depuis src/ s'ils existent
    for f in ["ui.h", "ui.c"]:
        src_path = os.path.join(SRC_DIR, f)
        if os.path.exists(src_path):
            shutil.copy2(src_path, os.path.join(TARGET_DIR, f))

    print(f"Tous les fichiers nécessaires ont été copiés dans '{TARGET_DIR}/'.")

if __name__ == "__main__":
    main()