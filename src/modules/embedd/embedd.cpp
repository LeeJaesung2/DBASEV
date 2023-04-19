#include <DBASEV/embedd.h>
#include <python2.7/Python.h>

int callPython(const char *src,const char *func, int arg, ...){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
    int i;

    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"./python_src\")"); //set python src path
    pName = PyString_FromString(src); //get python src

    /*Error checking of pName*/
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func);

        if (pFunc && PyCallable_Check(pFunc)) {

            va_list ap;
            va_start(ap,arg);
            pArgs = PyTuple_New(arg);
            for (i = 0; i < arg; ++i) {
                pValue = PyInt_FromLong(va_arg(ap,int));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs); // call function with argument
            Py_DECREF(pArgs);
            /*value checking*/
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }
            /*No return value*/
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
            va_end(ap);
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", src);
        return 1;
    }
    Py_Finalize();
    return PyInt_AsLong(pValue);
    
}

int callPythonStruct(const char *src,const char *func, const char * msg, carData car_data, droneData drone_data ){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue, *pDict;

    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append(\"./python_src\")"); //set python src path
    pName = PyString_FromString(src); //get python src

    /*Error checking of pName*/
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
        pFunc = PyObject_GetAttrString(pModule, func);
        if (pFunc && PyCallable_Check(pFunc)) {

            pArgs = PyTuple_New(2);
            pValue = Py_BuildValue("s", msg);
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = Py_BuildValue("iif", car_data.road_id, car_data.waypoint_id, car_data.velocity);
            PyTuple_SetItem(pArgs, 1, pValue);
            // pValue = Py_BuildValue("iiff[100]", drone_data.road_id, drone_data.waypoint_id, drone_data.velocity, drone_data.will_go_waypoint);
            // PyTuple_SetItem(pArgs, 2, pValue);
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            

            /*value checking*/
            if (pValue != NULL) {
                pDict = PyDict_Copy(pValue);
                Py_DECREF(pValue);

                // Extract values from dictionary
                PyObject *pRoadID = PyDict_GetItemString(pDict, "road_id");
                int road_id = PyLong_AsLong(pRoadID);

                PyObject *pWaypointID = PyDict_GetItemString(pDict, "waypoint_id");
                int waypoint_id = PyLong_AsLong(pWaypointID);

                PyObject *pVelocity = PyDict_GetItemString(pDict, "velocity");
                float velocity = PyFloat_AsDouble(pVelocity);

                printf("Road ID: %d, Waypoint ID: %d, Velocity: %.2f\n", road_id, waypoint_id, velocity);
                Py_DECREF(pDict);
            }
            /*No return value*/
            else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }
        else {
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", func);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    else {
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", src);
        return 1;
    }
    Py_Finalize();
    return PyInt_AsLong(pValue);
    
}