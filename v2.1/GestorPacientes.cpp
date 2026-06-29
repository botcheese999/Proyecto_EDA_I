/*
 * GestorPacientes.cpp — Implementación del gestor de pacientes (V2.1)
 */

#include "GestorPacientes.h"

GestorPacientes::GestorPacientes() {}

GestorPacientes::~GestorPacientes() {
    for (auto& par : catalogo_) delete par.second;
    catalogo_.clear();
}

std::pair<bool, std::string> GestorPacientes::registrar(
    const std::string& dni, const std::string& nombre,
    const std::string& tipoSangre, const std::string& fechaNacimiento,
    const std::string& telefono, const std::string& direccion,
    const std::string& email, const std::string& contactoEmergencia)
{
    if (!Paciente::validarDni(dni))
        return {false, "El DNI debe tener exactamente 8 digitos numericos."};
    if (!Paciente::validarTipoSangre(tipoSangre))
        return {false, "Tipo de sangre invalido."};
    if (nombre.empty())
        return {false, "El nombre no puede estar vacio."};
    if (catalogo_.count(dni) > 0)
        return {false, "Ya existe un paciente con DNI " + dni + "."};

    Paciente* nuevo = new Paciente(dni, nombre, tipoSangre, fechaNacimiento,
                                   telefono, direccion, email, contactoEmergencia);
    catalogo_[dni] = nuevo;
    return {true, "Paciente '" + nombre + "' registrado exitosamente."};
}

bool GestorPacientes::agregarPaciente(Paciente* paciente) {
    if (!paciente) return false;
    std::string dni = paciente->getId();
    if (catalogo_.count(dni) > 0) return false;
    catalogo_[dni] = paciente;
    return true;
}

Paciente* GestorPacientes::buscar(const std::string& dni) const {
    auto it = catalogo_.find(dni);
    return (it != catalogo_.end()) ? it->second : nullptr;
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
    for (const auto& par : catalogo_)
        resultado.push_back({par.first, par.second->getNombre()});
    return resultado;
}

std::pair<bool, std::string> GestorPacientes::actualizarEmail(
    const std::string& dni, const std::string& valor)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->setEmail(valor)) return {false, "Email invalido (debe contener '@')."};
    return {true, "Email actualizado."};
}

std::pair<bool, std::string> GestorPacientes::agregarAlergia(
    const std::string& dni, const std::string& alergia)
{
    Paciente* p = buscar(dni);
    if (!p) return {false, "No se encontro paciente con DNI " + dni + "."};
    if (!p->agregarAlergia(alergia))
        return {false, "La alergia ya esta registrada o es invalida."};
    return {true, "Alergia '" + alergia + "' agregada."};
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
    for (const auto& a : a1) {
        if (a2.count(a) > 0) comunes.insert(a);
    }
    return comunes;
}

const std::map<std::string, Paciente*>& GestorPacientes::getCatalogo() const {
    return catalogo_;
}

int GestorPacientes::getCantidad() const {
    return static_cast<int>(catalogo_.size());
}
