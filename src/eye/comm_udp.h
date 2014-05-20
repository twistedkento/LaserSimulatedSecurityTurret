#ifndef __COMM_UDP_H__
#define __COMM_UDP_H__

#include <string>
#include <thread>
#include "command.h"

#ifdef _WIN32

#include <windows.h>

#elif __linux

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#endif

class CommunicationUDP {
    private:
        int socketfd;
        std::string hostname;
        std::string port;
        bool is_thread_running;
        std::thread udp_response_thread;
        struct addrinfo *servinfo;
        struct addrinfo *p;
        struct addrinfo hints;
    public:
        CommunicationUDP(std::string hostname, std::string port);
        ~CommunicationUDP();
        void connect_udp(void);
        void sendCommand(Command);
        void getResponse(void);
        void disconnect(void);
};

#endif
