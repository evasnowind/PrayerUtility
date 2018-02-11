# encoding=utf-8
# author evasnowind
import os
import sys
input_path = ""

show_process = False


def getInputArguments():
    param_num = len(sys.argv)
    if param_num != 2:
        print("wrong parameter!")

    path = sys.argv[1]
    if path.endswith("/") != True:
        path = path + "/"
    return path

def traverseDir(dir_path):
    files = os.listdir(dir_path)
    for f in files:
        splite_name = os.path.splitext(f)
        lower_suffix = splite_name[1].lower()
        if lower_suffix != ".mobi":
            file_full_path = dir_path + f
            transFileToMobi(dir_path, file_full_path)

def transFileToMobi(dir_path, file_path):
    print("                             ")
    print("start transfer:" + file_path)
    command = dir_path + "kindlegen " + file_path
    os.system(command)
    print("finish transfer:" + file_path)
    print("                             ")


if __name__ == '__main__':
    input_path = getInputArguments()
    traverseDir(input_path)