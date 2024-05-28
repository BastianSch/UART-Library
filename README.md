# UART Lib

### Build:
```
make
```

### Test:
```
sudo apt-get install libcunit1 libcunit1-doc libcunit1-dev
gcc -I. -L. -o test_buffer test_buffer.c -lcunit -lbuffer
./test_buffer
```

### Run:
```
export LD_LIBRARY_PATH=$PROJECT_PATH:$LD_LIBRARY_PATH

./main
```