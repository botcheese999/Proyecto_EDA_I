/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Cita.h — Clase que representa una cita médica.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    Atributos privados con getters/setters validados.
 *    Constructor con lista de inicialización.
 */

#ifndef CITA_H
#define CITA_H

#include <string>
#include <vector>
#include <set>

class Cita {
// ── CRITERIO 1: Atributos estrictamente privados ──
private:
    std::string idCita_;
    std::string dniPaciente_;
    std::string idMedico_;
    std::string fecha_;
    std::string estado_;       // pendiente, en_atencion, completada, cancelada
    std::string motivo_;
    std::string notas_;
    std::vector<std::string> medicacionActual_;  // Lista de medicamentos

    // Estados válidos
    static const std::set<std::string> ESTADOS_VALIDOS;

public:
    // ── CRITERIO 1: Constructor con lista de inicialización ──
    Cita(const std::string& idCita, const std::string& dniPaciente,
         const std::string& idMedico, const std::string& fecha,
         const std::string& motivo = "");

    // ── CRITERIO 1: Getters públicos ──
    std::string getIdCita()       const;
    std::string getDniPaciente()  const;
    std::string getIdMedico()     const;
    std::string getFecha()        const;
    std::string getEstado()       const;
    std::string getMotivo()       const;
    std::string getNotas()        const;
    const std::vector<std::string>& getMedicacion() const;

    // ── CRITERIO 1: Setters con validación lógica ──
    bool setEstado(const std::string& estado);
    void setNotas(const std::string& notas);

    // ── Gestión de medicación ──
    bool agregarMedicamento(const std::string& medicamento);
    bool removerMedicamento(const std::string& medicamento);

    // ── Información ──
    void mostrarInfo() const;

    // ── Validación estática ──
    static bool validarEstado(const std::string& estado);

    // ── Contador de IDs ──
    static int contadorId;
    static std::string generarId();
};

#endif // CITA_H
