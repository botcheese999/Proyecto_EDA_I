/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Persona.h — Superclase abstracta de la jerarquía de herencia (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 3 – HERENCIA SIMPLE:
 *    Clase base con atributos comunes (protected) heredados por
 *    Paciente y Personal para maximizar la reutilización de código.
 *
 *  CRITERIO 4 – POLIMORFISMO Y ENLACE DINÁMICO:
 *    Destructor virtual y métodos virtuales puros (mostrarInfo, getTipo).
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    Atributos protected para subclases; externos usan getters públicos.
 */

#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>

class Persona {
// ── CRITERIO 1 + CRITERIO 3: protected para acceso desde subclases ──
protected:
    std::string id_;
    std::string nombre_;
    std::string telefono_;

public:
    // ── CRITERIO 1: Constructor seguro con lista de inicialización ──
    Persona(const std::string& id, const std::string& nombre,
            const std::string& telefono);

    // ── CRITERIO 4: Destructor virtual obligatorio ──
    virtual ~Persona();

    // ── CRITERIO 4: Métodos virtuales puros → enlace dinámico ──
    virtual void mostrarInfo() const = 0;
    virtual std::string getTipo() const = 0;

    // ── CRITERIO 1: Getters públicos ──
    std::string getId()       const;
    std::string getNombre()   const;
    std::string getTelefono() const;

    // ── CRITERIO 1: Setters con validación lógica ──
    bool setTelefono(const std::string& telefono);
    bool setNombre(const std::string& nombre);
};

#endif // PERSONA_H
