#include "Veterinario.hpp"
#include "Gato.hpp"
#include "Comida.hpp"

Veterinario::Veterinario(
    const std::string& nombre,
    const std::string& especialidad)
    : nombre(nombre), especialidad(especialidad)
{
}



void Veterinario::CurarGato(Gato& gato, void* windowHandle)
{
    gato.ReproducirVideo("assets/Images/Curar Gato.mp4", windowHandle);
    gato.Dormir();
}

void Veterinario::RevivirGato(Gato& gato, Comida& comida, void* windowHandle)
{
    gato.ReproducirVideo("assets/Images/Revivir.mp4", windowHandle);
    gato.Dormir();
    comida.Servir();
}