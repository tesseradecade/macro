#include "macro.h"

#define string char*
#define bool int
#define true 1
#define false 0

#include <string.h>
#include <stdlib.h>

#if     defined(_MSC_VER) ||    \
        defined(__WIN32) ||     \
        defined(__WIN32__) ||   \
        defined(_WIN32) ||      \
        defined(WIN32) ||       \
        defined(__WINNT) ||     \
        defined(__WINNT__) ||   \
        defined(__WINNT) ||     \
        defined(__WIN64) ||     \
        defined(_WIN64) ||      \
        defined(WIN64)

#   define OS_IS_WIN            1

#elif   defined(__linux) || defined(__linux__)

#   define OS_IS_UNIX           1
#   define OS_IS_UNIX_LINUX     1

#elif   defined(__APPLE__)

#   define OS_IS_UNIX           1
#   define OS_IS_UNIX_BSD       1
#   define OS_IS_UNIX_MACOS     1

#endif

struct Match {
    string s1;
    string s2;
};

struct Split {
    string* results;
    int length;
};


string string_slice(const string s, int from, int to) {
    string substring = malloc((to-from+1) * sizeof(char));
    strncpy(substring, s+from, to-from);
    substring[to-from] = '\0';
    return substring;
}

MacroContainer macro_container() {
    string* keys = (string*)malloc(0);
    string* values = (string*)malloc(0);
    MacroContainer c = {keys, values, 0};
    return c;
}

void macro_container_clean(MacroContainer* container) {
    container->length = 0;
    container->values = realloc(container->values, 0);
    container->keys = realloc(container->keys, 0);
}

void macro_container_push(MacroContainer *container, string key, string value) {
    container->keys = realloc(container->keys, sizeof(string) * (container->length + 1));
    container->values = realloc(container->values, sizeof(string) * (container->length + 1));
    container->keys[container->length] = key;
    container->values[container->length] = value;
    container->length++;
}

// Cmacro string split algorithm
struct Split string_split(string s, string d, bool one_match) {

    // Starting offset based on delimiter length
    int offset = (int)strlen(d) - 1;
    // Index of last arg to form iter index
    int last_index = 0;

    // Future values
    string* results = (string*)malloc(0);
    int results_length = 0;

    // Iter real string
    for (int i = offset; i < (int)strlen(s); i++) {
        bool match = true;

        // Checking {offset} amount chars of string if they form a delimiter
        // If they dont match is assigned to false
        for (int j = 0; j < (int)strlen(d); j++)
            if (d[j] != s[i-(offset-j)]) {
                match = false;
                break;
            }

        // Delimiter was found or this is the end of the string
        if (match == true || i == (int)strlen(s) - 1) {
            if (i == (int)strlen(s) - 1) offset = -1;

            // Slicing string to get element
            string element = malloc((i-offset-last_index+1)*sizeof(char));
            strncpy(element, s+last_index, i-last_index-offset);
            element[i-last_index-offset] = '\0';

            // Adding element to values
            results = realloc(results, (sizeof(string) * (results_length + 1)));
            results[results_length] = element;
            results_length++;

            // If only one match is requested
            if (one_match == true) {
                // Getting the rest of string consuming this is the last element
                string rest_s = malloc((strlen(s) - i)*sizeof(char));
                strncpy(rest_s, s+i+1, ((int)strlen(s)) - i);
                rest_s[((int)strlen(s)) - i] = '\0';

                // Adding element to values
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

// Shortcut to get basic split for strings in
// parsing algorithm.
struct Match match_split(string s, string d) {
    // Splitting string with one match
    // (always getting two elements)
    struct Split spl = string_split(s, d, true);

    struct Match m = {"", ""};

    // Cases:
    // 0 elements / anything else - returning match with empty strings
    // 2 elements - returning complete match
    if (spl.length == 2) {
        m.s1 = spl.results[0];
        m.s2 = spl.results[1];
    }
    return m;
}

// Macro pattern compiler
MacroPattern macro_compile(const string pattern) {
    // Symbol can be escaped with \ symbol
    bool escape = false;
    
    // Args which might be stored in pattern later
    struct Arg* data = malloc(0);
    int length = 0;
    int last_index = 0;

    for (int i = 0; i < (int)strlen(pattern); i++) {
        char c = pattern[i];

        // Needed to escape symbols like '<'
        if (escape == true) escape = false;
        else if (c == '\\') escape = true;

        else if (c == '<') {
            // If symbol is brace, ending brace will be
            // found and its index moved to j
            int j = i;
            for (; j < ((int)strlen(pattern)) && pattern[j] != '>'; j++);

            // Getting argument info
            string name = string_slice(pattern, i+1, j);
            string before = string_slice(pattern, last_index, i);
            string after_ctx = string_slice(pattern, j, (int)strlen(pattern));
            bool after = true;
            string d;

            // Check if argument marks end of the string
            // then use it to complete matching
            if (j == ((int)strlen(pattern)) - 1) after = false;

            enum ArgType type = ORDINARY;

            // Resolving argument type
            if (strcmp(name, "!") == 0) {
                // IGNORE ARGUMENT
                // Its data will be completely
                // ignored
                type = IGNORE;
                d = "";
            }
            else if (strcmp(name, "1") == 0) {
                // ANYTHING ARGUMENT
                // Its data is ignored though
                // its existence is required
                type = ANYTHING;
                d = "";
            }  else d = "";

            // Add argument to data
            data = realloc(data, sizeof(struct Arg) * (length + 1));
            struct Arg new_arg = {name, before, after, after_ctx, type, d};
            data[length] = new_arg;
            length++;

            // Set last_index to the end of the arg declaration
            last_index = j + 1;
        }
    }

    MacroPattern args = {pattern, data, length};
    return args;
}


int parse_argument(MacroContainer* container, const struct Arg argument, string value) {
    switch (argument.type) {
        case ORDINARY:
            macro_container_push(container, argument.name, value);
            break;
        case ANYTHING:
            if (strlen(value) == 0) return 0;
        default:
            break;
    }
    return 1;
}


int macro_parse(const MacroPattern compiled_pattern, const string real_const, MacroContainer *container) {
    string real = malloc((strlen(real_const) + 1) * sizeof(char));
    strcpy(real, real_const);

    if (compiled_pattern.length == 0) {
        int result = 0;
        if (strcmp(real, compiled_pattern.pattern) == 0) result = 1;
        return result;

    } else if (strlen(real) == 0) return 0;

    for (int i = 0; compiled_pattern.length; i++) {
        struct Arg arg = compiled_pattern.data[i];
        struct Match m = match_split(real, arg.before);

        if ((int)strlen(m.s1) == 0 && strlen(m.s2) == 0) return 0;

        if (i == 0) {
            if (strcmp(m.s1, "") != 0) {
                return 0;
            }
        }

        else {
            struct Arg last_arg = compiled_pattern.data[i-1];
            if (parse_argument(container, last_arg, m.s1) == 0) return 0;
        }

        real = realloc(real, sizeof(char) * ((int)strlen(m.s2) + 1));
        strcpy(real, m.s2);

        if (i == compiled_pattern.length - 1) {
            if (i == 0 && strlen(m.s2) == 0) return 0;
            char* value;
            if (arg.after == false) {
                value = m.s2;
            } else {
                if (strlen(arg.after_ctx) >= strlen(real)) return 0;
                // TODO string slice may be unsafe
                value = string_slice(real, 0, (int)(strlen(real) - strlen(arg.after_ctx) + 1));
            }
            if (parse_argument(container, arg, value) == 0) return 0;
            break;
        }

        free(m.s1);
        free(m.s2);
    }

    free(real);
    return 1;
}

char *str_replace(char *orig, char *rep, char *with) {
    // author: https://stackoverflow.com/a/779960
    char *result;
    char *ins;
    char *tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;

    if (!orig || !rep)
        return NULL;

    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL;

    if (!with)
        with = "";

    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;
    }

    strcpy(tmp, orig);
    return result;
}

void macro_container_to_json(MacroContainer container, string* result) {
    int length = 2;

    for (int i = 0; i < container.length; i++) {
        int esc_count = 0;
        for (int j = 0; j < strlen(container.values[i]); j++)
            if (container.values[i][j] == '\"')
                esc_count++;
        length += (int)strlen(container.values[i]) + (int)strlen(container.keys[i]) + esc_count + 5;
    }

#if OS_IS_UNIX
    char json[length];
#else
    char* json = malloc(sizeof(char) * length);
    if (json == NULL) return NULL;
    json = "";
#endif

    strcpy(json, "{");

    for (int i = 0; i < container.length; i++) {
        strcat(json, "\"");
        strcat(json, container.keys[i]);
        strcat(json, "\":\"");
        strcat(json, str_replace(container.values[i], "\"", "\\\""));
        strcat(json, "\"");
        if (i != container.length - 1)
            strcat(json, ",");
    }

    strcat(json, "}");
    if (result != NULL) strcpy(*result, json);
}

void macro_container_free(MacroContainer *container) {
    for (int i = 0; i < container->length; i++) {
        free(container->keys[i]);
        free(container->values[i]);
    }
}

void macro_pattern_free(MacroPattern *pattern) {
    free(pattern->data);
}