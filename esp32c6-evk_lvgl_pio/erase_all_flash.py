import subprocess
from SCons.Script import Import
import os

Import("env")

def before_upload(source, target, env):
    print("Erasing all flash...")
    port = "COM9"  # Remplace par ton port réel

    # Nom correct du package esptool pour ESP32
    package_dir = env.PioPlatform().get_package_dir("tool-esptool-espressif")
    pio_esptool = os.path.join(package_dir, "esptool.exe")

    cmd = [
        pio_esptool,
        "--chip", "esp32c6",
        "--port", port,
        "erase_flash"
    ]
    subprocess.run(cmd, check=True)

env.AddPreAction("upload", before_upload)