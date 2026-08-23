#include "Gato.hpp"
#include "Cama.hpp"
#include "Comida.hpp"

#include <algorithm>

#ifdef _WIN32
#include <mfplay.h>
#include <mfapi.h>
#include <windows.h>
#include <atomic>
#include <propvarutil.h>
#include <string>

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

Gato::Gato(const std::string& nombre, int edad)
    : nombre(nombre), edad(edad), energia(3)
{
}

void Gato::Comer(Comida& comida)
{
    comida.Servir();
}

bool Gato::Comer(Comida& comida, const std::string& videoPath,
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

void Gato::Dormir()
{
    energia = 3;
}

bool Gato::Dormir(const Cama& cama, const std::string& videoPath,
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

void Gato::Mover(sf::Sprite& cat, const sf::Texture& idleTexture,
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

void Gato::ReproducirVideo(const std::string& videoPath, void* windowHandle)
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

void Gato::PerderEnergia()
{
    if (energia > 0)
    {
        --energia;
    }
}

int Gato::ObtenerEnergia() const
{
    return energia;
}

