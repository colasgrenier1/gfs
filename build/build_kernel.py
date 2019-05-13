"""
Build the GFS kernel.

Usage: python3 (path/)build_kernel.py <kernel directory> <default configuration file path> <output file path>
"""

import os
import shutil
import sys
import tempfile

print(sys.argv)

#Check the arguments
if len(sys.argv) != 4:
	print("usage: python build_kernel.py <kernel dir> <def conf file> <output file>")
	sys.exit(-1)

#Process the arguments
kern = os.path.abspath(sys.argv[1])
conf = os.path.abspath(sys.argv[2])
outf = os.path.abspath(sys.argv[3])

#Create a temporary directory
tmpdir = tempfile.TemporaryDirectory()
dir = tmpdir.name

#Copy the source files into the temporary directory
for file in os.listdir(kern):
	try:
		shutil.copy(os.path.join(kern, file), os.path.join(dir, file))
	except IsADirectoryError:
		shutil.copytree(os.path.join(kern, file), os.path.join(dir, file))

os.system("ls -lhR " + dir)
