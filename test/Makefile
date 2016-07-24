.PHONY: all
all: bitmap etest events hello test

.PHONY: bitmap
bitmap:
	idris --build bitmap.ipkg

.PHONY: etest
etest:
	-idris --build etest.ipkg

.PHONY: events
events:
	idris --build events.ipkg

.PHONY: hello
hello:
	idris --build hello.ipkg

.PHONY: test
test:
	idris --build test.ipkg
