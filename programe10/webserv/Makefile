webserv : main.o process.o func.o socklib.o
		gcc -g -o webserv main.o process.o func.o socklib.o

main.o : main.c process.h socklib.h
		gcc -g -c main.c

process.o : process.c func.h
		gcc -g -c process.c

func.o : func.c 
		gcc -g -c func.c

socklib.o : socklib.c
		gcc -g -c socklib.c

clean :
		rm webserv main.o process.o func.o socklib.o
