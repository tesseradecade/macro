#ifndef MACRO_MACRO_H
#define MACRO_MACRO_H
#define string char*
#define bool int

enum ArgType {
    ORDINARY,
    IGNORE,
    ANYTHING,
};

struct Arg {
    string name;
    string before;
    bool after;
    string after_ctx;
    enum ArgType type;
};

typedef struct {
    const string pattern;
    struct Arg* data;
    int length;
} MacroPattern;

typedef struct {
    string* keys;
    string* values;
    int length;
} MacroContainer;

MacroContainer macro_container();

MacroPattern macro_compile(const string pattern);

int macro_parse(MacroPattern compiled_pattern, const string real_const, MacroContainer *container);

void macro_container_to_json(MacroContainer container, string* result);

#endif //MACRO_MACRO_H
