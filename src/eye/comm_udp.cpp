#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <iostream>

#include "command.h"
#include "comm_udp.h"

CommunicationUDP::CommunicationUDP(std::string hostname, std::string port) : udp_response_thread([](){}) {
    this->hostname = hostname;
    this->port = port;
    is_thread_running = false;
}

CommunicationUDP::~CommunicationUDP() {
    std::cout << "Deconstructing connection!" << std::endl;
}

void CommunicationUDP::connect_udp(void) {
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    //hints.ai_protocol = IPPROTO_UDP;
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
    /*struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));*/
    int nonBlocking = 1;
    if ( fcntl( socketfd, 
                F_SETFL, 
                O_NONBLOCK, 
                nonBlocking ) == -1 ) {
        printf( "failed to set non-blocking\n" );
        exit(1);
    }
    connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen);

}

void CommunicationUDP::sendCommand(Command command) {
    //udp_response_thread.join();
    if (!is_thread_running) {
        is_thread_running = true;
        udp_response_thread.join();
        int numbytes;
        /*if ((numbytes = sendto(socketfd, command.getValue(), command.getValueSize(), 0,
            p->ai_addr, p->ai_addrlen)) == -1) { */
        if ((numbytes = send(socketfd,command.getValue(),command.getValueSize(),0)) == -1) {
            perror("CommunicationUDP::sendCommand");
            exit(1);
        }
        udp_response_thread = std::thread([this]() {this->getResponse();});
        //udp_response_thread.detach();
    } else {
        //std::cout << "Not running thread!" << std::endl;
/*        struct timespec ts, ts2;
        ts.tv_sec = 0;
        ts.tv_nsec = 400000000L;
        nanosleep(&ts, &ts2);*/
    }
}

void CommunicationUDP::getResponse(void) {
    //std::cout << "Running thread!" << std::endl;
    time_t start_time,loop_time;
    time(&start_time);
    int numbytes = 0;
    char indata[1];
    //struct sockaddr addr;
    //struct sockaddr_in *their_addr;
    //socklen_t fromlen;
    //fromlen = sizeof &addr;
    while (numbytes == 0) {
        /*if (numbytes = recvfrom(socketfd, indata, sizeof indata, 0, &addr, &fromlen) != -1) {*/
        if (numbytes = recv(socketfd, indata, sizeof indata, 0) != -1) {
            /*their_addr = ((struct sockaddr_in*)(&addr));
            char *ip = inet_ntoa(their_addr->sin_addr);
            printf("ip: %s\n", ip);
            printf("Thing: %s\n", addr.sa_data);*/
            printf("Received bytes: %d\n", numbytes);
            for(int i = 0; i < numbytes; ++i) {
                printf("%d", indata[i]);
            }
            printf("\n");
        }
        if (numbytes != 0) {
            printf("%d\n",numbytes);
        }
        time(&loop_time);
        if (difftime(loop_time,start_time) > 0.05) {
            break;
        }
    }
    is_thread_running = false;
}

void CommunicationUDP::disconnect(void) {
    if (udp_response_thread.joinable()) {
        std::cout << "Joining thread" << std::endl;
        udp_response_thread.join();
        std::cout << "Thread joined" << std::endl;
    }
    freeaddrinfo(servinfo);
    //servinfo = 0;
    close(socketfd);
}
