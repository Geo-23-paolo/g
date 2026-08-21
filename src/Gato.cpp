#include "Gato.hpp"

Gato::Gato(const std::string& nombre, int edad)
    : nombre(nombre), edad(edad), hambre(0)
{
}

void Gato::Comer()
{
    hambre = 0;
}

void Gato::Dormir()
{
}

void Gato::Maullar()
{
}

void Gato::IrAlBano()
{
}