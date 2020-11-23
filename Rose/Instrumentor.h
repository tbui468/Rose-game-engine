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
                m_StartTimePoint = std::chrono::high_resolution_clock::now(); 
            }
            virtual ~InstrumentationTimer() {
                Stop();
            }
            void Stop() {
                auto endTimePoint = std::chrono::high_resolution_clock::now();
                auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
                auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
                auto duration = end - start;
                double ms = duration * 0.001;

                std::cout << m_Name << ": " << ms << std::endl;

//                Instrumentor::Get().WriteProfile({m_Name, HighResStartTime, elapsedTime, std::this_thread::get_id()});
            }
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
            const char* m_Name;
    };


}

#endif //INSTRUMENTOR
