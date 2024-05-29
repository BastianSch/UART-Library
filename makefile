main: libbuffer.so libcom.so
	gcc -I./include -L. -o main src/main.c -lcom -lbuffer

libcom.so: com.o
	gcc -L. -shared -o libcom.so com.o

com.o:
	gcc -I./include -c -Wall -Werror -fpic src/com.c

libbuffer.so: buffer.o
	gcc -L. -shared -o libbuffer.so buffer.o

buffer.o:
	gcc -I./include -c -Wall -Werror -fpic src/buffer.c

clean:
	rm -f main
	rm -f libcom.so libbuffer.so
	rm -f com.o buffer.o