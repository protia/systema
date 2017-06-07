MAIN     = syc
CORE     = syc-core
MAINSRC  = $(srcdir)/main.sh
SRCS     = $(wildcard $(srcdir)/*.c)
HFILES   = $(wildcard $(srcdir)/*.h)
OBJECTS  = $(shell basename -a $(patsubst %.c,%.o,$(SRCS)))

all-local: $(CORE) $(MAIN)

%.o:%.c $(HFILES)
	$(CC) -c -o $@ $<

$(CORE): $(OBJECTS)
	$(CC) -o $@ $^    

$(MAIN): $(MAINSRC) $(CORE)
	cp $< $@
	chmod +x $@

clean-local:
	rm -f $(MAIN) $(CORE) $(OBJECTS)
