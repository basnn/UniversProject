// Date.cpp - упрощенный
#include "Date.h"

namespace DataEngine {

    Date::Date() : DateTimeBase() {}
    
    Date::Date(const std::chrono::system_clock::time_point& tp)
        : DateTimeBase(tp)
    {
    }

    Date::Date(int year, int month, int day)
        : DateTimeBase(year, month, day, 0, 0, 0, 0)
    {}
    
    std::string Date::ToString() const
    {
        if (!m_valid) return "";
        DateTimeParser dateParser("dd.MM.yyyy");
        dateParser.AddInputFormat("yyyy-MM-dd");
        dateParser.AddInputFormat("MM/dd/yyyy");
        return dateParser.Format(m_year, m_month, m_day, 0, 0, 0, 0);
    }
    
    Date Date::Now()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm* tm = std::localtime(&tt);
        if (tm)
            return Date(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
        return Date();
    }
    
    bool Date::TryParse(const std::string& str, Date& result)
    {
        DateTimeParser dateParser("dd.MM.yyyy");
        dateParser.AddInputFormat("yyyy-MM-dd");
        dateParser.AddInputFormat("MM/dd/yyyy");
        
        int year, month, day, hour, minute, second, millisecond;
        if (dateParser.TryParse(str, year, month, day, hour, minute, second, millisecond))
        {
            result = Date(year, month, day);
            return result.IsValid();
        }
        result = Date();
        return false;
    }
    
    void Date::InitializeDefaultParser() {}

}