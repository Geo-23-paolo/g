#pragma once

#include <SFML/Graphics.hpp>

class Cama
{
public:
    Cama();

    void ConfigurarInterfaz(const sf::Font& font, const sf::FloatRect& backgroundBounds);
    bool EstaCerca(float catX) const;
    void DibujarAviso(sf::RenderWindow& window) const;

private:
    float zonaDerecha;
    sf::Text aviso;
    bool interfazConfigurada;
};
