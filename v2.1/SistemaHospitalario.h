/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  SistemaHospitalario.h — Controlador principal / Fachada (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 2: Fachada que orquesta GestorPacientes, GestorPersonal,
 *              GestorCitas y Persistencia.
 *  CRITERIO 4: vector<Persona*> con enlace dinámico.
 *  CRITERIO 5: identificarYProcesar() con dynamic_cast (RTTI).
 *
 *  V2.1: Integra Persistencia CSV, excepciones, y datos de prueba auto.
 */

#ifndef SISTEMA_HOSPITALARIO_H
#define SISTEMA_HOSPITALARIO_H

#include "GestorPacientes.h"
#include "GestorPersonal.h"
#include "GestorCitas.h"
#include "Persistencia.h"
#include "Persona.h"
#include <vector>

class SistemaHospitalario {
private:
    GestorPacientes gestorPacientes_;
    GestorPersonal gestorPersonal_;
    GestorCitas gestorCitas_;
    Persistencia persistencia_;

    // ── CRITERIO 4: Vector polimórfico de punteros base ──
    std::vector<Persona*> registroGeneral_;

public:
    SistemaHospitalario();
    ~SistemaHospitalario();

    // ── Inicialización con persistencia ──
    // Verifica CSV, genera mock data si no existen, y carga datos.
    // Captura excepciones internamente.
    void inicializar();

    // ── Guardar datos ──
    void guardarDatos();

    // ── Acceso a gestores ──
    GestorPacientes& getGestorPacientes();
    GestorPersonal& getGestorPersonal();
    GestorCitas& getGestorCitas();

    // ── CRITERIO 4: Vector polimórfico ──
    void agregarAlRegistroGeneral(Persona* persona);
    void listarRegistroGeneral() const;

    // ── CRITERIO 5: RTTI con dynamic_cast ──
    static void identificarYProcesar(Persona* persona);

    // ── Demostración integral ──
    void ejecutarDemostracion();
    void mostrarResumen() const;
};

#endif // SISTEMA_HOSPITALARIO_H
