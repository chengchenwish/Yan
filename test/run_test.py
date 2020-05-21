import os
import sys
import re
BUILD_DIR =  os.path.dirname(os.path.dirname(os.path.realpath(__file__)))+"/build"
TEST_DIR = os.path.dirname(os.path.realpath(__file__))
def run_make():
    if os.path.exists(BUILD_DIR+"/YAN"):
        os.remove(BUILD_DIR+"/YAN")
    CMD = "cd "+BUILD_DIR +" && cmake . && make"

    make = os.popen(CMD)
    out = make.read()
    if re.search( r'[ 100%]', out, re.M|re.I) is not None:
        print("make build successfully...")
    else:
        print(out)
        exit()

    if os.path.exists(BUILD_DIR+"/YAN"):
        os.popen("cp "+BUILD_DIR+"/YAN  "+ TEST_DIR+"/.")
    else:
        print("make build fail")

def compile(file):
    p = os.popen("./YAN "+file)
    out = p.read()
    print(out)
    if re.match(r'.*Successfully generated.*',out,re.S) is not None:
        print("complie successfully")
    else:
       # print(out)
        #print("fail")
       # exit()
       pass
    excute("printint.expect", "a.out")
def excute(expectedFile, out):
    expect = []
    with open(expectedFile,'r') as f:
        for line in f.readlines():
            expect.append(line.strip())
    print(expect)
    print("-------")
    p = os.popen(" ./"+out)
    s = p.read()

    result = s.split()
    print(result)
    assert len(expect) == len(result) , " expect doesn't match result\n"
    
    for a,b in zip(expect, result):
        assert a == b ,"element doesn't match"

    if os.path.exists(TEST_DIR+"/a.out"):
        os.remove(TEST_DIR+"/a.out")

        print("run test successfully")


    





def run_test():
    files = os.listdir(TEST_DIR)
    for file in files:
        if(re.match(r"^.*\.c$",file)):
            compile(file)
            



   
    

def main():
    run_make()
    run_test()
    print(BUILD_DIR)




if __name__ == '__main__':
    main()