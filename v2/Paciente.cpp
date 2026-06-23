/*
 * Paciente.cpp — Implementación de la clase Paciente.
 */

#include "Paciente.h"
#include <iostream>
#include <algorithm>

// Tipos de sangre válidos
static const std::set<std::string> TIPOS_SANGRE_VALIDOS = {
    "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
};

// ── CRITERIO 1: Constructor con lista de inicialización ──
// ── CRITERIO 3: Invoca al constructor de la superclase Persona ──
Paciente::Paciente(const std::string& dni, const std::string& nombre,
                   const std::string& tipoSangre, const std::string& fechaNacimiento,
                   const std::string& telefono, const std::string& direccion,
                   const std::string& email, const std::string& contactoEmergencia)
    : Persona(dni, nombre, telefono),   // Delegación al constructor base
      tipoSangre_(tipoSangre),
      fechaNacimiento_(fechaNacimiento),
      direccion_(direccion),
      email_(email),
      contactoEmergencia_(contactoEmergencia),
      alergias_(),
      historial_()
{
}

Paciente::~Paciente() {
    // Limpieza automática (vectores y sets gestionan su propia memoria)
}

// ── CRITERIO 4: Override — enlace dinámico con la vtable ──
void Paciente::mostrarInfo() const {
    std::cout << "  ── Paciente ──\n"
              << "  DNI:         " << id_ << "\n"        // Acceso a atributo protected heredado
              << "  Nombre:      " << nombre_ << "\n"    // CRITERIO 3: reutilización
              << "  Sangre:      " << tipoSangre_ << "\n"
              << "  Nacimiento:  " << fechaNacimiento_ << "\n"
              << "  Telefono:    " << telefono_ << "\n"
              << "  Direccion:   " << direccion_ << "\n"
              << "  Email:       " << email_ << "\n"
              << "  Contacto Em: " << contactoEmergencia_ << "\n"
              << "  Alergias:    " << alergias_.size() << "\n"
              << "  Historial:   " << historial_.size() << " entrada(s)\n";
}

// ── CRITERIO 4: Override — identifica el tipo concreto en tiempo de ejecución ──
std::string Paciente::getTipo() const {
    return "Paciente";
}

// ── CRITERIO 1: Getters ──
std::string Paciente::getTipoSangre()         const { return tipoSangre_; }
std::string Paciente::getFechaNacimiento()    const { return fechaNacimiento_; }
std::string Paciente::getDireccion()          const { return direccion_; }
std::string Paciente::getEmail()              const { return email_; }
std::string Paciente::getContactoEmergencia() const { return contactoEmergencia_; }
const std::set<std::string>& Paciente::getAlergias()         const { return alergias_; }
const std::vector<EntradaHistorial>& Paciente::getHistorial() const { return historial_; }

// ── CRITERIO 1: Setters con validación (no aceptan vacío) ──
bool Paciente::setDireccion(const std::string& direccion) {
    if (direccion.empty()) return false;
    direccion_ = direccion;
    return true;
}

bool Paciente::setEmail(const std::string& email) {
    if (email.empty()) return false;
    // Validación básica: debe contener '@'
    if (email.find('@') == std::string::npos) return false;
    email_ = email;
    return true;
}

bool Paciente::setContactoEmergencia(const std::string& contacto) {
    if (contacto.empty()) return false;
    contactoEmergencia_ = contacto;
    return true;
}

// ── Gestión de alergias con set ──
bool Paciente::agregarAlergia(const std::string& alergia) {
    if (alergia.empty()) return false;
    // Convertir a minúsculas para normalización
    std::string alergiaLower = alergia;
    std::transform(alergiaLower.begin(), alergiaLower.end(),
                   alergiaLower.begin(), ::tolower);
    auto resultado = alergias_.insert(alergiaLower);
    return resultado.second;  // true si se insertó (no existía)
}

bool Paciente::removerAlergia(const std::string& alergia) {
    std::string alergiaLower = alergia;
    std::transform(alergiaLower.begin(), alergiaLower.end(),
                   alergiaLower.begin(), ::tolower);
    return alergias_.erase(alergiaLower) > 0;
}

bool Paciente::tieneAlergia(const std::string& alergia) const {
    std::string alergiaLower = alergia;
    std::transform(alergiaLower.begin(), alergiaLower.end(),
                   alergiaLower.begin(), ::tolower);
    return alergias_.count(alergiaLower) > 0;
}

// ── Historial ──
void Paciente::agregarEntradaHistorial(const EntradaHistorial& entrada) {
    historial_.push_back(entrada);
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – MÉTODO EXCLUSIVO DE PACIENTE (para RTTI / dynamic_cast)
// Este método NO existe en la superclase Persona. Solo es accesible
// cuando un dynamic_cast<Paciente*> confirma que el objeto es Paciente.
// ═══════════════════════════════════════════════════════════════════════════
void Paciente::mostrarHistorialCompleto() const {
    std::cout << "\n  ══ Historial Medico Completo de " << nombre_
              << " (DNI: " << id_ << ") ══\n";

    if (historial_.empty()) {
        std::cout << "  No hay entradas en el historial.\n";
        return;
    }

    for (size_t i = 0; i < historial_.size(); ++i) {
        const auto& e = historial_[i];
        std::cout << "\n  [" << (i + 1) << "] " << e.fecha
                  << " — " << e.tipo << "\n"
                  << "      Medico:      " << e.medicoId << "\n"
                  << "      Diagnostico: " << e.diagnostico << "\n";

        std::cout << "      Medicacion:  ";
        if (e.medicacion.empty()) {
            std::cout << "Ninguna";
        } else {
            for (size_t j = 0; j < e.medicacion.size(); ++j) {
                if (j > 0) std::cout << ", ";
                std::cout << e.medicacion[j];
            }
        }
        std::cout << "\n";

        if (!e.notas.empty()) {
            std::cout << "      Notas:       " << e.notas << "\n";
        }
    }
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
