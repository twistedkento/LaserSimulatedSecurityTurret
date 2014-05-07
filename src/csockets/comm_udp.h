#ifndef __COMM_UDP_H__
#define __COMM_UDP_H__

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include "command.h"

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
        void connect(void);
        void sendCommand(Command);
        void getResponse(void);
        void disconnect(void);
};

#endif
