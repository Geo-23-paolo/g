#pragma once

#include "Arenero.hpp"
#include "Cama.hpp"
#include "Comida.hpp"
#include "Dueno.hpp"
#include "Gato.hpp"
#include "Veterinario.hpp"

#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
#include <mfapi.h>
#include <mfplay.h>
#include <propvarutil.h>
#include <windows.h>

#include <atomic>

namespace
{
class MediaPlayerCallback final : public IMFPMediaPlayerCallback
{
public:
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** object) override
    {
        if (object == nullptr)
        {
            return E_POINTER;
        }

        if (riid == __uuidof(IUnknown) || riid == __uuidof(IMFPMediaPlayerCallback))
        {
            *object = static_cast<IMFPMediaPlayerCallback*>(this);
            AddRef();
            return S_OK;
        }

        *object = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return ++referenceCount;
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        const ULONG count = --referenceCount;
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    void STDMETHODCALLTYPE OnMediaPlayerEvent(MFP_EVENT_HEADER* eventHeader) override
    {
        if (eventHeader != nullptr &&
            eventHeader->eEventType == MFP_EVENT_TYPE_PLAYBACK_ENDED)
        {
            playbackEnded = true;
        }
    }

    bool HasEnded() const
    {
        return playbackEnded;
    }

private:
    std::atomic<ULONG> referenceCount{1};
    std::atomic<bool> playbackEnded{false};
};
}
#endif

inline Arenero::Arenero(bool limpio, const std::string& ubicacion)
    : limpio(limpio), ubicacion(ubicacion), zonaIzquierda(0.0f),
      zonaDerecha(0.0f), interfazConfigurada(false)
{
}

inline void Arenero::Usar()
{
    limpio = false;
}

inline void Arenero::ConfigurarInterfaz(const sf::Font& font,
    const sf::FloatRect& backgroundBounds)
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

inline bool Arenero::EstaCerca(float catX) const
{
    return catX >= zonaIzquierda && catX <= zonaDerecha;
}

inline bool Arenero::Usar(Comida& comida)
{
    if (!comida.Consumir())
    {
        return false;
    }

    Usar();
    return true;
}

inline bool Arenero::Usar(Comida& comida, Gato& gato, void* windowHandle)
{
    if (comida.ObtenerCantidad() == 0)
    {
        return false;
    }

    gato.ReproducirVideo("assets/Images/Ir al baño.mp4", windowHandle);
    return Usar(comida);
}

inline void Arenero::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}

inline Cama::Cama()
    : zonaDerecha(0.0f), interfazConfigurada(false)
{
}

inline void Cama::ConfigurarInterfaz(const sf::Font& font,
    const sf::FloatRect& backgroundBounds)
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

inline bool Cama::EstaCerca(float catX) const
{
    return catX <= zonaDerecha;
}

inline void Cama::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}

inline Comida::Comida(const std::string& tipo, float cantidad)
    : tipo(tipo), cantidad(cantidad), interfazConfigurada(false),
      zonaIzquierda(0.0f), zonaDerecha(0.0f)
{
}

inline void Comida::Servir()
{
    cantidad = 3;
    ActualizarFrames();
}

inline bool Comida::Consumir()
{
    if (cantidad <= 0)
    {
        return false;
    }

    --cantidad;
    ActualizarFrames();
    return true;
}

inline int Comida::ObtenerCantidad() const
{
    return static_cast<int>(cantidad);
}

inline void Comida::ConfigurarInterfaz(const sf::Texture& texture,
    const sf::Font& font, const sf::FloatRect& backgroundBounds)
{
    zonaIzquierda = backgroundBounds.left + backgroundBounds.width * 0.50f;
    zonaDerecha = backgroundBounds.left + backgroundBounds.width * 0.75f;
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

inline bool Comida::CercaDeComida(float catX) const
{
    return catX >= zonaIzquierda && catX <= zonaDerecha;
}

inline void Comida::Dibujar(sf::RenderWindow& window) const
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

inline void Comida::DibujarAviso(sf::RenderWindow& window) const
{
    if (interfazConfigurada)
    {
        window.draw(aviso);
    }
}

inline void Comida::ActualizarFrames()
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

inline Gato::Gato(const std::string& nombre, int edad)
    : Personaje(nombre), edad(edad), energia(3)
{
}

inline void Gato::Comer(Comida& comida)
{
    comida.Servir();
}

inline bool Gato::Comer(Comida& comida, const std::string& videoPath,
    void* windowHandle, float catX)
{
    if (comida.ObtenerCantidad() != 0 || !comida.CercaDeComida(catX))
    {
        return false;
    }

    ReproducirVideo(videoPath, windowHandle);
    comida.Servir();
    return true;
}

inline bool Gato::IrAlBano(Arenero& arenero, Comida& comida,
    void* windowHandle, float catX)
{
    if (!arenero.EstaCerca(catX))
    {
        return false;
    }

    return arenero.Usar(comida, *this, windowHandle);
}

inline void Gato::Dormir()
{
    energia = 3;
}

inline bool Gato::Dormir(const Cama& cama, const std::string& videoPath,
    void* windowHandle, float catX)
{
    if (energia != 0 || !cama.EstaCerca(catX))
    {
        return false;
    }

    ReproducirVideo(videoPath, windowHandle);
    Dormir();
    return true;
}

inline void Gato::Mover(sf::Sprite& cat, const sf::Texture& idleTexture,
    const sf::Texture& rightTexture, const sf::Texture& leftTexture,
    int rightFrameWidth, int rightFrameHeight, int leftFrameWidth,
    int leftFrameHeight, float idleScale, float rightScale, float leftScale,
    float groundY, float rightGroundY, float leftGroundY,
    const sf::FloatRect& backgroundBounds, sf::Clock& animationClock,
    int& rightFrame, int& leftFrame, float rightFrameDuration,
    float leftFrameDuration, bool movingRight, bool movingLeft,
    float deltaTime, float speed)
{
    auto setFrame = [&cat](const sf::Texture& texture, int frame,
        int frameWidth, int frameHeight, float scale, float ground)
    {
        const float currentX = cat.getPosition().x;
        cat.setTexture(texture, true);
        cat.setTextureRect(sf::IntRect(
            frame * frameWidth, 0, frameWidth, frameHeight));
        cat.setOrigin(0.0f, cat.getLocalBounds().height);
        cat.setScale(scale, scale);
        cat.setPosition(currentX, ground);
    };

    if (movingRight && !movingLeft)
    {
        if (cat.getTexture() != &rightTexture)
        {
            rightFrame = 0;
            setFrame(rightTexture, rightFrame, rightFrameWidth, rightFrameHeight,
                rightScale, rightGroundY);
            animationClock.restart();
        }
        if (animationClock.getElapsedTime().asSeconds() >= rightFrameDuration)
        {
            rightFrame = (rightFrame + 1) % 4;
            setFrame(rightTexture, rightFrame, rightFrameWidth, rightFrameHeight,
                rightScale, rightGroundY);
            animationClock.restart();
        }
        const float rightLimit =
            backgroundBounds.left + backgroundBounds.width - cat.getGlobalBounds().width;
        cat.setPosition(std::min(
            cat.getPosition().x + speed * deltaTime, rightLimit), rightGroundY);
    }
    else if (movingLeft && !movingRight)
    {
        if (cat.getTexture() != &leftTexture)
        {
            leftFrame = 0;
            setFrame(leftTexture, leftFrame, leftFrameWidth, leftFrameHeight,
                leftScale, leftGroundY);
            animationClock.restart();
        }
        if (animationClock.getElapsedTime().asSeconds() >= leftFrameDuration)
        {
            leftFrame = (leftFrame + 1) % 4;
            setFrame(leftTexture, leftFrame, leftFrameWidth, leftFrameHeight,
                leftScale, leftGroundY);
            animationClock.restart();
        }
        cat.setPosition(std::max(
            cat.getPosition().x - speed * deltaTime, backgroundBounds.left), leftGroundY);
    }
    else if (cat.getTexture() != &idleTexture)
    {
        const sf::Vector2f basePosition = cat.getPosition();
        cat.setTexture(idleTexture, true);
        cat.setOrigin(0.0f, cat.getLocalBounds().height);
        cat.setScale(idleScale, idleScale);
        const float idleRightLimit =
            backgroundBounds.left + backgroundBounds.width - cat.getGlobalBounds().width;
        cat.setPosition(std::max(
            backgroundBounds.left, std::min(basePosition.x, idleRightLimit)), groundY);
        rightFrame = 0;
        leftFrame = 0;
        animationClock.restart();
    }
}

inline void Gato::ReproducirVideo(const std::string& videoPath, void* windowHandle)
{
#ifdef _WIN32
    wchar_t relativeVideoPath[MAX_PATH];
    const int relativePathLength = MultiByteToWideChar(
        CP_UTF8, MB_ERR_INVALID_CHARS, videoPath.c_str(), -1,
        relativeVideoPath, MAX_PATH);
    if (relativePathLength == 0)
    {
        Dormir();
        return;
    }

    wchar_t widePath[MAX_PATH];
    const DWORD pathLength = GetFullPathNameW(
        relativeVideoPath, MAX_PATH, widePath, nullptr);
    if (pathLength == 0 || pathLength >= MAX_PATH)
    {
        Dormir();
        return;
    }

    const HWND parentWindow = static_cast<HWND>(windowHandle);
    RECT clientArea;
    GetClientRect(parentWindow, &clientArea);
    const HWND videoWindow = CreateWindowExW(
        0, L"STATIC", nullptr, WS_CHILD | WS_VISIBLE,
        0, 0, clientArea.right, clientArea.bottom,
        parentWindow, nullptr, GetModuleHandle(nullptr), nullptr);
    if (videoWindow == nullptr)
    {
        Dormir();
        return;
    }

    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED)) ||
        FAILED(MFStartup(MF_VERSION)))
    {
        DestroyWindow(videoWindow);
        Dormir();
        return;
    }

    MediaPlayerCallback* callback = new MediaPlayerCallback();
    IMFPMediaPlayer* player = nullptr;
    const HRESULT result = MFPCreateMediaPlayer(
        widePath, TRUE, MFP_OPTION_NONE, callback,
        videoWindow, &player);

    if (SUCCEEDED(result))
    {
        bool playbackStarted = false;
        const ULONGLONG timeout = GetTickCount64() + 30000;
        while (!callback->HasEnded())
        {
            MFP_MEDIAPLAYER_STATE playerState;
            if (SUCCEEDED(player->GetState(&playerState)))
            {
                playbackStarted = playbackStarted ||
                    playerState == MFP_MEDIAPLAYER_STATE_PLAYING;
                if (playbackStarted && playerState == MFP_MEDIAPLAYER_STATE_STOPPED)
                {
                    break;
                }
            }

            PROPVARIANT durationValue;
            PROPVARIANT positionValue;
            PropVariantInit(&durationValue);
            PropVariantInit(&positionValue);
            const bool hasDuration =
                SUCCEEDED(player->GetDuration(MFP_POSITIONTYPE_100NS, &durationValue)) &&
                durationValue.vt == VT_I8;
            const bool hasPosition =
                SUCCEEDED(player->GetPosition(MFP_POSITIONTYPE_100NS, &positionValue)) &&
                positionValue.vt == VT_I8;
            const bool reachedEnd = hasDuration && hasPosition &&
                positionValue.hVal.QuadPart >= durationValue.hVal.QuadPart;
            PropVariantClear(&durationValue);
            PropVariantClear(&positionValue);
            if (reachedEnd)
            {
                break;
            }

            if (GetTickCount64() >= timeout)
            {
                break;
            }

            MSG message;
            while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                if (message.message == WM_QUIT)
                {
                    callback->Release();
                    player->Shutdown();
                    player->Release();
                    DestroyWindow(videoWindow);
                    MFShutdown();
                    CoUninitialize();
                    Dormir();
                    return;
                }
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            Sleep(10);
        }
        player->Stop();
        player->ClearMediaItem();
        player->Shutdown();
        player->Release();
    }

    callback->Release();
    if (player == nullptr || SUCCEEDED(result))
    {
        DestroyWindow(videoWindow);
    }
    MFShutdown();
    CoUninitialize();
    RedrawWindow(parentWindow, nullptr, nullptr,
        RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
#else
    (void)videoPath;
    (void)windowHandle;
#endif
}

inline void Gato::PerderEnergia()
{
    if (energia > 0)
    {
        --energia;
    }
}

inline int Gato::ObtenerEnergia() const
{
    return energia;
}

inline Veterinario::Veterinario(const std::string& nombre,
    const std::string& especialidad)
    : nombre(nombre), especialidad(especialidad)
{
}

inline void Veterinario::CurarGato(Gato& gato, void* windowHandle)
{
    gato.ReproducirVideo("assets/Images/Curar Gato.mp4", windowHandle);
    gato.Dormir();
}

inline void Veterinario::RevivirGato(Gato& gato, Comida& comida, void* windowHandle)
{
    gato.ReproducirVideo("assets/Images/Revivir.mp4", windowHandle);
    gato.Dormir();
    comida.Servir();
}

inline Dueno::Dueno(const std::string& nombre)
    : Personaje(nombre)
{
}

inline bool Dueno::RegistrarGato(const std::string& nombreGato,
    const std::string& edadTexto,
    const std::string& genero,
    Gato& gatoRegistrado)
{
    if (nombreGato.empty() || edadTexto.empty() || genero.empty())
    {
        return false;
    }

    try
    {
        std::size_t processedChars = 0;
        int edad = std::stoi(edadTexto, &processedChars);
        if (processedChars != edadTexto.size())
        {
            return false;
        }

        if (edad < 1 || edad > 30)
        {
            return false;
        }

        gatoRegistrado = Gato(nombreGato, edad);
        generoGato = genero;
        return true;
    }
    catch (const std::exception&)
    {
        return false;
    }
}

inline const std::string& Dueno::ObtenerNombre() const
{
    return Personaje::ObtenerNombre();
}

inline const std::string& Dueno::ObtenerGeneroGato() const
{
    return generoGato;
}
