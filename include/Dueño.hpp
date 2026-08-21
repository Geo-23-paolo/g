#pragma once

#include <string>

class Dueño
{
public:
    explicit Dueño(const std::string& nombre);

    void AlimentarGato();
    void LimpiarArenero();
    void LlevarAlVeterinario();

private:
    std::string nombre;
};