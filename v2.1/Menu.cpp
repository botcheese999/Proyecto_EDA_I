/*
 * Menu.cpp — Implementación de la clase Menu (V2.1)
 */

#include "Menu.h"
#include <iostream>
#include <limits>

Menu::Menu(const std::string& titulo)
    : titulo_(titulo)
{}

void Menu::add_option(const std::string& opcion) {
    opciones_.push_back(opcion);
}

void Menu::display() const {
    std::cout << "\n══════════════════════════════════════\n";
    std::cout << "  " << titulo_ << "\n";
    std::cout << "══════════════════════════════════════\n";
    for (size_t i = 0; i < opciones_.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] " << opciones_[i] << "\n";
    }
    std::cout << "══════════════════════════════════════\n";
    std::cout << "  Opcion: ";
}

int Menu::get_input() const {
    int opcion = 0;
    int max = static_cast<int>(opciones_.size());

    do {
        display();

        std::cin >> opcion;

        // ── cin.fail() se activa cuando el usuario ingresa texto no numérico.
        //    Sin clear() + ignore(), el buffer queda "atascado" y el do-while
        //    se convierte en un bucle infinito imprimiendo el menú sin pausa.
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Entrada invalida. Ingresa un numero.\n";
            opcion = 0;  // fuerza nueva iteración
            continue;
        }

        // Descartar el newline residual del buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (opcion < 1 || opcion > max) {
            std::cout << "  [!] Opcion fuera de rango. Elige entre 1 y " << max << ".\n";
        }

    } while (opcion < 1 || opcion > max);

    return opcion;
}
