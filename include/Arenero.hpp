#pragma once

#include <string>

class Arenero
{
public:
    Arenero(bool limpio, const std::string& ubicacion);

    void Limpiar();
    void Usar();

private:
    bool limpio;
    std::string ubicacion;
};