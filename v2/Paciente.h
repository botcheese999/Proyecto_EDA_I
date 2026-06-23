/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Paciente.h — Clase hija de Persona que representa a un paciente.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 3 – HERENCIA SIMPLE:
 *    Hereda de Persona (id_ = DNI, nombre_, telefono_) y agrega atributos
 *    propios del paciente: tipo de sangre, fecha de nacimiento, alergias,
 *    historial médico, etc.
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    Atributos estrictamente privados con getters/setters validados.
 *
 *  CRITERIO 4 – POLIMORFISMO:
 *    Implementa los métodos virtuales puros mostrarInfo() y getTipo()
 *    usando 'override'.
 *
 *  CRITERIO 5 – DOWNCASTING SEGURO (RTTI):
 *    Método exclusivo mostrarHistorialCompleto() que solo se puede
 *    invocar tras un dynamic_cast exitoso desde Persona*.
 */

#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"
#include <string>
#include <vector>
#include <set>

// Estructura para las entradas del historial médico
struct EntradaHistorial {
    std::string fecha;
    std::string tipo;        // consulta, emergencia, control
    std::string medicoId;
    std::string diagnostico;
    std::vector<std::string> medicacion;
    std::string notas;
};

// ── CRITERIO 3: Paciente hereda de Persona ──
class Paciente : public Persona {
// ── CRITERIO 1: Atributos estrictamente privados ──
private:
    std::string tipoSangre_;
    std::string fechaNacimiento_;
    std::string direccion_;
    std::string email_;
    std::string contactoEmergencia_;
    std::set<std::string> alergias_;               // SET para unicidad
    std::vector<EntradaHistorial> historial_;       // LISTA secuencial

public:
    // ── CRITERIO 1: Constructor con lista de inicialización ──
    Paciente(const std::string& dni, const std::string& nombre,
             const std::string& tipoSangre, const std::string& fechaNacimiento,
             const std::string& telefono = "", const std::string& direccion = "",
             const std::string& email = "", const std::string& contactoEmergencia = "");

    // Destructor (clase hija — base ya tiene virtual)
    ~Paciente() override;

    // ── CRITERIO 4: Override de métodos virtuales puros (enlace dinámico) ──
    void mostrarInfo() const override;
    std::string getTipo() const override;

    // ── CRITERIO 1: Getters públicos ──
    std::string getTipoSangre()         const;
    std::string getFechaNacimiento()    const;
    std::string getDireccion()          const;
    std::string getEmail()              const;
    std::string getContactoEmergencia() const;
    const std::set<std::string>& getAlergias()         const;
    const std::vector<EntradaHistorial>& getHistorial() const;

    // ── CRITERIO 1: Setters con validaciones lógicas ──
    bool setDireccion(const std::string& direccion);
    bool setEmail(const std::string& email);
    bool setContactoEmergencia(const std::string& contacto);

    // ── Gestión de alergias (operaciones con set) ──
    bool agregarAlergia(const std::string& alergia);
    bool removerAlergia(const std::string& alergia);
    bool tieneAlergia(const std::string& alergia) const;

    // ── Gestión de historial ──
    void agregarEntradaHistorial(const EntradaHistorial& entrada);

    // ── CRITERIO 5: Método EXCLUSIVO de Paciente para RTTI/dynamic_cast ──
    // Solo accesible tras un dynamic_cast<Paciente*> exitoso
    void mostrarHistorialCompleto() const;

    // ── Validaciones estáticas ──
    static bool validarDni(const std::string& dni);
    static bool validarTipoSangre(const std::string& tipo);
};

#endif // PACIENTE_H
