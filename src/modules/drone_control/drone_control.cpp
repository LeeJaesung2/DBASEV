#include <DBASEV/drone_control.h>
#include <DBASEV/embedd.h>
void * flight_control(void *arg)
{   

    const char* src = "main"; //source code file
    const char* func = "multiply"; //method for excute
    
    //func = "embedd";
    callPython(src, func, 1, 1);   
    
    return 0;
}