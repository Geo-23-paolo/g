#pragma once

#include <string>

class Comida
{
public:
    Comida(const std::string& tipo, float cantidad);

    void Servir();

private:
    std::string tipo;
    float cantidad;
};