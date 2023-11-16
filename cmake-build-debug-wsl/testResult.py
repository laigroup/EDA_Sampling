import os

for root, dirs, files in os.walk("testCase/simple/"):
    for dir in dirs:
        for root, dirs, files, in os.walk("testCase/simple/" + dir):
            os.system(f"./Project {os.path.join(root, files[0])} {os.path.join(root, files[2])}")
