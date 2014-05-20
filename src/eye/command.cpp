#include "command.h"

Command::Command() {
    data[0] = 0;
}

Command::Command(char data) {
    this->data[0] = data;
}

void Command::setServoX(CommandState state) {
    data[0] = (data[0] & 0x3f) | (((char)state & 3) << 6);
}

void Command::setServoY(CommandState state) {
    data[0] = (data[0] & 0xcf) | (((char)state & 3) << 4);
}

void Command::setLaser(CommandState state) {
    data[0] = (data[0] & 0xf7) | (((char)state & 1) << 3);
}

const char* Command::getValue(void) {
    return data;
}

int Command::getValueSize(void) {
    return ((int)(sizeof(data)/sizeof(char)));
}

void Command::reset(void) {
    data[0] = 0;
}

