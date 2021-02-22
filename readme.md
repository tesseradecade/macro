# macro

Parser based on C

```c
#include "macro.h"

int main() {
    MacroContainer container = macro_container();
    MacroPattern pattern = macro_compile("hello, <name>! do <action>.");
    int response = macro_parse(pattern, "hello, macro! do parse.", &container);
    
    if (response == 0) printf("Invalid sample for pattern!\n");
    else {
        char* json = malloc(128); 
        macro_container_to_json(container, &json); 
        printf("%s\n", json);
    }
    // STDOUT: {"name":"macro","action":"parse"}
}
```

[Documentation](/docs/index.md)

## Bindings

Currently available: [Python binding](/bindings/python)  
Planned: Prolog, Rust bindings

## Benchmarks

Due to active projects development benchmarks can change each commit, you can build them yourself with [benchmarks builder](benchmarks/benchmarks_builder.py)

## How to help

Contributions are welcome! If you want to help the head maintainer, you may be interested to sponsor [timoniq](https://github.com/timoniq), use "Sponsor" button