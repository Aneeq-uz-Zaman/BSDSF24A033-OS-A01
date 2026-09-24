.PHONY: all clean

all: ../bin/client_static ../bin/client_dynamic

../bin/client_static: ../obj/main.o ../lib/libmyutils.a
	gcc ../obj/main.o ../lib/libmyutils.a -o ../bin/client_static

../bin/client_dynamic: ../obj/main.o ../lib/libmyutils.so
	gcc ../obj/main.o -L../lib -lmyutils -o ../bin/client_dynamic

../lib/libmyutils.a: ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ar rc ../lib/libmyutils.a ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	ranlib ../lib/libmyutils.a

../lib/libmyutils.so: ../obj/mystrfunctions_pic.o ../obj/myfilefunctions_pic.o
	gcc --shared -o ../lib/libmyutils.so ../obj/mystrfunctions_pic.o ../obj/myfilefunctions_pic.o

../obj/main.o: main.c
	gcc -c main.c -o ../obj/main.o

../obj/mystrfunctions.o: mystrfunctions.c
	gcc -c mystrfunctions.c -o ../obj/mystrfunctions.o

../obj/myfilefunctions.o: myfilefunctions.c
	gcc -c myfilefunctions.c -o ../obj/myfilefunctions.o

../obj/mystrfunctions_pic.o: mystrfunctions.c
	gcc -fPIC -c mystrfunctions.c -o ../obj/mystrfunctions_pic.o

../obj/myfilefunctions_pic.o: myfilefunctions.c
	gcc -fPIC -c myfilefunctions.c -o ../obj/myfilefunctions_pic.o

clean:
	rm -f ../obj/*.o ../lib/libmyutils.a ../lib/libmyutils.so ../bin/client_static ../bin/client_dynamic
