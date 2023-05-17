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

// carAndDroneData callPythonStruct(const char *src,const char *func, const char * msg, carData car_data, droneData drone_data){
//     PyObject *pName, *pModule, *pFunc;
//     PyObject *pArgs, *pValue;
//     carAndDroneData reval = {car_data, drone_data, 0};
//     //printf("Before Change Car value Road ID: %d, Waypoint ID: %d, Velocity: %.2f\n", reval.car.road_id, reval.car.waypoint_id, reval.car.velocity);
//     //printf("Before Change Drone value Road ID: %d, Waypoint ID: %d, Velocity: %.2f, Latitude : %.2f, Longitude : %.2f, Altitude : %.2f, Countable: %d, last_point: %d\n", reval.drone.road_id, reval.drone.waypoint_id, reval.drone.velocity, reval.drone.will_go_waypoint[0].latitude, reval.drone.will_go_waypoint[0].longitude, reval.drone.will_go_waypoint[0].altitude, reval.drone.will_go_waypoint[0].countable, reval.drone.will_go_waypoint[0].last_point);       
//     Py_Initialize();
//     PyRun_SimpleString("import sys");
//     PyRun_SimpleString("sys.path.append(\"./python_src\")"); //set python src path
//     pName = PyString_FromString(src); //get python src

//     /*Error checking of pName*/
//     pModule = PyImport_Import(pName);
//     Py_DECREF(pName);

//     if (pModule != NULL) {
//         pFunc = PyObject_GetAttrString(pModule, func);
//         if (pFunc && PyCallable_Check(pFunc)) {

//             //set argument to sent python
//             pArgs = PyTuple_New(3);

//             /*set msg as python value*/
//             pValue = Py_BuildValue("s", msg);
//             PyTuple_SetItem(pArgs, 0, pValue);
            
//             /*set car data as dictionary*/
//             PyObject* py_car_data = PyDict_New();
//             PyDict_SetItemString(py_car_data, "road_id", PyInt_FromLong(car_data.road_id));
//             PyDict_SetItemString(py_car_data, "waypoint_id", PyInt_FromLong(car_data.waypoint_id));
//             PyDict_SetItemString(py_car_data, "velocity", PyFloat_FromDouble(car_data.velocity));
//             PyTuple_SetItem(pArgs, 1, py_car_data);

//             /*set drone data as dictionary*/
//             PyObject* py_drone_data = PyDict_New();
//             PyDict_SetItemString(py_drone_data, "road_id", PyInt_FromLong(drone_data.road_id));
//             PyDict_SetItemString(py_drone_data, "waypoint_id", PyInt_FromLong(drone_data.waypoint_id));
//             PyDict_SetItemString(py_drone_data, "velocity", PyFloat_FromDouble(drone_data.velocity));
//             int listsize = sizeof(drone_data.will_go_waypoint)/sizeof(waypoint);
//             PyObject* py_waypoint_list = PyList_New(listsize);
//             for (int i = 0; i < listsize; i++) {
//                 waypoint wp = drone_data.will_go_waypoint[i];
//                 PyObject* py_waypoint = PyDict_New();
//                 PyDict_SetItemString(py_waypoint, "id", PyInt_FromLong(wp.id));
//                 PyDict_SetItemString(py_waypoint, "latitude", PyFloat_FromDouble(wp.latitude));
//                 PyDict_SetItemString(py_waypoint, "longitude", PyFloat_FromDouble(wp.longitude));
//                 PyDict_SetItemString(py_waypoint, "altitude", PyFloat_FromDouble(wp.altitude));
//                 PyDict_SetItemString(py_waypoint, "countable", PyInt_FromLong(wp.countable));
//                 PyDict_SetItemString(py_waypoint, "last_point", PyInt_FromLong(wp.last_point));
//                 PyList_SetItem(py_waypoint_list, i, py_waypoint);
//             }
//             PyDict_SetItemString(py_drone_data, "will_go_waypoint", py_waypoint_list);
//             PyTuple_SetItem(pArgs, 2, py_drone_data);

//             pValue = PyObject_CallObject(pFunc, pArgs);
//             Py_DECREF(pArgs);
            

//             /*value checking*/
//             //get return value from pyton
//             if (pValue != NULL) {
//                 PyObject *pDict = PyDict_Copy(pValue);
//                 Py_DECREF(pValue);

//                 // Extract values from dictionary
//                 PyObject *pCarRoadID = PyDict_GetItemString(PyDict_GetItemString(pDict, "car"), "road_id");
//                 car_data.road_id = PyLong_AsLong(pCarRoadID);
//                 PyObject *pCarWaypointID = PyDict_GetItemString(PyDict_GetItemString(pDict, "car"), "waypoint_id");
//                 car_data.waypoint_id = PyLong_AsLong(pCarWaypointID);
//                 PyObject *pCarVelocity = PyDict_GetItemString(PyDict_GetItemString(pDict, "car"), "velocity");
//                 car_data.velocity = PyFloat_AsDouble(pCarVelocity);
                
//                 PyObject *pDroneRoadID = PyDict_GetItemString(PyDict_GetItemString(pDict, "done"), "road_id");
//                 drone_data.road_id = PyLong_AsLong(pDroneRoadID);
//                 PyObject *pDroneWaypointID = PyDict_GetItemString(PyDict_GetItemString(pDict, "done"), "waypoint_id");
//                 drone_data.waypoint_id = PyLong_AsLong(pDroneWaypointID);
//                 PyObject *pDroneVelocity = PyDict_GetItemString(PyDict_GetItemString(pDict, "done"), "velocity");
//                 drone_data.velocity = PyFloat_AsDouble(pDroneVelocity);
//                 PyObject* pDroneWillGoWaypointList = PyDict_GetItemString(PyDict_GetItemString(pDict, "done"), "will_go_waypoint");
//                 // Loop through the list and extract each waypoint's data
//                 for (int i = 0; i < PyList_Size(pDroneWillGoWaypointList); i++) {
//                     PyObject* pWaypoint = PyList_GetItem(pDroneWillGoWaypointList, i);
//                     PyObject* pId = PyDict_GetItemString(pWaypoint, "id");
//                     drone_data.will_go_waypoint[i].id = PyLong_AsLong(pId);
//                     PyObject* pLatitude = PyDict_GetItemString(pWaypoint, "latitude");
//                     drone_data.will_go_waypoint[i].latitude = PyFloat_AsDouble(pLatitude);
//                     PyObject* pLongitude = PyDict_GetItemString(pWaypoint, "longitude");
//                     drone_data.will_go_waypoint[i].longitude = PyFloat_AsDouble(pLongitude);
//                     PyObject* pAltitude = PyDict_GetItemString(pWaypoint, "altitude");
//                     drone_data.will_go_waypoint[i].altitude = PyFloat_AsDouble(pAltitude);
//                     PyObject* pCountable = PyDict_GetItemString(pWaypoint, "countable");
//                     drone_data.will_go_waypoint[i].countable = PyLong_AsLong(pCountable);
//                     PyObject* pLast_point = PyDict_GetItemString(pWaypoint, "last_point");
//                     drone_data.will_go_waypoint[i].last_point = PyLong_AsLong(pLast_point);
//                 }

//                 //save all of data at return value
//                 reval.car = car_data;
//                 reval.drone = drone_data;
//                 //printf("After Change Car value Road ID: %d, Waypoint ID: %d, Velocity: %.2f\n", reval.car.road_id, reval.car.waypoint_id, reval.car.velocity);
//                 //printf("After Change Drone value Road ID: %d, Waypoint ID: %d, Velocity: %.2f, Latitude : %.2f, Longitude : %.2f, Altitude : %.2f, Countable: %d, last_point: %d\n", reval.drone.road_id, reval.drone.waypoint_id, reval.drone.velocity, reval.drone.will_go_waypoint[0].latitude, reval.drone.will_go_waypoint[0].longitude, reval.drone.will_go_waypoint[0].altitude, reval.drone.will_go_waypoint[0].countable, reval.drone.will_go_waypoint[0].last_point);
//                 Py_DECREF(pDict);
                
//             }
//             /*No return value*/
//             else {
//                 Py_DECREF(pFunc);
//                 Py_DECREF(pModule);
//                 PyErr_Print();
//                 fprintf(stderr,"Call failed\n");
//                 reval.error = 1;
//                 return reval;
//             }
//         }
//         else {
//             if (PyErr_Occurred())
//                 PyErr_Print();
//             fprintf(stderr, "Cannot find function \"%s\"\n", func);
//             reval.error = 1;
//             return reval;
//         }
//         Py_XDECREF(pFunc);
//         Py_DECREF(pModule);
//     }
//     else {
//         PyErr_Print();
//         fprintf(stderr, "Failed to load \"%s\"\n", src);
//         reval.error = 1;
//         return reval;
//     }
//     Py_Finalize();
    
//     return reval;
// }