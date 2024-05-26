.SILENT:

SHELL := /bin/sh

.PHONY: install
install: check_clang
	@clang --std=c17 -fmodules -fprebuilt-module-path=. cmd.c zlib.c -o zlib

.PHONY: check_clang
check_clang:
	if ! command -v clang &> /dev/null; then \
		echo "clang not installed"; \
	fi;

.PHONY: clean
clean:
	@rm -vf zlib
