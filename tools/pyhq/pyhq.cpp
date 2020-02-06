#include <Python.h>

static PyObject* helloworld(PyObject* self) {
   return Py_BuildValue("s", "Hello, Python extensions!!");
}

static char helloworld_docs[] =
   "helloworld( ): Any message you want to put here!!\n";

static PyMethodDef helloworld_funcs[] = {
   {"helloworld", (PyCFunction)helloworld, 
      METH_NOARGS, helloworld_docs},
      {NULL}
};

/*
void inithelloworld(void) {
   Py_InitModule3("helloword", helloworld_funcs,
                  "Extension module example!");
}
*/
static struct PyModuleDef PYHq =
{
    PyModuleDef_HEAD_INIT,
    "Test", /* name of module */
    "usage: Combinations.uniqueCombinations(lstSortableItems, comboSize)\n", /* module documentation, may be NULL */
    -1,   /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    helloworld_funcs
};

PyMODINIT_FUNC PyInit_pyhq(void)
{
    return PyModule_Create(&PYHq);
}
