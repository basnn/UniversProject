// DateTimeFormat.h
#pragma once
#include <string>
#include <vector>
#include <regex>

namespace DataEngine {

    struct DateTimeToken
    {
        std::string code;
        size_t startPos;
        size_t length;
        int value;
    };

    class DateTimeFormat
    {
    private:
        std::string m_pattern;
        std::string m_regexPattern;
        std::vector<DateTimeToken> m_tokens;
        std::regex m_regex;

    public:
        DateTimeFormat() = default;
        explicit DateTimeFormat(const std::string& pattern);
        
        void Compile();
        
        const std::string& GetPattern() const { return m_pattern; }
        const std::regex& GetRegex() const { return m_regex; }
        const std::vector<DateTimeToken>& GetTokens() const { return m_tokens; }
        
        bool Parse(const std::string& input, std::vector<int>& values) const;
        std::string Format(const std::vector<int>& values) const;
        
        bool IsValid() const { return !m_regexPattern.empty(); }
    };

    class DateTimeParser
    {
    private:
        DateTimeFormat m_defaultFormat;
        std::vector<DateTimeFormat> m_inputFormats;

    public:
        DateTimeParser() = default;
        explicit DateTimeParser(const std::string& defaultPattern);
        
        // Настройка дефолтного формата (для вывода и приоритетного ввода)
        void SetDefaultFormat(const std::string& pattern);
        std::string GetDefaultPattern() const;
        
        // Добавление форматов для ввода
        void AddInputFormat(const std::string& pattern);
        void ClearInputFormats();
        const std::vector<DateTimeFormat>& GetInputFormats() const;
        
        // Парсинг строки в дату/время
        bool TryParse(const std::string& input, 
                      int& year, int& month, int& day,
                      int& hour, int& minute, int& second, int& millisecond) const;
        
        // Форматирование даты/времени в строку
        std::string Format(int year, int month, int day,
                           int hour, int minute, int second, int millisecond) const;
        
        // Вспомогательные методы
        static bool MapTokenValues(const DateTimeFormat& format, 
                                    const std::vector<int>& parsedValues,
                                    int& year, int& month, int& day,
                                    int& hour, int& minute, int& second, int& millisecond);
    };

}