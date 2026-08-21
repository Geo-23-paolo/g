#include "Arenero.hpp"

Arenero::Arenero(bool limpio, const std::string& ubicacion)
    : limpio(limpio), ubicacion(ubicacion)
{
}

void Arenero::Limpiar()
{
    limpio = true;
}

void Arenero::Usar()
{
    limpio = false;
}