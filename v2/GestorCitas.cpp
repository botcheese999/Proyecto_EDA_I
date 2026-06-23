/*
 * GestorCitas.cpp — Implementación del gestor de citas médicas.
 *
 * CRITERIO 2: Toda la lógica de negocio de citas centralizada aquí.
 */

#include "GestorCitas.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// ── Helper: obtener timestamp actual ──
static std::string timestampActual() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

static std::string fechaActual() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(local, "%Y-%m-%d");
    return oss.str();
}

GestorCitas::GestorCitas() {}

// Destructor: libera la memoria dinámica de todas las citas
GestorCitas::~GestorCitas() {
    for (auto& par : registroCitas_) {
        delete par.second;
    }
    registroCitas_.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
//  LISTA DE ESPERA (FIFO)
// ═══════════════════════════════════════════════════════════════════════════

std::pair<bool, std::string> GestorCitas::agregarAEspera(
    const std::string& dni, const std::string& motivo)
{
    // Verificar que no esté ya en espera
    for (const auto& entrada : listaEspera_) {
        if (entrada.dni == dni) {
            return {false, "El paciente " + dni + " ya esta en la lista de espera."};
        }
    }

    EntradaEspera nueva;
    nueva.timestamp = timestampActual();
    nueva.dni = dni;
    nueva.motivo = motivo;
    listaEspera_.push_back(nueva);

    return {true, "Paciente " + dni + " agregado a la lista de espera."};
}

std::pair<bool, EntradaEspera> GestorCitas::atenderSiguiente() {
    if (listaEspera_.empty()) {
        EntradaEspera vacia;
        return {false, vacia};
    }

    // FIFO: saca el primero
    EntradaEspera siguiente = listaEspera_.front();
    listaEspera_.erase(listaEspera_.begin());
    return {true, siguiente};
}

std::vector<EntradaEspera> GestorCitas::verListaEspera() const {
    return listaEspera_;  // Copia defensiva
}

int GestorCitas::cantidadEnEspera() const {
    return static_cast<int>(listaEspera_.size());
}

// ═══════════════════════════════════════════════════════════════════════════
//  CRUD DE CITAS
// ═══════════════════════════════════════════════════════════════════════════

std::pair<bool, std::string> GestorCitas::crearCita(
    const std::string& dniPaciente, const std::string& idMedico,
    const std::string& motivo)
{
    std::string idCita = Cita::generarId();
    std::string fecha = fechaActual();

    // ── CRITERIO 4: Instanciación dinámica con new ──
    Cita* nueva = new Cita(idCita, dniPaciente, idMedico, fecha, motivo);
    registroCitas_[idCita] = nueva;

    return {true, "Cita " + idCita + " creada — Paciente: " + dniPaciente
                  + ", Medico: " + idMedico};
}

Cita* GestorCitas::buscarCita(const std::string& idCita) const {
    auto it = registroCitas_.find(idCita);
    if (it == registroCitas_.end()) return nullptr;
    return it->second;
}

std::pair<bool, std::string> GestorCitas::completarCita(
    const std::string& idCita, const std::string& diagnostico,
    const std::string& notas, Paciente* paciente)
{
    Cita* cita = buscarCita(idCita);
    if (!cita) return {false, "No se encontro cita con ID " + idCita + "."};
    if (cita->getEstado() == "completada")
        return {false, "La cita " + idCita + " ya esta completada."};

    // Agregar al historial del paciente
    if (paciente) {
        EntradaHistorial entrada;
        entrada.fecha = timestampActual();
        entrada.tipo = "consulta";
        entrada.medicoId = cita->getIdMedico();
        entrada.diagnostico = diagnostico;
        entrada.medicacion = cita->getMedicacion();  // Copia del vector
        entrada.notas = notas;
        paciente->agregarEntradaHistorial(entrada);
    }

    cita->setEstado("completada");
    cita->setNotas(notas);

    return {true, "Cita " + idCita + " completada. Historial actualizado."};
}

std::pair<bool, std::string> GestorCitas::cancelarCita(
    const std::string& idCita, const std::string& motivoCancelacion)
{
    Cita* cita = buscarCita(idCita);
    if (!cita) return {false, "No se encontro cita con ID " + idCita + "."};

    std::string estado = cita->getEstado();
    if (estado == "completada" || estado == "cancelada")
        return {false, "La cita " + idCita + " ya esta " + estado + "."};

    cita->setEstado("cancelada");
    cita->setNotas("CANCELADA: " + motivoCancelacion);
    return {true, "Cita " + idCita + " cancelada."};
}

// ═══════════════════════════════════════════════════════════════════════════
//  MEDICACIÓN CON DESHACER (PILA LIFO)
// ═══════════════════════════════════════════════════════════════════════════

std::pair<bool, std::string> GestorCitas::agregarMedicacion(
    const std::string& idCita, const std::string& medicamento,
    const std::string& idMedico)
{
    Cita* cita = buscarCita(idCita);
    if (!cita) return {false, "No se encontro cita con ID " + idCita + "."};

    if (!cita->agregarMedicamento(medicamento))
        return {false, "No se puede modificar una cita " + cita->getEstado() + "."};

    // Push a la pila de deshacer (LIFO)
    AccionMedicacion accion;
    accion.citaId = idCita;
    accion.medicamento = medicamento;
    accion.medicoId = idMedico;
    accion.timestamp = timestampActual();
    pilaDeshacer_.push_back(accion);

    return {true, "Medicamento '" + medicamento + "' agregado a cita " + idCita + "."};
}

std::pair<bool, std::string> GestorCitas::deshacerMedicacion(
    const Personal* medicoSolicitante)
{
    // Verificar permisos
    if (!medicoSolicitante || !medicoSolicitante->tienePermiso("deshacer_medicacion"))
        return {false, "No tiene permisos para deshacer medicacion."};

    if (pilaDeshacer_.empty())
        return {false, "No hay acciones para deshacer."};

    // Pop de la pila (LIFO)
    AccionMedicacion ultimaAccion = pilaDeshacer_.back();
    pilaDeshacer_.pop_back();

    // Revertir la acción
    Cita* cita = buscarCita(ultimaAccion.citaId);
    if (!cita)
        return {false, "La cita " + ultimaAccion.citaId + " ya no existe."};

    cita->removerMedicamento(ultimaAccion.medicamento);

    return {true, "Medicacion revertida: '" + ultimaAccion.medicamento
                  + "' removido de cita " + ultimaAccion.citaId + "."};
}

// ── Consultas ──
std::vector<Cita*> GestorCitas::listarPorEstado(const std::string& estado) const {
    std::vector<Cita*> resultados;
    for (const auto& par : registroCitas_) {
        if (par.second->getEstado() == estado) {
            resultados.push_back(par.second);
        }
    }
    return resultados;
}

std::vector<Cita*> GestorCitas::listarPorPaciente(const std::string& dni) const {
    std::vector<Cita*> resultados;
    for (const auto& par : registroCitas_) {
        if (par.second->getDniPaciente() == dni) {
            resultados.push_back(par.second);
        }
    }
    return resultados;
}

int GestorCitas::getCantidad() const {
    return static_cast<int>(registroCitas_.size());
}
