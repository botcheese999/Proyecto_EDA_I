/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  GestorCitas.h — Lógica de negocio para citas (V2.1 SIMPLIFICADO)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CAMBIOS V2.1:
 *    - ELIMINADO: listaEspera_ (Cola FIFO), pilaDeshacer_ (Pila LIFO).
 *    - ELIMINADO: EntradaEspera, AccionMedicacion, y todos sus métodos.
 *    - Solo CRUD de citas + consultas por filtro.
 *
 *  CRITERIO 2: Gestor separado de la entidad Cita.
 */

#ifndef GESTOR_CITAS_H
#define GESTOR_CITAS_H

#include "Cita.h"
#include <map>
#include <vector>
#include <string>
#include <utility>

class GestorCitas {
private:
    std::map<std::string, Cita*> registroCitas_;

public:
    GestorCitas();
    ~GestorCitas();

    // ── CRUD ──
    std::pair<bool, std::string> crearCita(const std::string& dniPaciente,
                                            const std::string& idMedico,
                                            const std::string& motivo = "");

    // Agregar cita ya construida (para carga CSV). Toma ownership.
    bool agregarCita(Cita* cita);

    Cita* buscarCita(const std::string& idCita) const;

    std::pair<bool, std::string> completarCita(const std::string& idCita,
                                                const std::string& notas = "");
    std::pair<bool, std::string> cancelarCita(const std::string& idCita,
                                               const std::string& motivoCancelacion);

    // ── Consultas ──
    std::vector<Cita*> listarPorEstado(const std::string& estado) const;
    std::vector<Cita*> listarPorPaciente(const std::string& dni) const;

    // ── Acceso al catálogo completo (para persistencia CSV) ──
    const std::map<std::string, Cita*>& getRegistro() const;
    int getCantidad() const;
};

#endif // GESTOR_CITAS_H
