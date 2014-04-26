CC  = mips-gcc
CFLAGS  = -Wall -Wshadow -O0 

LINK	=	

OBJS	=   display904.o touchpanel904.o

BIN     =	display904

.c.o: 
	$(CC) -c $*.c $(CFLAGS)

all: $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LINK)

clean: 
	rm -f *.o $(BIN) 


