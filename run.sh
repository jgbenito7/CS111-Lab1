#!/bin/sh

make clean

wait 

make WERROR_CFLAGS=

wait

./profsh -t script.sh