# Macro Docs

* [Setup Macro](setup.md)
* [Arguments](arguments.md)

## Testing

You can test C API, with tests written in C with libcheck, to run them:

1. Build library with tests

```shell script
cmake -DMACRO_BUILD_TESTS=ON .
```

2. Run make

```shell script
make
```

3. Run output which was built

```shell script
./tests/macro_test
```

Or you can test Python API, read [this article](/bindings/python/readme.md)
