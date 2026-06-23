/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Persona.h — Superclase abstracta de la jerarquía de herencia.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 3 – HERENCIA SIMPLE:
 *    Clase base con atributos comunes (protected) que serán heredados
 *    por Paciente y Personal, maximizando la reutilización de código.
 *
 *  CRITERIO 4 – POLIMORFISMO Y ENLACE DINÁMICO:
 *    Destructor virtual y métodos virtuales puros (mostrarInfo, getTipo)
 *    que permiten el procesamiento uniforme mediante punteros base.
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    Los atributos son 'protected' para que las subclases accedan,
 *    pero externos usen getters públicos. Constructor con lista de
 *    inicialización.
 */

#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>

class Persona {
// ── CRITERIO 1: Atributos encapsulados ──
// ── CRITERIO 3: 'protected' para acceso desde subclases Paciente y Personal ──
protected:
    std::string id_;          // DNI (paciente) o ID autogenerado (personal)
    std::string nombre_;
    std::string telefono_;

public:
    // ── CRITERIO 1: Constructor seguro con lista de inicialización ──
    Persona(const std::string& id, const std::string& nombre,
            const std::string& telefono);

    // ── CRITERIO 4: Destructor virtual obligatorio para herencia polimórfica ──
    virtual ~Persona();

    // ── CRITERIO 4: Métodos virtuales puros → enlace dinámico ──
    virtual void mostrarInfo() const = 0;
    virtual std::string getTipo() const = 0;

    // ── CRITERIO 1: Getters públicos (interfaz de acceso seguro) ──
    std::string getId()       const;
    std::string getNombre()   const;
    std::string getTelefono() const;

    // ── CRITERIO 1: Setters con validación lógica ──
    bool setTelefono(const std::string& telefono);
    bool setNombre(const std::string& nombre);
};

#endif // PERSONA_H
