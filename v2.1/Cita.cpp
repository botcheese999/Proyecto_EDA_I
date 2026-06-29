/*
 * Cita.cpp — Implementación de Cita simplificada (V2.1)
 *
 * CAMBIO V2.1: Sin pila de deshacer medicación (LIFO eliminada).
 */

#include "Cita.h"
#include <iostream>

const std::set<std::string> Cita::ESTADOS_VALIDOS = {
    "pendiente", "en_atencion", "completada", "cancelada"
};

int Cita::contadorId = 0;

Cita::Cita(const std::string& idCita, const std::string& dniPaciente,
           const std::string& idMedico, const std::string& fecha,
           const std::string& estado, const std::string& motivo,
           const std::string& notas)
    : idCita_(idCita), dniPaciente_(dniPaciente), idMedico_(idMedico),
      fecha_(fecha), estado_(estado), motivo_(motivo), notas_(notas),
      medicacion_()
{
}

std::string Cita::getIdCita()       const { return idCita_; }
std::string Cita::getDniPaciente()  const { return dniPaciente_; }
std::string Cita::getIdMedico()     const { return idMedico_; }
std::string Cita::getFecha()        const { return fecha_; }
std::string Cita::getEstado()       const { return estado_; }
std::string Cita::getMotivo()       const { return motivo_; }
std::string Cita::getNotas()        const { return notas_; }
const std::vector<std::string>& Cita::getMedicacion() const { return medicacion_; }

bool Cita::setEstado(const std::string& estado) {
    if (ESTADOS_VALIDOS.count(estado) == 0) return false;
    estado_ = estado;
    return true;
}

void Cita::setNotas(const std::string& notas) { notas_ = notas; }

void Cita::agregarMedicamento(const std::string& medicamento) {
    if (!medicamento.empty()) {
        medicacion_.push_back(medicamento);
    }
}

void Cita::setMedicacion(const std::vector<std::string>& meds) {
    medicacion_ = meds;
}

void Cita::mostrarInfo() const {
    std::cout << "  ID:        " << idCita_ << "\n"
              << "  Paciente:  " << dniPaciente_ << "\n"
              << "  Medico:    " << idMedico_ << "\n"
              << "  Fecha:     " << fecha_ << "\n"
              << "  Estado:    " << estado_ << "\n"
              << "  Motivo:    " << motivo_ << "\n";
    std::cout << "  Medicacion: ";
    if (medicacion_.empty()) {
        std::cout << "Ninguna";
    } else {
        for (size_t i = 0; i < medicacion_.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << medicacion_[i];
        }
    }
    std::cout << "\n";
    if (!notas_.empty()) std::cout << "  Notas:     " << notas_ << "\n";
}

bool Cita::validarEstado(const std::string& estado) {
    return ESTADOS_VALIDOS.count(estado) > 0;
}

std::string Cita::generarId() {
    contadorId++;
    std::string num = std::to_string(contadorId);
    while (num.length() < 4) num = "0" + num;
    return "C" + num;
}
