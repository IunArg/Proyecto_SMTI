#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "Sensor.h"
#include "ListaGen.h"
#include "ServidorSockets.h"

int main(int argc, char* argv[]) {
    int cuantos = (argc > 1) ? std::atoi(argv[1]) : 3;   // clientes a atender

    std::cout << "==================================================" << std::endl;
    std::cout << " SMTI: SERVIDOR DE TELEMETRIA DISTRIBUIDA (UNIDAD 9)" << std::endl;
    std::cout << "==================================================" << std::endl;

    ListaGen<Sensor*> listaPlanta;

    try {
        ServidorSMTI servidor(8080, listaPlanta);
        servidor.iniciar();
        servidor.atenderClientes(cuantos);

        std::cout << "\n--- PROCESAMIENTO POLIMORFICO DE TELEMETRIA ("
                  << listaPlanta.tamano() << " sensores) ---" << std::endl;
        listaPlanta.procesarTodo();

        Sensor* s = listaPlanta.buscarPorId(101);
        if (s) std::cout << "\nBusqueda por ID 101: " << *s << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[EXCEPCION GRAVE SMTI] " << e.what() << std::endl;
        return 1;
    }

    return 0;   // ~ListaGen() libera todo el Heap
}
