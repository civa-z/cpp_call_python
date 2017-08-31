import os,sys

print ("python file loaded")

def plus_one(a):
    b = []
    for i in range(len(a)):
        b.append(a[i] + 1)
    return b

if __name__ == "__main__":
    a = [1,3,5]
    b =  plus_one(a)
    print b
