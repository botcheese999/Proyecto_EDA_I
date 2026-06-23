"""
mod_citas.py — Subsistema de gestión de citas y flujo de atención.

Gestiona el registro de citas (dict con ID como clave), la lista de espera
(lista simple, preparada para FIFO en fase posterior), y el registro de
medicación con capacidad de deshacer (lista simple, preparada para pila LIFO).

Nota: En esta fase se usan listas simples. En la siguiente fase se
implementarán colas FIFO y pilas LIFO con sus operaciones formales.
"""

import config
import utils
import mod_pacientes
import mod_personal


# ═══════════════════════════════════════════════════════════════════════════
#  LISTA DE ESPERA (preparada para FIFO)
# ═══════════════════════════════════════════════════════════════════════════

def agregar_a_espera(lista_espera, dni, motivo):
    """
    Agrega un paciente a la lista de espera.
    En fase posterior se convertirá en enqueue de una cola FIFO.

    Args:
        lista_espera: list — lista de espera actual.
        dni: str — DNI del paciente.
        motivo: str — motivo de la consulta.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    # Verificar que no esté ya en espera
    for entrada in lista_espera:
        if entrada[1] == dni:
            return False, f"El paciente {dni} ya está en la lista de espera."

    entrada = (utils.timestamp_actual(), dni, motivo)  # TUPLA inmutable
    lista_espera.append(entrada)
    return True, f"Paciente {dni} agregado a la lista de espera."


def atender_siguiente(lista_espera):
    """
    Atiende al siguiente paciente en la lista de espera (primero en entrar).
    En fase posterior se convertirá en dequeue de una cola FIFO.

    Returns:
        tuple(bool, tuple | str) — (éxito, tupla del paciente o mensaje).
    """
    if not lista_espera:
        return False, "La lista de espera está vacía."

    siguiente = lista_espera.pop(0)  # FIFO: saca el primero
    return True, siguiente


def ver_lista_espera(lista_espera):
    """Retorna la lista de espera actual sin modificarla."""
    return list(lista_espera)  # Copia defensiva


def cantidad_en_espera(lista_espera):
    """Retorna la cantidad de pacientes en espera."""
    return len(lista_espera)


# ═══════════════════════════════════════════════════════════════════════════
#  REGISTRO DE CITAS
# ═══════════════════════════════════════════════════════════════════════════

def crear_cita(registro_citas, catalogo_pacientes, catalogo_personal,
               dni_paciente, id_medico, motivo=""):
    """
    Crea una nueva cita entre un paciente y un médico.

    Args:
        registro_citas: dict — registro principal de citas.
        catalogo_pacientes: dict — para verificar que el paciente exista.
        catalogo_personal: dict — para verificar que el médico exista y tenga permisos.
        dni_paciente: str — DNI del paciente.
        id_medico: str — ID del médico asignado.
        motivo: str — motivo de la consulta.

    Returns:
        tuple(bool, str) — (éxito, mensaje con ID de cita).
    """
    # Validar existencia del paciente
    paciente = mod_pacientes.buscar_paciente(catalogo_pacientes, dni_paciente)
    if paciente is None:
        return False, f"No se encontró paciente con DNI {dni_paciente}."

    # Validar existencia y estado del médico
    medico = mod_personal.buscar_personal(catalogo_personal, id_medico)
    if medico is None:
        return False, f"No se encontró personal con ID {id_medico}."
    if not medico["datos_mutables"]["activo"]:
        return False, f"El personal {id_medico} no está activo."

    # Generar cita
    id_cita = utils.generar_id_cita()
    fecha_actual = utils.fecha_actual()

    registro_citas[id_cita] = {
        "datos_cita": (id_cita, dni_paciente, id_medico, fecha_actual),  # TUPLA
        "estado": "pendiente",
        "motivo": motivo,
        "medicacion_actual": [],   # LISTA
        "notas": "",
    }

    return True, f"Cita {id_cita} creada — Paciente: {dni_paciente}, Médico: {id_medico}"


def buscar_cita(registro_citas, id_cita):
    """Busca una cita por ID en O(1)."""
    return registro_citas.get(id_cita)


def cambiar_estado_cita(registro_citas, id_cita, nuevo_estado):
    """
    Cambia el estado de una cita.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    cita = buscar_cita(registro_citas, id_cita)
    if cita is None:
        return False, f"No se encontró cita con ID {id_cita}."

    ok, msg = utils.validar_estado_cita(nuevo_estado)
    if not ok:
        return False, msg

    estado_anterior = cita["estado"]
    cita["estado"] = nuevo_estado
    return True, f"Cita {id_cita}: {estado_anterior} → {nuevo_estado}"


def completar_cita(registro_citas, catalogo_pacientes, id_cita,
                   diagnostico, notas=""):
    """
    Completa una cita: cambia estado a 'completada' y agrega entrada al
    historial del paciente.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    cita = buscar_cita(registro_citas, id_cita)
    if cita is None:
        return False, f"No se encontró cita con ID {id_cita}."

    if cita["estado"] == "completada":
        return False, f"La cita {id_cita} ya está completada."

    # Extraer datos de la tupla de cita
    _, dni_paciente, id_medico, _ = cita["datos_cita"]

    # Agregar al historial del paciente
    mod_pacientes.agregar_entrada_historial(
        catalogo_pacientes,
        dni_paciente,
        tipo="consulta",
        medico_id=id_medico,
        diagnostico=diagnostico,
        medicacion=list(cita["medicacion_actual"]),
        notas=notas,
    )

    # Cambiar estado
    cita["estado"] = "completada"
    cita["notas"] = notas

    return True, f"Cita {id_cita} completada. Historial del paciente actualizado."


def cancelar_cita(registro_citas, id_cita, motivo_cancelacion=""):
    """Cancela una cita pendiente o en atención."""
    cita = buscar_cita(registro_citas, id_cita)
    if cita is None:
        return False, f"No se encontró cita con ID {id_cita}."

    if cita["estado"] in ("completada", "cancelada"):
        return False, f"La cita {id_cita} ya está {cita['estado']}."

    cita["estado"] = "cancelada"
    cita["notas"] = f"CANCELADA: {motivo_cancelacion}"
    return True, f"Cita {id_cita} cancelada."


def listar_citas_por_estado(registro_citas, estado):
    """Filtra y retorna citas por estado."""
    resultados = []
    for id_cita, cita in registro_citas.items():
        if cita["estado"] == estado:
            resultados.append(cita)
    return resultados


def listar_citas_por_paciente(registro_citas, dni):
    """Retorna todas las citas de un paciente."""
    resultados = []
    for id_cita, cita in registro_citas.items():
        if cita["datos_cita"][1] == dni:
            resultados.append(cita)
    return resultados


def listar_citas_por_medico(registro_citas, id_medico):
    """Retorna todas las citas de un médico."""
    resultados = []
    for id_cita, cita in registro_citas.items():
        if cita["datos_cita"][2] == id_medico:
            resultados.append(cita)
    return resultados


# ═══════════════════════════════════════════════════════════════════════════
#  MEDICACIÓN CON DESHACER (preparada para pila LIFO)
# ═══════════════════════════════════════════════════════════════════════════

def agregar_medicacion(registro_citas, pila_deshacer, id_cita,
                       medicamento, id_medico):
    """
    Agrega un medicamento a una cita y registra la acción en la pila
    de deshacer para posible reversión.

    Args:
        pila_deshacer: list — lista usada como pila (append/pop).
        medicamento: str — nombre del medicamento.
        id_medico: str — ID del médico que prescribe.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    cita = buscar_cita(registro_citas, id_cita)
    if cita is None:
        return False, f"No se encontró cita con ID {id_cita}."

    if cita["estado"] not in ("pendiente", "en_atencion"):
        return False, f"No se puede modificar una cita {cita['estado']}."

    medicamento = medicamento.strip()
    cita["medicacion_actual"].append(medicamento)

    # Registrar en pila para posible deshacer
    accion = {
        "accion": "agregar",
        "cita_id": id_cita,
        "medicamento": medicamento,
        "medico_id": id_medico,
        "timestamp": utils.timestamp_actual(),
    }
    pila_deshacer.append(accion)  # push

    return True, f"Medicamento '{medicamento}' agregado a cita {id_cita}."


def deshacer_medicacion(registro_citas, pila_deshacer, catalogo_personal,
                        id_medico_solicitante):
    """
    Deshace la última acción de medicación (pop de la pila).
    Solo personal con permiso 'deshacer_medicacion' puede ejecutar esta acción.

    Returns:
        tuple(bool, str) — (éxito, mensaje).
    """
    # Verificar permisos
    if not mod_personal.verificar_acceso(catalogo_personal, id_medico_solicitante,
                                        "deshacer_medicacion"):
        return False, "No tiene permisos para deshacer medicación."

    if not pila_deshacer:
        return False, "No hay acciones para deshacer."

    # Pop de la pila (LIFO)
    ultima_accion = pila_deshacer.pop()

    # Revertir la acción
    cita = buscar_cita(registro_citas, ultima_accion["cita_id"])
    if cita is None:
        return False, f"La cita {ultima_accion['cita_id']} ya no existe."

    medicamento = ultima_accion["medicamento"]
    if medicamento in cita["medicacion_actual"]:
        cita["medicacion_actual"].remove(medicamento)

    return True, (
        f"Medicación revertida: '{medicamento}' removido de cita "
        f"{ultima_accion['cita_id']} (prescrito por {ultima_accion['medico_id']} "
        f"a las {ultima_accion['timestamp']})"
    )


def ver_pila_deshacer(pila_deshacer):
    """Retorna la pila de deshacer sin modificarla (copia)."""
    return list(pila_deshacer)
