import os
import sys

os.system(sys.argv[2] + " 1> tests/"+sys.argv[1])
os.system(sys.argv[2] + " 2> tests/"+sys.argv[1]+"Err")

arguments = sys.argv[2].split()
arguments.pop(0)
argumentsStr = "["
first = True
for argument in arguments:
    if not first:
        argumentsStr += ","
    argumentsStr += "\""+argument+"\""
    first = False
argumentsStr += "]"

substr = "[\""+sys.argv[1]+"\", "+ argumentsStr +", False]"
print(substr)