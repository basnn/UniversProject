// Time.h
#pragma once
#include "DateTimeBase.h"

namespace DataEngine {

    class Time : public DateTimeBase
    {
    public:
        Time();
        Time(int hour, int minute, int second = 0, int millisecond = 0);
        Time(int year, int month, int day, int hour, int minute, int second = 0, int millisecond = 0);
        explicit Time(const std::chrono::system_clock::time_point& tp);
        
        std::string ToString() const override;
        
        static Time Now();
        static bool TryParse(const std::string& str, Time& result);
        
        static void InitializeDefaultParser();
    };

}