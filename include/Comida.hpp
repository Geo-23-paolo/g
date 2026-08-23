#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Comida
{
public:
    Comida(const std::string& tipo, float cantidad);

    void Servir();
    bool Consumir();
    int ObtenerCantidad() const;
    void ConfigurarInterfaz(const sf::Texture& texture, const sf::Font& font,
        const sf::FloatRect& backgroundBounds);
    bool CercaDeComida(float catX) const;
    void Dibujar(sf::RenderWindow& window) const;
    void DibujarAviso(sf::RenderWindow& window) const;

private:
    void ActualizarFrames();
    std::string tipo;
    float cantidad;
    sf::Sprite frames[3];
    sf::Text aviso;
    bool interfazConfigurada;
    float zonaIzquierda;
    float zonaDerecha;
};