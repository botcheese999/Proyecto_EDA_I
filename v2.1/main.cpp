/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  main.cpp — Interfaz de usuario por consola — Sistema Hospitalario V2.1
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  ARQUITECTURA:
 *    main() instancia la fachada SistemaHospitalario y la clase Menu.
 *    Toda la lógica de negocio se delega exclusivamente a la fachada;
 *    main() solo coordina la UI y captura excepciones para que el menú
 *    nunca colapse ante un error interno.
 *
 *  SUBMENÚS:
 *    1 — Gestión de Pacientes
 *    2 — Gestión de Personal
 *    3 — Gestión de Citas
 *    4 — Registro General POO (polimorfismo + RTTI)
 *    0 — Salir (guarda datos y termina)
 *
 *  Compilación:
 *    g++ -std=c++14 -o sistema_hospitalario \
 *        main.cpp Menu.cpp Persona.cpp Paciente.cpp Personal.cpp Cita.cpp \
 *        GestorPacientes.cpp GestorPersonal.cpp GestorCitas.cpp \
 *        Persistencia.cpp SistemaHospitalario.cpp
 */
#ifdef _WIN32
#include <windows.h>
#include "SistemaHospitalario.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <limits>
#endif

// ── Helpers de lectura de línea completa ────────────────────────────────────
// getline directo evita problemas con espacios en nombres/motivos.

static std::string leerLinea(const std::string &prompt)
{
    std::string valor;
    std::cout << "  " << prompt << ": ";
    std::getline(std::cin, valor);
    return valor;
}

// ════════════════════════════════════════════════════════════════════════════
//  SUBMENÚ 1 — Gestión de Pacientes
// ════════════════════════════════════════════════════════════════════════════
static void menuPacientes(SistemaHospitalario &sistema)
{
    Menu menu("GESTION DE PACIENTES");
    menu.add_option("Registrar nuevo paciente");
    menu.add_option("Buscar paciente por DNI");
    menu.add_option("Listar todos los pacientes");
    menu.add_option("Eliminar paciente");
    menu.add_option("Agregar alergia a paciente");
    menu.add_option("Volver al menu principal");

    bool activo = true;
    while (activo)
    {
        int op = menu.get_input();

        // Cada case envuelve la llamada a la fachada en try-catch.
        // Si la fachada lanza, se imprime e.what() y el menú continúa.
        switch (op)
        {

        case 1:
        {
            try
            {
                std::string dni = leerLinea("DNI");
                std::string nombre = leerLinea("Nombre completo");
                std::string sangre = leerLinea("Tipo de sangre (ej. A+)");
                std::string fnac = leerLinea("Fecha nacimiento (YYYY-MM-DD)");
                std::string tel = leerLinea("Telefono (Enter para omitir)");
                std::string dir = leerLinea("Direccion (Enter para omitir)");
                std::string email = leerLinea("Email (Enter para omitir)");
                std::string emerg = leerLinea("Contacto de emergencia (Enter para omitir)");

                auto res = sistema.getGestorPacientes().registrar(
                    dni, nombre, sangre, fnac, tel, dir, email, emerg);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 2:
        {
            try
            {
                std::string dni = leerLinea("DNI a buscar");
                auto *p = sistema.getGestorPacientes().buscar(dni);
                if (p)
                {
                    p->mostrarInfo();
                }
                else
                {
                    std::cout << "\n  [X] Paciente no encontrado.\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 3:
        {
            try
            {
                auto lista = sistema.getGestorPacientes().listar();
                std::cout << "\n  Pacientes registrados (" << lista.size() << "):\n";
                for (const auto &par : lista)
                {
                    std::cout << "    " << par.first << "  —  " << par.second << "\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 4:
        {
            try
            {
                std::string dni = leerLinea("DNI a eliminar");
                auto res = sistema.getGestorPacientes().eliminar(dni);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 5:
        {
            try
            {
                std::string dni = leerLinea("DNI del paciente");
                std::string alergia = leerLinea("Alergia a agregar");
                auto res = sistema.getGestorPacientes().agregarAlergia(dni, alergia);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 6:
            activo = false;
            break;
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  SUBMENÚ 2 — Gestión de Personal
// ════════════════════════════════════════════════════════════════════════════
static void menuPersonal(SistemaHospitalario &sistema)
{
    Menu menu("GESTION DE PERSONAL");
    menu.add_option("Registrar nuevo personal");
    menu.add_option("Buscar personal por ID");
    menu.add_option("Listar todo el personal");
    menu.add_option("Volver al menu principal");

    bool activo = true;
    while (activo)
    {
        int op = menu.get_input();

        switch (op)
        {

        case 1:
        {
            try
            {
                std::string nombre = leerLinea("Nombre completo");
                std::string rol = leerLinea("Rol (medico/enfermera/administrativo)");
                std::string espec = leerLinea("Especialidad (Enter = General)");
                std::string turno = leerLinea("Turno (manana/tarde/noche)");
                std::string tel = leerLinea("Telefono (Enter para omitir)");

                if (espec.empty())
                    espec = "General";
                if (turno.empty())
                    turno = "manana";

                auto res = sistema.getGestorPersonal().registrar(nombre, rol, espec, turno, tel);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 2:
        {
            try
            {
                std::string id = leerLinea("ID de personal (ej. P0001)");
                auto *p = sistema.getGestorPersonal().buscar(id);
                if (p)
                {
                    p->mostrarInfo();
                }
                else
                {
                    std::cout << "\n  [X] Personal no encontrado.\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 3:
        {
            try
            {
                auto lista = sistema.getGestorPersonal().listar();
                std::cout << "\n  Personal registrado (" << lista.size() << "):\n";
                for (const auto &par : lista)
                {
                    std::cout << "    " << par.first << "  —  " << par.second << "\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 4:
            activo = false;
            break;
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  SUBMENÚ 3 — Gestión de Citas
// ════════════════════════════════════════════════════════════════════════════
static void menuCitas(SistemaHospitalario &sistema)
{
    Menu menu("GESTION DE CITAS");
    menu.add_option("Crear nueva cita");
    menu.add_option("Completar cita");
    menu.add_option("Cancelar cita");
    menu.add_option("Listar citas por estado");
    menu.add_option("Listar citas por paciente");
    menu.add_option("Volver al menu principal");

    bool activo = true;
    while (activo)
    {
        int op = menu.get_input();

        switch (op)
        {

        case 1:
        {
            try
            {
                std::string dni = leerLinea("DNI del paciente");
                std::string idMed = leerLinea("ID del medico (ej. P0001)");
                std::string motivo = leerLinea("Motivo (Enter para omitir)");

                auto res = sistema.getGestorCitas().crearCita(dni, idMed, motivo);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 2:
        {
            try
            {
                std::string id = leerLinea("ID de la cita");
                std::string notas = leerLinea("Notas de cierre (Enter para omitir)");

                auto res = sistema.getGestorCitas().completarCita(id, notas);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 3:
        {
            try
            {
                std::string id = leerLinea("ID de la cita");
                std::string motivo = leerLinea("Motivo de cancelacion");

                auto res = sistema.getGestorCitas().cancelarCita(id, motivo);
                std::cout << "\n  " << (res.first ? "[OK]" : "[X]")
                          << " " << res.second << "\n";
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 4:
        {
            try
            {
                std::cout << "\n  Estados disponibles: pendiente | completada | cancelada\n";
                std::string estado = leerLinea("Estado");

                auto citas = sistema.getGestorCitas().listarPorEstado(estado);
                std::cout << "\n  Citas con estado '" << estado
                          << "' (" << citas.size() << "):\n";
                for (const auto *c : citas)
                {
                    std::cout << "    " << c->getIdCita()
                              << "  Pac: " << c->getDniPaciente()
                              << "  Med: " << c->getIdMedico()
                              << "  " << c->getMotivo() << "\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 5:
        {
            try
            {
                std::string dni = leerLinea("DNI del paciente");
                auto citas = sistema.getGestorCitas().listarPorPaciente(dni);
                std::cout << "\n  Citas del paciente " << dni
                          << " (" << citas.size() << "):\n";
                for (const auto *c : citas)
                {
                    std::cout << "    " << c->getIdCita()
                              << "  [" << c->getEstado() << "]"
                              << "  " << c->getMotivo() << "\n";
                }
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 6:
            activo = false;
            break;
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  SUBMENÚ 4 — Registro General POO
// ════════════════════════════════════════════════════════════════════════════
static void menuRegistroGeneral(SistemaHospitalario &sistema)
{
    Menu menu("REGISTRO GENERAL POO");
    menu.add_option("Listar registro general (polimorfismo)");
    menu.add_option("Ver resumen del sistema");
    menu.add_option("Volver al menu principal");

    bool activo = true;
    while (activo)
    {
        int op = menu.get_input();

        switch (op)
        {

        case 1:
        {
            try
            {
                sistema.listarRegistroGeneral();
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 2:
        {
            try
            {
                sistema.mostrarResumen();
            }
            catch (const std::exception &e)
            {
                std::cerr << "\n  [ERROR] " << e.what() << "\n";
            }
            break;
        }

        case 3:
            activo = false;
            break;
        }
    }
}

// ════════════════════════════════════════════════════════════════════════════
//  MAIN — Menú principal
// ════════════════════════════════════════════════════════════════════════════
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    try
    {
        SistemaHospitalario sistema;
        sistema.inicializar();

        Menu menuPrincipal("SISTEMA HOSPITALARIO V2.1");
        menuPrincipal.add_option("Gestion de Pacientes");
        menuPrincipal.add_option("Gestion de Personal");
        menuPrincipal.add_option("Gestion de Citas");
        menuPrincipal.add_option("Registro General POO");
        menuPrincipal.add_option("Guardar datos y salir");

        bool ejecutando = true;
        while (ejecutando)
        {
            int op = menuPrincipal.get_input();

            switch (op)
            {
            case 1:
                menuPacientes(sistema);
                break;
            case 2:
                menuPersonal(sistema);
                break;
            case 3:
                menuCitas(sistema);
                break;
            case 4:
                menuRegistroGeneral(sistema);
                break;
            case 5:
                sistema.guardarDatos();
                std::cout << "\n  Hasta luego.\n\n";
                ejecutando = false;
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n  [FATAL] " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "\n  [FATAL] Excepcion desconocida.\n";
        return 99;
    }

    return 0;
}
