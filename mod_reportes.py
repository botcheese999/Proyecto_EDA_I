"""
mod_reportes.py — Generación de reportes del sistema hospitalario.

Funciones de solo lectura que consultan los catálogos y preparan datos
para visualización en consola o exportación a CSV.
"""

from collections import Counter

import mod_pacientes
import mod_citas


# ═══════════════════════════════════════════════════════════════════════════
#  REPORTES DE PACIENTES
# ═══════════════════════════════════════════════════════════════════════════

def reporte_alergias_comunes(catalogo_pacientes):
    """
    Genera un reporte de las alergias más frecuentes entre todos los pacientes.
    Usa sets para recopilar y Counter para frecuencia.

    Returns:
        list[tuple(str, int)] — lista de (alergia, cantidad) ordenada desc.
    """
    contador = Counter()
    for dni, registro in catalogo_pacientes.items():
        for alergia in registro["alergias"]:
            contador[alergia] += 1

    return contador.most_common()


def reporte_pacientes_por_tipo_sangre(catalogo_pacientes):
    """
    Agrupa pacientes por tipo de sangre.

    Returns:
        dict[str, list[str]] — {tipo_sangre: [lista de DNIs]}.
    """
    agrupado = {}
    for dni, registro in catalogo_pacientes.items():
        tipo = registro["datos_fijos"][1]
        if tipo not in agrupado:
            agrupado[tipo] = []
        agrupado[tipo].append(dni)
    return agrupado


def reporte_pacientes_sin_alergias(catalogo_pacientes):
    """Retorna lista de DNIs de pacientes sin alergias registradas."""
    sin_alergias = []
    for dni, registro in catalogo_pacientes.items():
        if not registro["alergias"]:
            sin_alergias.append(dni)
    return sin_alergias


# ═══════════════════════════════════════════════════════════════════════════
#  REPORTES DE CITAS
# ═══════════════════════════════════════════════════════════════════════════

def reporte_citas_por_fecha(registro_citas, fecha):
    """
    Retorna las citas de una fecha específica (YYYY-MM-DD).

    Returns:
        list[dict] — citas del día.
    """
    resultados = []
    for id_cita, cita in registro_citas.items():
        if cita["datos_cita"][3] == fecha:
            resultados.append(cita)
    return resultados


def reporte_carga_medicos(registro_citas, catalogo_personal):
    """
    Calcula la carga de trabajo por médico (cantidad de citas).

    Returns:
        list[dict] — [{id_medico, nombre, rol, total_citas, pendientes,
                        completadas, canceladas}]
    """
    # Contar citas por médico
    citas_por_medico = {}
    for id_cita, cita in registro_citas.items():
        id_medico = cita["datos_cita"][2]
        if id_medico not in citas_por_medico:
            citas_por_medico[id_medico] = {
                "total": 0, "pendiente": 0, "en_atencion": 0,
                "completada": 0, "cancelada": 0,
            }
        citas_por_medico[id_medico]["total"] += 1
        estado = cita["estado"]
        if estado in citas_por_medico[id_medico]:
            citas_por_medico[id_medico][estado] += 1

    # Combinar con datos del personal
    resultado = []
    for id_medico, conteo in citas_por_medico.items():
        personal = catalogo_personal.get(id_medico)
        nombre = personal["datos_fijos"][1] if personal else "Desconocido"
        rol = personal["datos_fijos"][2] if personal else "N/A"

        resultado.append({
            "id_medico": id_medico,
            "nombre": nombre,
            "rol": rol,
            "total_citas": conteo["total"],
            "pendientes": conteo["pendiente"],
            "en_atencion": conteo["en_atencion"],
            "completadas": conteo["completada"],
            "canceladas": conteo["cancelada"],
        })

    # Ordenar por total de citas descendente
    resultado.sort(key=lambda x: x["total_citas"], reverse=True)
    return resultado


def reporte_resumen_general(catalogo_pacientes, catalogo_personal,
                            registro_citas):
    """
    Genera un resumen general del sistema.

    Returns:
        dict — estadísticas generales.
    """
    total_pacientes = len(catalogo_pacientes)
    total_personal = len(catalogo_personal)
    total_citas = len(registro_citas)

    # Contar personal activo
    personal_activo = sum(
        1 for reg in catalogo_personal.values()
        if reg["datos_mutables"]["activo"]
    )

    # Contar citas por estado
    estados = {"pendiente": 0, "en_atencion": 0, "completada": 0, "cancelada": 0}
    for cita in registro_citas.values():
        estado = cita["estado"]
        if estado in estados:
            estados[estado] += 1

    return {
        "total_pacientes": total_pacientes,
        "total_personal": total_personal,
        "personal_activo": personal_activo,
        "total_citas": total_citas,
        "citas_pendientes": estados["pendiente"],
        "citas_en_atencion": estados["en_atencion"],
        "citas_completadas": estados["completada"],
        "citas_canceladas": estados["cancelada"],
    }


# ═══════════════════════════════════════════════════════════════════════════
#  FORMATEO PARA CONSOLA
# ═══════════════════════════════════════════════════════════════════════════

def mostrar_resumen_general(catalogo_pacientes, catalogo_personal,
                            registro_citas):
    """Imprime el resumen general en consola con formato legible."""
    resumen = reporte_resumen_general(catalogo_pacientes, catalogo_personal,
                                     registro_citas)
    print("\n" + "═" * 50)
    print("  RESUMEN GENERAL DEL SISTEMA")
    print("═" * 50)
    print(f"  Pacientes registrados : {resumen['total_pacientes']}")
    print(f"  Personal total        : {resumen['total_personal']}")
    print(f"  Personal activo       : {resumen['personal_activo']}")
    print(f"  ─────────────────────────────")
    print(f"  Citas totales         : {resumen['total_citas']}")
    print(f"    ├─ Pendientes       : {resumen['citas_pendientes']}")
    print(f"    ├─ En atención      : {resumen['citas_en_atencion']}")
    print(f"    ├─ Completadas      : {resumen['citas_completadas']}")
    print(f"    └─ Canceladas       : {resumen['citas_canceladas']}")
    print("═" * 50)


def mostrar_carga_medicos(registro_citas, catalogo_personal):
    """Imprime la carga de médicos en consola."""
    carga = reporte_carga_medicos(registro_citas, catalogo_personal)
    if not carga:
        print("  No hay datos de carga médica.")
        return

    print(f"\n  {'ID':<8} {'Nombre':<20} {'Rol':<16} {'Total':<7} "
          f"{'Pend.':<7} {'Compl.':<7} {'Canc.':<7}")
    print("  " + "─" * 72)
    for m in carga:
        print(f"  {m['id_medico']:<8} {m['nombre']:<20} {m['rol']:<16} "
              f"{m['total_citas']:<7} {m['pendientes']:<7} "
              f"{m['completadas']:<7} {m['canceladas']:<7}")
