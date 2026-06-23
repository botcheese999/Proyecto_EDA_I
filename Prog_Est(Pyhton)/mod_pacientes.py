"""
mod_pacientes.py — Subsistema de gestión de pacientes.

Gestiona el catálogo de pacientes usando un diccionario con DNI como clave
para búsqueda O(1). Los datos inmutables se almacenan en tuplas, las alergias
en sets, y el historial médico en listas.

Patrón: todas las funciones reciben el catálogo como primer argumento,
facilitando la futura refactorización a métodos de clase.
"""

import utils


# ═══════════════════════════════════════════════════════════════════════════
#  CRUD DE PACIENTES
# ═══════════════════════════════════════════════════════════════════════════

def registrar_paciente(catalogo, dni, nombre, tipo_sangre, fecha_nacimiento,
                       telefono="", direccion="", email="",
                       contacto_emergencia=""):
    """
    Registra un nuevo paciente en el catálogo.

    Args:
        catalogo: dict — catálogo principal de pacientes.
        dni: str — documento de identidad (8 dígitos).
        nombre: str — nombre completo del paciente.
        tipo_sangre: str — tipo de sangre (ej: "O+").
        fecha_nacimiento: str — fecha en formato YYYY-MM-DD.
        telefono, direccion, email, contacto_emergencia: str — datos mutables.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    # Validaciones
    ok, msg = utils.validar_dni(dni)
    if not ok:
        return False, msg

    ok, msg = utils.validar_tipo_sangre(tipo_sangre)
    if not ok:
        return False, msg

    ok, msg = utils.validar_fecha(fecha_nacimiento)
    if not ok:
        return False, msg

    ok, msg = utils.validar_no_vacio(nombre, "nombre")
    if not ok:
        return False, msg

    if dni in catalogo:
        return False, f"Ya existe un paciente con DNI {dni}."

    # Crear registro
    catalogo[dni] = {
        "datos_fijos": (dni, tipo_sangre, fecha_nacimiento, nombre),  # TUPLA
        "datos_mutables": {
            "telefono": telefono,
            "direccion": direccion,
            "email": email,
            "contacto_emergencia": contacto_emergencia,
        },
        "alergias": set(),    # SET
        "historial": [],      # LISTA
    }

    return True, f"Paciente '{nombre}' registrado exitosamente."


def buscar_paciente(catalogo, dni):
    """
    Busca un paciente por DNI en O(1).

    Returns:
        dict | None — registro del paciente o None si no existe.
    """
    return catalogo.get(dni)


def eliminar_paciente(catalogo, dni):
    """
    Elimina un paciente del catálogo.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    if dni not in catalogo:
        return False, f"No se encontró paciente con DNI {dni}."

    nombre = catalogo[dni]["datos_fijos"][3]
    del catalogo[dni]
    return True, f"Paciente '{nombre}' eliminado."


def listar_pacientes(catalogo):
    """
    Retorna una lista de tuplas (dni, nombre) de todos los pacientes.
    """
    resultado = []
    for dni, registro in catalogo.items():
        nombre = registro["datos_fijos"][3]
        resultado.append((dni, nombre))
    return resultado


# ═══════════════════════════════════════════════════════════════════════════
#  DATOS MUTABLES
# ═══════════════════════════════════════════════════════════════════════════

def actualizar_dato_mutable(catalogo, dni, campo, valor):
    """
    Actualiza un campo mutable de un paciente existente.

    Args:
        campo: str — uno de: telefono, direccion, email, contacto_emergencia.
        valor: str — nuevo valor.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni}."

    campos_validos = {"telefono", "direccion", "email", "contacto_emergencia"}
    if campo not in campos_validos:
        return False, f"Campo inválido. Válidos: {', '.join(sorted(campos_validos))}"

    paciente["datos_mutables"][campo] = valor
    return True, f"Campo '{campo}' actualizado para paciente {dni}."


# ═══════════════════════════════════════════════════════════════════════════
#  ALERGIAS (OPERACIONES CON SETS)
# ═══════════════════════════════════════════════════════════════════════════

def agregar_alergia(catalogo, dni, alergia):
    """Agrega una alergia al set del paciente."""
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni}."

    alergia = alergia.strip().lower()
    if alergia in paciente["alergias"]:
        return False, f"La alergia '{alergia}' ya está registrada."

    paciente["alergias"].add(alergia)
    return True, f"Alergia '{alergia}' agregada."


def remover_alergia(catalogo, dni, alergia):
    """Remueve una alergia del set del paciente."""
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni}."

    alergia = alergia.strip().lower()
    if alergia not in paciente["alergias"]:
        return False, f"La alergia '{alergia}' no está registrada."

    paciente["alergias"].discard(alergia)
    return True, f"Alergia '{alergia}' removida."


def obtener_alergias(catalogo, dni):
    """Retorna el set de alergias de un paciente, o None si no existe."""
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return None
    return paciente["alergias"]


def cruzar_alergias(catalogo, dni1, dni2):
    """
    Retorna la intersección de alergias entre dos pacientes.
    Útil para identificar alergias comunes.

    Returns:
        tuple(bool, set | str) — (éxito, set de alergias comunes o mensaje de error).
    """
    alergias1 = obtener_alergias(catalogo, dni1)
    alergias2 = obtener_alergias(catalogo, dni2)

    if alergias1 is None:
        return False, f"No se encontró paciente con DNI {dni1}."
    if alergias2 is None:
        return False, f"No se encontró paciente con DNI {dni2}."

    comunes = alergias1 & alergias2  # Intersección de sets O(min(n,m))
    return True, comunes


def alergias_union(catalogo, dni1, dni2):
    """Retorna la unión de alergias entre dos pacientes."""
    alergias1 = obtener_alergias(catalogo, dni1)
    alergias2 = obtener_alergias(catalogo, dni2)

    if alergias1 is None:
        return False, f"No se encontró paciente con DNI {dni1}."
    if alergias2 is None:
        return False, f"No se encontró paciente con DNI {dni2}."

    union = alergias1 | alergias2  # Unión de sets
    return True, union


def alergias_diferencia(catalogo, dni1, dni2):
    """Retorna alergias que tiene dni1 pero no dni2."""
    alergias1 = obtener_alergias(catalogo, dni1)
    alergias2 = obtener_alergias(catalogo, dni2)

    if alergias1 is None:
        return False, f"No se encontró paciente con DNI {dni1}."
    if alergias2 is None:
        return False, f"No se encontró paciente con DNI {dni2}."

    diferencia = alergias1 - alergias2  # Diferencia de sets
    return True, diferencia


# ═══════════════════════════════════════════════════════════════════════════
#  HISTORIAL MÉDICO (LISTA SECUENCIAL)
# ═══════════════════════════════════════════════════════════════════════════

def agregar_entrada_historial(catalogo, dni, tipo, medico_id, diagnostico,
                              medicacion=None, notas=""):
    """
    Agrega una entrada al historial médico del paciente (append a lista).

    Args:
        tipo: str — tipo de entrada (consulta, emergencia, control, etc.)
        medico_id: str — ID del médico que atiende.
        diagnostico: str — descripción del diagnóstico.
        medicacion: list | None — lista de medicamentos recetados.
        notas: str — notas adicionales.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni}."

    entrada = {
        "fecha": utils.timestamp_actual(),
        "tipo": tipo,
        "medico_id": medico_id,
        "diagnostico": diagnostico,
        "medicacion": medicacion if medicacion else [],
        "notas": notas,
    }

    paciente["historial"].append(entrada)  # Append O(1) amortizado
    return True, "Entrada agregada al historial."


def obtener_historial(catalogo, dni):
    """Retorna la lista de historial de un paciente, o None si no existe."""
    paciente = buscar_paciente(catalogo, dni)
    if paciente is None:
        return None
    return paciente["historial"]


def buscar_en_historial(catalogo, dni, campo, valor):
    """
    Busca entradas en el historial que coincidan con un campo y valor.

    Args:
        campo: str — campo a buscar (tipo, medico_id, diagnostico).
        valor: str — valor a buscar (búsqueda parcial, case-insensitive).

    Returns:
        list — entradas que coinciden.
    """
    historial = obtener_historial(catalogo, dni)
    if historial is None:
        return []

    resultados = []
    for entrada in historial:
        if campo in entrada:
            if valor.lower() in str(entrada[campo]).lower():
                resultados.append(entrada)
    return resultados
