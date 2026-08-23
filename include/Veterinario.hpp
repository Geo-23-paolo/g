#pragma once

#include <string>

class Gato;
class Comida;

class Veterinario
{
public:
    Veterinario(const std::string& nombre, const std::string& especialidad);

 
    void CurarGato(Gato& gato, void* windowHandle);
    void RevivirGato(Gato& gato, Comida& comida, void* windowHandle);

private:
    std::string nombre;
    std::string especialidad;
};