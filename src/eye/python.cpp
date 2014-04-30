#include "python.h"
#include <Python.h>

python::python()
{
	Py_SetProgramName(L"Eye");
	Py_Initialize();
	run(init);
}

void python::run(std::string code)
{
	PyRun_SimpleString(code.c_str());
}

python::~python()
{
	run(deInit);
	Py_Finalize();
}
