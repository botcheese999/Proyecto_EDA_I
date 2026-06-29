/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Persistencia.h — Módulo de persistencia CSV (V2.1 NUEVO)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  PERSISTENCIA CSV con <fstream>:
 *    Lectura y escritura de Pacientes, Personal y Citas en archivos .csv.
 *
 *  RAII (Resource Acquisition Is Initialization):
 *    Los objetos std::ifstream y std::ofstream son variables locales dentro
 *    de cada método. Al salir del scope (incluso por excepción), su
 *    destructor cierra automáticamente el archivo. No se necesita close()
 *    manual: esto es el principio RAII aplicado a flujos de archivo.
 *
 *  MANEJO DE EXCEPCIONES:
 *    Se lanzan std::runtime_error al fallar la apertura de archivos y
 *    std::invalid_argument cuando los datos CSV están corruptos.
 *
 *  DATOS DE PRUEBA AUTOMÁTICOS (Plug-and-Play):
 *    Si los archivos CSV no existen en la primera ejecución, se generan
 *    automáticamente con datos mock realistas.
 */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "GestorPacientes.h"
#include "GestorPersonal.h"
#include "GestorCitas.h"
#include <string>

class Persistencia {
private:
    std::string directorio_;
    std::string rutaPacientes_;
    std::string rutaPersonal_;
    std::string rutaCitas_;

    // ── Helpers internos de parseo CSV ──
    static std::vector<std::string> splitCSV(const std::string& linea, char delimitador);
    static std::vector<std::string> splitMultivalor(const std::string& campo, char delimitador);
    static std::string joinMultivalor(const std::set<std::string>& valores, char delimitador);
    static std::string joinMultivalor(const std::vector<std::string>& valores, char delimitador);

public:
    Persistencia(const std::string& directorio = "datos");

    // ── Verificar existencia de archivos ──
    bool existenArchivos() const;

    // ── DATOS DE PRUEBA: genera archivos CSV mock si no existen ──
    // throw std::runtime_error si no puede crear los archivos
    void generarDatosPrueba();

    // ── CARGAR desde CSV ──
    // throw std::runtime_error si el archivo no se puede abrir
    // throw std::invalid_argument si los datos están corruptos
    void cargarPacientes(GestorPacientes& gestor);
    void cargarPersonal(GestorPersonal& gestor);
    void cargarCitas(GestorCitas& gestor);

    // ── GUARDAR a CSV ──
    // throw std::runtime_error si el archivo no se puede abrir
    void guardarPacientes(const GestorPacientes& gestor);
    void guardarPersonal(const GestorPersonal& gestor);
    void guardarCitas(const GestorCitas& gestor);

    // ── Cargar/Guardar todo (conveniencia) ──
    void cargarTodo(GestorPacientes& gp, GestorPersonal& gPers, GestorCitas& gc);
    void guardarTodo(const GestorPacientes& gp, const GestorPersonal& gPers,
                     const GestorCitas& gc);
};

#endif // PERSISTENCIA_H
