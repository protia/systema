SYSP=../../compiler/sysp
SYSC=../../compiler/sysc
SYSA=../../compiler/sysa
SYSL=../../compiler/sysl

SNAME=hello
SRC=$(srcdir)/$(SNAME).sys
INT=$(SNAME).int
ASM=$(SNAME).asm
OBJ=$(SNAME).obj
BIN=$(SNAME)

all-local: $(BIN)

$(BIN): $(SRC) $(SYSP) $(SYSC) $(SYSA) $(SYSL)
	$(SYSP) -o $(INT) $<
	$(SYSC) -o $(ASM) $(INT)
	$(SYSA) -o $(OBJ) $(ASM)
	$(SYSL) -o $(BIN) $(OBJ)

run: $(BIN)
	./$(BIN) arg1 arg2 arg3

clean-local:
	rm -f $(BIN) *.int *.asm *.obj

