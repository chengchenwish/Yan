#!/usr/bin/python
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
        print("-----make build successfully-----")
    else:
        print(out)
        exit()

    if os.path.exists(BUILD_DIR+"/YAN"):
        os.popen("cp "+BUILD_DIR+"/YAN  "+ TEST_DIR+"/.")
    else:
        print("make build fail")

def compile(file):
    out = ''
    print("complie :"+file+".......")
    p = os.popen("./YAN "+file +" 2> tmp")
    p.read()
    with open('tmp', 'r') as f:
        out = f.read()
    if re.search(u'.*Successfully generated.*',out,re.M|re.I) is not None:
        print("-----------complie successfully")
    else:
       # print(out)
       print(out)
       exit()
       pass
    excute(file, "a.out")
def excute(file, out):
    expect = []
    expectedFieldBegin = False;
    with open(file,'r') as f:
        for line in f.readlines():
            if "Expected output:" in line:
                expectedFieldBegin = True
                continue
            if expectedFieldBegin:
                if  "*/" in line:
                    break
                
                expect.append(line.strip())
    print("expected:"+str(expect))
    p = os.popen(" ./"+out)
    s = p.read()

    result = s.split()
    print("result: " + str(result))
    assert len(expect) == len(result) , " expect doesn't match result\n"
    
    for a,b in zip(expect, result):
        assert a == b ,"element doesn't match"

    clean_up()

    print("\033[1;32m"+file + " : --------------------Passed\033[0m")

def clean_up():
    if  os.path.exists(TEST_DIR+"/a.out"):
         os.remove(TEST_DIR+"/a.out")
    if  os.path.exists(TEST_DIR+"/a.s"):
         os.remove(TEST_DIR+"/a.s")
    if  os.path.exists(TEST_DIR+"/tmp"):
         os.remove(TEST_DIR+"/tmp")
    # if  os.path.exists(TEST_DIR+"/YAN"):
    #      os.remove(TEST_DIR+"/YAN")

def run_test():
    files = os.listdir(TEST_DIR)
    for file in files:
        if(re.match(r"^.*\.c$",file)):
            compile(file)   




def main():
    run_make()
    run_test()
    if  os.path.exists(TEST_DIR+"/YAN"):
          os.remove(TEST_DIR+"/YAN")
    




if __name__ == '__main__':
    main()