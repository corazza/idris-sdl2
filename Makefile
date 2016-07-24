.PHONY: install
install:
	idris --install sdl2.ipkg

.PHONY: clean
clean:
	idris --clean sdl2.ipkg
