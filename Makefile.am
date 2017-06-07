SUBDIRS=compiler samples doc misc

run: all
	make -C samples run SAMPLE=$(SAMPLE)

clean-local:
	rmdir --ignore-fail-on-non-empty $(SUBDIRS)
