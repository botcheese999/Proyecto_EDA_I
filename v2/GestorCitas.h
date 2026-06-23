/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  GestorCitas.h — Clase de lógica de negocio para citas médicas.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 2 – ARQUITECTURA DE GESTORES:
 *    Gestiona citas, lista de espera (FIFO) y pila de deshacer
 *    medicación (LIFO). Separa la lógica de negocio de la entidad Cita.
 */

#ifndef GESTOR_CITAS_H
#define GESTOR_CITAS_H

#include "Cita.h"
#include "Paciente.h"
#include "Personal.h"
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <tuple>

// Estructura para la pila de deshacer medicación
struct AccionMedicacion {
    std::string citaId;
    std::string medicamento;
    std::string medicoId;
    std::string timestamp;
};

// Estructura para la lista de espera
struct EntradaEspera {
    std::string timestamp;
    std::string dni;
    std::string motivo;
};

// ── CRITERIO 2: Gestor de lógica de negocio para citas ──
class GestorCitas {
// ── CRITERIO 1: Colecciones privadas ──
private:
    std::map<std::string, Cita*> registroCitas_;    // ID → Cita*
    std::vector<EntradaEspera> listaEspera_;         // FIFO
    std::vector<AccionMedicacion> pilaDeshacer_;      // LIFO

public:
    GestorCitas();
    ~GestorCitas();

    // ── Lista de Espera (FIFO) ──
    std::pair<bool, std::string> agregarAEspera(const std::string& dni,
                                                 const std::string& motivo);
    std::pair<bool, EntradaEspera> atenderSiguiente();
    std::vector<EntradaEspera> verListaEspera() const;
    int cantidadEnEspera() const;

    // ── CRUD de Citas ──
    std::pair<bool, std::string> crearCita(const std::string& dniPaciente,
                                            const std::string& idMedico,
                                            const std::string& motivo = "");
    Cita* buscarCita(const std::string& idCita) const;
    std::pair<bool, std::string> completarCita(const std::string& idCita,
                                                const std::string& diagnostico,
                                                const std::string& notas,
                                                Paciente* paciente);
    std::pair<bool, std::string> cancelarCita(const std::string& idCita,
                                               const std::string& motivoCancelacion);

    // ── Medicación con deshacer (LIFO) ──
    std::pair<bool, std::string> agregarMedicacion(const std::string& idCita,
                                                    const std::string& medicamento,
                                                    const std::string& idMedico);
    std::pair<bool, std::string> deshacerMedicacion(const Personal* medicoSolicitante);

    // ── Consultas ──
    std::vector<Cita*> listarPorEstado(const std::string& estado) const;
    std::vector<Cita*> listarPorPaciente(const std::string& dni) const;
    int getCantidad() const;
};

#endif // GESTOR_CITAS_H
