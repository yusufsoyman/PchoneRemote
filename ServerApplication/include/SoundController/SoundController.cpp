//Windows sound control codes is learned from http://www.codeproject.com/Tips/233484/Change-Master-Volume-in-Visual-Cplusplus



#include <cmath>
#include "SoundController.h"
#include "Logger.h"

#ifdef WIN32
    #include "mmdeviceapi.h"
    #include "endpointvolume.h"
#elif __APPLE__
#else
    #include <alsa/asoundlib.h>
#endif


SoundController::SoundController()
#ifdef WIN32
:defaultDevice(NULL), deviceEnumarator(NULL), endpointVolume(NULL)
#elif __APPLE__
#else
:elem(NULL), handle(NULL), sid(NULL)
#endif
{
    logger = Logger::getInstance();
    char buffer[1024];
    sprintf(buffer, "%s - %d: Initialized default device.", __FILE__, __LINE__);
#ifdef WIN32
    CoInitialize(NULL);
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    deviceEnumerator->Release();
    deviceEnumerator = NULL;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
    defaultDevice->Release();
    defaultDevice = NULL;
#elif __APPLE__
#else
    const char *card = "default";
    const char *selem_name = "Master";
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    elem = snd_mixer_find_selem(handle, sid);
    
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
#endif
    logger -> printInfoLog(buffer);
}

SoundController::~SoundController()
{
    char buffer[1024];
    sprintf(buffer, "%s - %d: Ended sound control settings.", __FILE__, __LINE__);
#ifdef WIN32
    endpointVolume->Release();
    CoUninitialize();
#elif __APPLE__
#else
    snd_mixer_close(handle);
#endif
    logger -> printInfoLog(buffer);
}

float SoundController::getCurrentVolumeLevel()
{
    float currentVolume = 0;
    char buffer[1024];
#ifdef WIN32
    endpointVolume->GetMasterVolumeLevel(&currentVolume);
    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
#elif __APPLE__
#else
    long value;
    snd_mixer_selem_get_playback_volume (elem, SND_MIXER_SCHN_FRONT_LEFT, &value);
    currentVolume = (100.0 * value) / max;
#endif
    sprintf(buffer, "%s - %d: Learned current volume as %f.", __FILE__, __LINE__, currentVolume);
    logger -> printDebugLog(buffer);
    return currentVolume;
}

bool SoundController::isMuted()
{
    char buffer[1024];
    int mute;
#ifdef WIN32
    endpointVolume -> GetMute(&mute);
#elif __APPLE__
#else
    snd_mixer_selem_get_playback_switch (elem, SND_MIXER_SCHN_FRONT_LEFT, &mute);//snd_mixer_selem_has_playback_switch(elem);
#endif
    sprintf(buffer, "%s - %d: System is currently %s.", __FILE__, __LINE__, (mute == 1)?"muted":"not muted");
    logger -> printDebugLog(buffer);
    return (mute == 1);
}

bool SoundController::muteUnmute(const int &state) //0: means mute, 1: unmute, 2: change behaviour
{
    char buffer[1024];
    bool mute = isMuted();
    int muteFlag;
    if(state == 0 || state == 1)
    {
        muteFlag = state;
    }
    else
    {
        muteFlag = (mute)?0:1;
    }
#ifdef WIN32
    endpointVolume->SetMute(muteFlag, NULL);
#elif __APPLE__
#else
    snd_mixer_selem_set_playback_switch_all(elem, muteFlag);
#endif
    sprintf(buffer, "%s - %d: System is now %s.", __FILE__, __LINE__, (mute == 0)?"muted":"not muted");
    logger -> printDebugLog(buffer);
    return true;
}

bool SoundController::setVolume(float currentVolume)
{
    char buffer[1024];
#ifdef WIN32
    hr = endpointVolume->SetMasterVolumeLevelScalar(currentVolume, NULL);
#elif __APPLE__
#else
    snd_mixer_selem_set_playback_volume_all(elem, trunc((currentVolume * max) / 100));
#endif
    sprintf(buffer, "%s - %d: System volume is set to %f.", __FILE__, __LINE__, currentVolume);
    logger -> printDebugLog(buffer);
}

bool SoundController::increaseVolume()
{
    char buffer[1024];
    float currentVolume = getCurrentVolumeLevel();
    ++currentVolume;
    sprintf(buffer, "%s - %d: System volume is increased.", __FILE__, __LINE__);
    logger -> printDebugLog(buffer);
    return setVolume(currentVolume);
}

bool SoundController::decreaseVolume()
{
    char buffer[1024];
    float currentVolume = getCurrentVolumeLevel();
    currentVolume = ((currentVolume - 1) < 0)? 0 : currentVolume - 1;
    sprintf(buffer, "%s - %d: System volume is decreased.", __FILE__, __LINE__);
    logger -> printDebugLog(buffer);
    return setVolume(currentVolume);
}
