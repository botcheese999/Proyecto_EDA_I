/*
 * GestorCitas.cpp — Implementación del gestor de citas (V2.1 SIMPLIFICADO)
 *
 * CAMBIO V2.1: Sin Cola FIFO ni Pila LIFO. Solo CRUD de citas.
 */

#include "GestorCitas.h"
#include <ctime>
#include <sstream>
#include <iomanip>

static std::string fechaActual() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d");
    return oss.str();
}

GestorCitas::GestorCitas() {}

GestorCitas::~GestorCitas() {
    for (auto& par : registroCitas_) delete par.second;
    registroCitas_.clear();
}

std::pair<bool, std::string> GestorCitas::crearCita(
    const std::string& dniPaciente, const std::string& idMedico,
    const std::string& motivo)
{
    std::string idCita = Cita::generarId();
    Cita* nueva = new Cita(idCita, dniPaciente, idMedico, fechaActual(),
                            "pendiente", motivo);
    registroCitas_[idCita] = nueva;
    return {true, "Cita " + idCita + " creada — Paciente: " + dniPaciente
                  + ", Medico: " + idMedico};
}

bool GestorCitas::agregarCita(Cita* cita) {
    if (!cita) return false;
    std::string id = cita->getIdCita();
    if (registroCitas_.count(id) > 0) return false;
    registroCitas_[id] = cita;
    return true;
}

Cita* GestorCitas::buscarCita(const std::string& idCita) const {
    auto it = registroCitas_.find(idCita);
    return (it != registroCitas_.end()) ? it->second : nullptr;
}

std::pair<bool, std::string> GestorCitas::completarCita(
    const std::string& idCita, const std::string& notas)
{
    Cita* cita = buscarCita(idCita);
    if (!cita) return {false, "No se encontro cita " + idCita + "."};
    if (cita->getEstado() == "completada")
        return {false, "La cita " + idCita + " ya esta completada."};
    cita->setEstado("completada");
    cita->setNotas(notas);
    return {true, "Cita " + idCita + " completada."};
}

std::pair<bool, std::string> GestorCitas::cancelarCita(
    const std::string& idCita, const std::string& motivoCancelacion)
{
    Cita* cita = buscarCita(idCita);
    if (!cita) return {false, "No se encontro cita " + idCita + "."};
    std::string estado = cita->getEstado();
    if (estado == "completada" || estado == "cancelada")
        return {false, "La cita " + idCita + " ya esta " + estado + "."};
    cita->setEstado("cancelada");
    cita->setNotas("CANCELADA: " + motivoCancelacion);
    return {true, "Cita " + idCita + " cancelada."};
}

std::vector<Cita*> GestorCitas::listarPorEstado(const std::string& estado) const {
    std::vector<Cita*> res;
    for (const auto& par : registroCitas_)
        if (par.second->getEstado() == estado) res.push_back(par.second);
    return res;
}

std::vector<Cita*> GestorCitas::listarPorPaciente(const std::string& dni) const {
    std::vector<Cita*> res;
    for (const auto& par : registroCitas_)
        if (par.second->getDniPaciente() == dni) res.push_back(par.second);
    return res;
}

const std::map<std::string, Cita*>& GestorCitas::getRegistro() const {
    return registroCitas_;
}

int GestorCitas::getCantidad() const {
    return static_cast<int>(registroCitas_.size());
}
