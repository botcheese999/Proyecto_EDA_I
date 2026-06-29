/*
 * Persona.cpp — Implementación de la superclase Persona (V2.1)
 */

#include "Persona.h"

// ── CRITERIO 1: Constructor con lista de inicialización ──
Persona::Persona(const std::string& id, const std::string& nombre,
                 const std::string& telefono)
    : id_(id), nombre_(nombre), telefono_(telefono)
{
}

// ── CRITERIO 4: Destructor virtual ──
Persona::~Persona() {}

// ── CRITERIO 1: Getters ──
std::string Persona::getId()       const { return id_; }
std::string Persona::getNombre()   const { return nombre_; }
std::string Persona::getTelefono() const { return telefono_; }

// ── CRITERIO 1: Setters con validación ──
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
