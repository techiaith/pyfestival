#include <stdio.h>
#include <festival.h>
#include "festival_private.h"
#include "Python.h"


#if PY_MAJOR_VERSION >= 3
#define PyUnicodeObject23 PyObject
#else
#define PyUnicodeObject23 PyUnicodeObject
#endif

static char setStretchFactor_doc[] = "setStretchFactor(int) -> bool success";
static PyObject* setStretchFactor(PyObject* self, PyObject* args) {
    
    float stretch_factor;
    if (!PyArg_ParseTuple(args, "f:setStretchFactor", &stretch_factor)) return NULL;
    
    char buffer[40];
    sprintf(buffer, "(Parameter.set 'Duration_Stretch %.2f)", stretch_factor);
    bool success = festival_eval_command(buffer);
    if (success) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static char execCommand_doc[] = "execCommand(command) -> bool success\n"
    "e.g. execCommand(\"(Parameter.set 'Duration_Stretch 1.2)\")";
static PyObject* execCommand(PyObject* self, PyObject* args) {
    
    const char* command;
    if (!PyArg_ParseTuple(args, "s:execCommand", &command)) return NULL;
    
    bool success = festival_eval_command(command);
    
    if (success) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static char _textToWav_doc[] = "_textToWav(text) -> unicode filename\n"
    "This is a private method.";
static PyObject* _textToWav(PyObject* self, PyObject* args) {
    const char* text;
    const char* saved_filename;

    EST_String tmpfile;
    int argc = PyTuple_GET_SIZE(args);
    if (argc == 1){
        if (!PyArg_ParseTuple(args, "s:_textToWav", &text)) return NULL;
        tmpfile = make_tmp_filename();
    } else {
        if (!PyArg_ParseTuple(args, "s|s:_textToWav", &text, &saved_filename)) return NULL;
        tmpfile = saved_filename;
    }

    EST_Wave wave;
    if (!festival_text_to_wave(text, wave)) {
        PyErr_SetString(PyExc_SystemError, "Unable to convert text to wave");
        return NULL;
    }

    FILE *fp = fopen(tmpfile, "wb");

    if (wave.save(fp, "riff") != write_ok) {
        fclose(fp);
        remove(tmpfile);
        PyErr_SetString(PyExc_SystemError, "Unable to create wav file");
        return NULL;
    }
    fclose(fp);

    PyObject *filename = PyUnicode_FromStringAndSize((const char *)tmpfile, tmpfile.length());
    return filename;
}

static char info_doc[] = "info() -> None\n"
"Prints out information to stdout on "
"the current festival version";
static PyObject* info(PyObject* self) {
    festival_banner();
    Py_RETURN_NONE;
}

static char _sayText_doc[] = "_sayText(text) -> bool success";
static PyObject* _sayText(PyObject* self, PyObject* args) {
    const char *text;
    if (!PyArg_ParseTuple(args, "s:_sayText", &text)) return NULL;
    bool success = festival_say_text(text);
    if (success) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static char sayFile_doc[] = "sayFile(filename) -> bool success";
static PyObject* sayFile(PyObject* self, PyObject* args) {
    const char *filename;
    if (!PyArg_ParseTuple(args, "s:sayFile", &filename)) return NULL;

    bool success = festival_say_file(filename);
    // The C++ API docs for festival say you should use this to wait for the audio to finish playing
    // but it seems to cause the audio spooler to die
    // festival_wait_for_spooler();
    if (success) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}
///////////////////////////////////////////////////////////////////////
// module stuff

static char module_doc[] = "Festival Python API";
static struct PyMethodDef festival_methods[] = {
    {"_sayText", (PyCFunction) _sayText, METH_VARARGS, _sayText_doc},
    {"_textToWav", (PyCFunction) _textToWav, METH_VARARGS, _textToWav_doc},
    {"execCommand", (PyCFunction) execCommand, METH_VARARGS, execCommand_doc},
    {"setStretchFactor", (PyCFunction) setStretchFactor, METH_VARARGS, setStretchFactor_doc},
    {"sayFile", (PyCFunction) sayFile, METH_VARARGS, sayFile_doc},
    {"info", (PyCFunction) info, METH_NOARGS, info_doc},
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
    // init festival - make the heap size 7MB
    festival_initialize (1, 7*1024*1024);    
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
