#include "Arenero.hpp"
#include "Comida.hpp"
#include "Gato.hpp"

Arenero::Arenero(bool limpio, const std::string& ubicacion)
        : limpio(limpio), ubicacion(ubicacion), zonaIzquierda(0.0f),
            zonaDerecha(0.0f), interfazConfigurada(false)
{
}

void Arenero::Limpiar()
{
    limpio = true;
}

void Arenero::Usar()
{
    limpio = false;
}

void Arenero::ConfigurarInterfaz(const sf::Font& font, const sf::FloatRect& backgroundBounds)
{
    zonaIzquierda = backgroundBounds.left + backgroundBounds.width * 0.75f;
    zonaDerecha = backgroundBounds.left + backgroundBounds.width * 0.95f;
    aviso.setFont(font);
    aviso.setString("Presiona M para ir al baño");
    aviso.setCharacterSize(22);
    aviso.setFillColor(sf::Color::White);
    aviso.setStyle(sf::Text::Bold);
    aviso.setPosition(
        backgroundBounds.left + backgroundBounds.width * 0.78f,
        backgroundBounds.top + backgroundBounds.height * 0.56f);
    aviso.setOrigin(
        aviso.getLocalBounds().left + aviso.getLocalBounds().width / 2.0f,
        aviso.getLocalBounds().top);
    interfazConfigurada = true;
}

bool Arenero::EstaCerca(float catX) const
{
    return catX >= zonaIzquierda && catX <= zonaDerecha;
}

bool Arenero::Usar(Comida& comida)
{
    if (!comida.Consumir())
    {
        return false;
    }

    Usar();
    return true;
}

bool Arenero::Usar(Comida& comida, Gato& gato, void* windowHandle)
{
    if (comida.ObtenerCantidad() == 0)
    {
        return false;
    }

    gato.ReproducirVideo("assets/Images/Ir al baño.mp4", windowHandle);
    return Usar(comida);
}

void Arenero::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}