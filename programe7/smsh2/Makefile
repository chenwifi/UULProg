smshm : main.o splitline.o controlflow1.o builtin.o process1.o varlib.o execute.o
		cc -o smshm main.o splitline.o controlflow1.o builtin.o process1.o varlib.o execute.o

main.o : main.c smsh.h varlib.h
		cc -c main.c smsh.h varlib.h

splitline.o : splitline.c smsh.h
		cc -c splitline.c smsh.h

controlflow1.o : controlflow1.c smsh.h
		cc -c controlflow1.c smsh.h

builtin.o : builtin.c smsh.h varlib.h
		cc -c builtin.c smsh.h varlib.h

process1.o : process1.c smsh.h
		cc -c process1.c smsh.h

varlib.o : varlib.c 
		cc -c varlib.c 

execute.o : execute.c varlib.h
		cc -c execute.c varlib.h

clean :
		rm smshm main.o splitline.o controlflow1.o builtin.o process1.o varlib.o execute.o
