#pragma once

#include <string>

class Gato
{
public:
    Gato(const std::string& nombre, int edad);

    void Comer();
    void Dormir();
    void Maullar();
    void IrAlBano();

private:
    std::string nombre;
    int edad;
    int hambre;
};