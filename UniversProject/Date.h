// Date.h
#pragma once
#include "DateTimeBase.h"

namespace DataEngine {

    class Date : public DateTimeBase
    {
    public:
        Date();
        Date(int year, int month, int day);
        explicit Date(const std::chrono::system_clock::time_point& tp);
        
        std::string ToString() const override;
        
        static Date Now();
        static bool TryParse(const std::string& str, Date& result);
        
        static void InitializeDefaultParser();
    };

}