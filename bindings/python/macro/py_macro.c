#include <Python.h>
#include "../../../macro.c"

static PyObject* macro_match(PyObject *Py_UNUSED(self), PyObject *args) {
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

static PyMethodDef Methods[] = {
        {"macro_match", &macro_match, METH_VARARGS, "Easy macro matching, string pattern, string real"},
        { NULL, NULL, 0, NULL }
};

static struct PyModuleDef cmacro =
        {
                PyModuleDef_HEAD_INIT,
                "c_extension",
                "test!",
                -1,
                Methods
        };

PyMODINIT_FUNC PyInit_cmacro(void)
{
    return PyModule_Create(&cmacro);
}
