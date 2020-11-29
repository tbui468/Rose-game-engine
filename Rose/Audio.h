#ifndef AUDIO_H
#define AUDIO_H

//fmod and COM
//#include <combaseapi.h>
#include <fmod.h>
#include <fmod.hpp>
//#include <fmod_errors.h>

namespace rose {

    class Audio {
        public:
            Audio();
            virtual ~Audio();
            void Shutdown();
            FMOD::System* GetID() const { return m_AudioID; }
        private:
            FMOD::System* m_AudioID;
    };


}


#endif //AUDIO_H
