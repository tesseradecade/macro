# macro

Parser based on C

```c
#include "macro.h"

int main() {
    struct MacroContainer container = macro_container();
    struct MacroPattern pattern = macro_compile("hello, <name>! do <action>.");
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
