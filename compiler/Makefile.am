CORE     = sysc-core
MAINSRC  = $(srcdir)/main.sh
SRCS     = $(wildcard $(srcdir)/*.c)
HFILES   = $(wildcard $(srcdir)/*.h)
OBJECTS  = $(shell basename -a $(patsubst %.c,%.o,$(SRCS)))
CFLAGS   = -Wall -Werror -pedantic

all-local: $(CORE) sysp sysc sysa sysl

%.o:%.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $@ $<

$(CORE): $(OBJECTS)
	$(CC) -o $@ $^    

sysp: sysp.sh $(CORE)
	cp $< $@
	chmod +x $@

sysc: sysc.sh $(CORE)
	cp $< $@
	chmod +x $@

sysa: sysa.sh $(CORE)
	cp $< $@
	chmod +x $@

sysl: sysl.sh $(CORE)
	cp $< $@
	chmod +x $@

clean-local:
	rm -f $(CORE) sysp sysc sysa sysl $(OBJECTS)
