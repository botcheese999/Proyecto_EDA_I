/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  GestorPersonal.h — Lógica de negocio para personal médico (V2.1 NUEVO)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 2: Gestor dedicado para el personal hospitalario.
 *  Nuevo en V2.1 — en V2 el personal se gestionaba directamente en la fachada.
 */

#ifndef GESTOR_PERSONAL_H
#define GESTOR_PERSONAL_H

#include "Personal.h"
#include <map>
#include <string>
#include <vector>
#include <utility>

class GestorPersonal {
private:
    std::map<std::string, Personal*> catalogo_;

public:
    GestorPersonal();
    ~GestorPersonal();

    // ── CRUD ──
    std::pair<bool, std::string> registrar(
        const std::string& nombre, const std::string& rol,
        const std::string& especialidad = "General",
        const std::string& turno = "mañana",
        const std::string& telefono = "");

    // Agregar personal ya construido (para carga CSV). Toma ownership.
    bool agregarPersonal(Personal* personal);

    Personal* buscar(const std::string& id) const;
    std::vector<std::pair<std::string, std::string>> listar() const;

    // ── Acceso al catálogo completo (para persistencia CSV) ──
    const std::map<std::string, Personal*>& getCatalogo() const;
    int getCantidad() const;
};

#endif // GESTOR_PERSONAL_H
