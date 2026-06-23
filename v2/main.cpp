/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  main.cpp — Punto de entrada del Sistema Hospitalario V2 (C++ / POO)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  Este archivo instancia el controlador SistemaHospitalario (fachada)
 *  y ejecuta la demostración integral que cubre los 5 criterios:
 *
 *    1. Abstracción y Encapsulamiento
 *    2. Arquitectura de Gestores
 *    3. Herencia Simple
 *    4. Polimorfismo y Enlace Dinámico
 *    5. Downcasting Seguro (RTTI)
 *
 *  Compilación (g++ / MinGW):
 *    g++ -std=c++17 -o sistema_hospitalario main.cpp Persona.cpp
 *        Paciente.cpp Personal.cpp Cita.cpp GestorPacientes.cpp
 *        GestorCitas.cpp SistemaHospitalario.cpp
 *
 *  Ejecución:
 *    ./sistema_hospitalario
 */

#include "SistemaHospitalario.h"
#include <iostream>

int main() {
    // ── CRITERIO 2: Se instancia la fachada que orquesta todo el sistema ──
    SistemaHospitalario sistema;

    // ── Ejecutar la demostración integral de los 5 criterios ──
    sistema.ejecutarDemostracion();

    std::cout << "\n  Programa finalizado correctamente.\n"
              << "  (Toda la memoria dinamica fue liberada por los destructores)\n\n";

    return 0;
}
