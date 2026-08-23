#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Comida;
class Gato;

class Arenero
{
public:
    Arenero(bool limpio, const std::string& ubicacion);

    void Usar();
    void ConfigurarInterfaz(const sf::Font& font, const sf::FloatRect& backgroundBounds);
    bool EstaCerca(float catX) const;
    bool Usar(Comida& comida);
    bool Usar(Comida& comida, Gato& gato, void* windowHandle);
    void DibujarAviso(sf::RenderWindow& window) const;

private:
    bool limpio;
    std::string ubicacion;
    float zonaIzquierda;
    float zonaDerecha;
    sf::Text aviso;
    bool interfazConfigurada;
};