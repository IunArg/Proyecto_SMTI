#include "ServidorSockets.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"
#include "SensorCaudal.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

ServidorSMTI::ServidorSMTI(int p, ListaGen<Sensor*>& l)
    : server_fd(-1), puerto(p), lista(l) {}

ServidorSMTI::~ServidorSMTI() {
    if (server_fd >= 0) close(server_fd);
}

void ServidorSMTI::iniciar() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) throw ErrorSocket("no se pudo crear el socket.");

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(puerto);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(server_fd);
        server_fd = -1;
        throw ErrorSocket("bind fallido, puerto " + std::to_string(puerto) + " ocupado o sin permiso.");
    }
    if (listen(server_fd, 5) < 0) {
        close(server_fd);
        server_fd = -1;
        throw ErrorSocket("listen fallido.");
    }
    std::cout << "[SERVIDOR SMTI] Escuchando telemetria en puerto " << puerto << "..." << std::endl;
}

Sensor* ServidorSMTI::crearSensor(const std::string& trama) {
    std::stringstream ss(trama);
    std::string tipo, sid, ub, sval;
    if (!std::getline(ss, tipo, ',') || !std::getline(ss, sid, ',') ||
        !std::getline(ss, ub, ',')   || !std::getline(ss, sval))
        throw ErrorTrama("se esperaba TIPO,ID,UBICACION,VALOR y llego: " + trama);

    MuestraSensor m;
    std::memset(&m, 0, sizeof(m));
    std::strncpy(m.tipo, tipo.c_str(), sizeof(m.tipo) - 1);

    char* fin = nullptr;
    m.idSensor = (int)std::strtol(sid.c_str(), &fin, 10);
    if (fin == sid.c_str() || *fin != '\0') throw ErrorTrama("ID no numerico: " + sid);
    m.valor = std::strtod(sval.c_str(), &fin);
    if (fin == sval.c_str() || *fin != '\0') throw ErrorTrama("valor no numerico: " + sval);

    std::string t(m.tipo);
    if (t == "TEMP") return new SensorTemperatura(m.idSensor, ub, m.valor);
    if (t == "PRES") return new SensorPresion(m.idSensor, ub, m.valor);
    if (t == "CAUD") return new SensorCaudal(m.idSensor, ub, m.valor);
    throw ErrorTrama("tipo de sensor desconocido: " + t);
}

void ServidorSMTI::atenderClientes(int cuantos) {
    for (int i = 0; i < cuantos; i++) {
        sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int client_fd = accept(server_fd, (struct sockaddr*)&cli, &len);
        if (client_fd < 0) throw ErrorSocket("accept fallido.");

        // Timeout de lectura: un cliente mudo no bloquea el servidor para siempre
        timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        char buffer[1024] = {0};
        ssize_t n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (n > 0) {
            std::string trama(buffer, n);
            while (!trama.empty() && (trama.back() == '\n' || trama.back() == '\r'))
                trama.pop_back();
            try {
                lista.insertar(crearSensor(trama));
                std::cout << "[SERVIDOR SMTI] Trama aceptada: " << trama << std::endl;
                send(client_fd, "ACK\n", 4, 0);
            } catch (const std::runtime_error& e) {
                std::cerr << "[TRAMA RECHAZADA] " << e.what() << std::endl;
                send(client_fd, "ERROR\n", 6, 0);
            }
        } else {
            std::cerr << "[SERVIDOR SMTI] Cliente sin datos (timeout o cierre)." << std::endl;
        }
        close(client_fd);
    }
}
