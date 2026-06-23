/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  GestorPacientes.h — Clase de lógica de negocio para pacientes.
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 2 – ARQUITECTURA DE GESTORES:
 *    Clase de lógica de negocio que encapsula las operaciones CRUD y
 *    de búsqueda sobre la colección de pacientes. Separa la lógica
 *    de negocio de las entidades de datos.
 *
 *  CRITERIO 1 – ABSTRACCIÓN Y ENCAPSULAMIENTO:
 *    El mapa interno es privado; se accede solo vía métodos públicos.
 */

#ifndef GESTOR_PACIENTES_H
#define GESTOR_PACIENTES_H

#include "Paciente.h"
#include <map>
#include <string>
#include <vector>
#include <utility> // pair

// ── CRITERIO 2: Gestor de lógica de negocio para pacientes ──
class GestorPacientes {
// ── CRITERIO 1: Colección privada ──
private:
    std::map<std::string, Paciente*> catalogo_;  // DNI → Paciente*

public:
    GestorPacientes();
    ~GestorPacientes();

    // ── CRUD ──
    std::pair<bool, std::string> registrar(
        const std::string& dni, const std::string& nombre,
        const std::string& tipoSangre, const std::string& fechaNacimiento,
        const std::string& telefono = "", const std::string& direccion = "",
        const std::string& email = "", const std::string& contactoEmergencia = "");

    Paciente* buscar(const std::string& dni) const;
    std::pair<bool, std::string> eliminar(const std::string& dni);
    std::vector<std::pair<std::string, std::string>> listar() const;

    // ── Datos mutables ──
    std::pair<bool, std::string> actualizarTelefono(const std::string& dni,
                                                     const std::string& valor);
    std::pair<bool, std::string> actualizarDireccion(const std::string& dni,
                                                      const std::string& valor);
    std::pair<bool, std::string> actualizarEmail(const std::string& dni,
                                                  const std::string& valor);
    std::pair<bool, std::string> actualizarContactoEmergencia(const std::string& dni,
                                                               const std::string& valor);

    // ── Alergias ──
    std::pair<bool, std::string> agregarAlergia(const std::string& dni,
                                                 const std::string& alergia);
    std::pair<bool, std::string> removerAlergia(const std::string& dni,
                                                 const std::string& alergia);
    std::set<std::string> cruzarAlergias(const std::string& dni1,
                                          const std::string& dni2) const;

    // ── Cantidad ──
    int getCantidad() const;
};

#endif // GESTOR_PACIENTES_H
