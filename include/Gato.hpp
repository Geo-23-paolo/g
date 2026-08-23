#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Cama;
class Comida;

class Gato
{
public:
    Gato(const std::string& nombre, int edad);

    void Comer();
    void Comer(Comida& comida);
    void Comer(const std::string& videoPath, void* windowHandle);
    bool Comer(Comida& comida, const std::string& videoPath,
        void* windowHandle, float catX);
    void Dormir();
    void Dormir(const std::string& videoPath, void* windowHandle);
    bool Dormir(const Cama& cama, const std::string& videoPath,
        void* windowHandle, float catX);
    void ConfigurarAvisos(const sf::Font& font, const sf::FloatRect& backgroundBounds);
    bool CercaDeComida(float catX) const;
    void DibujarAvisoComer(sf::RenderWindow& window) const;
    void DibujarAvisoDormir(sf::RenderWindow& window) const;
    void PerderEnergia();
    int ObtenerEnergia() const;
    void Maullar();
    void ReproducirVideo(const std::string& videoPath, void* windowHandle);
    void Mover(sf::Sprite& cat, const sf::Texture& idleTexture,
        const sf::Texture& rightTexture, const sf::Texture& leftTexture,
        int rightFrameWidth, int rightFrameHeight, int leftFrameWidth,
        int leftFrameHeight, float idleScale, float rightScale, float leftScale,
        float groundY, float rightGroundY, float leftGroundY,
        const sf::FloatRect& backgroundBounds, sf::Clock& animationClock,
        int& rightFrame, int& leftFrame, float rightFrameDuration,
        float leftFrameDuration, bool movingRight, bool movingLeft,
        float deltaTime, float speed);

private:
    std::string nombre;
    int edad;
    int energia;
    float zonaComidaIzquierda;
    float zonaComidaDerecha;
    sf::Text avisoComer;
    sf::Text avisoDormir;
    bool avisosConfigurados;
};