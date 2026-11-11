#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <string>

// Clase base abstracta (tiene un metodo virtual puro)
class Sensor {
protected:
    int id;
    std::string ubicacion;
    double lectura;

public:
    Sensor(int _id, const std::string& _ub, double _lect);

    // Destructor virtual: obliga a llamar tambien al destructor de la clase derivada
    virtual ~Sensor();

    int getId() const { return id; }
    double getLectura() const { return lectura; }

    // Metodo virtual puro: cada sensor concreto debe implementarlo
    virtual void procesarLectura() = 0;

    friend std::ostream& operator<<(std::ostream& os, const Sensor& s);
};

#endif
