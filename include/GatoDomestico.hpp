#pragma once

#include "Gato.hpp"

#include <string>

class GatoDomestico : public Gato
{
public:
    GatoDomestico(const std::string& nombre, int edad, const std::string& raza);

    void Jugar();

private:
    std::string raza;
};