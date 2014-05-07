#include <stdio.h>
#include <iostream>
#include <cassert>

#include "comm_udp.h"
#include "command.h"

/*#ifndef __clang__
#include <cxxabi.h>
template <class T> const char* TypeName(const T & input) {
    char * Temp = abi::__cxa_demangle(typeid(input).name(), 0, 0, 0);
    std::string result = Temp;
    delete Temp;
    return result.c_str();
}
#endif
#ifdef __clang__
template <class T> char* TypeName(const T & input) {
    return typeid(input).name();
}
#endif*/

int main(int argc, char *argv[]) {
#ifdef NDEBUG
    printf("Yolo is carpe diem for stupid people!\n");
#endif
    Command com = Command();
    com.setServoY(Command::CommandState::SERVO_RESET);
    com.setServoX(Command::CommandState::SERVO_INCREASE);
    com.setLaser(Command::CommandState::LASER_ON);

    struct timespec ts, ts2;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000L;
    CommunicationUDP server("127.0.0.1", "4950");
    server.connect();
    for (int i = 0; i <= 10; ++i) {
        server.sendCommand(com);
        nanosleep(&ts, &ts2);
    }
    server.disconnect();


    return 0;
}
