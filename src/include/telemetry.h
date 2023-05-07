#include "visibility.h"
#include <fcntl.h>
#include <termios.h>
#include <DBASEV/telemetry/mavlink.h>
#include <DBASEV/telemetry/mavlink_helpers.h>

int serialSetting(int serial_port);

int sender_serialSetting(int serial_port);

int receiver_serialSetting(int serial_port);

bool receiveMessage(int fd, mavlink_message_t& msg, mavlink_status_t& status, mavlink_channel_t channel, std::string& message);