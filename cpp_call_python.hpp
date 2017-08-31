#pragma once

#include <Python.h>
#include <vector>

template<typename T1, typename T2>
class PythonCaller{
public:
    PythonCaller(std::string file_path, std::string file_name, std::string function_name):
        file_path(file_path),
        file_name(file_name),
        function_name(function_name)
    {
        Py_Initialize();
        if ( !Py_IsInitialized() ){
            std::cout<<"Initialize false\n";
            initialize_status = false;
            return;
        }

		//http://stackoverflow.com/questions/41323576/attributeerror-due-to-sys-argv0-for-import-tensorflow-as-tf-inside-c
		//PyRun_SimpleString("import sys\nsys.argv = ['']");
        //PyRun_SimpleString("import tensorflow as tf");

        PyRun_SimpleString("import sys");
        std::string command = "sys.path.append('" + file_path + "')";
        PyRun_SimpleString(command.c_str());
        pyName = PyString_FromString(file_name.c_str());
        pyModule = PyImport_Import(pyName);
		if (!pyModule){
            std::cout<<"Can not open file: "<<file_name<<std::endl;
            initialize_status = false;
			return;
		}

		pyDict = PyModule_GetDict(pyModule);
		if (!pyDict) {
            std::cout<<"Get dictory error\n";
            initialize_status = false;
			return;
		}

		pyFunction = PyDict_GetItemString(pyDict, function_name.c_str());
		if (!pyFunction || !PyCallable_Check(pyFunction)) {
            std::cout<<"can't find function: "<<function_name<<std::endl;
            initialize_status = false;
			return;
		}
        initialize_status = true;
    }

    ~PythonCaller(){
        Py_CLEAR(pyName);
        Py_CLEAR(pyModule);
        Py_CLEAR(pyDict);
        Py_CLEAR(pyFunction);
        Py_Finalize();
    }
    
    std::vector<T2> call(const std::vector<T1> &inPut){
        std::vector<T2> outPut;
        if (!initialize_status)
            return outPut;

        PyObject* pyInPut = to_pyList(inPut);
        PyObject* pyArgs = PyTuple_New(1);
        PyTuple_SetItem(pyArgs, 0, pyInPut);
        PyObject* pyResult = PyObject_CallObject(pyFunction, pyArgs);
        outPut = to_vector(pyResult);

        Py_XDECREF(pyInPut);
        Py_XDECREF(pyResult);
        Py_XDECREF(pyArgs);
        return  outPut;
    }

    PyObject* to_pyList(const std::vector<T1> &inPut){
        int size = inPut.size();
        PyObject* pyList = PyList_New(size);
        for (int i = 0; i < size; ++i){
            PyList_SetItem(pyList, i, Py_BuildValue("d", inPut[i]));
        }
        return pyList;
    }
    
    std::vector<T2> to_vector(PyObject* pyList){
        int size = PyList_Size(pyList);
        std::vector<T2> outPut;
        for (int i = 0; i < size; ++i){
            PyObject* pyItem = PyList_GET_ITEM(pyList, i);
            double item;
            PyArg_Parse(pyItem, "d", &item);
            outPut.push_back(item);
        }
        return outPut;
    }

private:
    const std::string file_path;
    const std::string file_name;
    const std::string function_name;

    bool initialize_status;
	PyObject* pyName;
	PyObject* pyModule;
	PyObject* pyDict;
	PyObject* pyFunction;
};

