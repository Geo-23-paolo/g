#pragma once

#include <string>

class Veterinario
{
public:
    Veterinario(const std::string& nombre, const std::string& especialidad);

    void RevisarGato();
    void Vacunar();

private:
    std::string nombre;
    std::string especialidad;
};