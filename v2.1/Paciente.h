/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Paciente.h — Clase hija de Persona (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CAMBIOS V2.1:
 *    - ELIMINADO: EntradaHistorial, historial_, agregarEntradaHistorial(),
 *      mostrarHistorialCompleto(). Se removió la lista/historial clínico.
 *    - NUEVO método RTTI exclusivo: mostrarFichaMedica()
 *
 *  CRITERIO 3: Hereda de Persona (id_, nombre_, telefono_).
 *  CRITERIO 1: Atributos private, getters/setters validados.
 *  CRITERIO 4: Override de mostrarInfo() y getTipo().
 *  CRITERIO 5: mostrarFichaMedica() solo accesible vía dynamic_cast.
 */

#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"
#include <string>
#include <set>

// ── CRITERIO 3: Paciente hereda de Persona ──
class Paciente : public Persona {
// ── CRITERIO 1: Atributos estrictamente privados ──
private:
    std::string tipoSangre_;
    std::string fechaNacimiento_;
    std::string direccion_;
    std::string email_;
    std::string contactoEmergencia_;
    std::set<std::string> alergias_;  // SET para unicidad

public:
    // ── CRITERIO 1: Constructor con lista de inicialización ──
    Paciente(const std::string& dni, const std::string& nombre,
             const std::string& tipoSangre, const std::string& fechaNacimiento,
             const std::string& telefono = "", const std::string& direccion = "",
             const std::string& email = "", const std::string& contactoEmergencia = "");

    ~Paciente() override;

    // ── CRITERIO 4: Override — enlace dinámico ──
    void mostrarInfo() const override;
    std::string getTipo() const override;

    // ── CRITERIO 1: Getters ──
    std::string getTipoSangre()         const;
    std::string getFechaNacimiento()    const;
    std::string getDireccion()          const;
    std::string getEmail()              const;
    std::string getContactoEmergencia() const;
    const std::set<std::string>& getAlergias() const;

    // ── CRITERIO 1: Setters con validación ──
    bool setDireccion(const std::string& direccion);
    bool setEmail(const std::string& email);
    bool setContactoEmergencia(const std::string& contacto);

    // ── Alergias (set) ──
    bool agregarAlergia(const std::string& alergia);
    bool removerAlergia(const std::string& alergia);
    bool tieneAlergia(const std::string& alergia) const;

    // ── CRITERIO 5: Método EXCLUSIVO para RTTI / dynamic_cast ──
    // Muestra la ficha médica completa (NO existe en Persona).
    void mostrarFichaMedica() const;

    // ── Validaciones estáticas ──
    static bool validarDni(const std::string& dni);
    static bool validarTipoSangre(const std::string& tipo);
};

#endif // PACIENTE_H
