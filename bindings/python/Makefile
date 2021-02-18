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

tests:
	@python -m pip install pytest
	@pytest ../../
