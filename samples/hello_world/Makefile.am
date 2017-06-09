SYC=../../compiler/syc

SNAME=hello_world
SRC=$(SNAME).sy
OBJ=$(SNAME)

all-local: $(OBJ)

$(OBJ): $(SRC) $(SYC)
	$(SYC) -o $@ $<

run: $(OBJ)
	./$(OBJ) arg1 arg2 arg3

clean-local:
	rm -f $(OBJ) *.i *.s *.o
