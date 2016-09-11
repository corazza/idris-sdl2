PROGRAMS := bitmap etest events hello test

.PHONY: all
all: $(PROGRAMS)

.PHONY: bitmap
bitmap:
	idris --build bitmap.ipkg

.PHONY: etest
etest:
	@echo etest disabled due to non-termination in type checker.
	@echo idris --build etest.ipkg

.PHONY: events
events:
	idris --build events.ipkg

.PHONY: hello
hello:
	idris --build hello.ipkg

.PHONY: test
test:
	idris --build test.ipkg

.PHONY: clean
clean:
	rm -f *.ibc $(PROGRAMS)
