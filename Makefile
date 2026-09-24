# Top-level Makefile: delegates the actual build to src/ (recursive make)

.PHONY: all clean

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
