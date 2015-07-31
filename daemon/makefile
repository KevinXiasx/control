cc=gcc -g 
LIB= -lpthread -lrt -lanl -levent
ELF=main
src=$(shell find ./ -name '*.c')
obj=$(src:%.c=%.o)

$(ELF):$(obj)
	$(cc) -o $(ELF) $(^) $(LIB)

.c.o:
	$(cc) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)

clean:
	rm -rf $(ELF) $(obj)
all:
	echo $(src)

#       arm-linux-gnueabihf-  -static 