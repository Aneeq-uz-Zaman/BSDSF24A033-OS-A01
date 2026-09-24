.PHONY: all clean

all: ../bin/client_static

../bin/client_static: ../obj/main.o ../lib/libmyutils.a
	gcc ../obj/main.o -L../lib -lmyutils -o ../bin/client_static

../lib/libmyutils.a: ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ar rc ../lib/libmyutils.a ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ranlib ../lib/libmyutils.a

../obj/main.o: main.c
	gcc -c main.c -o ../obj/main.o

../obj/mystrfunctions.o: mystrfunctions.c
	gcc -c mystrfunctions.c -o ../obj/mystrfunctions.o

../obj/myfilefunctions.o: myfilefunctions.c
	gcc -c myfilefunctions.c -o ../obj/myfilefunctions.o

clean:
	rm -f ../obj/*.o ../lib/libmyutils.a ../bin/client_static
