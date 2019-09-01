import os
import sys
import shutil
argvs = sys.argv

if len(argvs) < 3:
    print('please specify a destination path.')
    sys.exit()

os.chdir(os.path.dirname(os.path.abspath(__file__)))

if not os.path.exists(argvs[2] + '/TestData'):
    shutil.copytree(argvs[1], argvs[2] + '/TestData')