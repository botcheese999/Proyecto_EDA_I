"""
mod_exportacion_csv.py — Exportación de datos a archivos CSV.

Proporciona funciones genéricas y específicas para exportar datos del
sistema hospitalario a formato CSV, utilizable en Excel/LibreOffice.
"""

import csv
import os
from datetime import datetime

import config


# ═══════════════════════════════════════════════════════════════════════════
#  EXPORTACIÓN GENÉRICA
# ═══════════════════════════════════════════════════════════════════════════

def exportar_a_csv(datos, nombre_archivo, encabezados):
    """
    Exporta una lista de diccionarios a un archivo CSV.

    Args:
        datos: list[dict] — lista de filas, cada una un diccionario.
        nombre_archivo: str — nombre del archivo (sin ruta).
        encabezados: list[str] — nombres de las columnas.

    Returns:
        tuple(bool, str) — (éxito, ruta completa o mensaje de error).
    """
    if not datos:
        return False, "No hay datos para exportar."

    # Asegurar que el directorio de reportes exista
    if not os.path.exists(config.REPORTES_DIR):
        os.makedirs(config.REPORTES_DIR)

    # Agregar timestamp al nombre del archivo
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    nombre_final = f"{nombre_archivo}_{timestamp}.csv"
    ruta = os.path.join(config.REPORTES_DIR, nombre_final)

    try:
        with open(ruta, "w", newline="", encoding="utf-8-sig") as archivo:
            escritor = csv.DictWriter(archivo, fieldnames=encabezados,
                                      extrasaction="ignore")
            escritor.writeheader()
            for fila in datos:
                escritor.writerow(fila)

        return True, ruta

    except (IOError, OSError) as e:
        return False, f"Error al exportar CSV: {str(e)}"


# ═══════════════════════════════════════════════════════════════════════════
#  EXPORTACIONES ESPECÍFICAS
# ═══════════════════════════════════════════════════════════════════════════

def exportar_reporte_pacientes(catalogo_pacientes):
    """
    Exporta el catálogo de pacientes a CSV.

    Returns:
        tuple(bool, str) — (éxito, ruta o mensaje).
    """
    filas = []
    for dni, registro in catalogo_pacientes.items():
        datos_fijos = registro["datos_fijos"]
        datos_mut = registro["datos_mutables"]
        alergias = registro["alergias"]

        fila = {
            "DNI": datos_fijos[0],
            "Nombre": datos_fijos[3],
            "Tipo_Sangre": datos_fijos[1],
            "Fecha_Nacimiento": datos_fijos[2],
            "Telefono": datos_mut.get("telefono", ""),
            "Direccion": datos_mut.get("direccion", ""),
            "Email": datos_mut.get("email", ""),
            "Contacto_Emergencia": datos_mut.get("contacto_emergencia", ""),
            "Alergias": ", ".join(sorted(alergias)) if alergias else "Ninguna",
            "Total_Consultas": len(registro["historial"]),
        }
        filas.append(fila)

    encabezados = [
        "DNI", "Nombre", "Tipo_Sangre", "Fecha_Nacimiento",
        "Telefono", "Direccion", "Email", "Contacto_Emergencia",
        "Alergias", "Total_Consultas",
    ]

    return exportar_a_csv(filas, "reporte_pacientes", encabezados)


def exportar_reporte_personal(catalogo_personal):
    """
    Exporta el catálogo del personal a CSV.

    Returns:
        tuple(bool, str) — (éxito, ruta o mensaje).
    """
    filas = []
    for id_pers, registro in catalogo_personal.items():
        datos_fijos = registro["datos_fijos"]
        datos_mut = registro["datos_mutables"]

        fila = {
            "ID": datos_fijos[0],
            "Nombre": datos_fijos[1],
            "Rol": datos_fijos[2],
            "Especialidad": datos_fijos[3],
            "Nivel_Acceso": registro["nivel_acceso"],
            "Turno": datos_mut.get("turno", ""),
            "Telefono": datos_mut.get("telefono", ""),
            "Activo": "Sí" if datos_mut.get("activo", False) else "No",
            "Permisos": ", ".join(sorted(registro["permisos"])),
        }
        filas.append(fila)

    encabezados = [
        "ID", "Nombre", "Rol", "Especialidad", "Nivel_Acceso",
        "Turno", "Telefono", "Activo", "Permisos",
    ]

    return exportar_a_csv(filas, "reporte_personal", encabezados)


def exportar_reporte_citas(registro_citas):
    """
    Exporta el registro de citas a CSV.

    Returns:
        tuple(bool, str) — (éxito, ruta o mensaje).
    """
    filas = []
    for id_cita, cita in registro_citas.items():
        datos_cita = cita["datos_cita"]
        medicacion = cita["medicacion_actual"]

        fila = {
            "ID_Cita": datos_cita[0],
            "DNI_Paciente": datos_cita[1],
            "ID_Medico": datos_cita[2],
            "Fecha": datos_cita[3],
            "Estado": cita["estado"],
            "Motivo": cita.get("motivo", ""),
            "Medicacion": ", ".join(medicacion) if medicacion else "Ninguna",
            "Notas": cita.get("notas", ""),
        }
        filas.append(fila)

    encabezados = [
        "ID_Cita", "DNI_Paciente", "ID_Medico", "Fecha",
        "Estado", "Motivo", "Medicacion", "Notas",
    ]

    return exportar_a_csv(filas, "reporte_citas", encabezados)


def exportar_historial_paciente(catalogo_pacientes, dni):
    """
    Exporta el historial médico de un paciente específico a CSV.

    Returns:
        tuple(bool, str) — (éxito, ruta o mensaje).
    """
    paciente = catalogo_pacientes.get(dni)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni}."

    historial = paciente["historial"]
    if not historial:
        return False, f"El paciente {dni} no tiene historial."

    nombre = paciente["datos_fijos"][3]
    filas = []
    for entrada in historial:
        fila = {
            "Fecha": entrada.get("fecha", ""),
            "Tipo": entrada.get("tipo", ""),
            "Medico_ID": entrada.get("medico_id", ""),
            "Diagnostico": entrada.get("diagnostico", ""),
            "Medicacion": ", ".join(entrada.get("medicacion", [])),
            "Notas": entrada.get("notas", ""),
        }
        filas.append(fila)

    encabezados = [
        "Fecha", "Tipo", "Medico_ID", "Diagnostico", "Medicacion", "Notas",
    ]

    nombre_archivo = f"historial_{dni}_{nombre.replace(' ', '_')}"
    return exportar_a_csv(filas, nombre_archivo, encabezados)
