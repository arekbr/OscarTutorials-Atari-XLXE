OSCAR64 ?= $(or $(wildcard build/toolchain/bin/oscar64),oscar64)
OSCARFLAGS ?= -O2
PYTHON ?= python3

.PHONY: all clean list
all:
	$(PYTHON) tools/build.py --compiler "$(OSCAR64)" --flags="$(OSCARFLAGS)"

list:
	$(PYTHON) tools/build.py --list

clean:
	$(PYTHON) tools/build.py --clean
