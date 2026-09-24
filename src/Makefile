.PHONY: all clean

all: ../bin/client

../bin/client: ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o
	gcc ../obj/main.o ../obj/mystrfunctions.o ../obj/myfilefunctions.o -o ../bin/client

../obj/main.o: main.c
	gcc -c main.c -o ../obj/main.o

../obj/mystrfunctions.o: mystrfunctions.c
	gcc -c mystrfunctions.c -o ../obj/mystrfunctions.o

../obj/myfilefunctions.o: myfilefunctions.c
	gcc -c myfilefunctions.c -o ../obj/myfilefunctions.o

clean:
	rm -f ../obj/*.o ../bin/client
