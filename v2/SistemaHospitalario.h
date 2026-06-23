/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  SistemaHospitalario.h — Controlador principal (Patrón Fachada).
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  CRITERIO 2 – ARQUITECTURA DE GESTORES:
 *    Actúa como FACHADA que orquesta los gestores (GestorPacientes,
 *    GestorCitas) y coordina operaciones entre subsistemas.
 *
 *  CRITERIO 4 – POLIMORFISMO Y ENLACE DINÁMICO:
 *    Mantiene un std::vector<Persona*> con objetos Paciente y Personal
 *    instanciados con new. Los procesa uniformemente con métodos virtual.
 *
 *  CRITERIO 5 – DOWNCASTING SEGURO (RTTI):
 *    Implementa identificarYProcesar() que usa dynamic_cast para
 *    verificar el tipo concreto y ejecutar métodos exclusivos.
 */

#ifndef SISTEMA_HOSPITALARIO_H
#define SISTEMA_HOSPITALARIO_H

#include "GestorPacientes.h"
#include "GestorCitas.h"
#include "Persona.h"
#include <vector>

// ── CRITERIO 2: Fachada del sistema hospitalario ──
class SistemaHospitalario {
// ── CRITERIO 1: Componentes privados del sistema ──
private:
    GestorPacientes gestorPacientes_;
    GestorCitas gestorCitas_;

    // ── CRITERIO 4: Vector de punteros base para polimorfismo ──
    // Almacena Persona* que apuntan a objetos Paciente o Personal
    // instanciados dinámicamente con new.
    std::vector<Persona*> registroGeneral_;

public:
    SistemaHospitalario();
    ~SistemaHospitalario();

    // ── Acceso a los gestores ──
    GestorPacientes& getGestorPacientes();
    GestorCitas& getGestorCitas();

    // ── CRITERIO 4: Agregar al vector polimórfico ──
    // Los objetos se instancian con new y se almacenan como Persona*
    void agregarAlRegistroGeneral(Persona* persona);

    // ── CRITERIO 4: Procesamiento polimórfico uniforme ──
    // Recorre el vector de Persona* e invoca mostrarInfo() y getTipo()
    // mediante enlace dinámico (vtable).
    void listarRegistroGeneral() const;

    // ═══════════════════════════════════════════════════════════════════════
    // CRITERIO 5 – DOWNCASTING SEGURO CON dynamic_cast (RTTI)
    //
    // Recibe un puntero genérico Persona* y utiliza dynamic_cast para
    // verificar de forma segura si el objeto en memoria es de una clase
    // hija específica (Paciente o Personal). Solo si la conversión es
    // exitosa (puntero != nullptr), ejecuta un método exclusivo de la
    // clase hija que NO existe en la superclase.
    // ═══════════════════════════════════════════════════════════════════════
    static void identificarYProcesar(Persona* persona);

    // ── Demostración de todos los criterios ──
    void ejecutarDemostracion();

    // ── Resumen del sistema ──
    void mostrarResumen() const;
};

#endif // SISTEMA_HOSPITALARIO_H
