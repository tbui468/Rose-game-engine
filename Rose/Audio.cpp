#include "Audio.h"

#include <combaseapi.h>
#include <fmod_errors.h>

namespace rose {


    //COM needs to be initialized and unizitialized for windows systems
    //not uninitializing COM is a memory leak
    Audio::Audio() {
        CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

        FMOD_RESULT result;
        result = FMOD::System_Create(&m_AudioID);
        if(result != FMOD_OK) {
            CoUninitialize();
            //printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }

        //init fmod
        result = m_AudioID->init(512, FMOD_INIT_NORMAL, 0);
        if(result != FMOD_OK) {
            CoUninitialize();
            //printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }
    }

    Audio::~Audio() {
        Shutdown();
    }

    void Audio::Shutdown() {
        CoUninitialize();
    }


}
