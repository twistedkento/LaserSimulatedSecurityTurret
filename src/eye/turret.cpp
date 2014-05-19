#include "turret.h"
#include <string>
#include <iostream>

Turret::Turret() : connection("129.16.199.120","9999") {
    connection.connect_udp();
}
Turret::~Turret() {
    connection.disconnect();
}

void Turret::send(Command com){
    connection.sendCommand(com);
}
