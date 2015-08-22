ELF=main
src=$(shell find ./lib/ -name '*.c')
obj=$(src:%.c=%.o)
ifeq ($(PLATM),arm)
	CC=arm-linux-gnueabihf-gcc -g
else
	CC=gcc -g
endif

LIB= -lpthread

ifeq ($(OS),android)
	DEF=-D OS=ANDROID 
	CFLAGS=-static
else
	OS=linux
	DEF=-D OS=LINUX 
endif

ifeq ($(NET),wan)
	DEF+= -D NET=WAN 
else
	NET=lan
	DEF+= -D NET=LAN
endif

$(ELF): $(obj)
	$(CC) $(CFLAGS) -o $(ELF)-$(OS)-$(NET)-client $(^) $(LIB)

.c.o:
	$(CC)  $(DEF) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)

.PHONY :clean 
clean:
	rm -rf $(ELF)-*-client lib/*.o
all:
	echo $(src)
