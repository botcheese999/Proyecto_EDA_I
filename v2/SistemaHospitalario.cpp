/*
 * SistemaHospitalario.cpp — Implementación de la fachada del sistema.
 *
 * Aquí se demuestra el cumplimiento de los 5 criterios del sílabo
 * mediante la orquestación de los gestores y el uso de polimorfismo,
 * herencia y RTTI.
 */

#include "SistemaHospitalario.h"
#include "Paciente.h"
#include "Personal.h"
#include <iostream>

SistemaHospitalario::SistemaHospitalario() {}

// Destructor: libera la memoria de los objetos en el registro general
SistemaHospitalario::~SistemaHospitalario() {
    for (Persona* p : registroGeneral_) {
        delete p;  // CRITERIO 4: delete sobre Persona* invoca el
                   // destructor virtual correcto de la clase hija
    }
    registroGeneral_.clear();
}

GestorPacientes& SistemaHospitalario::getGestorPacientes() {
    return gestorPacientes_;
}

GestorCitas& SistemaHospitalario::getGestorCitas() {
    return gestorCitas_;
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 4 – POLIMORFISMO Y ENLACE DINÁMICO
// Agrega un objeto (instanciado con new) al vector de punteros base.
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::agregarAlRegistroGeneral(Persona* persona) {
    if (persona) {
        registroGeneral_.push_back(persona);
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 4 – PROCESAMIENTO UNIFORME POLIMÓRFICO
// Recorre el std::vector<Persona*> e invoca mostrarInfo() y getTipo()
// mediante enlace dinámico. En tiempo de ejecución, la vtable determina
// si se ejecuta Paciente::mostrarInfo() o Personal::mostrarInfo().
// ═══════════════════════════════════════════════════════════════════════════
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

        // ── CRITERIO 4: Llamada polimórfica ──
        // En tiempo de ejecución, la vtable resuelve si llama a
        // Paciente::mostrarInfo() o Personal::mostrarInfo()
        registroGeneral_[i]->mostrarInfo();
    }
}

// ═══════════════════════════════════════════════════════════════════════════
// CRITERIO 5 – DOWNCASTING SEGURO CON dynamic_cast (RTTI)
//
// Esta función recibe un puntero genérico Persona* y utiliza
// dynamic_cast para verificar de forma segura si el objeto en memoria
// es de una clase hija específica (Paciente* o Personal*).
//
// Solo si la conversión es exitosa (puntero resultante != nullptr),
// se ejecuta un método exclusivo de la clase hija:
//   - Paciente::mostrarHistorialCompleto()  → NO existe en Persona
//   - Personal::mostrarPermisos()           → NO existe en Persona
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::identificarYProcesar(Persona* persona) {
    std::cout << "\n  ── RTTI: Intentando identificar tipo concreto... ──\n";

    if (!persona) {
        std::cout << "  [!] Puntero nulo recibido.\n";
        return;
    }

    // Intento 1: ¿Es un Paciente?
    Paciente* paciente = dynamic_cast<Paciente*>(persona);
    if (paciente != nullptr) {
        // dynamic_cast exitoso → el objeto ES un Paciente
        std::cout << "  [OK] dynamic_cast<Paciente*> exitoso.\n"
                  << "  Ejecutando metodo EXCLUSIVO: mostrarHistorialCompleto()\n";
        paciente->mostrarHistorialCompleto();  // Método que NO existe en Persona
        return;
    }

    // Intento 2: ¿Es un Personal?
    Personal* personal = dynamic_cast<Personal*>(persona);
    if (personal != nullptr) {
        // dynamic_cast exitoso → el objeto ES un Personal
        std::cout << "  [OK] dynamic_cast<Personal*> exitoso.\n"
                  << "  Ejecutando metodo EXCLUSIVO: mostrarPermisos()\n";
        personal->mostrarPermisos();  // Método que NO existe en Persona
        return;
    }

    // Si ningún cast fue exitoso
    std::cout << "  [!] No se pudo identificar el tipo concreto.\n";
}

// ═══════════════════════════════════════════════════════════════════════════
// DEMOSTRACIÓN INTEGRAL DE LOS 5 CRITERIOS
// ═══════════════════════════════════════════════════════════════════════════
void SistemaHospitalario::ejecutarDemostracion() {
    std::cout << "\n"
              << "################################################################\n"
              << "#                                                              #\n"
              << "#     SISTEMA HOSPITALARIO V2 — DEMOSTRACION POO EN C++        #\n"
              << "#     Cumplimiento de los 5 Criterios del Silabo               #\n"
              << "#                                                              #\n"
              << "################################################################\n";

    // ═════════════════════════════════════════════════════════════════════
    // CRITERIO 1: ABSTRACCIÓN Y ENCAPSULAMIENTO
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 1: ABSTRACCION Y ENCAPSULAMIENTO\n"
              << "  - Clases con atributos private\n"
              << "  - Constructores con listas de inicializacion\n"
              << "  - Getters/Setters con validaciones logicas\n"
              << "══════════════════════════════════════════════════════════\n";

    // Registrar pacientes vía gestor (usa setters validados internamente)
    auto res1 = gestorPacientes_.registrar(
        "12345678", "Maria Garcia Lopez", "O+", "1990-05-15",
        "987654321", "Av. Lima 123", "maria@email.com", "Juan Garcia");
    std::cout << "\n  " << (res1.first ? "[OK]" : "[X]") << " " << res1.second << "\n";

    auto res2 = gestorPacientes_.registrar(
        "87654321", "Carlos Perez Ruiz", "A+", "1985-03-20",
        "912345678", "Jr. Cusco 456", "carlos@email.com", "Ana Perez");
    std::cout << "  " << (res2.first ? "[OK]" : "[X]") << " " << res2.second << "\n";

    // Demostrar validación del setter (email sin '@' es rechazado)
    auto resE = gestorPacientes_.actualizarEmail("12345678", "email-invalido");
    std::cout << "  " << (resE.first ? "[OK]" : "[X]") << " " << resE.second
              << " (validacion del setter)\n";

    auto resE2 = gestorPacientes_.actualizarEmail("12345678", "maria.nueva@email.com");
    std::cout << "  " << (resE2.first ? "[OK]" : "[X]") << " " << resE2.second << "\n";

    // Demostrar validación de DNI duplicado
    auto res3 = gestorPacientes_.registrar(
        "12345678", "Duplicado", "B+", "2000-01-01");
    std::cout << "  " << (res3.first ? "[OK]" : "[X]") << " " << res3.second
              << " (validacion de unicidad)\n";

    // ═════════════════════════════════════════════════════════════════════
    // CRITERIO 2: ARQUITECTURA DE GESTORES
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 2: ARQUITECTURA DE GESTORES\n"
              << "  - GestorPacientes, GestorCitas: logica de negocio\n"
              << "  - SistemaHospitalario: fachada orquestadora\n"
              << "══════════════════════════════════════════════════════════\n";

    // Agregar alergias
    auto resA1 = gestorPacientes_.agregarAlergia("12345678", "penicilina");
    std::cout << "\n  " << (resA1.first ? "[OK]" : "[X]") << " " << resA1.second << "\n";
    auto resA2 = gestorPacientes_.agregarAlergia("12345678", "aspirina");
    std::cout << "  " << (resA2.first ? "[OK]" : "[X]") << " " << resA2.second << "\n";
    auto resA3 = gestorPacientes_.agregarAlergia("87654321", "penicilina");
    std::cout << "  " << (resA3.first ? "[OK]" : "[X]") << " " << resA3.second << "\n";
    auto resA4 = gestorPacientes_.agregarAlergia("87654321", "ibuprofeno");
    std::cout << "  " << (resA4.first ? "[OK]" : "[X]") << " " << resA4.second << "\n";

    // Cruzar alergias (intersección de sets)
    auto comunes = gestorPacientes_.cruzarAlergias("12345678", "87654321");
    std::cout << "  Alergias en comun: ";
    if (comunes.empty()) {
        std::cout << "Ninguna";
    } else {
        for (const auto& a : comunes) std::cout << a << " ";
    }
    std::cout << "\n";

    // Crear cita usando el gestor
    auto resC = gestorCitas_.crearCita("12345678", "P0001", "Control general");
    std::cout << "\n  " << (resC.first ? "[OK]" : "[X]") << " " << resC.second << "\n";

    // Agregar medicación
    auto resM = gestorCitas_.agregarMedicacion("C0001", "Paracetamol", "P0001");
    std::cout << "  " << (resM.first ? "[OK]" : "[X]") << " " << resM.second << "\n";

    // Listar pacientes
    auto lista = gestorPacientes_.listar();
    std::cout << "\n  Pacientes registrados (" << lista.size() << "):\n";
    for (const auto& par : lista) {
        std::cout << "    " << par.first << " — " << par.second << "\n";
    }

    // ═════════════════════════════════════════════════════════════════════
    // CRITERIO 3: HERENCIA SIMPLE
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 3: HERENCIA SIMPLE\n"
              << "  - Persona (superclase, atributos protected)\n"
              << "    |── Paciente (hereda id_, nombre_, telefono_)\n"
              << "    |── Personal (hereda id_, nombre_, telefono_)\n"
              << "══════════════════════════════════════════════════════════\n";

    std::cout << "\n  La jerarquia de herencia permite que Paciente y Personal\n"
              << "  reutilicen los atributos y metodos de Persona:\n"
              << "  - getId(), getNombre(), getTelefono() → heredados\n"
              << "  - mostrarInfo(), getTipo() → override en cada hija\n";

    // ═════════════════════════════════════════════════════════════════════
    // CRITERIO 4: POLIMORFISMO Y ENLACE DINÁMICO
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 4: POLIMORFISMO Y ENLACE DINAMICO\n"
              << "  - std::vector<Persona*> con objetos hijos (new)\n"
              << "  - Metodos virtual + override\n"
              << "  - Destructor virtual obligatorio\n"
              << "══════════════════════════════════════════════════════════\n";

    // Crear objetos con new y asignarlos a punteros de la superclase
    // Estos objetos se almacenan en registroGeneral_ (vector<Persona*>)
    Persona* p1 = new Paciente("11111111", "Ana Torres Vega", "AB+", "1992-08-10",
                                "999000111", "Calle Sol 789", "ana@mail.com", "Pedro Torres");
    Persona* p2 = new Personal(Personal::generarId(), "Dr. Roberto Diaz",
                                "especialista", "Cardiologia", "mañana", "955000111");
    Persona* p3 = new Paciente("22222222", "Luis Mendoza Rios", "B-", "1988-12-01",
                                "955000222", "Av. Mar 101", "luis@mail.com", "Rosa Mendoza");
    Persona* p4 = new Personal(Personal::generarId(), "Enf. Carmen Soto",
                                "enfermera", "General", "tarde", "955000333");

    // Agregar al vector polimórfico
    agregarAlRegistroGeneral(p1);
    agregarAlRegistroGeneral(p2);
    agregarAlRegistroGeneral(p3);
    agregarAlRegistroGeneral(p4);

    // Procesamiento uniforme: mostrarInfo() resuelto por enlace dinámico
    listarRegistroGeneral();

    // ═════════════════════════════════════════════════════════════════════
    // CRITERIO 5: DOWNCASTING SEGURO (RTTI)
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  CRITERIO 5: DOWNCASTING SEGURO (RTTI)\n"
              << "  - dynamic_cast<ClaseHija*>(punteroBase)\n"
              << "  - Verifica tipo en tiempo de ejecucion\n"
              << "  - Ejecuta metodo exclusivo si cast es exitoso\n"
              << "══════════════════════════════════════════════════════════\n";

    // Agregar historial al paciente p1 para que la demo muestre datos
    Paciente* pacTemp = dynamic_cast<Paciente*>(p1);
    if (pacTemp) {
        EntradaHistorial entrada;
        entrada.fecha = "2025-06-20 10:30:00";
        entrada.tipo = "consulta";
        entrada.medicoId = "P0001";
        entrada.diagnostico = "Hipertension arterial leve";
        entrada.medicacion = {"Losartan 50mg", "Aspirina 100mg"};
        entrada.notas = "Control en 3 meses";
        pacTemp->agregarEntradaHistorial(entrada);
    }

    // Ahora recorrer el vector y aplicar dynamic_cast a cada elemento
    std::cout << "\n  Recorriendo vector<Persona*> con dynamic_cast:\n";
    for (size_t i = 0; i < registroGeneral_.size(); ++i) {
        std::cout << "\n  ─── Elemento [" << i << "]: "
                  << registroGeneral_[i]->getNombre() << " ───";
        identificarYProcesar(registroGeneral_[i]);
    }

    // ═════════════════════════════════════════════════════════════════════
    // RESUMEN FINAL
    // ═════════════════════════════════════════════════════════════════════
    std::cout << "\n\n";
    mostrarResumen();
}

void SistemaHospitalario::mostrarResumen() const {
    std::cout << "══════════════════════════════════════════════════════════\n"
              << "  RESUMEN DEL SISTEMA\n"
              << "══════════════════════════════════════════════════════════\n"
              << "  Pacientes en gestor:    " << gestorPacientes_.getCantidad() << "\n"
              << "  Citas en gestor:        " << gestorCitas_.getCantidad() << "\n"
              << "  Registro general (POO): " << registroGeneral_.size() << "\n"
              << "  En espera:              " << gestorCitas_.cantidadEnEspera() << "\n"
              << "══════════════════════════════════════════════════════════\n";
}
