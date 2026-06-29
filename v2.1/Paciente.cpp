/*
 * Paciente.cpp — Implementación de Paciente (V2.1)
 *
 * CAMBIO V2.1: Eliminado historial clínico (EntradaHistorial, historial_,
 * mostrarHistorialCompleto). Reemplazado por mostrarFichaMedica() para RTTI.
 */

#include "Paciente.h"
#include <iostream>
#include <algorithm>

static const std::set<std::string> TIPOS_SANGRE_VALIDOS = {
    "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
};

// ── CRITERIO 1 + CRITERIO 3: Lista de inicialización, delegación a Persona ──
Paciente::Paciente(const std::string& dni, const std::string& nombre,
                   const std::string& tipoSangre, const std::string& fechaNacimiento,
                   const std::string& telefono, const std::string& direccion,
                   const std::string& email, const std::string& contactoEmergencia)
    : Persona(dni, nombre, telefono),
      tipoSangre_(tipoSangre),
      fechaNacimiento_(fechaNacimiento),
      direccion_(direccion),
      email_(email),
      contactoEmergencia_(contactoEmergencia),
      alergias_()
{
}

Paciente::~Paciente() {}

// ── CRITERIO 4: Override ──
void Paciente::mostrarInfo() const {
    std::cout << "  -- Paciente --\n"
              << "  DNI:         " << id_ << "\n"
              << "  Nombre:      " << nombre_ << "\n"
              << "  Sangre:      " << tipoSangre_ << "\n"
              << "  Nacimiento:  " << fechaNacimiento_ << "\n"
              << "  Telefono:    " << telefono_ << "\n"
              << "  Direccion:   " << direccion_ << "\n"
              << "  Email:       " << email_ << "\n"
              << "  Contacto Em: " << contactoEmergencia_ << "\n"
              << "  Alergias:    " << alergias_.size() << "\n";
}

std::string Paciente::getTipo() const { return "Paciente"; }

// ── Getters ──
std::string Paciente::getTipoSangre()         const { return tipoSangre_; }
std::string Paciente::getFechaNacimiento()    const { return fechaNacimiento_; }
std::string Paciente::getDireccion()          const { return direccion_; }
std::string Paciente::getEmail()              const { return email_; }
std::string Paciente::getContactoEmergencia() const { return contactoEmergencia_; }
const std::set<std::string>& Paciente::getAlergias() const { return alergias_; }

// ── Setters con validación ──
bool Paciente::setDireccion(const std::string& direccion) {
    if (direccion.empty()) return false;
    direccion_ = direccion;
    return true;
}

bool Paciente::setEmail(const std::string& email) {
    if (email.empty()) return false;
    if (email.find('@') == std::string::npos) return false;
    email_ = email;
    return true;
}

bool Paciente::setContactoEmergencia(const std::string& contacto) {
    if (contacto.empty()) return false;
    contactoEmergencia_ = contacto;
    return true;
}

// ── Alergias ──
bool Paciente::agregarAlergia(const std::string& alergia) {
    if (alergia.empty()) return false;
    std::string lower = alergia;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return alergias_.insert(lower).second;
}

bool Paciente::removerAlergia(const std::string& alergia) {
    std::string lower = alergia;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return alergias_.erase(lower) > 0;
}

bool Paciente::tieneAlergia(const std::string& alergia) const {
    std::string lower = alergia;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return alergias_.count(lower) > 0;
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – MÉTODO EXCLUSIVO DE PACIENTE (RTTI / dynamic_cast)
// Muestra la ficha médica completa del paciente. Este método NO existe
// en la superclase Persona. Solo accesible tras dynamic_cast exitoso.
// ═══════════════════════════════════════════════════════════════════════════
void Paciente::mostrarFichaMedica() const {
    std::cout << "\n  == Ficha Medica Completa de " << nombre_
              << " (DNI: " << id_ << ") ==\n"
              << "  Tipo de sangre:       " << tipoSangre_ << "\n"
              << "  Fecha de nacimiento:  " << fechaNacimiento_ << "\n"
              << "  Telefono:             " << telefono_ << "\n"
              << "  Direccion:            " << direccion_ << "\n"
              << "  Email:                " << email_ << "\n"
              << "  Contacto emergencia:  " << contactoEmergencia_ << "\n"
              << "  Alergias registradas: ";
    if (alergias_.empty()) {
        std::cout << "Ninguna";
    } else {
        bool first = true;
        for (const auto& a : alergias_) {
            if (!first) std::cout << ", ";
            std::cout << a;
            first = false;
        }
    }
    std::cout << "\n";
}

// ── Validaciones estáticas ──
bool Paciente::validarDni(const std::string& dni) {
    if (dni.length() != 8) return false;
    for (char c : dni) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool Paciente::validarTipoSangre(const std::string& tipo) {
    return TIPOS_SANGRE_VALIDOS.count(tipo) > 0;
}
