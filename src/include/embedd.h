#include <stdarg.h>
#include "visibility.h"

int embedd(const char *src, const char *func, int arg, ...);
int embedd_msg(const char *src, const char *func, message msg, carData car_data, droneData drone_data);