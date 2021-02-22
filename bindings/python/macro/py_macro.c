#include <Python.h>
#include "../../../macro.c"

/* Pattern Type
*/

typedef struct {
    PyObject_HEAD
    MacroPattern* inner;
} PyMP;

static PyTypeObject PyMPType = {
    PyVarObject_HEAD_INIT(NULL, 0)
            .tp_name = "MacroPattern",
    .tp_doc = "MacroPattern",
    .tp_basicsize = sizeof(PyMP),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

PyMP* make_py_macro_pattern(MacroPattern macro_pattern) {
    PyMP *obj = PyObject_New(PyMP, &PyMPType);
    obj->inner = (malloc(sizeof(MacroPattern)));
    obj->inner->pattern = strdup(macro_pattern.pattern);
    obj->inner->data = macro_pattern.data;
    obj->inner->length = macro_pattern.length;
    return obj;
}


static PyObject* cmacro_match(PyObject *Py_UNUSED(self), PyObject *args) {
    const char* s_pattern;
    const char* s_real;
    int to_json;

    if (!PyArg_ParseTuple(args, "ssp", &s_pattern, &s_real, &to_json))
        return NULL;

    MacroContainer container = macro_container();
    MacroPattern c_pattern = macro_compile(s_pattern);
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
    if (!PyArg_ParseTuple(args, "s", &pattern)) {
        PyErr_SetString(PyExc_TypeError, "(pattern: str) must be passed");
        return NULL;
    }
    MacroPattern compiled_pattern = macro_compile(pattern);
    PyMP* t_pattern = make_py_macro_pattern(compiled_pattern);
    return (PyObject*)t_pattern;
}

static PyObject* cmacro_parse(PyObject *Py_UNUSED(self), PyObject *args) {
    const char *real;
    PyMP *py_mp;
    int to_json;

    if (!PyArg_ParseTuple(args, "sO!p", &real, &PyMPType, &py_mp, &to_json))
        return NULL;

    MacroPattern* pattern = (MacroPattern*)py_mp->inner;
    MacroContainer container = macro_container();

    int result = macro_parse(*pattern, real, &container);
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
    PyObject *m;

    if (PyType_Ready(&PyMPType) < 0)
        return NULL;

    m = PyModule_Create(&cmacro);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyMPType);
    if (PyModule_AddObject(m, "MacroPattern", (PyObject *) &PyMPType) < 0) {
        Py_DECREF(&PyMPType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
