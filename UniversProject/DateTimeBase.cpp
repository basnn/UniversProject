// DateTimeBase.cpp
#include "DateTimeBase.h"
#include <ctime>
#include <chrono>

namespace DataEngine {

    DateTimeParser DateTimeBase::s_parser("dd.MM.yyyy hh:mm:ss");

    DateTimeBase::DateTimeBase()
        : m_year(0), m_month(0), m_day(0)
        , m_hour(0), m_minute(0), m_second(0), m_millisecond(0)
        , m_valid(false)
    {}

    DateTimeBase::DateTimeBase(int year, int month, int day, int hour, int minute, int second, int millisecond)
        : m_year(year), m_month(month), m_day(day)
        , m_hour(hour), m_minute(minute), m_second(second), m_millisecond(millisecond)
    {
        m_valid = ValidateDate(year, month, day) && ValidateTime(hour, minute, second, millisecond);
    }

    DateTimeBase::DateTimeBase(const std::chrono::system_clock::time_point& tp)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(tp);
        std::tm* tm = std::localtime(&tt);
        if (tm)
        {
            m_year = tm->tm_year + 1900;
            m_month = tm->tm_mon + 1;
            m_day = tm->tm_mday;
            m_hour = tm->tm_hour;
            m_minute = tm->tm_min;
            m_second = tm->tm_sec;
            m_millisecond = 0;
            m_valid = ValidateDate(m_year, m_month, m_day) && ValidateTime(m_hour, m_minute, m_second, m_millisecond);
        }
        else
        {
            m_valid = false;
        }
    }







    bool DateTimeBase::ValidateDate(int year, int month, int day)
    {
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;
        
        // Проверка количества дней в месяце
        static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        int maxDay = daysInMonth[month - 1];
        
        // Високосный год
        if (month == 2)
        {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (isLeap) maxDay = 29;
        }
        
        return day <= maxDay;
    }

    bool DateTimeBase::ValidateTime(int hour, int minute, int second, int millisecond)
    {
        return hour >= 0 && hour <= 23 &&
               minute >= 0 && minute <= 59 &&
               second >= 0 && second <= 59 &&
               millisecond >= 0 && millisecond <= 999;
    }

    long long DateTimeBase::ToTimestamp() const
    {
        if (!m_valid) return 0;
        
        std::tm tm = {};
        tm.tm_year = m_year - 1900;
        tm.tm_mon = m_month - 1;
        tm.tm_mday = m_day;
        tm.tm_hour = m_hour;
        tm.tm_min = m_minute;
        tm.tm_sec = m_second;
        
        return std::mktime(&tm);
    }

    int DateTimeBase::Compare(const DateTimeBase& other) const
    {
        if (!m_valid && !other.m_valid) return 0;
        if (!m_valid) return -1;
        if (!other.m_valid) return 1;
        
        long long thisTs = ToTimestamp();
        long long otherTs = other.ToTimestamp();
        
        if (thisTs < otherTs) return -1;
        if (thisTs > otherTs) return 1;
        
        // Сравниваем миллисекунды
        if (m_millisecond < other.m_millisecond) return -1;
        if (m_millisecond > other.m_millisecond) return 1;
        
        return 0;
    }

    bool DateTimeBase::operator==(const DateTimeBase& other) const { return Compare(other) == 0; }
    bool DateTimeBase::operator!=(const DateTimeBase& other) const { return Compare(other) != 0; }
    bool DateTimeBase::operator<(const DateTimeBase& other) const { return Compare(other) < 0; }
    bool DateTimeBase::operator<=(const DateTimeBase& other) const { return Compare(other) <= 0; }
    bool DateTimeBase::operator>(const DateTimeBase& other) const { return Compare(other) > 0; }
    bool DateTimeBase::operator>=(const DateTimeBase& other) const { return Compare(other) >= 0; }

    std::string DateTimeBase::ToString() const
    {
        if (!m_valid) return "";
        return s_parser.Format(m_year, m_month, m_day, m_hour, m_minute, m_second, m_millisecond);
    }

    DateTimeParser& DateTimeBase::GetParser()
    {
        return s_parser;
    }

    void DateTimeBase::SetParser(const DateTimeParser& parser)
    {
        s_parser = parser;
    }

    void DateTimeBase::InitializeDefaultParser()
    {
        s_parser.AddInputFormat("yyyy-MM-dd hh:mm:ss");
        s_parser.AddInputFormat("dd.MM.yyyy hh:mm:ss");
        s_parser.AddInputFormat("dd/MM/yyyy hh:mm:ss");
        s_parser.AddInputFormat("MM/dd/yyyy hh:mm:ss");
        s_parser.AddInputFormat("hh:mm:ss");
        s_parser.AddInputFormat("dd.MM.yyyy");
        s_parser.AddInputFormat("yyyy-MM-dd");
        s_parser.AddInputFormat("MM/dd/yyyy");
    }

}