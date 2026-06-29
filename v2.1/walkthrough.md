# Sistema Hospitalario V2.1 — Walkthrough

## Resumen de Cambios V2 → V2.1

| Cambio                         | Detalle                                                                                                |
| ------------------------------ | ------------------------------------------------------------------------------------------------------ |
| ❌ **FIFO (Cola)**             | Eliminada `listaEspera_`, `EntradaEspera`, todos los métodos de espera                                 |
| ❌ **LIFO (Pila)**             | Eliminada `pilaDeshacer_`, `AccionMedicacion`, `deshacerMedicacion()`                                  |
| ❌ **Historial clínico**       | Eliminados `EntradaHistorial`, `historial_`, `agregarEntradaHistorial()`, `mostrarHistorialCompleto()` |
| ❌ **ejecutarDemostracion()**  | Reemplazado por la interfaz de usuario interactiva por consola                                         |
| ✅ **Persistencia CSV**        | `<fstream>` con RAII para lectura/escritura de pacientes, personal y citas                             |
| ✅ **Excepciones**             | `try/catch/throw` con `std::runtime_error` y `std::invalid_argument`                                   |
| ✅ **Mock Data**               | Generación automática de CSV si no existen (Plug-and-Play)                                             |
| ✅ **GestorPersonal**          | NUEVO gestor dedicado para personal médico                                                             |
| ✅ **Método RTTI**             | `mostrarFichaMedica()` reemplaza `mostrarHistorialCompleto()` en Paciente                              |
| ✅ **Clase Menu**              | NUEVA clase encapsulada para la interfaz de usuario por consola                                        |
| ✅ **4 Submenús interactivos** | Pacientes, Personal, Citas, Registro General POO                                                       |
| ✅ **Soporte UTF-8**           | `SetConsoleOutputCP(65001)` para output correcto en Windows                                            |

## Arquitectura V2.1

```
                    ┌─────────────────────────┐
                    │        main.cpp          │   ← UI + orquestación
                    │  Menu (x4 submenús)      │
                    └─────────┬───────────────┘
                              │
                    ┌─────────▼───────────────┐
                    │   SistemaHospitalario    │   ← Fachada
                    │  (try/catch top-level)   │
                    └─────────┬───────────────┘
                              │
          ┌───────────────────┼───────────────────┐
          │                   │                    │
  GestorPacientes    GestorPersonal       GestorCitas
          │                   │                    │
          └───────────────────┼───────────────────┘
                              │
                       Persistencia           ← CSV I/O (RAII)
                      /      |       \
              pacientes.csv  personal.csv  citas.csv

  Herencia:  Persona (abstracta)
               ├── Paciente   → mostrarFichaMedica() [RTTI]
               └── Personal   → mostrarPermisos()    [RTTI]

  Polimorfismo: vector<Persona*> + virtual/override + destructor virtual
```

## Archivos Creados en `v2.1/`

| Archivo                   | Propósito                                                                     |
| ------------------------- | ----------------------------------------------------------------------------- |
| `Persona.h`               | Superclase abstracta (protected, virtual)                                     |
| `Persona.cpp`             | Implementación base                                                           |
| `Paciente.h`              | Sin historial, RTTI = `mostrarFichaMedica()`                                  |
| `Paciente.cpp`            | Implementación con alergias (set)                                             |
| `Personal.h`              | Jerarquía, permisos, RTTI = `mostrarPermisos()`                               |
| `Personal.cpp`            | Implementación con roles/turnos                                               |
| `Cita.h`                  | Simplificada (sin LIFO undo)                                                  |
| `Cita.cpp`                | CRUD básico de citas                                                          |
| `GestorPacientes.h`       | Sin historial, con `getCatalogo()` para CSV                                   |
| `GestorPacientes.cpp`     | CRUD + alergias                                                               |
| `GestorPersonal.h`        | **NUEVO** — Gestor dedicado para personal                                     |
| `GestorPersonal.cpp`      | CRUD + registro por ID auto                                                   |
| `GestorCitas.h`           | Sin FIFO/LIFO — solo CRUD                                                     |
| `GestorCitas.cpp`         | Crear, completar, cancelar, filtrar                                           |
| `Persistencia.h`          | **NUEVO** — CSV I/O + RAII + excepciones + mock data                          |
| `Persistencia.cpp`        | Implementación completa con split/join CSV                                    |
| `SistemaHospitalario.h`   | Fachada con Persistencia integrada                                            |
| `SistemaHospitalario.cpp` | Lógica de negocio + polimorfismo + RTTI                                       |
| `Menu.h`                  | **NUEVO** — Clase encapsulada de UI por consola                               |
| `Menu.cpp`                | **NUEVO** — `add_option()`, `display()`, `get_input()` con validación robusta |
| `main.cpp`                | **MODIFICADO** — 4 submenús interactivos orquestados vía fachada              |
| `build.bat`               | Script de compilación y ejecución                                             |

**Generados automáticamente en `datos/`:**

| Archivo               | Contenido                                              |
| --------------------- | ------------------------------------------------------ |
| `datos/pacientes.csv` | 5 pacientes con alergias                               |
| `datos/personal.csv`  | 4 personal (director, especialista, médico, enfermera) |
| `datos/citas.csv`     | 4+ citas en diferentes estados                         |

## Clase Menu — Diseño

```cpp
class Menu {
private:
    std::string titulo_;
    std::vector<std::string> opciones_;  // opciones almacenadas en vector
public:
    void add_option(const std::string& opcion);
    void display() const;
    int  get_input() const;   // do-while + cin.fail() + cin.clear() + cin.ignore()
};
```

`Menu` es completamente agnóstica al dominio hospitalario — no incluye ningún header de negocio. Cumple responsabilidad única: solo gestiona opciones y valida entrada.

### Validación Robusta en `get_input()`

```cpp
do {
    display();
    std::cin >> opcion;

    if (std::cin.fail()) {
        std::cin.clear();   // resetea failbit del stream
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // vacía buffer
        opcion = 0;
        continue;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

} while (opcion < 1 || opcion > max);
```

Sin `clear()` + `ignore()`, un input de texto activa `cin.fail()` y el buffer queda "atascado": el `do-while` gira infinitamente sin pausar porque el stream nunca avanza.

## Submenús Interactivos

| Submenú                      | Opciones                                                           |
| ---------------------------- | ------------------------------------------------------------------ |
| **1 — Pacientes**            | Registrar, Buscar por DNI, Listar, Eliminar, Agregar alergia       |
| **2 — Personal**             | Registrar, Buscar por ID, Listar                                   |
| **3 — Citas**                | Crear, Completar, Cancelar, Listar por estado, Listar por paciente |
| **4 — Registro General POO** | Listar polimórfico (`vector<Persona*>`), Resumen del sistema       |

Cada `case` en los submenús envuelve la llamada a la fachada en `try-catch(std::exception&)`. Si la lógica de negocio lanza, se imprime `e.what()` y el menú continúa sin colapsar.

## Soporte UTF-8 en Windows

```cpp
#ifdef _WIN32
#include <windows.h>
#endif

// Dentro de main(), antes de todo:
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
```

`65001` es el code page de UTF-8 en Windows. Los `#ifdef _WIN32` garantizan compilación cruzada sin modificar código en Linux.

## Criterios del Sílabo + V2.1

### RAII (Resource Acquisition Is Initialization)

Los `std::ifstream` y `std::ofstream` son **variables locales** en cada método de Persistencia. Al salir del scope (incluso por excepción), su destructor cierra el archivo automáticamente. **No se usa `close()` manual**.

### Excepciones — Jerarquía de Captura

```cpp
// main.cpp — try/catch por cada case del menú
try {
    sistema.getGestorPacientes().registrar(...);
} catch (const std::exception& e) {
    std::cerr << "[ERROR] " << e.what() << "\n";
    // el menú continúa
}

// SistemaHospitalario::inicializar() — nivel interno
try {
    persistencia_.cargarTodo(...);
} catch (const std::runtime_error& e) { ... }
  catch (const std::invalid_argument& e) { ... }
```

### Plug-and-Play

1. Clonar repositorio
2. Compilar: `g++ -std=c++14 -Wall -o sistema_hospitalario main.cpp Menu.cpp Persona.cpp Paciente.cpp Personal.cpp Cita.cpp GestorPacientes.cpp GestorPersonal.cpp GestorCitas.cpp Persistencia.cpp SistemaHospitalario.cpp`
3. Ejecutar: `./sistema_hospitalario`
4. → CSV generados automáticamente si no existen, menú interactivo listo

## Verificación

- ✅ Compilación limpia con `-Wall` (C++14, 0 warnings)
- ✅ Clase `Menu` encapsulada e independiente del dominio
- ✅ `get_input()` con `do-while` + `cin.fail()` + `cin.clear()` + `cin.ignore()`
- ✅ 4 submenús interactivos orquestados exclusivamente vía fachada
- ✅ `try-catch` por operación en cada submenú — menú nunca colapsa ante error
- ✅ Mock data generado automáticamente en `datos/`
- ✅ Persistencia bidireccional: cambios en menú → guardados en CSV al salir
- ✅ Los 5 criterios POO funcionan correctamente
- ✅ Soporte UTF-8 en Windows con `SetConsoleOutputCP(65001)`
- ✅ Memoria dinámica liberada por destructores
