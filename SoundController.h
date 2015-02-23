/* 
 * File:   SoundController.h
 * Author: zgrw
 *
 * Created on February 16, 2015, 10:09 PM
 */

#ifndef SOUNDCONTROLLER_H
#define	SOUNDCONTROLLER_H

#ifdef WIN32
    #include "mmdeviceapi.h"
    #include "endpointvolume.h"
#elif __APPLE__
#else
    #include <alsa/asoundlib.h>
#endif

class SoundController
{
public:
    SoundController();
    ~SoundController();
    float getCurrentVolumeLevel();
    bool isMuted();
    bool muteUnmute(const int &state = 2); //0: means mute, 1: unmute, 2: change behaviour
    bool increaseVolume();
    bool decreaseVolume();
protected:
    bool setVolume(float);
private:
#ifdef WIN32
    HRESULT hr;
    IMMDeviceEnumerator *deviceEnumarator;
    IMMDevice *defaultDevice;
    IAudioEndPointVolume *endpointVolume;
#elif __APPLE__
#else
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    snd_mixer_elem_t* elem;
    long min, max;
#endif
};



#endif	/* SOUNDCONTROLLER_H */

