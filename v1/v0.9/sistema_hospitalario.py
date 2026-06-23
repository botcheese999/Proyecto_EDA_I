"""
Sistema Hospitalario — Fase 1: Prototipado y Modelado de Datos.

Estructura la lógica de datos:
- Diccionarios: catálogos de pacientes y personal (búsqueda O(1) por clave).
- Tuplas: datos inmutables (DNI, tipo de sangre).
- Sets (Conjuntos): alergias para búsqueda rápida O(1) y cruce de información.
- Listas: historial secuencial

"""

import csv


#  SECCIÓN 1: Estructuras de Datos Básicas y Carga de Archivos

# --- BASE DE DATOS EN MEMORIA ---
# Diccionario principal de pacientes. Llave: DNI
pacientes = {}

# Diccionario de personal médico. Llave: ID_Empleado
# Jerarquía de acceso: 1 (Enfermera), 2 (Médico General), 3 (Especialista)
personal = {
    "EMP01": {"nombre": "Dra. Gómez", "rol": "Especialista", "nivel_acceso": 3},
    "EMP02": {"nombre": "Dr. Pérez", "rol": "Médico General", "nivel_acceso": 2},
    "EMP03": {"nombre": "Enf. Ruiz", "rol": "Enfermera", "nivel_acceso": 1}
}


def registrar_paciente(dni, nombre, tipo_sangre, lista_alergias):
    """
    Registra un paciente nuevo en el diccionario 'pacientes'.
    """
    # 1. Tupla con datos inmutables — garantiza que DNI y sangre no se modifiquen
    datos_inmutables = (dni, tipo_sangre)

    # 2. Set de alergias — permite búsqueda O(1) con el operador 'in'
    alergias = set(lista_alergias)

    # 3. Almacenar en el diccionario con DNI como clave (búsqueda O(1))
    pacientes[dni] = {
        "nombre": nombre,
        "datos_inmutables": datos_inmutables,   # TUPLA
        "alergias": alergias,                   # SET
    }


def cargar_pacientes_csv(ruta_archivo):
    """
    Lee un archivo CSV y registra a los pacientes masivamente.
    El CSV tiene el formato: DNI,Nombre,Tipo_Sangre,Alergias(separadas por -)
    """
    # 1. Abrir y leer con la librería csv
    with open(ruta_archivo, newline="", encoding="utf-8") as archivo:
        lector = csv.reader(archivo)
        next(lector)  # Saltar la cabecera

        for fila in lector:
            dni = fila[0].strip()
            nombre = fila[1].strip()
            tipo_sangre = fila[2].strip()

            # 2. Separar alergias por guion '-' y limpiar espacios
            alergias_texto = fila[3].strip() if len(fila) > 3 else ""
            if alergias_texto:
                lista_alergias = [a.strip() for a in alergias_texto.split("-")]
            else:
                lista_alergias = []

            # 3. Registrar cada paciente
            registrar_paciente(dni, nombre, tipo_sangre, lista_alergias)


#  SECCIÓN 2: Flujo de Atención 

# --- ESTRUCTURAS DINÁMICAS ---
lista_espera = []       # Lista usada como cola para el orden de llegada
lista_medicacion = []   # Lista usada como pila para deshacer errores


def llegada_paciente(dni):
    """
    Agrega un paciente a la cola de espera.
    """
    # Insertar al final (append) — simula enqueue en una cola FIFO
    lista_espera.append(dni)


def atender_siguiente_paciente():
    """
    Atiende al paciente que lleva más tiempo esperando.
    PARA HACER (Estudiante):
    """
    # 1. Verificar si hay pacientes
    if not lista_espera:
        print("No hay pacientes en la cola de espera.")
        return None

    # 2. Extraer el primero (pop(0))
    paciente_atendido = lista_espera.pop(0)
    return paciente_atendido


def recetar_medicamento(dni, medicamento, id_empleado):
    """
    Registra una receta médica si el personal tiene acceso suficiente (Nivel 2 o 3).
    """
    # 1. Validar que el empleado exista y tenga nivel suficiente
    if id_empleado not in personal:
        print(f"Error: El empleado '{id_empleado}' no existe en el sistema.")
        return

    nivel = personal[id_empleado]["nivel_acceso"]
    if nivel < 2:
        print(f"Acceso denegado: {personal[id_empleado]['nombre']} "
              f"(nivel {nivel}) no tiene permiso para recetar.")
        return

    # 2. Registrar la receta como tupla en la pila (push con append)
    receta = (dni, medicamento)
    lista_medicacion.append(receta)
    print(f"Receta registrada: {medicamento} para paciente {dni} "
          f"— por {personal[id_empleado]['nombre']}")


def deshacer_error_medicacion():
    """
    Deshace la última receta administrada en caso de error.
    """
    # 1. Verificar si hay recetas para deshacer
    if not lista_medicacion:
        print("No hay recetas para deshacer.")
        return None

    # 2. Sacar el último elemento (pop) — operación LIFO
    receta_anulada = lista_medicacion.pop(-1)
    dni, medicamento = receta_anulada
    print(f"[!] ALERTA: Receta ANULADA -- {medicamento} del paciente {dni}")
    return receta_anulada



#  SECCIÓN 3: Búsqueda, Ordenamiento e Historiales (Listas)

# --- HISTORIAL CLÍNICO (Lista Enlazada Simulada con Diccionarios) ---
# Llave: DNI, Valor: El primer nodo (diccionario) del historial
cabeza_historial = {}


def agregar_registro_historial(dni, diagnostico):
    """
    Agrega un nuevo diagnóstico al final de la lista enlazada del paciente.
    Nota: Un Nodo es un diccionario con la forma: {"diagnostico": str, "siguiente": Nodo/None}
    """
    # 1. Crear el nuevo nodo
    nuevo_nodo = {"diagnostico": diagnostico, "siguiente": None}

    # 2. Si no existe historial para este paciente, es el primer nodo
    if dni not in cabeza_historial:
        cabeza_historial[dni] = nuevo_nodo
        return

    # 3. Recorrer hasta el final de la lista enlazada
    nodo_actual = cabeza_historial[dni]
    while nodo_actual["siguiente"] is not None:
        nodo_actual = nodo_actual["siguiente"]

    # Enlazar el nuevo nodo al final
    nodo_actual["siguiente"] = nuevo_nodo


def ordenar_pacientes_por_dni():
    """
    Ordena la lista de pacientes registrados.
    """
    # 1. Extraer las llaves (DNI) a una lista
    lista_dnis = list(pacientes.keys())

    # 2. Ordenamiento Burbuja (manual, sin sort())
    n = len(lista_dnis)
    for i in range(n - 1):
        for j in range(0, n - i - 1):
            if lista_dnis[j] > lista_dnis[j + 1]:
                # Intercambiar
                lista_dnis[j], lista_dnis[j + 1] = lista_dnis[j + 1], lista_dnis[j]

    return lista_dnis


def busqueda_rapida_alergia(dni, alergia_a_buscar):
    """
    Busca de manera instantánea si un paciente tiene una alergia específica.
    PARA HACER (Estudiante):
    """
    # 1. Buscar al paciente
    if dni not in pacientes:
        print(f"Paciente con DNI {dni} no encontrado.")
        return False

    # 2. Búsqueda O(1) en el set de alergias
    alergias = pacientes[dni]["alergias"]
    return alergia_a_buscar in alergias  # Operación de pertenencia en SET



#  FUNCIONES AUXILIARES (utilidades adicionales para el sistema)


def mostrar_historial(dni):
    """Recorre e imprime la lista enlazada de historial de un paciente."""
    if dni not in cabeza_historial:
        print(f"No hay historial para el paciente {dni}.")
        return

    print(f"Historial clínico del paciente {dni}:")
    nodo = cabeza_historial[dni]
    indice = 1
    while nodo is not None:
        print(f"  {indice}. {nodo['diagnostico']}")
        nodo = nodo["siguiente"]
        indice += 1


def cruzar_alergias(dni1, dni2):
    """
    Cruza las alergias entre dos pacientes usando operaciones de conjuntos.
    Retorna: intersección, unión y diferencia.
    """
    if dni1 not in pacientes or dni2 not in pacientes:
        print("Uno o ambos pacientes no están registrados.")
        return None

    alergias_1 = pacientes[dni1]["alergias"]
    alergias_2 = pacientes[dni2]["alergias"]

    return {
        "comunes": alergias_1 & alergias_2,         # Intersección
        "todas": alergias_1 | alergias_2,            # Unión
        "solo_en_primero": alergias_1 - alergias_2,   # Diferencia
        "solo_en_segundo": alergias_2 - alergias_1,   # Diferencia inversa
    }



#  ZONA DE PRUEBAS


if __name__ == "__main__":
    print("--- INICIANDO PRUEBA 1: Registro y Estructuras ---")

    # Registrar pacientes manualmente
    registrar_paciente("12345678", "Juan Pérez", "O+", ["Penicilina", "Látex"])
    registrar_paciente("87654321", "María López", "A+", ["Penicilina", "Aspirina"])
    registrar_paciente("11223344", "Carlos Ríos", "B-", [])

    # Verificar estructura interna
    print(f"Pacientes registrados: {len(pacientes)}")
    for dni, datos in pacientes.items():
        print(f"  DNI: {dni}")
        print(f"    Nombre: {datos['nombre']}")
        print(f"    Datos inmutables (tupla): {datos['datos_inmutables']}")
        print(f"    Alergias (set): {datos['alergias']}")
        print(f"    Tipo datos_inmutables: {type(datos['datos_inmutables']).__name__}")
        print(f"    Tipo alergias: {type(datos['alergias']).__name__}")

    print("\n--- PRUEBA 2: Búsqueda rápida de alergias (Sets) ---")
    resultado = busqueda_rapida_alergia("12345678", "Penicilina")
    print(f"  ¿Juan Pérez tiene alergia a Penicilina? {resultado}")  # True
    resultado = busqueda_rapida_alergia("12345678", "Ibuprofeno")
    print(f"  ¿Juan Pérez tiene alergia a Ibuprofeno? {resultado}")  # False

    print("\n--- PRUEBA 3: Cruce de alergias (Operaciones de Sets) ---")
    cruce = cruzar_alergias("12345678", "87654321")
    print(f"  Alergias comunes: {cruce['comunes']}")
    print(f"  Todas las alergias: {cruce['todas']}")
    print(f"  Solo Juan: {cruce['solo_en_primero']}")
    print(f"  Solo María: {cruce['solo_en_segundo']}")

    print("\n--- PRUEBA 4: Cola de espera (FIFO) ---")
    llegada_paciente("12345678")
    llegada_paciente("87654321")
    llegada_paciente("11223344")
    print(f"  Cola de espera: {lista_espera}")
    atendido = atender_siguiente_paciente()
    print(f"  Atendido: {atendido}")  # 12345678 (primero en entrar)
    print(f"  Cola restante: {lista_espera}")

    print("\n--- PRUEBA 5: Pila de medicación (LIFO) ---")
    recetar_medicamento("12345678", "Amoxicilina", "EMP01")    # OK (nivel 3)
    recetar_medicamento("12345678", "Ibuprofeno", "EMP02")     # OK (nivel 2)
    recetar_medicamento("87654321", "Paracetamol", "EMP03")    # DENEGADO (nivel 1)
    print(f"  Pila de medicación: {lista_medicacion}")
    deshacer_error_medicacion()  # Deshace Ibuprofeno (último)
    print(f"  Pila tras deshacer: {lista_medicacion}")

    print("\n--- PRUEBA 6: Historial clínico (Lista enlazada) ---")
    agregar_registro_historial("12345678", "Gripe estacional")
    agregar_registro_historial("12345678", "Control post-tratamiento")
    agregar_registro_historial("12345678", "Alergia cutánea")
    mostrar_historial("12345678")

    print("\n--- PRUEBA 7: Ordenamiento manual por DNI (Burbuja) ---")
    dnis_ordenados = ordenar_pacientes_por_dni()
    print(f"  DNIs ordenados: {dnis_ordenados}")

    print("\n=== TODAS LAS PRUEBAS COMPLETADAS ===")
