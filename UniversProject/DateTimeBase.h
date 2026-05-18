// DateTimeBase.h
#pragma once
#include <string>
#include<chrono>
#include "DateTimeFormat.h"

namespace DataEngine {

    class DateTimeBase
    {
    protected:
        int m_year;
        int m_month;
        int m_day;
        int m_hour;
        int m_minute;
        int m_second;
        int m_millisecond;
        bool m_valid;

        static DateTimeParser s_parser;

    public:
        DateTimeBase();
        DateTimeBase(int year, int month, int day, int hour, int minute, int second, int millisecond);
        explicit DateTimeBase(const std::chrono::system_clock::time_point& tp);
        virtual ~DateTimeBase() = default;

        bool IsValid() const { return m_valid; }
        
        int GetYear() const { return m_year; }
        int GetMonth() const { return m_month; }
        int GetDay() const { return m_day; }
        int GetHour() const { return m_hour; }
        int GetMinute() const { return m_minute; }
        int GetSecond() const { return m_second; }
        int GetMillisecond() const { return m_millisecond; }
        
        int Compare(const DateTimeBase& other) const;
        bool operator==(const DateTimeBase& other) const;
        bool operator!=(const DateTimeBase& other) const;
        bool operator<(const DateTimeBase& other) const;
        bool operator<=(const DateTimeBase& other) const;
        bool operator>(const DateTimeBase& other) const;
        bool operator>=(const DateTimeBase& other) const;
        
        virtual std::string ToString() const;
        
        static DateTimeParser& GetParser();
        static void SetParser(const DateTimeParser& parser);
        static void InitializeDefaultParser();

    protected:
        bool ValidateDate(int year, int month, int day);
        bool ValidateTime(int hour, int minute, int second, int millisecond);
        long long ToTimestamp() const;
    };

}