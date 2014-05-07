#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>

#include "command.h"
#include "comm_udp.h"

//CommunicationUDP::CommunicationUDP(std::string hostname, std::string port) : udp_response_thread([this]() {this->getResponse();}) {
CommunicationUDP::CommunicationUDP(std::string hostname, std::string port) : udp_response_thread([](){}) {
    this->hostname = hostname;
    this->port = port;
    is_thread_running = false;
}

CommunicationUDP::~CommunicationUDP() {
    std::cout << "Deconstructing connection!" << std::endl;
}

void CommunicationUDP::connect(void) {
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if ((rv = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((socketfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("CommunicationUDP: socket");
            continue;
        }
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "CommunicationUDP: failed to bind socket\n");
        return;
    }
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 200;
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

}

void CommunicationUDP::sendCommand(Command command) {
    if (!is_thread_running) {
        int numbytes;
        if ((numbytes = sendto(socketfd, command.getValue(), command.getValueSize(), 0,
            p->ai_addr, p->ai_addrlen)) == -1) {
            perror("CommunicationUDP::sendCommand");
            exit(1);
        }
        is_thread_running = true;
        udp_response_thread.join();
        udp_response_thread = std::thread([this]() {this->getResponse();});
        //udp_response_thread.detach();
    } else {
        //std::cout << "Not running thread!" << std::endl;
    }
}

void CommunicationUDP::getResponse(void) {
    std::cout << "Running thread!" << std::endl;
    int numbytes;
    char kento[1];
    struct sockaddr addr;
    struct sockaddr_in *their_addr;
    socklen_t fromlen;
    fromlen = sizeof &addr;
    if (numbytes = recvfrom(socketfd, kento, sizeof kento, 0, &addr, &fromlen) != -1) {
        their_addr = ((struct sockaddr_in*)(&addr));
        char *ip = inet_ntoa(their_addr->sin_addr);
        std::cout << "ip: " <<  ip << std::endl;
        printf("Thing: %s\n", addr.sa_data);
        printf("Received bytes: %d\n", numbytes);
        for(int i = 0; i < numbytes; ++i) {
            printf("%d", kento[i]);
        }
        printf("\n");

    } else {
        printf("Received no data from server!");
    }
    is_thread_running = false;
}

void CommunicationUDP::disconnect(void) {
    if (udp_response_thread.joinable()) {
        std::cout << "Joining thread" << std::endl;
        udp_response_thread.join();
    }
    freeaddrinfo(servinfo);
    servinfo = 0;
    close(socketfd);
}
