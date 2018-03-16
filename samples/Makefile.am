SUBDIRS=hello

run: all
	make -C $(SAMPLE) run

clean-local:
	rmdir --ignore-fail-on-non-empty $(SUBDIRS)

