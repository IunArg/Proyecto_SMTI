#ifndef LISTAGEN_H
#define LISTAGEN_H

#include <iostream>
#include <stdexcept>

template <typename T>
struct NodoGen {
    T dato;
    NodoGen<T>* siguiente;
    NodoGen(T _d) : dato(_d), siguiente(nullptr) {}
};

// Lista enlazada generica. Pensada para T = puntero (ej: Sensor*):
// el destructor hace delete del objeto apuntado y luego del nodo.
template <typename T>
class ListaGen {
private:
    NodoGen<T>* head;
    int cantidad;

public:
    ListaGen() : head(nullptr), cantidad(0) {}

    // Evita copias accidentales (dos listas liberando los mismos punteros)
    ListaGen(const ListaGen&) = delete;
    ListaGen& operator=(const ListaGen&) = delete;

    ~ListaGen() {
        NodoGen<T>* actual = head;
        while (actual != nullptr) {
            NodoGen<T>* aux = actual->siguiente;
            delete actual->dato;   // libera el objeto en el Heap (destructor virtual)
            delete actual;         // libera el nodo
            actual = aux;
        }
        head = nullptr;
        cantidad = 0;
    }

    // Insercion al inicio
    void insertar(T nuevoDato) {
        NodoGen<T>* nuevo = new NodoGen<T>(nuevoDato);
        nuevo->siguiente = head;
        head = nuevo;
        cantidad++;
    }

    // Recorrido polimorfico
    void procesarTodo() {
        NodoGen<T>* actual = head;
        while (actual != nullptr) {
            actual->dato->procesarLectura();   // dynamic binding via vtable
            actual = actual->siguiente;
        }
    }

    // Busqueda por ID: devuelve nullptr si no existe
    T buscarPorId(int id) const {
        NodoGen<T>* actual = head;
        while (actual != nullptr) {
            if (actual->dato->getId() == id) return actual->dato;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    int tamano() const { return cantidad; }
};

#endif
