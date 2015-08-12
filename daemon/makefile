#cc=arm-linux-gnueabihf-gcc -g
cc=gcc -g
LIB= -lpthread 
ELF=main
src=$(shell find ./lib/ -name '*.c')
obj=$(src:%.c=%.o)

linux: $(obj)
	$(cc) -o $(ELF)-linux-client $(^) $(LIB)

android:  $(obj)
	$(cc)  -static -o $(ELF)-andorid-client $(^) $(LIB)

.c.o:
	$(cc) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)

clean:
	rm -rf main-* lib/*.o
all:
	echo $(src)
