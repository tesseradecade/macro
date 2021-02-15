#ifndef MACRO_MACRO_H
#define MACRO_MACRO_H
#define string char*

struct MacroPattern;

struct MacroContainer;

struct MacroContainer macro_container();

struct MacroPattern macro_compile(const string pattern);

int macro_parse(struct MacroPattern compiled_pattern, const string real_const, struct MacroContainer *container);

void macro_container_to_json(struct MacroContainer container, string* result);

#endif //MACRO_MACRO_H
