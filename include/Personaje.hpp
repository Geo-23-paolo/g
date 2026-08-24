#pragma once

#include <string>

class Personaje
{
public:
    explicit Personaje(const std::string& nombre)
        : nombre(nombre)
    {
    }

    virtual ~Personaje() = default;

    const std::string& ObtenerNombre() const
    {
        return nombre;
    }

    void CambiarNombre(const std::string& nuevoNombre)
    {
        nombre = nuevoNombre;
    }

protected:
    std::string nombre;
};