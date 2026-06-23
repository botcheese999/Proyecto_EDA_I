"""
mod_persistencia.py — Persistencia de datos con JSON.

Maneja la serialización/deserialización de los catálogos del sistema,
incluyendo la conversión especial de sets y tuplas que JSON no soporta
nativamente. Usa un esquema de marcado {"__type__": "set"|"tuple", "data": [...]}.
"""

import json
import os

import config


# ═══════════════════════════════════════════════════════════════════════════
#  CODIFICADOR/DECODIFICADOR PERSONALIZADO
# ═══════════════════════════════════════════════════════════════════════════

def _codificar_estructura(obj):
    """
    Convierte recursivamente sets y tuplas a un formato serializable.

    - set  → {"__type__": "set",   "data": [...]}
    - tuple → {"__type__": "tuple", "data": [...]}
    """
    if isinstance(obj, set):
        return {"__type__": "set", "data": sorted(list(obj))}
    elif isinstance(obj, tuple):
        return {"__type__": "tuple", "data": [_codificar_estructura(item) for item in obj]}
    elif isinstance(obj, dict):
        return {k: _codificar_estructura(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [_codificar_estructura(item) for item in obj]
    else:
        return obj


def _decodificar_estructura(obj):
    """
    Reconstruye sets y tuplas a partir del formato serializado.
    """
    if isinstance(obj, dict):
        if "__type__" in obj and "data" in obj:
            tipo = obj["__type__"]
            datos = obj["data"]
            if tipo == "set":
                return set(_decodificar_estructura(item) for item in datos)
            elif tipo == "tuple":
                return tuple(_decodificar_estructura(item) for item in datos)
        return {k: _decodificar_estructura(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [_decodificar_estructura(item) for item in obj]
    else:
        return obj


# ═══════════════════════════════════════════════════════════════════════════
#  OPERACIONES DE ARCHIVO
# ═══════════════════════════════════════════════════════════════════════════

def guardar_json(datos, ruta):
    """
    Serializa datos a JSON con soporte para sets y tuplas.

    Args:
        datos: dict | list — datos a serializar.
        ruta: str — ruta del archivo destino.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    try:
        # Asegurar que el directorio exista
        directorio = os.path.dirname(ruta)
        if directorio and not os.path.exists(directorio):
            os.makedirs(directorio)

        datos_codificados = _codificar_estructura(datos)

        with open(ruta, "w", encoding="utf-8") as archivo:
            json.dump(datos_codificados, archivo, ensure_ascii=False, indent=2)

        return True, f"Datos guardados en {ruta}"

    except (IOError, OSError) as e:
        return False, f"Error al guardar {ruta}: {str(e)}"


def cargar_json(ruta):
    """
    Deserializa datos desde JSON reconstruyendo sets y tuplas.

    Args:
        ruta: str — ruta del archivo fuente.

    Returns:
        tuple(bool, dict | list | str) — (éxito, datos o mensaje de error).
    """
    if not os.path.exists(ruta):
        return False, f"Archivo no encontrado: {ruta}"

    try:
        with open(ruta, "r", encoding="utf-8") as archivo:
            datos_crudos = json.load(archivo)

        datos = _decodificar_estructura(datos_crudos)
        return True, datos

    except json.JSONDecodeError as e:
        return False, f"Error de formato JSON en {ruta}: {str(e)}"
    except (IOError, OSError) as e:
        return False, f"Error al leer {ruta}: {str(e)}"


# ═══════════════════════════════════════════════════════════════════════════
#  GUARDAR / CARGAR TODO EL SISTEMA
# ═══════════════════════════════════════════════════════════════════════════

def guardar_todo(catalogo_pacientes, catalogo_personal, registro_citas):
    """
    Guarda los tres catálogos principales a sus archivos JSON.

    Returns:
        tuple(bool, str) — (éxito, mensaje resumen).
    """
    resultados = []

    ok, msg = guardar_json(catalogo_pacientes, config.RUTA_PACIENTES)
    resultados.append(("Pacientes", ok, msg))

    ok, msg = guardar_json(catalogo_personal, config.RUTA_PERSONAL)
    resultados.append(("Personal", ok, msg))

    ok, msg = guardar_json(registro_citas, config.RUTA_CITAS)
    resultados.append(("Citas", ok, msg))

    # Guardar contadores
    ruta_contadores = os.path.join(config.DATOS_DIR, "contadores.json")
    ok, msg = guardar_json(config.contadores, ruta_contadores)
    resultados.append(("Contadores", ok, msg))

    errores = [r for r in resultados if not r[1]]
    if errores:
        mensajes_error = "; ".join(f"{r[0]}: {r[2]}" for r in errores)
        return False, f"Errores al guardar: {mensajes_error}"

    return True, "Todos los datos guardados exitosamente."


def cargar_todo():
    """
    Carga los tres catálogos principales desde sus archivos JSON.
    Si un archivo no existe, retorna un dict/list vacío para ese catálogo.

    Returns:
        tuple(dict, dict, dict) — (catalogo_pacientes, catalogo_personal,
                                    registro_citas).
    """
    # Cargar pacientes
    ok, datos = cargar_json(config.RUTA_PACIENTES)
    catalogo_pacientes = datos if ok else {}

    # Cargar personal
    ok, datos = cargar_json(config.RUTA_PERSONAL)
    catalogo_personal = datos if ok else {}

    # Cargar citas
    ok, datos = cargar_json(config.RUTA_CITAS)
    registro_citas = datos if ok else {}

    # Cargar contadores
    ruta_contadores = os.path.join(config.DATOS_DIR, "contadores.json")
    ok, datos = cargar_json(ruta_contadores)
    if ok and isinstance(datos, dict):
        config.contadores.update(datos)
    else:
        # Recalcular contadores basándose en IDs existentes
        _recalcular_contadores(catalogo_personal, registro_citas)

    return catalogo_pacientes, catalogo_personal, registro_citas


def _recalcular_contadores(catalogo_personal, registro_citas):
    """
    Recalcula los contadores de IDs a partir de los datos existentes
    para evitar colisiones al generar nuevos IDs.
    """
    # Contador de personal: buscar el mayor ID P####
    max_personal = 0
    for id_pers in catalogo_personal:
        if id_pers.startswith("P") and id_pers[1:].isdigit():
            num = int(id_pers[1:])
            if num > max_personal:
                max_personal = num
    config.contadores["personal"] = max_personal

    # Contador de citas: buscar el mayor ID C####
    max_cita = 0
    for id_cita in registro_citas:
        if id_cita.startswith("C") and id_cita[1:].isdigit():
            num = int(id_cita[1:])
            if num > max_cita:
                max_cita = num
    config.contadores["cita"] = max_cita
