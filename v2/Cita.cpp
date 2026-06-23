/*
 * Cita.cpp — Implementación de la clase Cita.
 */

#include "Cita.h"
#include <iostream>
#include <algorithm>

// ── Estados válidos de una cita ──
const std::set<std::string> Cita::ESTADOS_VALIDOS = {
    "pendiente", "en_atencion", "completada", "cancelada"
};

int Cita::contadorId = 0;

// ── CRITERIO 1: Constructor con lista de inicialización ──
Cita::Cita(const std::string& idCita, const std::string& dniPaciente,
           const std::string& idMedico, const std::string& fecha,
           const std::string& motivo)
    : idCita_(idCita),
      dniPaciente_(dniPaciente),
      idMedico_(idMedico),
      fecha_(fecha),
      estado_("pendiente"),
      motivo_(motivo),
      notas_(),
      medicacionActual_()
{
}

// ── CRITERIO 1: Getters ──
std::string Cita::getIdCita()       const { return idCita_; }
std::string Cita::getDniPaciente()  const { return dniPaciente_; }
std::string Cita::getIdMedico()     const { return idMedico_; }
std::string Cita::getFecha()        const { return fecha_; }
std::string Cita::getEstado()       const { return estado_; }
std::string Cita::getMotivo()       const { return motivo_; }
std::string Cita::getNotas()        const { return notas_; }
const std::vector<std::string>& Cita::getMedicacion() const { return medicacionActual_; }

// ── CRITERIO 1: Setter con validación (solo estados válidos) ──
bool Cita::setEstado(const std::string& estado) {
    if (ESTADOS_VALIDOS.count(estado) == 0) return false;
    estado_ = estado;
    return true;
}

void Cita::setNotas(const std::string& notas) {
    notas_ = notas;
}

// ── Gestión de medicación ──
bool Cita::agregarMedicamento(const std::string& medicamento) {
    if (medicamento.empty()) return false;
    if (estado_ != "pendiente" && estado_ != "en_atencion") return false;
    medicacionActual_.push_back(medicamento);
    return true;
}

bool Cita::removerMedicamento(const std::string& medicamento) {
    auto it = std::find(medicacionActual_.begin(), medicacionActual_.end(), medicamento);
    if (it == medicacionActual_.end()) return false;
    medicacionActual_.erase(it);
    return true;
}

// ── Mostrar información de la cita ──
void Cita::mostrarInfo() const {
    std::cout << "  ID:        " << idCita_ << "\n"
              << "  Paciente:  " << dniPaciente_ << "\n"
              << "  Medico:    " << idMedico_ << "\n"
              << "  Fecha:     " << fecha_ << "\n"
              << "  Estado:    " << estado_ << "\n"
              << "  Motivo:    " << motivo_ << "\n";

    std::cout << "  Medicacion: ";
    if (medicacionActual_.empty()) {
        std::cout << "Ninguna";
    } else {
        for (size_t i = 0; i < medicacionActual_.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << medicacionActual_[i];
        }
    }
    std::cout << "\n";

    if (!notas_.empty()) {
        std::cout << "  Notas:     " << notas_ << "\n";
    }
}

// ── Validación estática ──
bool Cita::validarEstado(const std::string& estado) {
    return ESTADOS_VALIDOS.count(estado) > 0;
}

// ── Generador de IDs auto-incrementales (C0001, C0002, ...) ──
std::string Cita::generarId() {
    contadorId++;
    std::string id = "C";
    std::string num = std::to_string(contadorId);
    while (num.length() < 4) num = "0" + num;
    return id + num;
}
