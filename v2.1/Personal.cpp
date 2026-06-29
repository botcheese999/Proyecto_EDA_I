/*
 * Personal.cpp — Implementación de Personal (V2.1)
 */

#include "Personal.h"
#include <iostream>

int Personal::contadorId = 0;

const std::map<std::string, int> Personal::JERARQUIA = {
    {"director", 4}, {"especialista", 3},
    {"medico_general", 2}, {"enfermera", 1}
};

const std::map<int, std::set<std::string>> Personal::PERMISOS_POR_NIVEL = {
    {4, {"leer_todo", "escribir_todo", "gestionar_personal",
         "ver_reportes", "deshacer_medicacion"}},
    {3, {"leer_historial", "escribir_historial", "recetar",
         "ver_reportes", "deshacer_medicacion"}},
    {2, {"leer_historial", "escribir_historial", "recetar"}},
    {1, {"leer_historial", "registrar_signos_vitales"}}
};

const std::set<std::string> Personal::ROLES_VALIDOS = {
    "director", "especialista", "medico_general", "enfermera"
};

const std::set<std::string> Personal::TURNOS_VALIDOS = {
    "mañana", "tarde", "noche"
};

// ── CRITERIO 1 + 3: Lista de inicialización, delegación a Persona ──
Personal::Personal(const std::string& id, const std::string& nombre,
                   const std::string& rol, const std::string& especialidad,
                   const std::string& turno, const std::string& telefono,
                   bool activo)
    : Persona(id, nombre, telefono),
      rol_(rol), especialidad_(especialidad), turno_(turno),
      nivelAcceso_(0), activo_(activo), permisos_()
{
    auto it = JERARQUIA.find(rol_);
    if (it != JERARQUIA.end()) {
        nivelAcceso_ = it->second;
        auto itP = PERMISOS_POR_NIVEL.find(nivelAcceso_);
        if (itP != PERMISOS_POR_NIVEL.end()) {
            permisos_ = itP->second;
        }
    }
}

Personal::~Personal() {}

// ── CRITERIO 4: Override ──
void Personal::mostrarInfo() const {
    std::cout << "  -- Personal --\n"
              << "  ID:           " << id_ << "\n"
              << "  Nombre:       " << nombre_ << "\n"
              << "  Rol:          " << rol_ << "\n"
              << "  Especialidad: " << especialidad_ << "\n"
              << "  Turno:        " << turno_ << "\n"
              << "  Nivel acceso: " << nivelAcceso_ << "\n"
              << "  Activo:       " << (activo_ ? "Si" : "No") << "\n"
              << "  Permisos:     " << permisos_.size() << "\n";
}

std::string Personal::getTipo() const { return "Personal"; }

// ── Getters ──
std::string Personal::getRol()          const { return rol_; }
std::string Personal::getEspecialidad() const { return especialidad_; }
std::string Personal::getTurno()        const { return turno_; }
int         Personal::getNivelAcceso()  const { return nivelAcceso_; }
bool        Personal::isActivo()        const { return activo_; }
const std::set<std::string>& Personal::getPermisos() const { return permisos_; }

// ── Setters ──
bool Personal::setTurno(const std::string& turno) {
    if (TURNOS_VALIDOS.count(turno) == 0) return false;
    turno_ = turno;
    return true;
}

void Personal::setActivo(bool activo) { activo_ = activo; }

bool Personal::tienePermiso(const std::string& permiso) const {
    if (!activo_) return false;
    return permisos_.count(permiso) > 0;
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – Método EXCLUSIVO de Personal (RTTI)
// ═══════════════════════════════════════════════════════════════════════════
void Personal::mostrarPermisos() const {
    std::cout << "\n  == Permisos de " << nombre_
              << " (ID: " << id_ << ", Rol: " << rol_ << ") ==\n"
              << "  Nivel de acceso: " << nivelAcceso_ << "\n"
              << "  Permisos asignados:\n";
    for (const auto& perm : permisos_) {
        std::cout << "    - " << perm << "\n";
    }
    std::cout << "  Estado: " << (activo_ ? "ACTIVO" : "INACTIVO") << "\n";
}

// ── Validaciones estáticas ──
bool Personal::validarRol(const std::string& rol) {
    return ROLES_VALIDOS.count(rol) > 0;
}
bool Personal::validarTurno(const std::string& turno) {
    return TURNOS_VALIDOS.count(turno) > 0;
}

std::string Personal::generarId() {
    contadorId++;
    std::string num = std::to_string(contadorId);
    while (num.length() < 4) num = "0" + num;
    return "P" + num;
}
