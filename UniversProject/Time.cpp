// Time.cpp - упрощенный
#include "Time.h"

namespace DataEngine {

    Time::Time() : DateTimeBase() {}
    
    Time::Time(int hour, int minute, int second, int millisecond)
        : DateTimeBase(1970, 1, 1, hour, minute, second, millisecond)
    {}
    Time::Time(const std::chrono::system_clock::time_point& tp)
        : DateTimeBase(tp)
    {
    }

    Time::Time(int year, int month, int day, int hour, int minute, int second, int millisecond)
        : DateTimeBase(year, month, day, hour, minute, second, millisecond)
    {}
    
    std::string Time::ToString() const
    {
        if (!m_valid) return "";
        
        // Если дата не является базовой (1970-01-01), выводим полную дату+время
        if (m_year != 1970 || m_month != 1 || m_day != 1)
        {
            DateTimeParser fullParser("dd.MM.yyyy hh:mm:ss");
            return fullParser.Format(m_year, m_month, m_day, m_hour, m_minute, m_second, 0);
        }
        else
        {
            // Только время
            DateTimeParser timeOnlyParser("hh:mm:ss");
            return timeOnlyParser.Format(1970, 1, 1, m_hour, m_minute, m_second, 0);
        }
    }
    
    Time Time::Now()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&tt);
        if (tm)
            return Time(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, 
                        tm->tm_hour, tm->tm_min, tm->tm_sec, 0);
        return Time();
    }
    
    bool Time::TryParse(const std::string& str, Time& result)
    {
        DateTimeParser fullParser("dd.MM.yyyy hh:mm:ss");
        fullParser.AddInputFormat("yyyy-MM-dd hh:mm:ss");
        fullParser.AddInputFormat("dd/MM/yyyy hh:mm:ss");
        fullParser.AddInputFormat("MM/dd/yyyy hh:mm:ss");
        
        int year, month, day, hour, minute, second, millisecond;
        if (fullParser.TryParse(str, year, month, day, hour, minute, second, millisecond))
        {
            result = Time(year, month, day, hour, minute, second, millisecond);
            return result.IsValid();
        }
        
        DateTimeParser timeOnlyParser("hh:mm:ss");
        if (timeOnlyParser.TryParse(str, year, month, day, hour, minute, second, millisecond))
        {
            result = Time(hour, minute, second, millisecond);
            return result.IsValid();
        }
        
        result = Time();
        return false;
    }
    
    void Time::InitializeDefaultParser() {}

}