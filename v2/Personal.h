/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Personal.h — Clase hija de Persona que representa al personal médico.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 3 – HERENCIA SIMPLE:
 *    Hereda de Persona (id_, nombre_, telefono_) y agrega rol, especialidad,
 *    turno, nivel de acceso y permisos.
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    Atributos privados, getters/setters validados.
 *
 *  CRITERIO 4 – POLIMORFISMO:
 *    Override de mostrarInfo() y getTipo().
 *
 *  CRITERIO 5 – DOWNCASTING SEGURO (RTTI):
 *    Método exclusivo mostrarPermisos() accesible solo vía dynamic_cast.
 */

#ifndef PERSONAL_H
#define PERSONAL_H

#include "Persona.h"
#include <string>
#include <set>
#include <map>

// ── CRITERIO 3: Personal hereda de Persona ──
class Personal : public Persona {
// ── CRITERIO 1: Atributos estrictamente privados ──
private:
    std::string rol_;          // director, especialista, medico_general, enfermera
    std::string especialidad_;
    std::string turno_;        // mañana, tarde, noche
    int nivelAcceso_;
    bool activo_;
    std::set<std::string> permisos_;  // SET para verificación O(1)

    // ── Datos de configuración interna ──
    static const std::map<std::string, int> JERARQUIA;
    static const std::map<int, std::set<std::string>> PERMISOS_POR_NIVEL;
    static const std::set<std::string> ROLES_VALIDOS;
    static const std::set<std::string> TURNOS_VALIDOS;

public:
    // ── CRITERIO 1: Constructor con lista de inicialización ──
    Personal(const std::string& id, const std::string& nombre,
             const std::string& rol, const std::string& especialidad = "General",
             const std::string& turno = "mañana", const std::string& telefono = "");

    // Destructor
    ~Personal() override;

    // ── CRITERIO 4: Override de métodos virtuales (enlace dinámico) ──
    void mostrarInfo() const override;
    std::string getTipo() const override;

    // ── CRITERIO 1: Getters públicos ──
    std::string getRol()          const;
    std::string getEspecialidad() const;
    std::string getTurno()        const;
    int         getNivelAcceso()  const;
    bool        isActivo()        const;
    const std::set<std::string>& getPermisos() const;

    // ── CRITERIO 1: Setters con validación lógica ──
    bool setTurno(const std::string& turno);
    void setActivo(bool activo);

    // ── Control de acceso ──
    bool tienePermiso(const std::string& permiso) const;
    bool tieneMayorNivel(const Personal& otro)    const;

    // ── CRITERIO 5: Método EXCLUSIVO de Personal para RTTI/dynamic_cast ──
    // Solo accesible tras un dynamic_cast<Personal*> exitoso
    void mostrarPermisos() const;

    // ── Validaciones estáticas ──
    static bool validarRol(const std::string& rol);
    static bool validarTurno(const std::string& turno);

    // ── Contador de IDs auto-incrementales ──
    static int contadorId;
    static std::string generarId();
};

#endif // PERSONAL_H
