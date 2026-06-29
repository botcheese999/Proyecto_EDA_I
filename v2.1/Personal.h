/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Personal.h — Clase hija de Persona: personal hospitalario (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 3: Hereda de Persona (id_, nombre_, telefono_).
 *  CRITERIO 1: Atributos private, getters/setters validados.
 *  CRITERIO 4: Override de mostrarInfo() y getTipo().
 *  CRITERIO 5: mostrarPermisos() exclusivo para RTTI/dynamic_cast.
 */

#ifndef PERSONAL_H
#define PERSONAL_H

#include "Persona.h"
#include <string>
#include <set>
#include <map>

// ── CRITERIO 3: Personal hereda de Persona ──
class Personal : public Persona {
private:
    std::string rol_;
    std::string especialidad_;
    std::string turno_;
    int nivelAcceso_;
    bool activo_;
    std::set<std::string> permisos_;

    static const std::map<std::string, int> JERARQUIA;
    static const std::map<int, std::set<std::string>> PERMISOS_POR_NIVEL;
    static const std::set<std::string> ROLES_VALIDOS;
    static const std::set<std::string> TURNOS_VALIDOS;

public:
    // ── CRITERIO 1: Constructor con lista de inicialización ──
    Personal(const std::string& id, const std::string& nombre,
             const std::string& rol, const std::string& especialidad = "General",
             const std::string& turno = "mañana",
             const std::string& telefono = "", bool activo = true);

    ~Personal() override;

    // ── CRITERIO 4: Override ──
    void mostrarInfo() const override;
    std::string getTipo() const override;

    // ── Getters ──
    std::string getRol()          const;
    std::string getEspecialidad() const;
    std::string getTurno()        const;
    int         getNivelAcceso()  const;
    bool        isActivo()        const;
    const std::set<std::string>& getPermisos() const;

    // ── Setters con validación ──
    bool setTurno(const std::string& turno);
    void setActivo(bool activo);

    // ── Control de acceso ──
    bool tienePermiso(const std::string& permiso) const;

    // ── CRITERIO 5: Método EXCLUSIVO para RTTI ──
    void mostrarPermisos() const;

    // ── Validaciones estáticas ──
    static bool validarRol(const std::string& rol);
    static bool validarTurno(const std::string& turno);

    // ── ID auto-incremental ──
    static int contadorId;
    static std::string generarId();
};

#endif // PERSONAL_H
