/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  GestorPacientes.h — Lógica de negocio para pacientes (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CAMBIOS V2.1: Eliminado historial clínico y métodos asociados.
 *
 *  CRITERIO 2: Gestor de lógica de negocio separada de la entidad.
 *  CRITERIO 1: Colección privada, acceso solo vía métodos públicos.
 */

#ifndef GESTOR_PACIENTES_H
#define GESTOR_PACIENTES_H

#include "Paciente.h"
#include <map>
#include <string>
#include <vector>
#include <utility>

class GestorPacientes {
private:
    std::map<std::string, Paciente*> catalogo_;

public:
    GestorPacientes();
    ~GestorPacientes();

    // ── CRUD ──
    std::pair<bool, std::string> registrar(
        const std::string& dni, const std::string& nombre,
        const std::string& tipoSangre, const std::string& fechaNacimiento,
        const std::string& telefono = "", const std::string& direccion = "",
        const std::string& email = "", const std::string& contactoEmergencia = "");

    // Agregar un paciente ya construido (para carga CSV). Toma ownership.
    bool agregarPaciente(Paciente* paciente);

    Paciente* buscar(const std::string& dni) const;
    std::pair<bool, std::string> eliminar(const std::string& dni);
    std::vector<std::pair<std::string, std::string>> listar() const;

    // ── Actualizaciones ──
    std::pair<bool, std::string> actualizarEmail(const std::string& dni,
                                                  const std::string& valor);

    // ── Alergias ──
    std::pair<bool, std::string> agregarAlergia(const std::string& dni,
                                                 const std::string& alergia);
    std::set<std::string> cruzarAlergias(const std::string& dni1,
                                          const std::string& dni2) const;

    // ── Acceso al catálogo completo (para persistencia CSV) ──
    const std::map<std::string, Paciente*>& getCatalogo() const;
    int getCantidad() const;
};

#endif // GESTOR_PACIENTES_H
