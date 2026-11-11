#ifndef SERVIDOR_SOCKETS_H
#define SERVIDOR_SOCKETS_H

#include <string>
#include "Sensor.h"
#include "ListaGen.h"
#include "ExcepcionesSMTI.h"

// Trama industrial ya interpretada
struct MuestraSensor {
    int idSensor;
    double valor;
    char tipo[12];
};

// Trama de texto esperada: TIPO,ID,UBICACION,VALOR
//   ejemplos: TEMP,101,Caldera-01,92.5
//             PRES,201,Linea-A,12.3
//             CAUD,301,Tanque-1,3.2
class ServidorSMTI {
private:
    int server_fd;
    int puerto;
    ListaGen<Sensor*>& lista;

    Sensor* crearSensor(const std::string& trama);

public:
    ServidorSMTI(int p, ListaGen<Sensor*>& l);
    ~ServidorSMTI();

    void iniciar();                    // socket + bind + listen
    void atenderClientes(int cuantos); // accept + recv + insertar en la lista
};

#endif
