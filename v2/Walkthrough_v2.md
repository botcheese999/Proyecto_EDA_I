# Sistema Hospitalario V2 — Refactorización POO en C++

## Resumen

Refactorización completa del sistema hospitalario desde Python estructurado (V1) a C++ con Programación Orientada a Objetos (V2). **Compilación limpia con 0 warnings** y ejecución exitosa.

## Archivos Creados en `v2/`

| Archivo | Tipo | Propósito |
|---------|------|-----------|
| [Persona.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Persona.h) | Header | Superclase abstracta con atributos `protected` |
| [Persona.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Persona.cpp) | Source | Implementación base (destructor virtual, getters/setters) |
| [Paciente.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Paciente.h) | Header | Clase hija con datos médicos, alergias, historial |
| [Paciente.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Paciente.cpp) | Source | Implementación con `mostrarHistorialCompleto()` (RTTI) |
| [Personal.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Personal.h) | Header | Clase hija con jerarquía, permisos, turnos |
| [Personal.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Personal.cpp) | Source | Implementación con `mostrarPermisos()` (RTTI) |
| [Cita.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Cita.h) | Header | Entidad de cita médica encapsulada |
| [Cita.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/Cita.cpp) | Source | Estados validados, medicación |
| [GestorPacientes.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/GestorPacientes.h) | Header | Lógica de negocio de pacientes |
| [GestorPacientes.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/GestorPacientes.cpp) | Source | CRUD, alergias, intersección de sets |
| [GestorCitas.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/GestorCitas.h) | Header | Lógica de negocio de citas |
| [GestorCitas.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/GestorCitas.cpp) | Source | FIFO espera, LIFO deshacer, CRUD citas |
| [SistemaHospitalario.h](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/SistemaHospitalario.h) | Header | Fachada orquestadora + vector polimórfico |
| [SistemaHospitalario.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/SistemaHospitalario.cpp) | Source | Demostración integral de los 5 criterios |
| [main.cpp](file:///c:/Users/Lolie/.gemini/antigravity/scratch/clinica_system/v2/main.cpp) | Entry | Punto de entrada con instrucciones de compilación |

## Cumplimiento de los 5 Criterios

### Criterio 1: Abstracción y Encapsulamiento
- Atributos **`private`** en Paciente, Personal, Cita
- Atributos **`protected`** en Persona (para herencia)
- Constructores con **listas de inicialización** en todas las clases
- Getters/Setters con **validaciones lógicas** (DNI 8 dígitos, email con '@', tipos de sangre, roles, turnos)

### Criterio 2: Arquitectura de Gestores
- **`GestorPacientes`**: CRUD, alergias (set), intersección
- **`GestorCitas`**: CRUD, lista de espera FIFO, pila deshacer LIFO
- **`SistemaHospitalario`**: patrón **Fachada** que orquesta los gestores

### Criterio 3: Herencia Simple
```
Persona (superclase abstracta)
  ├── Paciente (hereda id_, nombre_, telefono_)
  └── Personal (hereda id_, nombre_, telefono_)
```

### Criterio 4: Polimorfismo y Enlace Dinámico
- `std::vector<Persona*>` con objetos instanciados con **`new`**
- Métodos **`virtual`** (`mostrarInfo()`, `getTipo()`) + **`override`**
- **Destructor virtual** en Persona (obligatorio)
- Procesamiento uniforme en `listarRegistroGeneral()`

### Criterio 5: Downcasting Seguro (RTTI)
- Función `identificarYProcesar(Persona*)` usa **`dynamic_cast`**
- Si `dynamic_cast<Paciente*>` exitoso → llama `mostrarHistorialCompleto()` (exclusivo)
- Si `dynamic_cast<Personal*>` exitoso → llama `mostrarPermisos()` (exclusivo)
- Estos métodos **NO existen** en la superclase Persona

## Compilación y Ejecución

```bash
g++ -std=c++17 -Wall -o sistema_hospitalario main.cpp Persona.cpp Paciente.cpp Personal.cpp Cita.cpp GestorPacientes.cpp GestorCitas.cpp SistemaHospitalario.cpp
./sistema_hospitalario
```

## Verificación
- ✅ Compilación limpia con `-Wall` (0 warnings)
- ✅ Ejecución completa de los 5 criterios
- ✅ Toda la memoria dinámica liberada por destructores
