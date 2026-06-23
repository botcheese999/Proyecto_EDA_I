"""
main.py — Punto de entrada del sistema hospitalario.
Menú interactivo por consola con submenús por subsistema.
"""

import mod_pacientes
import mod_personal
import mod_citas
import mod_reportes
import mod_persistencia
import mod_exportacion_csv
import utils


# ═══════════════════════════════════════════════════════════════════════════
#  DATOS GLOBALES DEL SISTEMA (se cargan al iniciar)
# ═══════════════════════════════════════════════════════════════════════════

catalogo_pacientes = {}
catalogo_personal = {}
registro_citas = {}
lista_espera = []
pila_deshacer = []


# ═══════════════════════════════════════════════════════════════════════════
#  HELPERS DE MENÚ
# ═══════════════════════════════════════════════════════════════════════════

def leer_opcion(mensaje="Seleccione una opción: "):
    """Lee una opción del usuario y la retorna como string."""
    return input(mensaje).strip()


def pausar():
    """Pausa hasta que el usuario presione Enter."""
    input("\n  Presione Enter para continuar...")


def mostrar_menu(titulo, opciones):
    """Muestra un menú numerado y retorna la opción elegida."""
    utils.imprimir_separador(titulo)
    for num, texto in opciones:
        print(f"  {num}. {texto}")
    print()
    return leer_opcion()


# ═══════════════════════════════════════════════════════════════════════════
#  SUBMENÚ: PACIENTES
# ═══════════════════════════════════════════════════════════════════════════

def menu_pacientes():
    """Submenú de gestión de pacientes."""
    while True:
        opcion = mostrar_menu("GESTIÓN DE PACIENTES", [
            ("1", "Registrar paciente"),
            ("2", "Buscar paciente por DNI"),
            ("3", "Actualizar datos de paciente"),
            ("4", "Gestionar alergias"),
            ("5", "Ver historial médico"),
            ("6", "Cruzar alergias entre pacientes"),
            ("7", "Listar todos los pacientes"),
            ("8", "Eliminar paciente"),
            ("0", "Volver al menú principal"),
        ])

        if opcion == "1":
            _registrar_paciente()
        elif opcion == "2":
            _buscar_paciente()
        elif opcion == "3":
            _actualizar_paciente()
        elif opcion == "4":
            _gestionar_alergias()
        elif opcion == "5":
            _ver_historial()
        elif opcion == "6":
            _cruzar_alergias()
        elif opcion == "7":
            _listar_pacientes()
        elif opcion == "8":
            _eliminar_paciente()
        elif opcion == "0":
            break


def _registrar_paciente():
    print("\n  ── Registrar Nuevo Paciente ──")
    dni = input("  DNI (8 dígitos): ").strip()
    nombre = input("  Nombre completo: ").strip()
    sangre = input("  Tipo de sangre (A+, A-, B+, B-, AB+, AB-, O+, O-): ").strip()
    fecha_nac = input("  Fecha de nacimiento (YYYY-MM-DD): ").strip()
    telefono = input("  Teléfono: ").strip()
    direccion = input("  Dirección: ").strip()
    email = input("  Email: ").strip()
    contacto = input("  Contacto de emergencia: ").strip()

    ok, msg = mod_pacientes.registrar_paciente(
        catalogo_pacientes, dni, nombre, sangre, fecha_nac,
        telefono, direccion, email, contacto
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _buscar_paciente():
    dni = input("\n  DNI a buscar: ").strip()
    paciente = mod_pacientes.buscar_paciente(catalogo_pacientes, dni)
    if paciente is None:
        print(f"\n  ✗ No se encontró paciente con DNI {dni}.")
    else:
        print("\n  ── Datos Fijos ──")
        print(utils.formatear_tupla_paciente(paciente["datos_fijos"]))
        print("\n  ── Datos Mutables ──")
        for campo, valor in paciente["datos_mutables"].items():
            print(f"  {campo}: {valor}")
        alergias = paciente["alergias"]
        print(f"\n  Alergias: {', '.join(sorted(alergias)) if alergias else 'Ninguna'}")
        print(f"  Entradas en historial: {len(paciente['historial'])}")
    pausar()


def _actualizar_paciente():
    dni = input("\n  DNI del paciente: ").strip()
    campos = ["telefono", "direccion", "email", "contacto_emergencia"]
    print("  Campos actualizables: " + ", ".join(campos))
    campo = input("  Campo a actualizar: ").strip()
    valor = input(f"  Nuevo valor para '{campo}': ").strip()
    ok, msg = mod_pacientes.actualizar_dato_mutable(catalogo_pacientes, dni, campo, valor)
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _gestionar_alergias():
    dni = input("\n  DNI del paciente: ").strip()
    paciente = mod_pacientes.buscar_paciente(catalogo_pacientes, dni)
    if paciente is None:
        print(f"\n  ✗ No se encontró paciente con DNI {dni}.")
        pausar()
        return

    alergias = paciente["alergias"]
    print(f"  Alergias actuales: {', '.join(sorted(alergias)) if alergias else 'Ninguna'}")
    accion = input("  ¿Agregar (A) o Remover (R)? ").strip().upper()

    if accion == "A":
        alergia = input("  Alergia a agregar: ").strip()
        ok, msg = mod_pacientes.agregar_alergia(catalogo_pacientes, dni, alergia)
    elif accion == "R":
        alergia = input("  Alergia a remover: ").strip()
        ok, msg = mod_pacientes.remover_alergia(catalogo_pacientes, dni, alergia)
    else:
        print("  Opción no válida.")
        pausar()
        return

    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _ver_historial():
    dni = input("\n  DNI del paciente: ").strip()
    historial = mod_pacientes.obtener_historial(catalogo_pacientes, dni)
    if historial is None:
        print(f"\n  ✗ No se encontró paciente con DNI {dni}.")
    elif not historial:
        print("  El paciente no tiene historial registrado.")
    else:
        print(f"\n  ── Historial ({len(historial)} entradas) ──")
        for i, entrada in enumerate(historial, 1):
            print(f"\n  [{i}] {entrada['fecha']} — {entrada['tipo']}")
            print(f"      Médico: {entrada['medico_id']}")
            print(f"      Diagnóstico: {entrada['diagnostico']}")
            meds = entrada.get('medicacion', [])
            print(f"      Medicación: {', '.join(meds) if meds else 'Ninguna'}")
            if entrada.get('notas'):
                print(f"      Notas: {entrada['notas']}")
    pausar()


def _cruzar_alergias():
    dni1 = input("\n  DNI del paciente 1: ").strip()
    dni2 = input("  DNI del paciente 2: ").strip()
    ok, resultado = mod_pacientes.cruzar_alergias(catalogo_pacientes, dni1, dni2)
    if not ok:
        print(f"\n  ✗ {resultado}")
    elif not resultado:
        print("  No hay alergias en común.")
    else:
        print(f"  Alergias en común: {', '.join(sorted(resultado))}")
    pausar()


def _listar_pacientes():
    lista = mod_pacientes.listar_pacientes(catalogo_pacientes)
    if not lista:
        print("\n  No hay pacientes registrados.")
    else:
        print(f"\n  ── Pacientes Registrados ({len(lista)}) ──")
        for dni, nombre in lista:
            print(f"  {dni} — {nombre}")
    pausar()


def _eliminar_paciente():
    dni = input("\n  DNI del paciente a eliminar: ").strip()
    confirm = input(f"  ¿Confirmar eliminación de {dni}? (S/N): ").strip().upper()
    if confirm == "S":
        ok, msg = mod_pacientes.eliminar_paciente(catalogo_pacientes, dni)
        print(f"\n  {'✓' if ok else '✗'} {msg}")
    else:
        print("  Operación cancelada.")
    pausar()


# ═══════════════════════════════════════════════════════════════════════════
#  SUBMENÚ: PERSONAL
# ═══════════════════════════════════════════════════════════════════════════

def menu_personal():
    """Submenú de gestión del personal."""
    while True:
        opcion = mostrar_menu("GESTIÓN DE PERSONAL", [
            ("1", "Registrar personal"),
            ("2", "Buscar personal por ID"),
            ("3", "Verificar acceso/permisos"),
            ("4", "Filtrar por rol"),
            ("5", "Actualizar turno"),
            ("6", "Activar/Desactivar personal"),
            ("7", "Listar todo el personal"),
            ("0", "Volver al menú principal"),
        ])

        if opcion == "1":
            _registrar_personal()
        elif opcion == "2":
            _buscar_personal()
        elif opcion == "3":
            _verificar_acceso()
        elif opcion == "4":
            _filtrar_por_rol()
        elif opcion == "5":
            _actualizar_turno()
        elif opcion == "6":
            _activar_desactivar()
        elif opcion == "7":
            _listar_personal()
        elif opcion == "0":
            break


def _registrar_personal():
    print("\n  ── Registrar Nuevo Personal ──")
    nombre = input("  Nombre completo: ").strip()
    print("  Roles: director, especialista, medico_general, enfermera")
    rol = input("  Rol: ").strip().lower()
    especialidad = input("  Especialidad (o 'General'): ").strip()
    print("  Turnos: mañana, tarde, noche")
    turno = input("  Turno: ").strip().lower()
    telefono = input("  Teléfono: ").strip()

    ok, msg = mod_personal.registrar_personal(
        catalogo_personal, nombre, rol,
        especialidad if especialidad else "General",
        turno if turno else "mañana", telefono
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _buscar_personal():
    id_pers = input("\n  ID del personal: ").strip()
    personal = mod_personal.buscar_personal(catalogo_personal, id_pers)
    if personal is None:
        print(f"\n  ✗ No se encontró personal con ID {id_pers}.")
    else:
        print("\n  ── Datos Fijos ──")
        print(utils.formatear_tupla_personal(personal["datos_fijos"]))
        print(f"\n  Nivel de acceso: {personal['nivel_acceso']}")
        print(f"  Turno: {personal['datos_mutables']['turno']}")
        print(f"  Activo: {'Sí' if personal['datos_mutables']['activo'] else 'No'}")
        print(f"  Permisos: {', '.join(sorted(personal['permisos']))}")
    pausar()


def _verificar_acceso():
    id_pers = input("\n  ID del personal: ").strip()
    permiso = input("  Permiso a verificar: ").strip()
    tiene = mod_personal.verificar_acceso(catalogo_personal, id_pers, permiso)
    print(f"\n  {'✓ PERMITIDO' if tiene else '✗ DENEGADO'}")
    pausar()


def _filtrar_por_rol():
    print("  Roles: director, especialista, medico_general, enfermera")
    rol = input("  Filtrar por rol: ").strip().lower()
    resultados = mod_personal.filtrar_por_rol(catalogo_personal, rol)
    if not resultados:
        print(f"\n  No se encontró personal con rol '{rol}'.")
    else:
        print(f"\n  ── Personal con rol '{rol}' ({len(resultados)}) ──")
        for reg in resultados:
            print(f"  {reg['datos_fijos'][0]} — {reg['datos_fijos'][1]} "
                  f"({reg['datos_fijos'][3]})")
    pausar()


def _actualizar_turno():
    id_pers = input("\n  ID del personal: ").strip()
    turno = input("  Nuevo turno (mañana/tarde/noche): ").strip().lower()
    ok, msg = mod_personal.actualizar_turno(catalogo_personal, id_pers, turno)
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _activar_desactivar():
    id_pers = input("\n  ID del personal: ").strip()
    accion = input("  ¿Activar (A) o Desactivar (D)? ").strip().upper()
    if accion == "A":
        ok, msg = mod_personal.activar_personal(catalogo_personal, id_pers)
    elif accion == "D":
        ok, msg = mod_personal.desactivar_personal(catalogo_personal, id_pers)
    else:
        print("  Opción no válida.")
        pausar()
        return
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _listar_personal():
    lista = mod_personal.listar_personal(catalogo_personal)
    if not lista:
        print("\n  No hay personal registrado.")
    else:
        print(f"\n  ── Personal Registrado ({len(lista)}) ──")
        print(f"  {'ID':<8} {'Nombre':<25} {'Rol':<18} {'Activo':<8}")
        print("  " + "─" * 60)
        for id_p, nombre, rol, activo in lista:
            estado = "Sí" if activo else "No"
            print(f"  {id_p:<8} {nombre:<25} {rol:<18} {estado:<8}")
    pausar()


# ═══════════════════════════════════════════════════════════════════════════
#  SUBMENÚ: CITAS
# ═══════════════════════════════════════════════════════════════════════════

def menu_citas():
    """Submenú de gestión de citas."""
    while True:
        opcion = mostrar_menu("GESTIÓN DE CITAS", [
            ("1", "Agregar paciente a lista de espera"),
            ("2", "Atender siguiente paciente"),
            ("3", "Ver lista de espera"),
            ("4", "Crear cita"),
            ("5", "Agregar medicación a cita"),
            ("6", "Deshacer última medicación"),
            ("7", "Completar cita"),
            ("8", "Cancelar cita"),
            ("9", "Buscar cita por ID"),
            ("0", "Volver al menú principal"),
        ])

        if opcion == "1":
            _agregar_espera()
        elif opcion == "2":
            _atender_siguiente()
        elif opcion == "3":
            _ver_espera()
        elif opcion == "4":
            _crear_cita()
        elif opcion == "5":
            _agregar_medicacion()
        elif opcion == "6":
            _deshacer_medicacion()
        elif opcion == "7":
            _completar_cita()
        elif opcion == "8":
            _cancelar_cita()
        elif opcion == "9":
            _buscar_cita()
        elif opcion == "0":
            break


def _agregar_espera():
    dni = input("\n  DNI del paciente: ").strip()
    if mod_pacientes.buscar_paciente(catalogo_pacientes, dni) is None:
        print(f"\n  ✗ No se encontró paciente con DNI {dni}.")
        pausar()
        return
    motivo = input("  Motivo de consulta: ").strip()
    ok, msg = mod_citas.agregar_a_espera(lista_espera, dni, motivo)
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _atender_siguiente():
    ok, resultado = mod_citas.atender_siguiente(lista_espera)
    if not ok:
        print(f"\n  ✗ {resultado}")
    else:
        timestamp, dni, motivo = resultado
        nombre = catalogo_pacientes.get(dni, {}).get("datos_fijos", ("", "", "", "N/A"))[3]
        print(f"\n  ✓ Atendiendo a: {nombre} (DNI: {dni})")
        print(f"    Llegada: {timestamp}")
        print(f"    Motivo: {motivo}")
    pausar()


def _ver_espera():
    espera = mod_citas.ver_lista_espera(lista_espera)
    if not espera:
        print("\n  La lista de espera está vacía.")
    else:
        print(f"\n  ── Lista de Espera ({len(espera)}) ──")
        for i, (ts, dni, motivo) in enumerate(espera, 1):
            nombre = catalogo_pacientes.get(dni, {}).get("datos_fijos", ("", "", "", "N/A"))[3]
            print(f"  {i}. {nombre} (DNI: {dni}) — {motivo} [{ts}]")
    pausar()


def _crear_cita():
    dni = input("\n  DNI del paciente: ").strip()
    id_medico = input("  ID del médico: ").strip()
    motivo = input("  Motivo: ").strip()
    ok, msg = mod_citas.crear_cita(
        registro_citas, catalogo_pacientes, catalogo_personal,
        dni, id_medico, motivo
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _agregar_medicacion():
    id_cita = input("\n  ID de la cita: ").strip()
    medicamento = input("  Medicamento: ").strip()
    id_medico = input("  ID del médico que prescribe: ").strip()
    ok, msg = mod_citas.agregar_medicacion(
        registro_citas, pila_deshacer, id_cita, medicamento, id_medico
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _deshacer_medicacion():
    id_medico = input("\n  ID del médico solicitante: ").strip()
    ok, msg = mod_citas.deshacer_medicacion(
        registro_citas, pila_deshacer, catalogo_personal, id_medico
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _completar_cita():
    id_cita = input("\n  ID de la cita: ").strip()
    diagnostico = input("  Diagnóstico: ").strip()
    notas = input("  Notas: ").strip()
    ok, msg = mod_citas.completar_cita(
        registro_citas, catalogo_pacientes, id_cita, diagnostico, notas
    )
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _cancelar_cita():
    id_cita = input("\n  ID de la cita a cancelar: ").strip()
    motivo = input("  Motivo de cancelación: ").strip()
    ok, msg = mod_citas.cancelar_cita(registro_citas, id_cita, motivo)
    print(f"\n  {'✓' if ok else '✗'} {msg}")
    pausar()


def _buscar_cita():
    id_cita = input("\n  ID de la cita: ").strip()
    cita = mod_citas.buscar_cita(registro_citas, id_cita)
    if cita is None:
        print(f"\n  ✗ No se encontró cita {id_cita}.")
    else:
        datos = cita["datos_cita"]
        print(f"\n  ID:       {datos[0]}")
        print(f"  Paciente: {datos[1]}")
        print(f"  Médico:   {datos[2]}")
        print(f"  Fecha:    {datos[3]}")
        print(f"  Estado:   {cita['estado']}")
        print(f"  Motivo:   {cita.get('motivo', '')}")
        meds = cita["medicacion_actual"]
        print(f"  Medicación: {', '.join(meds) if meds else 'Ninguna'}")
        print(f"  Notas:    {cita.get('notas', '')}")
    pausar()


# ═══════════════════════════════════════════════════════════════════════════
#  SUBMENÚ: REPORTES Y EXPORTACIÓN
# ═══════════════════════════════════════════════════════════════════════════

def menu_reportes():
    """Submenú de reportes y exportación CSV."""
    while True:
        opcion = mostrar_menu("REPORTES Y EXPORTACIÓN", [
            ("1", "Resumen general del sistema"),
            ("2", "Carga de trabajo por médico"),
            ("3", "Alergias más comunes"),
            ("4", "Exportar pacientes a CSV"),
            ("5", "Exportar personal a CSV"),
            ("6", "Exportar citas a CSV"),
            ("7", "Exportar historial de paciente a CSV"),
            ("0", "Volver al menú principal"),
        ])

        if opcion == "1":
            mod_reportes.mostrar_resumen_general(
                catalogo_pacientes, catalogo_personal, registro_citas)
            pausar()
        elif opcion == "2":
            mod_reportes.mostrar_carga_medicos(registro_citas, catalogo_personal)
            pausar()
        elif opcion == "3":
            comunes = mod_reportes.reporte_alergias_comunes(catalogo_pacientes)
            if not comunes:
                print("\n  No hay alergias registradas.")
            else:
                print("\n  ── Alergias Más Comunes ──")
                for alergia, count in comunes:
                    print(f"  {alergia}: {count} paciente(s)")
            pausar()
        elif opcion == "4":
            ok, msg = mod_exportacion_csv.exportar_reporte_pacientes(catalogo_pacientes)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            pausar()
        elif opcion == "5":
            ok, msg = mod_exportacion_csv.exportar_reporte_personal(catalogo_personal)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            pausar()
        elif opcion == "6":
            ok, msg = mod_exportacion_csv.exportar_reporte_citas(registro_citas)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            pausar()
        elif opcion == "7":
            dni = input("\n  DNI del paciente: ").strip()
            ok, msg = mod_exportacion_csv.exportar_historial_paciente(
                catalogo_pacientes, dni)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            pausar()
        elif opcion == "0":
            break


# ═══════════════════════════════════════════════════════════════════════════
#  MENÚ PRINCIPAL
# ═══════════════════════════════════════════════════════════════════════════

def menu_principal():
    """Bucle principal del sistema."""
    global catalogo_pacientes, catalogo_personal, registro_citas

    # Cargar datos persistidos
    print("\n  Cargando datos...")
    catalogo_pacientes, catalogo_personal, registro_citas = (
        mod_persistencia.cargar_todo()
    )
    print(f"  ✓ Pacientes: {len(catalogo_pacientes)} | "
          f"Personal: {len(catalogo_personal)} | "
          f"Citas: {len(registro_citas)}")

    while True:
        opcion = mostrar_menu("SISTEMA HOSPITALARIO — MENÚ PRINCIPAL", [
            ("1", "Gestión de Pacientes"),
            ("2", "Gestión de Personal"),
            ("3", "Gestión de Citas"),
            ("4", "Reportes y Exportación"),
            ("5", "Guardar datos"),
            ("0", "Guardar y Salir"),
        ])

        if opcion == "1":
            menu_pacientes()
        elif opcion == "2":
            menu_personal()
        elif opcion == "3":
            menu_citas()
        elif opcion == "4":
            menu_reportes()
        elif opcion == "5":
            ok, msg = mod_persistencia.guardar_todo(
                catalogo_pacientes, catalogo_personal, registro_citas)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            pausar()
        elif opcion == "0":
            ok, msg = mod_persistencia.guardar_todo(
                catalogo_pacientes, catalogo_personal, registro_citas)
            print(f"\n  {'✓' if ok else '✗'} {msg}")
            print("  ¡Hasta luego!\n")
            break


# ═══════════════════════════════════════════════════════════════════════════

if __name__ == "__main__":
    menu_principal()
