#ifndef EXCEPCIONES_SMTI_H
#define EXCEPCIONES_SMTI_H

#include <stdexcept>
#include <string>

// Ambas heredan de std::runtime_error: se pueden capturar con
// catch (const std::runtime_error&) o catch (const std::exception&).
class ErrorSocket : public std::runtime_error {
public:
    explicit ErrorSocket(const std::string& msg)
        : std::runtime_error("Error POSIX: " + msg) {}
};

class ErrorTrama : public std::runtime_error {
public:
    explicit ErrorTrama(const std::string& msg)
        : std::runtime_error("Trama invalida: " + msg) {}
};

#endif
