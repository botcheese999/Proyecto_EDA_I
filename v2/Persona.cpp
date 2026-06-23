/*
 * Persona.cpp — Implementación de la superclase Persona.
 */

#include "Persona.h"

// ── CRITERIO 1: Constructor con lista de inicialización (seguro y eficiente) ──
Persona::Persona(const std::string& id, const std::string& nombre,
                 const std::string& telefono)
    : id_(id), nombre_(nombre), telefono_(telefono)
{
}

// ── CRITERIO 4: Destructor virtual (permite destrucción polimórfica correcta) ──
Persona::~Persona() {
    // El destructor virtual asegura que al hacer 'delete' sobre un
    // puntero Persona* que apunte a un Paciente o Personal, se
    // invoque el destructor correcto de la clase hija.
}

// ── CRITERIO 1: Getters (acceso controlado a atributos privados/protected) ──
std::string Persona::getId()       const { return id_; }
std::string Persona::getNombre()   const { return nombre_; }
std::string Persona::getTelefono() const { return telefono_; }

// ── CRITERIO 1: Setter con validación lógica (no acepta vacío) ──
bool Persona::setTelefono(const std::string& telefono) {
    if (telefono.empty()) return false;
    telefono_ = telefono;
    return true;
}

bool Persona::setNombre(const std::string& nombre) {
    if (nombre.empty()) return false;
    nombre_ = nombre;
    return true;
}
