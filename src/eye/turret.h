//#pragma once
//#include "python.h"
#include "comm_udp.h"
#include "command.h"

class Turret
{
    public:
        Turret();
        ~Turret();

        void send(Command);
    private:
        CommunicationUDP connection;
};

