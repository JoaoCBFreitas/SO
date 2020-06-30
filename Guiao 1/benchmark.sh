#!/bin/bash

time cp in.txt out.txt
md5sum in.txt out.txt

rm out.txt

time ./myCopy in.txt out.txt 1
md5sum in.txt out.txt

rm out.txt

time ./myCopy in.txt out.txt 1024
md5sum in.txt out.txt

rm out.txt
