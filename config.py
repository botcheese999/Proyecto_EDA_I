"""
config.py — Constantes globales y configuración del sistema hospitalario.

Define rutas de persistencia, jerarquía del personal, permisos por nivel,
y valores válidos para validaciones.
"""

import os

# ── Rutas de archivos ──────────────────────────────────────────────────────
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
DATOS_DIR = os.path.join(BASE_DIR, "datos")
REPORTES_DIR = os.path.join(BASE_DIR, "reportes")

RUTA_PACIENTES = os.path.join(DATOS_DIR, "pacientes.json")
RUTA_PERSONAL = os.path.join(DATOS_DIR, "personal.json")
RUTA_CITAS = os.path.join(DATOS_DIR, "citas.json")

# ── Jerarquía del personal (nivel numérico) ────────────────────────────────
JERARQUIA = {
    "director": 4,
    "especialista": 3,
    "medico_general": 2,
    "enfermera": 1,
}

# ── Permisos por nivel jerárquico ──────────────────────────────────────────
PERMISOS = {
    4: {"leer_todo", "escribir_todo", "gestionar_personal",
        "ver_reportes", "deshacer_medicacion"},
    3: {"leer_historial", "escribir_historial", "recetar",
        "ver_reportes", "deshacer_medicacion"},
    2: {"leer_historial", "escribir_historial", "recetar"},
    1: {"leer_historial", "registrar_signos_vitales"},
}

# ── Valores válidos ────────────────────────────────────────────────────────
TIPOS_SANGRE = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"}

ROLES_VALIDOS = set(JERARQUIA.keys())

ESTADOS_CITA = {"pendiente", "en_atencion", "completada", "cancelada"}

TURNOS_VALIDOS = {"mañana", "tarde", "noche"}

# ── Contadores para IDs auto-incrementales ─────────────────────────────────
# Se actualizan al cargar datos existentes en mod_persistencia.
contadores = {
    "cita": 0,
    "personal": 0,
}
