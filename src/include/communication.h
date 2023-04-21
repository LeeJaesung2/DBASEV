#include "visibility.h"
#include <fcntl.h>
#include <termios.h>
#include <DBASEV/telemetry/mavlink.h>
#include <DBASEV/telemetry/mavlink_helpers.h>

void *thread_func2(void *arg);
void *sender(void *arg);
void *receiver(void *arg);
