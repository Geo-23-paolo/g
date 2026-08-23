#include "Cama.hpp"

Cama::Cama()
    : zonaDerecha(0.0f), interfazConfigurada(false)
{
}

void Cama::ConfigurarInterfaz(const sf::Font& font, const sf::FloatRect& backgroundBounds)
{
    zonaDerecha = backgroundBounds.left + backgroundBounds.width * 0.22f;
    aviso.setFont(font);
    aviso.setString("Presiona E para dormir");
    aviso.setCharacterSize(22);
    aviso.setFillColor(sf::Color::White);
    aviso.setStyle(sf::Text::Bold);
    aviso.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.40f,
        backgroundBounds.top + backgroundBounds.height * 0.56f);
    aviso.setOrigin(
        aviso.getLocalBounds().left + aviso.getLocalBounds().width / 2.0f,
        aviso.getLocalBounds().top);
    interfazConfigurada = true;
}

bool Cama::EstaCerca(float catX) const
{
    return catX <= zonaDerecha;
}

void Cama::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}
