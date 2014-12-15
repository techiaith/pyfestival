#include <stdio.h>
#include <festival.h>

#include "Python.h"


#if PY_MAJOR_VERSION >= 3
#define PyUnicodeObject23 PyObject
#else
#define PyUnicodeObject23 PyUnicodeObject
#endif

static char _textToWav_doc[] = "textToWav(test) -> wav (bytes)\n"
    "This is a private method.";
static PyObject* _textToWav(PyObject* self, PyObject* args) {
    const char* text;
    if (!PyArg_ParseTuple(args, "s:sayText", &text)) return NULL;
    
    EST_Wave wave;
    festival_text_to_wave("helo world world", wave);
    
    EST_String tmpfile = make_tmp_filename();
    FILE *fp = fopen(tmpfile, "wb");
    if (!wave.save(fp, "nist")) {
        PyErr_SetString(PyExc_SystemError, "Unable to create wav file");
        return NULL;
    }
    fclose(fp);
    
    PyObject *filename = PyUnicode_FromStringAndSize((const char *)tmpfile, tmpfile.length());
    return filename;
}



static char sayText_doc[] = "sayText(text) -> None";
static PyObject* sayText(PyObject* self, PyObject* args) {
    const char *text;
    if (!PyArg_ParseTuple(args, "s:sayText", &text)) return NULL;
    festival_say_text("helo");
    festival_wait_for_spooler();
    Py_INCREF(Py_None);
    return Py_None;
}
///////////////////////////////////////////////////////////////////////
// module stuff

static char module_doc[] = "Festival Python API";
static struct PyMethodDef festival_methods[] = {
    {"sayText", (PyCFunction) sayText, METH_VARARGS, sayText_doc},
    {"_textToWav", (PyCFunction) _textToWav, METH_VARARGS, _textToWav_doc},
    {NULL, NULL} /* sentinel */ };

#ifndef Py_TYPE
    #define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#endif

static char module_name[] = "_festival";

static PyObject *festivalinit(void)
{
    PyObject* module;
#if PY_MAJOR_VERSION >= 3
    static struct PyModuleDef moduledef = {
            PyModuleDef_HEAD_INIT,
            module_name,     /* m_name */
            module_doc,  /* m_doc */
            -1,                  /* m_size */
            festival_methods,    /* m_methods */
            NULL,                /* m_reload */
            NULL,                /* m_traverse */
            NULL,                /* m_clear */
            NULL,                /* m_free */
        };
    module = PyModule_Create(&moduledef);
#else
    module = Py_InitModule3(module_name, festival_methods, module_doc);
#endif
    if (module == NULL) {
        return NULL;
    }
    // init festival
    festival_initialize (1, 210000);    
    festival_eval_command("(set! voice_default voice_cb_cy_llg_diphone)");
    return module;
}

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit__festival(void)
{
    return festivalinit();
}
#else
PyMODINIT_FUNC init_festival(void)
{
    festivalinit();
}
#endif
