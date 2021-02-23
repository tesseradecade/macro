# Macro CPython API

## Install

```shell script
git clone https://github.com/tesseradecade/macro macro
cd macro/bindings/python
python setup.py install
```

Or use makefile

```shell script
git clone https://github.com/tesseradecade/macro macro
cd macro/bindings/python
make install
```

## Usage

Simple `match` compiles `pattern` and completes a match with `real` string, not appropriate for production, use this feature to test smt, also `as_json` is experimental feature turning macro's `MacroContainer` into json data (when `True` returns string)

```python
from macro import match

match("hello <name> lets <action>", "hello dear lets sleep")
# dict({"name": "dear", "action": "sleep"})
```

`Pattern` is basic, use this API in major demands

```python
from macro import Pattern

pattern = Pattern("hi my password is <password>, is it ok?")

pattern.check("hi my password is qwerty123, is it ok?")
# {"password": "qwerty123"}

pattern.check("i'm not gonna tell you my password!")
# None
```

## Makefile

To uninstall macro use `make uninstall`, to run tests use `make tests`