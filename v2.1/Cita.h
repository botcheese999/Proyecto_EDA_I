/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Cita.h — Entidad de cita médica simplificada (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CAMBIOS V2.1:
 *    - ELIMINADO: Pila LIFO de deshacer medicación (pilaDeshacer_).
 *    - La medicación se mantiene como dato simple, sin mecanismo de undo.
 *
 *  CRITERIO 1: Atributos privados, constructor con lista de inicialización,
 *              getters/setters con validación.
 */

#ifndef CITA_H
#define CITA_H

#include <string>
#include <vector>
#include <set>

class Cita {
private:
    std::string idCita_;
    std::string dniPaciente_;
    std::string idMedico_;
    std::string fecha_;
    std::string estado_;
    std::string motivo_;
    std::string notas_;
    std::vector<std::string> medicacion_;

    static const std::set<std::string> ESTADOS_VALIDOS;

public:
    Cita(const std::string& idCita, const std::string& dniPaciente,
         const std::string& idMedico, const std::string& fecha,
         const std::string& estado = "pendiente",
         const std::string& motivo = "", const std::string& notas = "");

    // ── Getters ──
    std::string getIdCita()       const;
    std::string getDniPaciente()  const;
    std::string getIdMedico()     const;
    std::string getFecha()        const;
    std::string getEstado()       const;
    std::string getMotivo()       const;
    std::string getNotas()        const;
    const std::vector<std::string>& getMedicacion() const;

    // ── Setters con validación ──
    bool setEstado(const std::string& estado);
    void setNotas(const std::string& notas);

    // ── Medicación (simple, sin undo) ──
    void agregarMedicamento(const std::string& medicamento);
    void setMedicacion(const std::vector<std::string>& meds);

    void mostrarInfo() const;

    static bool validarEstado(const std::string& estado);
    static int contadorId;
    static std::string generarId();
};

#endif // CITA_H
