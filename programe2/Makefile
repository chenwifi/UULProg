who1 : who1.o utmplib.o
	cc -o who1 who1.o utmplib.o

who1.o : who1.c utmplib.h
	cc -c who1.c -o who1.o

utmplib.o : utmplib.h utmplib.c
	cc -c utmplib.c -o utmplib.o

.PHONY : clean

clean : 
	-rm who1.o utmplib.o
