Build:
gcc -c -Wall -Werror -fpic buffer.c
gcc -L. -shared -o libbuffer.so buffer.o
gcc -c -Wall -Werror -fpic com.c
gcc -L. -shared -o libcom.so com.o

gcc -I. -L. -o main main.c -lcom -lbuffer

export LD_LIBRARY_PATH=$PROJECT_PATH:$LD_LIBRARY_PATH

Test:
sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
gcc -I. -L. -o test_buffer test_buffer.c -lcunit -lbuffer
./test_buffer

Run:
./main
