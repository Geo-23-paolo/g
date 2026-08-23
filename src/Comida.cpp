#include "Comida.hpp"

Comida::Comida(const std::string& tipo, float cantidad)
    : tipo(tipo), cantidad(cantidad), interfazConfigurada(false)
{
}

void Comida::Servir()
{
    cantidad = 3;
    ActualizarFrames();
}

bool Comida::Consumir()
{
    if (cantidad <= 0)
    {
        return false;
    }

    --cantidad;
    ActualizarFrames();
    return true;
}

int Comida::ObtenerCantidad() const
{
    return static_cast<int>(cantidad);
}

void Comida::ConfigurarInterfaz(const sf::Texture& texture, const sf::Font& font,
    const sf::FloatRect& backgroundBounds)
{
    const sf::Vector2u sheetSize = texture.getSize();
    const int frameWidth = static_cast<int>(sheetSize.x / 2);
    const float scale = 0.10f;
    const float margin = 20.0f;

    for (int index = 0; index < 3; ++index)
    {
        frames[index].setTexture(texture);
        frames[index].setScale(scale, scale);
        frames[index].setPosition(
            margin + frameWidth * scale * static_cast<float>(index), margin);
    }

    aviso.setFont(font);
    aviso.setString("Presiona C para comer");
    aviso.setCharacterSize(22);
    aviso.setFillColor(sf::Color::White);
    aviso.setStyle(sf::Text::Bold);
    aviso.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.61f,
        backgroundBounds.top + backgroundBounds.height * 0.56f);
    aviso.setOrigin(
        aviso.getLocalBounds().left + aviso.getLocalBounds().width / 2.0f,
        aviso.getLocalBounds().top);
    interfazConfigurada = true;
    ActualizarFrames();
}

void Comida::Dibujar(sf::RenderWindow& window) const
{
    if (!interfazConfigurada)
    {
        return;
    }

    for (const sf::Sprite& frame : frames)
    {
        window.draw(frame);
    }
}

void Comida::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}

void Comida::ActualizarFrames()
{
    if (!interfazConfigurada)
    {
        return;
    }

    const sf::Vector2u sheetSize = frames[0].getTexture()->getSize();
    const int frameWidth = static_cast<int>(sheetSize.x / 2);
    const int frameHeight = static_cast<int>(sheetSize.y);
    const int amount = ObtenerCantidad();
    for (int index = 0; index < 3; ++index)
    {
        frames[index].setTextureRect(sf::IntRect(
            index < amount ? frameWidth : 0, 0, frameWidth, frameHeight));
    }
}