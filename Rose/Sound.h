#ifndef SOUND_H
#define SOUND_H

#include <fmod.h>
#include <fmod.hpp>
#include <string>
#include <memory>

namespace rose {

    class Audio;

    class Sound {
        public:
            Sound(const std::string& pathFromAssets);
            virtual ~Sound() {}
            void Play();
            bool IsPlaying() const;
        private:
            std::shared_ptr<Audio> m_Audio {nullptr};
            FMOD::Sound* m_SoundID {NULL};
            FMOD::Channel* m_ChannelID {NULL};
    };


}


#endif //SOUND_H
