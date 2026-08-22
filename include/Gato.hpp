#pragma once

#include <string>

class Gato
{
public:
    Gato(const std::string& nombre, int edad);

    void Comer();
    void Comer(const std::string& videoPath, void* windowHandle);
    void Dormir();
    void Dormir(const std::string& videoPath, void* windowHandle);
    void PerderEnergia();
    int ObtenerEnergia() const;
    void Maullar();
    void IrAlBano();

private:
    void ReproducirVideo(const std::string& videoPath, void* windowHandle);
    std::string nombre;
    int edad;
    int hambre;
    int energia;
};