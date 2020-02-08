# Copies the Python standard library to the target dir
# If the target_dir already exists, it doesn't do anything
# an easy way is to find the __init__ file for a known standard lib package and go two parents up
# should result in something like `/usr/lib/python3.7`

# this script must be called with a target directory to copy this into
import ctypes
import os
import shutil
import sys

if len(sys.argv) == 3:
    exe_path = sys.argv[1]
    folder_name = sys.argv[2]
else:
    print("Must call " + sys.argv[0] + "with two command line arguments: the path to the energyplus exe and the name "
                                       "of the new library directory")
    sys.exit(1)
exe_dir = os.path.dirname(exe_path)
target_dir = os.path.join(exe_dir, folder_name)
if os.path.exists(target_dir):
    sys.exit(0)

ctypes_import_file = os.path.abspath(ctypes.__file__)
ctypes_package_dir = os.path.dirname(ctypes_import_file)
standard_lib_dir = os.path.dirname(ctypes_package_dir)
shutil.copytree(standard_lib_dir, target_dir)

# then I'm going to try to clean up any __pycache__ folders in the target dir to reduce installer size
for root, dirs, _ in os.walk(target_dir):
    for this_dir in dirs:
        if this_dir == "__pycache__":
            shutil.rmtree(os.path.join(root, this_dir))
