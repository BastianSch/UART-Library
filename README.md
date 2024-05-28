Build:
gcc -c -Wall -Werror -fpic uart.c
gcc -shared -o libuart.so uart.o
gcc -L. -Wall -o main main.c -luart

export LD_LIBRARY_PATH=$PROJECT_PATH:$LD_LIBRARY_PATH

Test:
sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
gcc -L. -o test test.c -lcunit -luart
./test

Run:
./main
