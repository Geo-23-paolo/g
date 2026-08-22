#include "Gato.hpp"

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
    : nombre(nombre), edad(edad), hambre(0), energia(3)
{
}

void Gato::Comer()
{
    hambre = 0;
}

void Gato::Comer(const std::string& videoPath, void* windowHandle)
{
    ReproducirVideo(videoPath, windowHandle);
    Comer();
}

void Gato::Dormir()
{
    energia = 3;
}

void Gato::Dormir(const std::string& videoPath, void* windowHandle)
{
    ReproducirVideo(videoPath, windowHandle);
    Dormir();
}

void Gato::ReproducirVideo(const std::string& videoPath, void* windowHandle)
{
#ifdef _WIN32
    char absoluteVideoPath[MAX_PATH];
    const DWORD pathLength = GetFullPathNameA(
        videoPath.c_str(), MAX_PATH, absoluteVideoPath, nullptr);
    if (pathLength == 0 || pathLength >= MAX_PATH)
    {
        Dormir();
        return;
    }

    wchar_t widePath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, absoluteVideoPath, -1,
        widePath, MAX_PATH);

    const HWND parentWindow = static_cast<HWND>(windowHandle);
    RECT clientArea;
    GetClientRect(parentWindow, &clientArea);
    const HWND videoWindow = CreateWindowExA(
        0, "STATIC", nullptr, WS_CHILD | WS_VISIBLE,
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

void Gato::Maullar()
{
}

void Gato::IrAlBano()
{
}