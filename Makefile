# Top-level Makefile: delegates the actual build to src/ (recursive make)

.PHONY: all clean install uninstall

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

install: all
	$(MAKE) -C src install

uninstall:
	$(MAKE) -C src uninstall
