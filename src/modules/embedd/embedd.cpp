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

int callPythonStruct(const char *src,const char *func, const char * msg, carData car_data, droneData drone_data){
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

            pArgs = PyTuple_New(3);
            pValue = Py_BuildValue("s", msg);
            PyTuple_SetItem(pArgs, 0, pValue);
            pValue = Py_BuildValue("iif", car_data.road_id, car_data.waypoint_id, car_data.velocity);
            PyTuple_SetItem(pArgs, 1, pValue);

            PyObject* py_drone_data = PyDict_New();
            PyDict_SetItemString(py_drone_data, "road_id", PyLong_FromLong(drone_data.road_id));
            PyDict_SetItemString(py_drone_data, "waypoint_id", PyLong_FromLong(drone_data.waypoint_id));
            PyDict_SetItemString(py_drone_data, "velocity", PyFloat_FromDouble(drone_data.velocity));
            int listsize = sizeof(drone_data.will_go_waypoint)/sizeof(waypoint);
            PyObject* py_waypoint_list = PyList_New(listsize);
            for (int i = 0; i < listsize; i++) {
                waypoint wp = drone_data.will_go_waypoint[i];
                PyObject* py_waypoint = PyDict_New();
                PyDict_SetItemString(py_waypoint, "id", PyLong_FromLong(wp.id));
                PyDict_SetItemString(py_waypoint, "latitude", PyFloat_FromDouble(wp.latitude));
                PyDict_SetItemString(py_waypoint, "longitude", PyFloat_FromDouble(wp.longitude));
                PyDict_SetItemString(py_waypoint, "altitude", PyFloat_FromDouble(wp.altitude));
                PyDict_SetItemString(py_waypoint, "countable", PyLong_FromLong(wp.countable));
                PyDict_SetItemString(py_waypoint, "last_point", PyLong_FromLong(wp.last_point));
                PyList_SetItem(py_waypoint_list, i, py_waypoint);
            }
            PyDict_SetItemString(py_drone_data, "will_go_waypoint", py_waypoint_list);
            PyTuple_SetItem(pArgs, 2, py_drone_data);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            

            /*value checking*/
            if (pValue != NULL) {
                pDict = PyDict_Copy(pValue);
                Py_DECREF(pValue);

                // Extract values from dictionary
                PyObject *pRoadID = PyDict_GetItemString(pDict, "road_id");
                car_data.road_id = PyLong_AsLong(pRoadID);

                PyObject *pWaypointID = PyDict_GetItemString(pDict, "waypoint_id");
                car_data.waypoint_id = PyLong_AsLong(pWaypointID);

                PyObject *pVelocity = PyDict_GetItemString(pDict, "velocity");
                car_data.velocity = PyFloat_AsDouble(pVelocity);

                printf("Road ID: %d, Waypoint ID: %d, Velocity: %.2f\n", car_data.road_id, car_data.waypoint_id, car_data.velocity);
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