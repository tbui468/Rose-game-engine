#ifndef INSTRUMENTOR_H
#define INSTRUMENTOR_H

#include <chrono>
#include <thread>

namespace rose {

    struct ProfileResult {
        std::string Name;
        std::chrono::duration<double, std::micro> StartTime;
        std::chrono::microseconds ElapsedTime;
        std::thread::id ThreadID;
    };

    class Instrumentor {
        public:
        private:
    };


    class InstrumentationTimer {
        public:
            InstrumentationTimer(const char* name) : m_Name(name) {
                m_StartTimePoint = std::chrono::steady_clock::now(); 
            }
            virtual ~InstrumentationTimer() {
                Stop();
            }
            void Stop() {
                auto HighResStartTime = std::chrono::duration<double, std::micro>(m_StartTimePoint.time_since_epoch());
                auto endTimePoint = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() -
                    std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch();

//                Instrumentor::Get().WriteProfile({m_Name, HighResStartTime, elapsedTime, std::this_thread::get_id()});
            }
        private:
            std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
            const char* m_Name;
    };


}

#endif //INSTRUMENTOR
