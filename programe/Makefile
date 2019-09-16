objects = test.o stack.o getnum.o

all : test test1 test2

.PHONY : all

CFLAGS = -g

test : $(objects) 
		cc -o $(CFLAGS) test $(objects)

test1 : $(objects) 
		cc -o test1 $(objects)

test2 : $(objects) 
		cc -o test2 $(objects)

test.o : stack.h getnum.h

#stack.o : stack.h getnum.h

objs = getnum.o stack.o

$(objs) : %.o : %.c
	$(CC) -c $< -o $@

#getnum.o : 

.PHONY : clean

clean : 
		-rm test1 test2 test $(objects) 
