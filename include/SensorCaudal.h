#ifndef SENSOR_CAUDAL_H
#define SENSOR_CAUDAL_H

#include "Sensor.h"

class SensorCaudal : public Sensor {
private:
    double caudalMinimo;   // en L/min

public:
    SensorCaudal(int _id, const std::string& _ub, double _lect, double _min = 5.0);
    ~SensorCaudal() override;
    void procesarLectura() override;
};

#endif
