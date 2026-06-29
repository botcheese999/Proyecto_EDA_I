/*
 * SistemaHospitalario.cpp — Implementación de la fachada (V2.1)
 *
 * Integra:
 *   - Persistencia CSV con RAII y excepciones
 *   - Datos de prueba automáticos (Plug-and-Play)
 *   - Los 5 criterios POO del sílabo
 *   - ELIMINADOS: Cola FIFO, Pila LIFO, Historial clínico
 */

#include "SistemaHospitalario.h"
#include "Paciente.h"
#include "Personal.h"
#include <iostream>
#include <stdexcept>

SistemaHospitalario::SistemaHospitalario()
    : persistencia_("datos")
{
}

SistemaHospitalario::~SistemaHospitalario() {
    for (Persona* p : registroGeneral_) {
        delete p;  // CRITERIO 4: destructor virtual invoca la clase hija correcta
    }
    registroGeneral_.clear();
}

// ═══════════════════════════════════════════════════════════════════════════
// INICIALIZACIÓN CON PERSISTENCIA CSV + EXCEPCIONES
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::inicializar() {
    std::cout << "\n  Inicializando sistema...\n";

    // ── MANEJO DE EXCEPCIONES: try/catch para capturar errores de I/O ──
    try {
        // ── PLUG-AND-PLAY: Verificar si los CSV existen ──
        if (!persistencia_.existenArchivos()) {
            std::cout << "  [!] Archivos CSV no encontrados. Generando datos de prueba...\n";

            // ── throw std::runtime_error si falla la creación ──
            persistencia_.generarDatosPrueba();

            std::cout << "  [OK] Archivos CSV generados en datos/\n";
            std::cout << "       - datos/pacientes.csv (5 pacientes)\n"
                      << "       - datos/personal.csv  (4 personal)\n"
                      << "       - datos/citas.csv     (4 citas)\n";
        }

        // ── CARGAR datos desde CSV (throw si archivo corrupto) ──
        std::cout << "\n  Cargando datos desde archivos CSV...\n";
        persistencia_.cargarTodo(gestorPacientes_, gestorPersonal_, gestorCitas_);

        std::cout << "  [OK] Datos cargados exitosamente:\n"
                  << "       Pacientes: " << gestorPacientes_.getCantidad() << "\n"
                  << "       Personal:  " << gestorPersonal_.getCantidad() << "\n"
                  << "       Citas:     " << gestorCitas_.getCantidad() << "\n";

    // ── CATCH: Captura excepciones estándar para evitar colapso ──
    } catch (const std::runtime_error& e) {
        // Error de I/O (archivo no accesible)
        std::cerr << "\n  [ERROR runtime_error] " << e.what() << "\n"
                  << "  El sistema continuara con datos vacios.\n";

    } catch (const std::invalid_argument& e) {
        // Datos corruptos en CSV
        std::cerr << "\n  [ERROR invalid_argument] " << e.what() << "\n"
                  << "  El sistema continuara con los datos cargados hasta el error.\n";

    } catch (const std::exception& e) {
        // Cualquier otra excepción estándar
        std::cerr << "\n  [ERROR generico] " << e.what() << "\n";
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// GUARDAR DATOS A CSV
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::guardarDatos() {
    try {
        persistencia_.guardarTodo(gestorPacientes_, gestorPersonal_, gestorCitas_);
        std::cout << "  [OK] Datos guardados en archivos CSV.\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "  [ERROR] No se pudieron guardar los datos: " << e.what() << "\n";
    }
}

// ── Acceso a gestores ──
GestorPacientes& SistemaHospitalario::getGestorPacientes() { return gestorPacientes_; }
GestorPersonal& SistemaHospitalario::getGestorPersonal() { return gestorPersonal_; }
GestorCitas& SistemaHospitalario::getGestorCitas() { return gestorCitas_; }

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 4 – POLIMORFISMO Y ENLACE DINÁMICO
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::agregarAlRegistroGeneral(Persona* persona) {
    if (persona) registroGeneral_.push_back(persona);
}

void SistemaHospitalario::listarRegistroGeneral() const {
    std::cout << "\n══════════════════════════════════════════════════════════\n"
              << "  REGISTRO GENERAL — Procesamiento Polimorfico\n"
              << "  (std::vector<Persona*> con enlace dinamico)\n"
              << "══════════════════════════════════════════════════════════\n";

    if (registroGeneral_.empty()) {
        std::cout << "  El registro esta vacio.\n";
        return;
    }

    for (size_t i = 0; i < registroGeneral_.size(); ++i) {
        std::cout << "\n  --- Elemento [" << i << "] | Tipo: "
                  << registroGeneral_[i]->getTipo() << " ---\n";
        // ── CRITERIO 4: Enlace dinámico — vtable resuelve el método ──
        registroGeneral_[i]->mostrarInfo();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – DOWNCASTING SEGURO CON dynamic_cast (RTTI)
//
// Recibe Persona* y usa dynamic_cast para verificar el tipo concreto.
// Solo si la conversión es exitosa ejecuta el método EXCLUSIVO:
//   - Paciente::mostrarFichaMedica()  → NO existe en Persona
//   - Personal::mostrarPermisos()     → NO existe en Persona
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::identificarYProcesar(Persona* persona) {
    std::cout << "\n  -- RTTI: Intentando identificar tipo concreto... --\n";

    if (!persona) {
        std::cout << "  [!] Puntero nulo recibido.\n";
        return;
    }

    // Intento 1: ¿Es Paciente?
    Paciente* paciente = dynamic_cast<Paciente*>(persona);
    if (paciente != nullptr) {
        std::cout << "  [OK] dynamic_cast<Paciente*> exitoso.\n"
                  << "  Ejecutando metodo EXCLUSIVO: mostrarFichaMedica()\n";
        paciente->mostrarFichaMedica();
        return;
    }

    // Intento 2: ¿Es Personal?
    Personal* personal = dynamic_cast<Personal*>(persona);
    if (personal != nullptr) {
        std::cout << "  [OK] dynamic_cast<Personal*> exitoso.\n"
                  << "  Ejecutando metodo EXCLUSIVO: mostrarPermisos()\n";
        personal->mostrarPermisos();
        return;
    }

    std::cout << "  [!] No se pudo identificar el tipo concreto.\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// DEMOSTRACIÓN INTEGRAL — V2.1
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::ejecutarDemostracion() {
    std::cout << "\n"
              << "################################################################\n"
              << "#                                                              #\n"
              << "#   SISTEMA HOSPITALARIO V2.1 — DEMOSTRACION POO EN C++       #\n"
              << "#   + Persistencia CSV (RAII) + Excepciones + Mock Data        #\n"
              << "#                                                              #\n"
              << "################################################################\n";

    // ═══════════════════════════════════════════════════════════════════
    // PERSISTENCIA CSV + EXCEPCIONES + PLUG-AND-PLAY
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  PERSISTENCIA CSV + EXCEPCIONES + MOCK DATA\n"
              << "  - <fstream> con RAII (cierre automatico)\n"
              << "  - try/catch/throw para errores de I/O y datos\n"
              << "  - Generacion automatica de CSV si no existen\n"
              << "══════════════════════════════════════════════════════════\n";

    // La carga de datos ya se hizo en inicializar()
    // Mostrar los datos cargados desde CSV
    std::cout << "\n  Pacientes cargados desde CSV:\n";
    auto listaPac = gestorPacientes_.listar();
    for (const auto& par : listaPac) {
        std::cout << "    " << par.first << " - " << par.second << "\n";
    }

    std::cout << "\n  Personal cargado desde CSV:\n";
    auto listaPers = gestorPersonal_.listar();
    for (const auto& par : listaPers) {
        std::cout << "    " << par.first << " - " << par.second << "\n";
    }

    std::cout << "\n  Citas cargadas desde CSV:\n";
    auto citasPendientes = gestorCitas_.listarPorEstado("pendiente");
    auto citasCompletadas = gestorCitas_.listarPorEstado("completada");
    auto citasCanceladas = gestorCitas_.listarPorEstado("cancelada");
    std::cout << "    Pendientes:  " << citasPendientes.size() << "\n"
              << "    Completadas: " << citasCompletadas.size() << "\n"
              << "    Canceladas:  " << citasCanceladas.size() << "\n";

    // ── Demostración de EXCEPCIÓN con archivo inválido ──
    std::cout << "\n  Demostrando manejo de excepciones con archivo invalido:\n";
    try {
        // Intentar cargar desde un archivo que no existe
        GestorPacientes gestorTmp;
        Persistencia persTmp("directorio_inexistente");
        persTmp.cargarPacientes(gestorTmp);
        std::cout << "  [OK] No deberia llegar aqui.\n";  // No se ejecuta
    } catch (const std::runtime_error& e) {
        // ── CATCH: Se captura la excepción y el programa NO colapsa ──
        std::cout << "  [CAPTURADO] runtime_error: " << e.what() << "\n";
        std::cout << "  El programa continua normalmente tras la excepcion.\n";
    }

    // ═══════════════════════════════════════════════════════════════════
    // CRITERIO 1: ABSTRACCIÓN Y ENCAPSULAMIENTO
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 1: ABSTRACCION Y ENCAPSULAMIENTO\n"
              << "  - Clases con atributos private\n"
              << "  - Constructores con listas de inicializacion\n"
              << "  - Getters/Setters con validaciones logicas\n"
              << "══════════════════════════════════════════════════════════\n";

    // Demostrar validación del setter (email sin '@' rechazado)
    auto resE = gestorPacientes_.actualizarEmail("12345678", "email-invalido");
    std::cout << "\n  " << (resE.first ? "[OK]" : "[X]") << " " << resE.second
              << " (validacion del setter)\n";

    auto resE2 = gestorPacientes_.actualizarEmail("12345678", "maria.nueva@email.com");
    std::cout << "  " << (resE2.first ? "[OK]" : "[X]") << " " << resE2.second << "\n";

    // Demostrar validación de DNI duplicado
    auto res3 = gestorPacientes_.registrar("12345678", "Duplicado", "B+", "2000-01-01");
    std::cout << "  " << (res3.first ? "[OK]" : "[X]") << " " << res3.second
              << " (validacion de unicidad)\n";

    // ═══════════════════════════════════════════════════════════════════
    // CRITERIO 2: ARQUITECTURA DE GESTORES
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 2: ARQUITECTURA DE GESTORES\n"
              << "  - GestorPacientes, GestorPersonal, GestorCitas\n"
              << "  - Persistencia (CSV I/O)\n"
              << "  - SistemaHospitalario: fachada orquestadora\n"
              << "══════════════════════════════════════════════════════════\n";

    // Cruzar alergias entre pacientes cargados del CSV
    auto comunes = gestorPacientes_.cruzarAlergias("12345678", "55667788");
    std::cout << "\n  Cruce de alergias entre 12345678 y 55667788: ";
    if (comunes.empty()) {
        std::cout << "Ninguna en comun";
    } else {
        for (const auto& a : comunes) std::cout << a << " ";
    }
    std::cout << "\n";

    // Crear nueva cita usando el gestor
    auto resC = gestorCitas_.crearCita("99887766", "P0003", "Consulta de prueba V2.1");
    std::cout << "  " << (resC.first ? "[OK]" : "[X]") << " " << resC.second << "\n";

    // ═══════════════════════════════════════════════════════════════════
    // CRITERIO 3: HERENCIA SIMPLE
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 3: HERENCIA SIMPLE\n"
              << "  - Persona (superclase, atributos protected)\n"
              << "    |-- Paciente (hereda id_, nombre_, telefono_)\n"
              << "    |-- Personal (hereda id_, nombre_, telefono_)\n"
              << "══════════════════════════════════════════════════════════\n";

    std::cout << "\n  La jerarquia de herencia permite que Paciente y Personal\n"
              << "  reutilicen los atributos y metodos de Persona:\n"
              << "  - getId(), getNombre(), getTelefono() -> heredados\n"
              << "  - mostrarInfo(), getTipo() -> override en cada hija\n";

    // ═══════════════════════════════════════════════════════════════════
    // CRITERIO 4: POLIMORFISMO Y ENLACE DINÁMICO
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 4: POLIMORFISMO Y ENLACE DINAMICO\n"
              << "  - std::vector<Persona*> con objetos hijos (new)\n"
              << "  - Metodos virtual + override\n"
              << "  - Destructor virtual obligatorio\n"
              << "══════════════════════════════════════════════════════════\n";

    // Crear objetos con new y asignarlos a punteros de la superclase
    Persona* p1 = new Paciente("33344455", "Elena Vargas Lima", "A-", "1993-07-14",
                                "966111222", "Av. Paz 303", "elena@mail.com", "Jorge Vargas");
    // Agregar alergias al paciente polimórfico para la demo RTTI
    dynamic_cast<Paciente*>(p1)->agregarAlergia("latex");
    dynamic_cast<Paciente*>(p1)->agregarAlergia("mariscos");

    Persona* p2 = new Personal(Personal::generarId(), "Dr. Alberto Ramos",
                                "especialista", "Neurologia", "mañana", "966333444");
    Persona* p3 = new Paciente("44455566", "Ricardo Flores Paz", "B+", "1991-02-28",
                                "966555666", "Jr. Rio 404", "ricardo@mail.com", "Marta Flores");
    Persona* p4 = new Personal(Personal::generarId(), "Enf. Diana Lopez",
                                "enfermera", "General", "tarde", "966777888");

    agregarAlRegistroGeneral(p1);
    agregarAlRegistroGeneral(p2);
    agregarAlRegistroGeneral(p3);
    agregarAlRegistroGeneral(p4);

    // Procesamiento uniforme polimórfico
    listarRegistroGeneral();

    // ═══════════════════════════════════════════════════════════════════
    // CRITERIO 5: DOWNCASTING SEGURO (RTTI)
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 5: DOWNCASTING SEGURO (RTTI)\n"
              << "  - dynamic_cast<ClaseHija*>(punteroBase)\n"
              << "  - Verifica tipo en tiempo de ejecucion\n"
              << "  - Ejecuta metodo exclusivo si cast es exitoso\n"
              << "══════════════════════════════════════════════════════════\n";

    std::cout << "\n  Recorriendo vector<Persona*> con dynamic_cast:\n";
    for (size_t i = 0; i < registroGeneral_.size(); ++i) {
        std::cout << "\n  --- Elemento [" << i << "]: "
                  << registroGeneral_[i]->getNombre() << " ---";
        identificarYProcesar(registroGeneral_[i]);
    }

    // ═══════════════════════════════════════════════════════════════════
    // GUARDAR DATOS MODIFICADOS A CSV
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  GUARDANDO DATOS MODIFICADOS A CSV\n"
              << "══════════════════════════════════════════════════════════\n\n";

    // ── MANEJO DE EXCEPCIONES al guardar ──
    try {
        guardarDatos();
    } catch (const std::exception& e) {
        std::cerr << "  [ERROR] " << e.what() << "\n";
    }

    // ═══════════════════════════════════════════════════════════════════
    // RESUMEN FINAL
    // ═══════════════════════════════════════════════════════════════════
    std::cout << "\n";
    mostrarResumen();
}

void SistemaHospitalario::mostrarResumen() const {
    std::cout << "══════════════════════════════════════════════════════════\n"
              << "  RESUMEN DEL SISTEMA V2.1\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  Pacientes en gestor:    " << gestorPacientes_.getCantidad() << "\n"
              << "  Personal en gestor:     " << gestorPersonal_.getCantidad() << "\n"
              << "  Citas en gestor:        " << gestorCitas_.getCantidad() << "\n"
              << "  Registro general (POO): " << registroGeneral_.size() << "\n"
              << "══════════════════════════════════════════════════════════\n";
}
