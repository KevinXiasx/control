cc=arm-linux-gnueabihf-gcc -g 
LIB= -lpthread -lrt -lanl
ELF=main
src=$(shell find ./ -name '*.c')
obj=$(src:%.c=%.o)

$(ELF):$(obj)
	$(cc) -static -o $(ELF) $(^) $(LIB)

.c.o:
	$(cc) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)

clean:
	rm -rf $(ELF) $(obj)
all:
	echo $(src)

#       