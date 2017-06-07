SYC=../../compiler/syc

SNAME=hello_world
SRC=$(SNAME).sy
OBJ=$(SNAME)

all-local: $(OBJ)

$(OBJ): $(SRC) $(SYC)
	$(SYC) -o $@ $<

run: $(OBJ)
	./$(OBJ)

clean-local:
	rm $(OBJ) *.o *.m4 *.s
