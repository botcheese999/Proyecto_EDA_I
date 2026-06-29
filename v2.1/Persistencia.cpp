/*
 * Persistencia.cpp — Implementación de persistencia CSV (V2.1 NUEVO)
 *
 * RAII: Todos los std::ifstream / std::ofstream son variables locales.
 * Su destructor cierra el archivo automáticamente al salir del scope,
 * incluso si se lanza una excepción. No se necesita close() manual.
 *
 * EXCEPCIONES:
 *   - std::runtime_error → fallo al abrir archivos
 *   - std::invalid_argument → datos CSV corruptos (campos insuficientes)
 */

#include "Persistencia.h"
#include <fstream>    // ifstream, ofstream — flujos de archivo
#include <sstream>
#include <stdexcept>  // runtime_error, invalid_argument
#include <iostream>
#include <algorithm>

// ── Crear directorio (cross-platform) ──
#ifdef _WIN32
#include <direct.h>
#define CREAR_DIR(d) _mkdir(d)
#else
#include <sys/stat.h>
#define CREAR_DIR(d) mkdir(d, 0755)
#endif

// ═══════════════════════════════════════════════════════════════════════════
// HELPERS DE PARSEO CSV
// ═══════════════════════════════════════════════════════════════════════════

std::vector<std::string> Persistencia::splitCSV(const std::string& linea,
                                                 char delimitador) {
    std::vector<std::string> campos;
    std::stringstream ss(linea);
    std::string campo;
    while (std::getline(ss, campo, delimitador)) {
        campos.push_back(campo);
    }
    return campos;
}

std::vector<std::string> Persistencia::splitMultivalor(const std::string& campo,
                                                        char delimitador) {
    std::vector<std::string> valores;
    if (campo.empty()) return valores;
    std::stringstream ss(campo);
    std::string valor;
    while (std::getline(ss, valor, delimitador)) {
        if (!valor.empty()) valores.push_back(valor);
    }
    return valores;
}

std::string Persistencia::joinMultivalor(const std::set<std::string>& valores,
                                          char delimitador) {
    std::string resultado;
    bool first = true;
    for (const auto& v : valores) {
        if (!first) resultado += delimitador;
        resultado += v;
        first = false;
    }
    return resultado;
}

std::string Persistencia::joinMultivalor(const std::vector<std::string>& valores,
                                          char delimitador) {
    std::string resultado;
    for (size_t i = 0; i < valores.size(); ++i) {
        if (i > 0) resultado += delimitador;
        resultado += valores[i];
    }
    return resultado;
}

// ═══════════════════════════════════════════════════════════════════════════
// CONSTRUCTOR
// ═══════════════════════════════════════════════════════════════════════════

Persistencia::Persistencia(const std::string& directorio)
    : directorio_(directorio),
      rutaPacientes_(directorio + "/pacientes.csv"),
      rutaPersonal_(directorio + "/personal.csv"),
      rutaCitas_(directorio + "/citas.csv")
{
    // Intentar crear el directorio (ignora error si ya existe)
    CREAR_DIR(directorio_.c_str());
}

// ═══════════════════════════════════════════════════════════════════════════
// VERIFICAR EXISTENCIA
// ═══════════════════════════════════════════════════════════════════════════

bool Persistencia::existenArchivos() const {
    // ── RAII: ifstream se cierra automáticamente al salir del scope ──
    std::ifstream f1(rutaPacientes_);
    std::ifstream f2(rutaPersonal_);
    std::ifstream f3(rutaCitas_);
    return f1.good() && f2.good() && f3.good();
    // Los 3 ifstream se destruyen aquí → archivos cerrados automáticamente
}

// ═══════════════════════════════════════════════════════════════════════════
// DATOS DE PRUEBA AUTOMÁTICOS (Plug-and-Play)
// Genera archivos CSV con mock data realista para demostración inmediata.
// ═══════════════════════════════════════════════════════════════════════════

void Persistencia::generarDatosPrueba() {
    // ── RAII: ofstream se cierra automáticamente al salir del scope ──
    // Si throw ocurre, el destructor de ofstream cierra el archivo igualmente.

    // ── pacientes.csv ──
    {
        std::ofstream archivo(rutaPacientes_);
        if (!archivo.is_open()) {
            // ── EXCEPCIÓN: fallo al abrir archivo ──
            throw std::runtime_error(
                "Error critico: No se pudo crear el archivo '" + rutaPacientes_
                + "'. Verifique permisos del directorio.");
        }

        archivo << "dni,nombre,tipoSangre,fechaNacimiento,telefono,direccion,email,contactoEmergencia,alergias\n"
                << "12345678,Maria Garcia Lopez,O+,1990-05-15,987654321,Av. Lima 123,maria@email.com,Juan Garcia,penicilina;aspirina\n"
                << "87654321,Carlos Perez Ruiz,A+,1985-03-20,912345678,Jr. Cusco 456,carlos@email.com,Ana Perez,ibuprofeno\n"
                << "11223344,Ana Torres Vega,AB+,1992-08-10,999000111,Calle Sol 789,ana@email.com,Pedro Torres,\n"
                << "55667788,Luis Mendoza Rios,B-,1988-12-01,955000222,Av. Mar 101,luis@email.com,Rosa Mendoza,penicilina;sulfamidas\n"
                << "99887766,Sofia Ramirez Cruz,O-,1995-04-25,944000333,Jr. Luna 202,sofia@email.com,Miguel Ramirez,\n";
        // ofstream::~ofstream() cierra el archivo aquí (RAII)
    }

    // ── personal.csv ──
    {
        std::ofstream archivo(rutaPersonal_);
        if (!archivo.is_open()) {
            throw std::runtime_error(
                "Error critico: No se pudo crear el archivo '" + rutaPersonal_ + "'.");
        }

        archivo << "id,nombre,rol,especialidad,turno,telefono,activo\n"
                << "P0001,Dr. Juan Martinez,director,Administracion,mañana,955111222,1\n"
                << "P0002,Dra. Laura Sanchez,especialista,Cardiologia,mañana,955333444,1\n"
                << "P0003,Dr. Roberto Diaz,medico_general,General,tarde,955555666,1\n"
                << "P0004,Enf. Carmen Soto,enfermera,General,noche,955777888,1\n";
    }

    // ── citas.csv ──
    {
        std::ofstream archivo(rutaCitas_);
        if (!archivo.is_open()) {
            throw std::runtime_error(
                "Error critico: No se pudo crear el archivo '" + rutaCitas_ + "'.");
        }

        archivo << "idCita,dniPaciente,idMedico,fecha,estado,motivo,notas,medicacion\n"
                << "C0001,12345678,P0002,2025-06-20,completada,Control cardiologico,Paciente estable,Losartan;Aspirina\n"
                << "C0002,87654321,P0003,2025-06-21,pendiente,Dolor de cabeza,,,\n"
                << "C0003,11223344,P0002,2025-06-22,cancelada,Control rutinario,Cancelada por paciente,\n"
                << "C0004,55667788,P0003,2025-06-23,pendiente,Revision general,,Paracetamol\n";
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CARGAR DESDE CSV
// ═══════════════════════════════════════════════════════════════════════════

void Persistencia::cargarPacientes(GestorPacientes& gestor) {
    // ── RAII: ifstream se cierra automáticamente al salir del método ──
    std::ifstream archivo(rutaPacientes_);

    if (!archivo.is_open()) {
        // ── EXCEPCIÓN: archivo no accesible ──
        throw std::runtime_error(
            "No se pudo abrir '" + rutaPacientes_ + "' para lectura.");
    }

    std::string linea;
    std::getline(archivo, linea);  // Saltar header

    int lineaNum = 1;
    while (std::getline(archivo, linea)) {
        lineaNum++;
        if (linea.empty()) continue;

        auto campos = splitCSV(linea, ',');

        // ── EXCEPCIÓN: datos corruptos (campos insuficientes) ──
        if (campos.size() < 8) {
            throw std::invalid_argument(
                "Datos corruptos en '" + rutaPacientes_ + "' linea "
                + std::to_string(lineaNum) + ": se esperaban al menos 8 campos, "
                + "se encontraron " + std::to_string(campos.size()) + ".");
        }

        // Crear paciente con new (instanciación dinámica)
        Paciente* p = new Paciente(
            campos[0], campos[1], campos[2], campos[3],
            campos[4], campos[5], campos[6], campos[7]);

        // Cargar alergias (campo 9, separadas por ';')
        if (campos.size() > 8 && !campos[8].empty()) {
            auto alergias = splitMultivalor(campos[8], ';');
            for (const auto& a : alergias) {
                p->agregarAlergia(a);
            }
        }

        gestor.agregarPaciente(p);
    }
    // archivo se cierra automáticamente aquí (RAII: destructor de ifstream)
}

void Persistencia::cargarPersonal(GestorPersonal& gestor) {
    std::ifstream archivo(rutaPersonal_);  // RAII

    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir '" + rutaPersonal_ + "' para lectura.");
    }

    std::string linea;
    std::getline(archivo, linea);  // Saltar header

    int lineaNum = 1;
    int maxId = 0;

    while (std::getline(archivo, linea)) {
        lineaNum++;
        if (linea.empty()) continue;

        auto campos = splitCSV(linea, ',');

        if (campos.size() < 7) {
            throw std::invalid_argument(
                "Datos corruptos en '" + rutaPersonal_ + "' linea "
                + std::to_string(lineaNum) + ": se esperaban 7 campos, "
                + "se encontraron " + std::to_string(campos.size()) + ".");
        }

        bool activo = (campos[6] == "1");

        Personal* p = new Personal(
            campos[0], campos[1], campos[2], campos[3],
            campos[4], campos[5], activo);

        gestor.agregarPersonal(p);

        // Actualizar el contador de IDs para evitar colisiones
        // Extraer número del ID (ej: "P0003" → 3)
        std::string idNum = campos[0].substr(1);
        int num = std::stoi(idNum);
        if (num > maxId) maxId = num;
    }

    // Sincronizar el contador estático
    if (maxId >= Personal::contadorId) {
        Personal::contadorId = maxId;
    }
    // archivo cerrado por RAII
}

void Persistencia::cargarCitas(GestorCitas& gestor) {
    std::ifstream archivo(rutaCitas_);  // RAII

    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir '" + rutaCitas_ + "' para lectura.");
    }

    std::string linea;
    std::getline(archivo, linea);  // Saltar header

    int lineaNum = 1;
    int maxId = 0;

    while (std::getline(archivo, linea)) {
        lineaNum++;
        if (linea.empty()) continue;

        auto campos = splitCSV(linea, ',');

        if (campos.size() < 7) {
            throw std::invalid_argument(
                "Datos corruptos en '" + rutaCitas_ + "' linea "
                + std::to_string(lineaNum) + ": se esperaban al menos 7 campos, "
                + "se encontraron " + std::to_string(campos.size()) + ".");
        }

        Cita* c = new Cita(campos[0], campos[1], campos[2], campos[3],
                            campos[4], campos[5],
                            (campos.size() > 6) ? campos[6] : "");

        // Cargar medicación (campo 8, separada por ';')
        if (campos.size() > 7 && !campos[7].empty()) {
            auto meds = splitMultivalor(campos[7], ';');
            c->setMedicacion(meds);
        }

        gestor.agregarCita(c);

        // Actualizar contador
        std::string idNum = campos[0].substr(1);
        int num = std::stoi(idNum);
        if (num > maxId) maxId = num;
    }

    if (maxId >= Cita::contadorId) {
        Cita::contadorId = maxId;
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// GUARDAR A CSV
// ═══════════════════════════════════════════════════════════════════════════

void Persistencia::guardarPacientes(const GestorPacientes& gestor) {
    // ── RAII: ofstream variable local → cierre automático ──
    std::ofstream archivo(rutaPacientes_);

    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir '" + rutaPacientes_ + "' para escritura.");
    }

    archivo << "dni,nombre,tipoSangre,fechaNacimiento,telefono,direccion,email,contactoEmergencia,alergias\n";

    for (const auto& par : gestor.getCatalogo()) {
        const Paciente* p = par.second;
        archivo << p->getId() << ","
                << p->getNombre() << ","
                << p->getTipoSangre() << ","
                << p->getFechaNacimiento() << ","
                << p->getTelefono() << ","
                << p->getDireccion() << ","
                << p->getEmail() << ","
                << p->getContactoEmergencia() << ","
                << joinMultivalor(p->getAlergias(), ';') << "\n";
    }
    // archivo cerrado automáticamente (RAII)
}

void Persistencia::guardarPersonal(const GestorPersonal& gestor) {
    std::ofstream archivo(rutaPersonal_);  // RAII

    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir '" + rutaPersonal_ + "' para escritura.");
    }

    archivo << "id,nombre,rol,especialidad,turno,telefono,activo\n";

    for (const auto& par : gestor.getCatalogo()) {
        const Personal* p = par.second;
        archivo << p->getId() << ","
                << p->getNombre() << ","
                << p->getRol() << ","
                << p->getEspecialidad() << ","
                << p->getTurno() << ","
                << p->getTelefono() << ","
                << (p->isActivo() ? "1" : "0") << "\n";
    }
}

void Persistencia::guardarCitas(const GestorCitas& gestor) {
    std::ofstream archivo(rutaCitas_);  // RAII

    if (!archivo.is_open()) {
        throw std::runtime_error(
            "No se pudo abrir '" + rutaCitas_ + "' para escritura.");
    }

    archivo << "idCita,dniPaciente,idMedico,fecha,estado,motivo,notas,medicacion\n";

    for (const auto& par : gestor.getRegistro()) {
        const Cita* c = par.second;
        archivo << c->getIdCita() << ","
                << c->getDniPaciente() << ","
                << c->getIdMedico() << ","
                << c->getFecha() << ","
                << c->getEstado() << ","
                << c->getMotivo() << ","
                << c->getNotas() << ","
                << joinMultivalor(c->getMedicacion(), ';') << "\n";
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CARGAR / GUARDAR TODO (Conveniencia)
// ═══════════════════════════════════════════════════════════════════════════

void Persistencia::cargarTodo(GestorPacientes& gp, GestorPersonal& gPers,
                               GestorCitas& gc) {
    cargarPacientes(gp);
    cargarPersonal(gPers);
    cargarCitas(gc);
}

void Persistencia::guardarTodo(const GestorPacientes& gp,
                                const GestorPersonal& gPers,
                                const GestorCitas& gc) {
    guardarPacientes(gp);
    guardarPersonal(gPers);
    guardarCitas(gc);
}
