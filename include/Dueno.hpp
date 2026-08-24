#pragma once

#include <string>

#include "Personaje.hpp"

class Gato;

class Dueno : public Personaje
{
public:
    explicit Dueno(const std::string& nombre);

    bool RegistrarGato(const std::string& nombreGato,
        const std::string& edadTexto,
        const std::string& genero,
        Gato& gatoRegistrado);

    const std::string& ObtenerNombre() const;
    const std::string& ObtenerGeneroGato() const;

private:
    std::string generoGato;
};
