#include "macro.h"

#define string char*
#define bool int
#define true 1
#define false 0

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Arg {
    string name;
    string before;
    bool after;
    string after_ctx;
};

struct MacroPattern {
    struct Arg* data;
    int length;
};

struct Match {
    string s1;
    string s2;
};

struct Split {
    string* results;
    int length;
};

struct MacroContainer {
    string* keys;
    string* values;
    int length;
};


string string_slice(const string s, int from, int to) {
    string substring = malloc(to-from);
    strncpy(substring, s+from, to-from);
    substring[to-from] = '\0';
    return substring;
}

struct MacroContainer macro_container() {
    string* keys = (string*)malloc(0);
    string* values = (string*)malloc(0);
    struct MacroContainer c = {keys, values, 0};
    return c;
}

void macro_container_push(struct MacroContainer *container, string key, string value) {
    container->keys = realloc(container->keys, sizeof(string) * (container->length + 1));
    container->values = realloc(container->values, sizeof(string) * (container->length + 1));
    container->keys[container->length] = key;
    container->values[container->length] = value;
    container->length++;
}

struct Split string_split(string s, string d, bool one_match) {

    int offset = (int)strlen(d) - 1;
    int last_index = 0;

    string* results = (string*)malloc(0);
    int results_length = 0;

    for (int i = offset; i < strlen(s); i++) {
        bool match = true;
        for (int j = 0; j < strlen(d); j++)
            if (d[j] != s[i-(offset-j)]) {
                match = false;
                break;
            }

        if (match == true || i == strlen(s) - 1) {

            if (i == strlen(s) - 1) offset = -1;

            string element = malloc(i-offset-last_index);
            strncpy(element, s+last_index, i-last_index-offset);
            element[i-last_index-offset] = '\0';

            results = realloc(results, (sizeof(string) * (results_length + 1)));
            results[results_length] = element;

            results_length++;

            if (one_match == true) {
                string rest_s = malloc(strlen(s) - i);
                strncpy(rest_s, s+i+1, strlen(s) - i);
                rest_s[strlen(s) - i] = '\0';

                results = realloc(results, (sizeof(string) * (results_length + 1)));
                results[results_length] = rest_s;
                results_length++;

                break;
            }
            else if (match == true) {
                i = i + offset - 1;
                last_index = i;
            }

        }
    }

    struct Split r =  {results, results_length};
    return r;
}

struct Match match_split(string s, string d) {
    struct Split spl = string_split(s, d, true);

    struct Match m = {"", ""};

    if (spl.length == 2) {
        m.s1 = spl.results[0];
        m.s2 = spl.results[1];
    }
    return m;
}

struct MacroPattern macro_compile(string pattern) {
    bool escape = false;
    int last_index = 0;
    struct Arg* data = malloc(0);
    int length = 0;

    for (int i = 0; i < strlen(pattern); i++) {
        char c = pattern[i];
        if (c == '\\') escape = true;
        else if (c == '<' && escape == false) {
            int j = i;
            for (; j < strlen(pattern) && pattern[j] != '>'; j++);

            string name = string_slice(pattern, i+1, j);
            string before = string_slice(pattern, last_index, i);
            string after_ctx = string_slice(pattern, j, (int)strlen(pattern));
            bool after = true;

            if (j == strlen(pattern) - 1) after = false;
            data = realloc(data, sizeof(struct Arg) * (length + 1));
            struct Arg new_arg = {name, before, after, after_ctx};
            data[length] = new_arg;
            length++;
        }
    }

    struct MacroPattern args = {data, length};
    return args;
}


int macro_parse(const struct MacroPattern compiled_pattern, const string real_const, struct MacroContainer *container) {
    string real = malloc(sizeof(char) * strlen(real_const));
    strcpy(real, real_const);
    real[strlen(real_const)] = '\0';

    for (int i = 0; compiled_pattern.length; i++) {
        struct Arg arg = compiled_pattern.data[i];
        struct Match m = match_split(real, arg.before);

        if (strlen(m.s1) == 0 && strlen(m.s2) == 0) return 0;

        if (i == 0) {}
        else {
            struct Arg last_arg = compiled_pattern.data[i-1];
            macro_container_push(container, last_arg.name, m.s1);
        }

        real = realloc(real, sizeof(char) * strlen(m.s2));
        strcpy(real, m.s2);
        real[strlen(m.s2)] = '\0';

        if (i == compiled_pattern.length - 1) {
            char* value;
            if (arg.after == false) {
                value = m.s2;
            } else {
                value = string_slice(real, 0, (int)(strlen(real) - strlen(arg.after_ctx) + 1));
            }
            macro_container_push(container, arg.name, value);
            break;
        }
    }

    return 1;
}

void macro_container_to_json(struct MacroContainer container, string* result) {
    int length = 2;

    for (int i = 0; i < container.length; i++) {
        length += (int)strlen(container.values[i]) + (int)strlen(container.keys[i]) + 5;
    }

    char json[length];
    strcpy(json, "{");

    for (int i = 0; i < container.length; i++) {
        strcat(json, "\"");
        strcat(json, container.keys[i]);
        strcat(json, "\":\"");
        strcat(json, container.values[i]);
        strcat(json, "\"");
        if (i != container.length - 1)
            strcat(json, ",");
    }

    strcat(json, "}");
    if (result != NULL) strcpy(*result, json);
}