@echo off
REM ═══════════════════════════════════════════════════════════════════
REM  build.bat — Script de compilacion para Sistema Hospitalario V2.1
REM  Uso: Abrir terminal en la carpeta v2.1/ y ejecutar: build.bat
REM ═══════════════════════════════════════════════════════════════════

echo Compilando Sistema Hospitalario V2.1...

g++ -std=c++14 -Wall -Wextra -o sistema_hospitalario.exe ^
    main.cpp Persona.cpp Paciente.cpp Personal.cpp Cita.cpp ^
    GestorPacientes.cpp GestorPersonal.cpp GestorCitas.cpp ^
    Persistencia.cpp SistemaHospitalario.cpp

if %ERRORLEVEL% EQU 0 (
    echo [OK] Compilacion exitosa con 0 errores.
    echo.
    echo Ejecutando...
    echo ═══════════════════════════════════════════════════════════════════
    sistema_hospitalario.exe
) else (
    echo [ERROR] La compilacion fallo con codigo %ERRORLEVEL%.
)
