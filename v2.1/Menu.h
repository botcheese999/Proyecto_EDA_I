/*
 * ═══════════════════════════════════════════════════════════════════════════
 *  Menu.h — Clase de interfaz de usuario por consola (V2.1)
 * ═══════════════════════════════════════════════════════════════════════════
 *
 *  DISEÑO:
 *    Clase independiente y encapsulada. No conoce nada del dominio
 *    hospitalario — solo gestiona opciones, las imprime y valida la
 *    entrada del usuario.
 *
 *  RESPONSABILIDADES:
 *    - Almacenar opciones en std::vector<std::string>.
 *    - add_option()  → agrega una opción al vector.
 *    - display()     → imprime el menú numerado.
 *    - get_input()   → valida la selección con do-while + cin.fail().
 */

#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>

class Menu {
private:
    std::string titulo_;
    std::vector<std::string> opciones_;

public:
    // Constructor con título opcional
    explicit Menu(const std::string& titulo = "MENU");

    // Agrega una opción al vector
    void add_option(const std::string& opcion);

    // Imprime el menú numerado en consola
    void display() const;

    /*
     * Solicita y valida la entrada del usuario.
     *
     * CONTRATO:
     *   - Retorna un entero en el rango [1, opciones_.size()].
     *   - Usa do-while para repreguntar ante entrada inválida.
     *   - Maneja cin.fail() (texto en lugar de número):
     *       cin.clear()  → limpia el flag de error del stream
     *       cin.ignore() → descarta los caracteres residuales del buffer
     *     Sin esto, cin.fail() activado causa un bucle infinito porque el
     *     stream no avanza y el buffer nunca se vacía.
     */
    int get_input() const;
};

#endif // MENU_H
