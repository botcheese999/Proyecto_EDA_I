/*
 * GestorPersonal.cpp — Implementación del gestor de personal (V2.1 NUEVO)
 */

#include "GestorPersonal.h"

GestorPersonal::GestorPersonal() {}

GestorPersonal::~GestorPersonal() {
    for (auto& par : catalogo_) delete par.second;
    catalogo_.clear();
}

std::pair<bool, std::string> GestorPersonal::registrar(
    const std::string& nombre, const std::string& rol,
    const std::string& especialidad, const std::string& turno,
    const std::string& telefono)
{
    if (nombre.empty())
        return {false, "El nombre no puede estar vacio."};
    if (!Personal::validarRol(rol))
        return {false, "Rol invalido."};
    if (!Personal::validarTurno(turno))
        return {false, "Turno invalido."};

    std::string id = Personal::generarId();
    Personal* nuevo = new Personal(id, nombre, rol, especialidad,
                                    turno, telefono, true);
    catalogo_[id] = nuevo;
    return {true, "Personal '" + nombre + "' registrado con ID: " + id};
}

bool GestorPersonal::agregarPersonal(Personal* personal) {
    if (!personal) return false;
    std::string id = personal->getId();
    if (catalogo_.count(id) > 0) return false;
    catalogo_[id] = personal;
    return true;
}

Personal* GestorPersonal::buscar(const std::string& id) const {
    auto it = catalogo_.find(id);
    return (it != catalogo_.end()) ? it->second : nullptr;
}

std::vector<std::pair<std::string, std::string>> GestorPersonal::listar() const {
    std::vector<std::pair<std::string, std::string>> resultado;
    for (const auto& par : catalogo_)
        resultado.push_back({par.first, par.second->getNombre()});
    return resultado;
}

const std::map<std::string, Personal*>& GestorPersonal::getCatalogo() const {
    return catalogo_;
}

int GestorPersonal::getCantidad() const {
    return static_cast<int>(catalogo_.size());
}
