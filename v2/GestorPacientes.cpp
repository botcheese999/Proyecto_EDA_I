/*
 * GestorPacientes.cpp — Implementación del gestor de pacientes.
 *
 * CRITERIO 2: Toda la lógica de negocio relacionada con pacientes
 * está centralizada aquí, separada de la entidad Paciente.
 */

#include "GestorPacientes.h"

GestorPacientes::GestorPacientes() {}

// Destructor: libera la memoria dinámica de todos los pacientes
GestorPacientes::~GestorPacientes() {
    for (auto& par : catalogo_) {
        delete par.second;
    }
    catalogo_.clear();
}

// ── Registrar nuevo paciente con validaciones ──
std::pair<bool, std::string> GestorPacientes::registrar(
    const std::string& dni, const std::string& nombre,
    const std::string& tipoSangre, const std::string& fechaNacimiento,
    const std::string& telefono, const std::string& direccion,
    const std::string& email, const std::string& contactoEmergencia)
{
    // Validaciones
    if (!Paciente::validarDni(dni))
        return {false, "El DNI debe tener exactamente 8 digitos numericos."};

    if (!Paciente::validarTipoSangre(tipoSangre))
        return {false, "Tipo de sangre invalido."};

    if (nombre.empty())
        return {false, "El nombre no puede estar vacio."};

    if (catalogo_.count(dni) > 0)
        return {false, "Ya existe un paciente con DNI " + dni + "."};

    // ── CRITERIO 4: Instanciación dinámica con new ──
    Paciente* nuevo = new Paciente(dni, nombre, tipoSangre, fechaNacimiento,
                                   telefono, direccion, email, contactoEmergencia);
    catalogo_[dni] = nuevo;

    return {true, "Paciente '" + nombre + "' registrado exitosamente."};
}

Paciente* GestorPacientes::buscar(const std::string& dni) const {
    auto it = catalogo_.find(dni);
    if (it == catalogo_.end()) return nullptr;
    return it->second;
}

std::pair<bool, std::string> GestorPacientes::eliminar(const std::string& dni) {
    auto it = catalogo_.find(dni);
    if (it == catalogo_.end())
        return {false, "No se encontro paciente con DNI " + dni + "."};

    std::string nombre = it->second->getNombre();
    delete it->second;
    catalogo_.erase(it);
    return {true, "Paciente '" + nombre + "' eliminado."};
}

std::vector<std::pair<std::string, std::string>> GestorPacientes::listar() const {
    std::vector<std::pair<std::string, std::string>> resultado;
    for (const auto& par : catalogo_) {
        resultado.push_back({par.first, par.second->getNombre()});
    }
    return resultado;
}

// ── Actualizaciones de datos mutables ──
std::pair<bool, std::string> GestorPacientes::actualizarTelefono(
    const std::string& dni, const std::string& valor)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->setTelefono(valor)) return {false, "Telefono invalido."};
    return {true, "Telefono actualizado."};
}

std::pair<bool, std::string> GestorPacientes::actualizarDireccion(
    const std::string& dni, const std::string& valor)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->setDireccion(valor)) return {false, "Direccion invalida."};
    return {true, "Direccion actualizada."};
}

std::pair<bool, std::string> GestorPacientes::actualizarEmail(
    const std::string& dni, const std::string& valor)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->setEmail(valor)) return {false, "Email invalido (debe contener '@')."};
    return {true, "Email actualizado."};
}

std::pair<bool, std::string> GestorPacientes::actualizarContactoEmergencia(
    const std::string& dni, const std::string& valor)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->setContactoEmergencia(valor)) return {false, "Contacto invalido."};
    return {true, "Contacto de emergencia actualizado."};
}

// ── Alergias ──
std::pair<bool, std::string> GestorPacientes::agregarAlergia(
    const std::string& dni, const std::string& alergia)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->agregarAlergia(alergia))
        return {false, "La alergia ya esta registrada o es invalida."};
    return {true, "Alergia '" + alergia + "' agregada."};
}

std::pair<bool, std::string> GestorPacientes::removerAlergia(
    const std::string& dni, const std::string& alergia)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->removerAlergia(alergia))
        return {false, "La alergia no esta registrada."};
    return {true, "Alergia '" + alergia + "' removida."};
}

std::set<std::string> GestorPacientes::cruzarAlergias(
    const std::string& dni1, const std::string& dni2) const
{
    std::set<std::string> comunes;
    Paciente* p1 = buscar(dni1);
    Paciente* p2 = buscar(dni2);
    if (!p1 || !p2) return comunes;

    const auto& a1 = p1->getAlergias();
    const auto& a2 = p2->getAlergias();

    // Intersección de sets
    for (const auto& alergia : a1) {
        if (a2.count(alergia) > 0) {
            comunes.insert(alergia);
        }
    }
    return comunes;
}

int GestorPacientes::getCantidad() const {
    return static_cast<int>(catalogo_.size());
}
