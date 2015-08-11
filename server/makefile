cc=g++ -g 
LIB= -lpthread -lrt -lmysql -levent 
ELF=control-server
src=$(shell find ./lib/ -name '*.cpp')
obj=$(src:%.cpp=%.o)

$(ELF):$(obj)
	$(cc) -o $(ELF) $(^) $(LIB)

.cpp.o:
	$(cc) -c $(<D)/$(<F) -o $(@D)/$(@F) $(LIB)
clean:
	rm -rf $(ELF) lib/*.o
all:
	echo $(src)
	
