"""
mod_personal.py — Subsistema de gestión del personal hospitalario.

Modela la jerarquía del personal (director > especialista > médico general >
enfermera) con permisos basados en nivel de acceso. El catálogo usa un dict
con ID como clave para búsqueda O(1). Los permisos se almacenan en sets para
verificación O(1).

Patrón: todas las funciones reciben el catálogo como primer argumento.
"""

import config
import utils


# ═══════════════════════════════════════════════════════════════════════════
#  CRUD DE PERSONAL
# ═══════════════════════════════════════════════════════════════════════════

def registrar_personal(catalogo, nombre, rol, especialidad="General",
                       turno="mañana", telefono=""):
    """
    Registra un nuevo miembro del personal con permisos automáticos
    según su rol jerárquico.

    Args:
        catalogo: dict — catálogo principal del personal.
        nombre: str — nombre completo.
        rol: str — rol en la jerarquía (director, especialista, etc.)
        especialidad: str — especialidad médica.
        turno: str — turno asignado (mañana, tarde, noche).
        telefono: str — teléfono de contacto.

    Returns:
        tuple(bool, str) — (éxito, mensaje con ID generado).
    """
    # Validaciones
    ok, msg = utils.validar_no_vacio(nombre, "nombre")
    if not ok:
        return False, msg

    ok, msg = utils.validar_rol(rol)
    if not ok:
        return False, msg

    ok, msg = utils.validar_turno(turno)
    if not ok:
        return False, msg

    # Generar ID y obtener nivel/permisos
    id_personal = utils.generar_id_personal()
    nivel = config.JERARQUIA[rol]
    permisos = set(config.PERMISOS[nivel])  # Copia del set de permisos

    # Crear registro
    catalogo[id_personal] = {
        "datos_fijos": (id_personal, nombre, rol, especialidad),  # TUPLA
        "datos_mutables": {
            "turno": turno,
            "telefono": telefono,
            "activo": True,
        },
        "nivel_acceso": nivel,
        "permisos": permisos,  # SET
    }

    return True, f"Personal '{nombre}' registrado con ID: {id_personal}"


def buscar_personal(catalogo, id_personal):
    """
    Busca un miembro del personal por ID en O(1).

    Returns:
        dict | None — registro del personal o None si no existe.
    """
    return catalogo.get(id_personal)


def eliminar_personal(catalogo, id_personal):
    """
    Elimina un miembro del personal del catálogo.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    if id_personal not in catalogo:
        return False, f"No se encontró personal con ID {id_personal}."

    nombre = catalogo[id_personal]["datos_fijos"][1]
    del catalogo[id_personal]
    return True, f"Personal '{nombre}' eliminado."


def listar_personal(catalogo):
    """
    Retorna una lista de tuplas (id, nombre, rol, activo) de todo el personal.
    """
    resultado = []
    for id_pers, registro in catalogo.items():
        datos = registro["datos_fijos"]
        activo = registro["datos_mutables"]["activo"]
        resultado.append((datos[0], datos[1], datos[2], activo))
    return resultado


# ═══════════════════════════════════════════════════════════════════════════
#  JERARQUÍA Y CONTROL DE ACCESO
# ═══════════════════════════════════════════════════════════════════════════

def verificar_acceso(catalogo, id_personal, permiso):
    """
    Verifica si un miembro del personal tiene un permiso específico.
    Búsqueda en set: O(1).

    Args:
        id_personal: str — ID del miembro del personal.
        permiso: str — nombre del permiso a verificar.

    Returns:
        bool — True si tiene el permiso, False en caso contrario.
    """
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return False
    if not personal["datos_mutables"]["activo"]:
        return False
    return permiso in personal["permisos"]  # O(1) en set


def obtener_nivel(catalogo, id_personal):
    """
    Retorna el nivel jerárquico de un miembro del personal.

    Returns:
        int | None — nivel numérico o None si no existe.
    """
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return None
    return personal["nivel_acceso"]


def obtener_permisos(catalogo, id_personal):
    """
    Retorna el set de permisos de un miembro del personal.

    Returns:
        set | None — conjunto de permisos o None si no existe.
    """
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return None
    return personal["permisos"]


def tiene_mayor_nivel(catalogo, id_personal_1, id_personal_2):
    """
    Compara si el personal 1 tiene mayor nivel jerárquico que el 2.

    Returns:
        tuple(bool, str) — (resultado, mensaje descriptivo).
    """
    nivel_1 = obtener_nivel(catalogo, id_personal_1)
    nivel_2 = obtener_nivel(catalogo, id_personal_2)

    if nivel_1 is None:
        return False, f"No se encontró personal con ID {id_personal_1}."
    if nivel_2 is None:
        return False, f"No se encontró personal con ID {id_personal_2}."

    if nivel_1 > nivel_2:
        return True, f"{id_personal_1} (nivel {nivel_1}) > {id_personal_2} (nivel {nivel_2})"
    elif nivel_1 == nivel_2:
        return False, f"Ambos tienen el mismo nivel ({nivel_1})."
    else:
        return False, f"{id_personal_1} (nivel {nivel_1}) < {id_personal_2} (nivel {nivel_2})"


# ═══════════════════════════════════════════════════════════════════════════
#  FILTRADO Y BÚSQUEDA
# ═══════════════════════════════════════════════════════════════════════════

def filtrar_por_rol(catalogo, rol):
    """
    Retorna lista de registros del personal que tengan el rol indicado.
    """
    resultados = []
    for id_pers, registro in catalogo.items():
        if registro["datos_fijos"][2] == rol:
            resultados.append(registro)
    return resultados


def filtrar_por_especialidad(catalogo, especialidad):
    """
    Retorna lista de registros del personal con la especialidad indicada.
    """
    resultados = []
    for id_pers, registro in catalogo.items():
        if registro["datos_fijos"][3].lower() == especialidad.lower():
            resultados.append(registro)
    return resultados


def filtrar_activos(catalogo):
    """Retorna lista de registros del personal activo."""
    resultados = []
    for id_pers, registro in catalogo.items():
        if registro["datos_mutables"]["activo"]:
            resultados.append(registro)
    return resultados


# ═══════════════════════════════════════════════════════════════════════════
#  ACTUALIZACIONES
# ═══════════════════════════════════════════════════════════════════════════

def actualizar_turno(catalogo, id_personal, nuevo_turno):
    """Actualiza el turno de un miembro del personal."""
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return False, f"No se encontró personal con ID {id_personal}."

    ok, msg = utils.validar_turno(nuevo_turno)
    if not ok:
        return False, msg

    personal["datos_mutables"]["turno"] = nuevo_turno
    return True, f"Turno actualizado a '{nuevo_turno}'."


def desactivar_personal(catalogo, id_personal):
    """Marca a un miembro del personal como inactivo."""
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return False, f"No se encontró personal con ID {id_personal}."

    personal["datos_mutables"]["activo"] = False
    return True, f"Personal {id_personal} desactivado."


def activar_personal(catalogo, id_personal):
    """Marca a un miembro del personal como activo."""
    personal = buscar_personal(catalogo, id_personal)
    if personal is None:
        return False, f"No se encontró personal con ID {id_personal}."

    personal["datos_mutables"]["activo"] = True
    return True, f"Personal {id_personal} activado."
