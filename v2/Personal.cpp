/*
 * Personal.cpp — Implementación de la clase Personal.
 */

#include "Personal.h"
#include <iostream>

// ── Inicialización de contadores y datos estáticos ──

int Personal::contadorId = 0;

// ── CRITERIO 3: Datos de la jerarquía heredados del diseño V1 ──
const std::map<std::string, int> Personal::JERARQUIA = {
    {"director",       4},
    {"especialista",   3},
    {"medico_general", 2},
    {"enfermera",      1}
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

// ── CRITERIO 1: Constructor con lista de inicialización ──
// ── CRITERIO 3: Delega al constructor de la superclase Persona ──
Personal::Personal(const std::string& id, const std::string& nombre,
                   const std::string& rol, const std::string& especialidad,
                   const std::string& turno, const std::string& telefono)
    : Persona(id, nombre, telefono),  // Delegación a la base
      rol_(rol),
      especialidad_(especialidad),
      turno_(turno),
      nivelAcceso_(0),
      activo_(true),
      permisos_()
{
    // Asignar nivel y permisos según rol jerárquico
    auto itJerarquia = JERARQUIA.find(rol_);
    if (itJerarquia != JERARQUIA.end()) {
        nivelAcceso_ = itJerarquia->second;
        auto itPermisos = PERMISOS_POR_NIVEL.find(nivelAcceso_);
        if (itPermisos != PERMISOS_POR_NIVEL.end()) {
            permisos_ = itPermisos->second;  // Copia del set de permisos
        }
    }
}

Personal::~Personal() {
    // Limpieza automática por contenedores STL
}

// ── CRITERIO 4: Override de mostrarInfo() — enlace dinámico ──
void Personal::mostrarInfo() const {
    std::cout << "  ── Personal ──\n"
              << "  ID:           " << id_ << "\n"          // Atributo protected heredado
              << "  Nombre:       " << nombre_ << "\n"      // CRITERIO 3: reutilización
              << "  Rol:          " << rol_ << "\n"
              << "  Especialidad: " << especialidad_ << "\n"
              << "  Turno:        " << turno_ << "\n"
              << "  Nivel acceso: " << nivelAcceso_ << "\n"
              << "  Activo:       " << (activo_ ? "Si" : "No") << "\n"
              << "  Permisos:     " << permisos_.size() << "\n";
}

// ── CRITERIO 4: Override de getTipo() ──
std::string Personal::getTipo() const {
    return "Personal";
}

// ── CRITERIO 1: Getters ──
std::string Personal::getRol()          const { return rol_; }
std::string Personal::getEspecialidad() const { return especialidad_; }
std::string Personal::getTurno()        const { return turno_; }
int         Personal::getNivelAcceso()  const { return nivelAcceso_; }
bool        Personal::isActivo()        const { return activo_; }
const std::set<std::string>& Personal::getPermisos() const { return permisos_; }

// ── CRITERIO 1: Setter con validación (solo turnos válidos) ──
bool Personal::setTurno(const std::string& turno) {
    if (TURNOS_VALIDOS.count(turno) == 0) return false;
    turno_ = turno;
    return true;
}

void Personal::setActivo(bool activo) {
    activo_ = activo;
}

// ── Control de acceso con set (O(log n) en std::set) ──
bool Personal::tienePermiso(const std::string& permiso) const {
    if (!activo_) return false;
    return permisos_.count(permiso) > 0;
}

bool Personal::tieneMayorNivel(const Personal& otro) const {
    return nivelAcceso_ > otro.nivelAcceso_;
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – MÉTODO EXCLUSIVO DE PERSONAL (para RTTI / dynamic_cast)
// Este método NO existe en la superclase Persona. Solo es invocable
// cuando dynamic_cast<Personal*> confirma el tipo en tiempo de ejecución.
// ═══════════════════════════════════════════════════════════════════════════
void Personal::mostrarPermisos() const {
    std::cout << "\n  ══ Permisos de " << nombre_
              << " (ID: " << id_ << ", Rol: " << rol_ << ") ══\n"
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

// ── Generador de IDs auto-incrementales (P0001, P0002, ...) ──
std::string Personal::generarId() {
    contadorId++;
    std::string id = "P";
    std::string num = std::to_string(contadorId);
    // Padding con ceros hasta 4 dígitos
    while (num.length() < 4) num = "0" + num;
    return id + num;
}
