/*
** talker.c -- a datagram "client" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

//
// Porten som klienten ansluter sig mot
// den skall vara const char*
//
#define SERVERPORT "4950"

int main(int argc, char *argv[])
{
    //
    // Filedescriptorn till socket:en
    //
    int sockfd;
    /*
     struct addrinfo {
         int ai_flags;       // input flags
         int ai_family;      // protocol family for socket
         int ai_socktype;        // socket type
         int ai_protocol;        // protocol for socket
         socklen_t ai_addrlen;   // length of socket-address
         struct sockaddr *ai_addr; // socket-address for socket
         char *ai_canonname;     // canonical name for service location
         struct addrinfo *ai_next; // pointer to next in list
     };
     */
    struct addrinfo hints, *servinfo, *p;

    //
    // rv kollar om getaddrinfo returnerade en address
    //
    int rv;
    //
    // numbytes för send och receive för att spara hur många bytes man skickat/läst
    //
    int numbytes;

    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }

    //
    // Fyll hints minne med nollor
    // Krav från API:t
    //
    memset(&hints, 0, sizeof hints);

    //Family är om man använder ip4 eller ip6
    hints.ai_family = AF_UNSPEC;

    //Specificerar udp typ
    hints.ai_socktype = SOCK_DGRAM;

    //
    // Kolla upp addressen
    //
    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    //
    // getaddrinfo kan returnera många addresser så kolla igenom svaren
    //
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    //
    //Om p är NULL så hittade programmet ingen socket
    //
    if (p == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    //
    // Sätter timeouten till en sek
    //
    struct timeval tv;

    //
    // Sekunder
    //
    tv.tv_sec = 30;

    //
    //Sätta denna till 0 eller köra memset på hela structen
    //så man inte får massa garbage values
    //
    tv.tv_usec = 0;

    //
    //Sätter socket SO_RCVTIMEO till den valda tiden
    //
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

    //
    // Om programmet inte har failat så kan man skicka upd paketet
    // även om ingen lyssnar så får man success
    //
    printf("Opened connection towards: %s\n", argv[1]);

    //
    // Testar skicka massa bös
    //
    for (int i = 0; i < 10; ++i) {
        if ((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
                 p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
        printf("Sent bytes: %d\n", numbytes);
    }
    //
    // Buffer (Behöver bara en char)
    //
    char kento[1];

    //
    // Hämta serverns address
    //
    struct sockaddr addr;
    struct sockaddr_in *their_addr;
    socklen_t fromlen;
    fromlen = sizeof &addr;

    //
    // Hämta data
    //
    if (numbytes = recvfrom(sockfd, kento, sizeof kento, 0, &addr, &fromlen) != -1) {
        their_addr = ((struct sockaddr_in*)(&addr));
        char *ip = inet_ntoa(their_addr->sin_addr);
        printf("ip: %s\n", ip);
        printf("Thing: %s\n", addr.sa_data);
        printf("Received bytes: %d\n", numbytes);
        for(int i = 0; i < numbytes; ++i) {
            printf("%c", kento[i]);
        }
        printf("\n");

    } else {
        printf("Received no data from server!");
    }

    //
    // Frigör addrinfo structen ett måste då den är en linkedlist
    //
    freeaddrinfo(servinfo);

    //
    // Stäng alltid sockets!
    //
    close(sockfd);

    return 0;
}
