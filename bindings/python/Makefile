.PHONY: help python
.ONESHELL:
.DEFAULT: help

help:
	@echo "make install"
	@echo "	   install python binding"
	@echo "make tests"
	@echo "	   run tests and linters"

install:
	@echo;
	@echo "Building macro";
	@echo "=================";
	@python setup.py install
	@echo;

uninstall:
	@echo;
	@echo "Uninstalling macro";
	@echo "=================";
	@sh uninstall.sh;
	@echo;

tests:
	@echo;
	@echo "Running tests";
	@echo "=================";
	@python -m pip install pytest
	@pytest ../../
