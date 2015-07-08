cc=g++ -g 
LIB= -lpthread -lrt 
ELF=main
src=$(shell find ./ -name '*.cpp')
obj=$(src:%.cpp=%.o)

$(ELF):$(obj)
	$(cc) -o $(ELF) $(^) $(LIB)

.cpp.o:
	$(cc) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)
clean:
	rm -rf $(ELF) $(obj)
all:
	echo $(src)
	
