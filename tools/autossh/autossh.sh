#!/bin/sh
autossh -c "ssh -q -f -N -o "ServerAliveInterval 60" -o "ServerAliveCountMax 3" -R 4321:localhost:22 autossh@hem.kento.se -p 22"
