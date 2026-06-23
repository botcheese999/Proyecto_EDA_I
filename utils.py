"""
utils.py — Funciones utilitarias de validación y formateo.

Contiene validadores reutilizables que son consumidos por los módulos
del sistema. Diseñado para extraerse a una clase Validator en la
refactorización a POO.
"""

from datetime import datetime
import config


# ── Validaciones ───────────────────────────────────────────────────────────

def validar_dni(dni):
    """Valida que el DNI sea una cadena de exactamente 8 dígitos."""
    if not isinstance(dni, str):
        return False, "El DNI debe ser una cadena de texto."
    if not dni.isdigit() or len(dni) != 8:
        return False, "El DNI debe tener exactamente 8 dígitos numéricos."
    return True, ""


def validar_tipo_sangre(tipo):
    """Valida que el tipo de sangre esté en los valores permitidos."""
    if tipo not in config.TIPOS_SANGRE:
        tipos = ", ".join(sorted(config.TIPOS_SANGRE))
        return False, f"Tipo de sangre inválido. Válidos: {tipos}"
    return True, ""


def validar_rol(rol):
    """Valida que el rol del personal sea uno de los definidos en la jerarquía."""
    if rol not in config.ROLES_VALIDOS:
        roles = ", ".join(sorted(config.ROLES_VALIDOS))
        return False, f"Rol inválido. Válidos: {roles}"
    return True, ""


def validar_estado_cita(estado):
    """Valida que el estado de la cita sea uno de los permitidos."""
    if estado not in config.ESTADOS_CITA:
        estados = ", ".join(sorted(config.ESTADOS_CITA))
        return False, f"Estado inválido. Válidos: {estados}"
    return True, ""


def validar_turno(turno):
    """Valida que el turno del personal sea uno de los permitidos."""
    if turno not in config.TURNOS_VALIDOS:
        turnos = ", ".join(sorted(config.TURNOS_VALIDOS))
        return False, f"Turno inválido. Válidos: {turnos}"
    return True, ""


def validar_fecha(fecha_str, formato="%Y-%m-%d"):
    """Valida que la cadena represente una fecha válida con el formato dado."""
    try:
        datetime.strptime(fecha_str, formato)
        return True, ""
    except ValueError:
        return False, f"Fecha inválida. Formato esperado: {formato}"


def validar_no_vacio(valor, nombre_campo):
    """Valida que un valor no sea una cadena vacía."""
    if not isinstance(valor, str) or valor.strip() == "":
        return False, f"El campo '{nombre_campo}' no puede estar vacío."
    return True, ""


# ── Helpers de formateo ────────────────────────────────────────────────────

def timestamp_actual():
    """Retorna la fecha y hora actual como cadena ISO simplificada."""
    return datetime.now().strftime("%Y-%m-%d %H:%M:%S")


def fecha_actual():
    """Retorna la fecha actual como cadena YYYY-MM-DD."""
    return datetime.now().strftime("%Y-%m-%d")


def generar_id_cita():
    """Genera un ID de cita auto-incremental con formato C0001."""
    config.contadores["cita"] += 1
    return f"C{config.contadores['cita']:04d}"


def generar_id_personal():
    """Genera un ID de personal auto-incremental con formato P0001."""
    config.contadores["personal"] += 1
    return f"P{config.contadores['personal']:04d}"


def formatear_tupla_paciente(datos_fijos):
    """Formatea la tupla de datos fijos de un paciente para mostrar."""
    dni, sangre, fecha_nac, nombre = datos_fijos
    return (
        f"  Nombre:     {nombre}\n"
        f"  DNI:        {dni}\n"
        f"  Sangre:     {sangre}\n"
        f"  Nacimiento: {fecha_nac}"
    )


def formatear_tupla_personal(datos_fijos):
    """Formatea la tupla de datos fijos del personal para mostrar."""
    id_personal, nombre, rol, especialidad = datos_fijos
    return (
        f"  ID:           {id_personal}\n"
        f"  Nombre:       {nombre}\n"
        f"  Rol:          {rol}\n"
        f"  Especialidad: {especialidad}"
    )


def imprimir_separador(titulo="", ancho=60):
    """Imprime un separador visual con título centrado opcional."""
    if titulo:
        print(f"\n{'═' * ancho}")
        print(f"  {titulo}")
        print(f"{'═' * ancho}")
    else:
        print(f"{'─' * ancho}")
