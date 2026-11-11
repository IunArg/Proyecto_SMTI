#!/bin/bash
# Con el servidor corriendo (./smti 4), ejecuta este script en otra terminal.
for t in "TEMP,101,Caldera-01,92.5" "PRES,201,Linea-A,12.3" "CAUD,301,Tanque-1,3.2" "XXXX,1,Nada,1.0"; do
    echo "$t" | nc localhost 8080
    sleep 0.3
done
