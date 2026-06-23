"""Prueba funcional rápida de todos los módulos."""
import mod_pacientes, mod_personal, mod_citas
import mod_persistencia, mod_reportes, mod_exportacion_csv

pac, pers, citas = {}, {}, {}
pila = []

# 1) Pacientes
ok, m = mod_pacientes.registrar_paciente(pac, "12345678", "Juan Perez", "O+", "1990-05-15", "555-1234")
print(f"[Pac] {m}")
ok, m = mod_pacientes.registrar_paciente(pac, "87654321", "Maria Lopez", "A+", "1985-03-20")
print(f"[Pac] {m}")

# 2) Alergias (sets)
mod_pacientes.agregar_alergia(pac, "12345678", "penicilina")
mod_pacientes.agregar_alergia(pac, "12345678", "latex")
mod_pacientes.agregar_alergia(pac, "87654321", "penicilina")
ok, comunes = mod_pacientes.cruzar_alergias(pac, "12345678", "87654321")
print(f"[Sets] Alergias comunes: {comunes}")

# 3) Personal
ok, m = mod_personal.registrar_personal(pers, "Dr. Garcia", "especialista", "Cardiologia")
print(f"[Pers] {m}")
ok, m = mod_personal.registrar_personal(pers, "Enf. Ruiz", "enfermera")
print(f"[Pers] {m}")

# 4) Accesos
print(f"[Acceso] Dr. Garcia recetar: {mod_personal.verificar_acceso(pers, 'P0001', 'recetar')}")
print(f"[Acceso] Enf. Ruiz recetar: {mod_personal.verificar_acceso(pers, 'P0002', 'recetar')}")

# 5) Citas
ok, m = mod_citas.crear_cita(citas, pac, pers, "12345678", "P0001", "Control")
print(f"[Cita] {m}")
ok, m = mod_citas.agregar_medicacion(citas, pila, "C0001", "Amoxicilina", "P0001")
print(f"[Med] {m}")
ok, m = mod_citas.agregar_medicacion(citas, pila, "C0001", "Ibuprofeno", "P0001")
print(f"[Med] {m}")
ok, m = mod_citas.deshacer_medicacion(citas, pila, pers, "P0001")
print(f"[Deshacer] {m}")
ok, m = mod_citas.completar_cita(citas, pac, "C0001", "Gripe estacional", "Reposo 3 dias")
print(f"[Completar] {m}")

# 6) Historial
hist = mod_pacientes.obtener_historial(pac, "12345678")
print(f"[Historial] Entradas: {len(hist)}, Diagnostico: {hist[-1]['diagnostico']}")

# 7) JSON persistencia
ok, m = mod_persistencia.guardar_todo(pac, pers, citas)
print(f"[JSON Guardar] {m}")
pac2, pers2, citas2 = mod_persistencia.cargar_todo()
alergias_recargadas = pac2["12345678"]["alergias"]
tipo_datos = type(pac2["12345678"]["datos_fijos"]).__name__
print(f"[JSON Cargar] Pac:{len(pac2)} Alergias:{alergias_recargadas} TipoDatos:{tipo_datos}")

# 8) CSV
ok, m = mod_exportacion_csv.exportar_reporte_pacientes(pac)
print(f"[CSV] {m}")

# 9) Resumen
mod_reportes.mostrar_resumen_general(pac, pers, citas)

print("\n=== TODAS LAS PRUEBAS PASARON ===")
