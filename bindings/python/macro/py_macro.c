#include <Python.h>
#include "../../../macro.c"

static PyObject* cmacro_match(PyObject *Py_UNUSED(self), PyObject *args) {
    const char* s_pattern;
    const char* s_real;
    int to_json;

    if (!PyArg_ParseTuple(args, "ssp", &s_pattern, &s_real, &to_json))
        return NULL;

    struct MacroContainer container = macro_container();
    struct MacroPattern c_pattern = macro_compile(s_pattern);
    int result = macro_parse(c_pattern, s_real, &container);
    if (result == 0) return Py_BuildValue("s", NULL);

    if (to_json == 1) {
        char *json = malloc(128 * 2);
        macro_container_to_json(container, &json);
        return Py_BuildValue("s", json);
    } else {
        PyObject* groupdict = PyDict_New();

        for (int i = 0; i < container.length; i++) {
            PyDict_SetItem(
                    groupdict,
                    Py_BuildValue("s", container.keys[i]),
                    Py_BuildValue("s", container.values[i])
                    );
        }
        return groupdict;
    }
}

static PyObject* cmacro_pattern(PyObject *Py_UNUSED(self), PyObject *args) {
    const char* pattern;
    if (!PyArg_ParseTuple(args, "s", &pattern))
        return NULL;
    struct MacroPattern compiled_pattern = macro_compile(pattern);
    PyObject* t_args = PyTuple_New(compiled_pattern.length);
    for (int i = 0; i < compiled_pattern.length; i++)
        PyTuple_SET_ITEM(t_args, i, Py_BuildValue(
                "(ssis)",
                compiled_pattern.data[i].name,
                compiled_pattern.data[i].before,
                compiled_pattern.data[i].after,
                compiled_pattern.data[i].after_ctx
                ));

    PyObject* t_pattern = PyTuple_New(3);
    PyTuple_SET_ITEM(t_pattern, 0, Py_BuildValue("s", compiled_pattern.pattern));
    PyTuple_SET_ITEM(t_pattern, 1, t_args);
    PyTuple_SET_ITEM(t_pattern, 2, Py_BuildValue("i", compiled_pattern.length));
    return t_pattern;
}

static PyObject* cmacro_parse(PyObject *Py_UNUSED(self), PyObject *args) {
    const char *real;
    const char *s_pattern;
    PyObject *t_args;
    int length;
    int to_json;

    if (!PyArg_ParseTuple(args, "ssOip", &real, &s_pattern, &t_args, &length, &to_json))
        return NULL;

    PyObject *iter_args = PyObject_GetIter(t_args);
    struct Arg *p_args = malloc(sizeof(struct Arg) * (length));

    int i = 0;
    while (1) {
        PyObject *next = PyIter_Next(iter_args);
        struct Arg arg = {NULL, NULL, 1, NULL};
        if (!next) break;
        if (!PyArg_ParseTuple(next, "ssis", &arg.name, &arg.before, &arg.after, &arg.after_ctx)) {
            return NULL;
        }
        p_args[i] = arg;
        i++;
    }

    struct MacroPattern pattern = {s_pattern, p_args, length};
    struct MacroContainer container = macro_container();

    int result = macro_parse(pattern, real, &container);
    if (result == 0) return Py_BuildValue("s", NULL);

    if (to_json == 1) {
        char *json = malloc(128 * 2);
        macro_container_to_json(container, &json);
        return Py_BuildValue("s", json);
    } else {
        PyObject *groupdict = PyDict_New();

        for (int i = 0; i < container.length; i++) {
            PyDict_SetItem(
                    groupdict,
                    Py_BuildValue("s", container.keys[i]),
                    Py_BuildValue("s", container.values[i])
            );
        }
        return groupdict;
    }
}


static PyMethodDef Methods[] = {
        {"macro_match", &cmacro_match, METH_VARARGS, "Easy macro matching, takes string pattern, string real"},
        {"macro_pattern", &cmacro_pattern, METH_VARARGS, "Makes pattern, takes string pattern"},
        {"macro_parse", &cmacro_parse, METH_VARARGS, "Parses real with compiled pattern options"},
        { NULL, NULL, 0, NULL }
};

static struct PyModuleDef cmacro =
        {
                PyModuleDef_HEAD_INIT,
                "cmacro",
                "test!",
                -1,
                Methods
        };

PyMODINIT_FUNC PyInit_cmacro(void)
{
    return PyModule_Create(&cmacro);
}
