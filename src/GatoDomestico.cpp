#include "GatoDomestico.hpp"

GatoDomestico::GatoDomestico(
    const std::string& nombre,
    int edad,
    const std::string& raza)
    : Gato(nombre, edad), raza(raza)
{
}

void GatoDomestico::Jugar()
{
}