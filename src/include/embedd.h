#include <stdarg.h>
#include "visibility.h"

int callPython(const char *src, const char *func, int arg, ...);
int callPythonStruct(const char *src,const char *func, const char * msg, carData car_data, droneData drone_data );