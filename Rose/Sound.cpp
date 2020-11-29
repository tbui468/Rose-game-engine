#include "Sound.h"
#include "Audio.h"
#include "Application.h"

namespace rose {

    
    Sound::Sound(const std::string& pathFromAssets) {
        Application* app = Application::GetApplication();
        m_Audio = app->GetAudio();
        std::string exePath(app->GetExecutablePath());
        (m_Audio->GetID())->createSound((exePath + "../../assets/" + pathFromAssets).c_str(), FMOD_DEFAULT, NULL, &m_SoundID);
    }

    void Sound::Play() {
        (m_Audio->GetID())->playSound(m_SoundID, NULL, false, &m_ChannelID);
    }


    bool Sound::IsPlaying() const {
        //temp: 
        return true;
        //ChannelControl::isPlaying(...) to monitor channel, channel handle will be = FMOD_ERR_INVALID_HANDLE*/
    }


}
