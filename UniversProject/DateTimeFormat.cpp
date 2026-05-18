// DateTimeFormat.cpp
#include "DateTimeFormat.h"
#include <algorithm>
#include <map>

namespace DataEngine {

    // ============ DateTimeFormat ============

    DateTimeFormat::DateTimeFormat(const std::string& pattern)
        : m_pattern(pattern)
    {
        Compile();
    }

    void DateTimeFormat::Compile()
    {
        m_tokens.clear();
        m_regexPattern.clear();
        
        std::string regexStr;
        size_t pos = 0;
        
        while (pos < m_pattern.length())
        {
            if (isalpha(m_pattern[pos]))
            {
                char codeChar = m_pattern[pos];
                size_t start = pos;
                while (pos < m_pattern.length() && m_pattern[pos] == codeChar)
                    pos++;
                
                std::string code = m_pattern.substr(start, pos - start);
                int digitCount = static_cast<int>(code.length());
                
                DateTimeToken token;
                token.code = code;
                token.startPos = start;
                token.length = digitCount;
                m_tokens.push_back(token);
                
                regexStr += "(\\d{" + std::to_string(digitCount) + "})";
            }
            else
            {
                regexStr += m_pattern[pos];
                pos++;
            }
        }
        
        m_regexPattern = regexStr;
        if (!regexStr.empty())
            m_regex = std::regex(regexStr);
    }

    bool DateTimeFormat::Parse(const std::string& input, std::vector<int>& values) const
    {
        if (m_regexPattern.empty())
            return false;
        
        std::smatch match;
        if (!std::regex_match(input, match, m_regex))
            return false;
        
        values.clear();
        for (size_t i = 1; i < match.size(); i++)
        {
            try
            {
                values.push_back(std::stoi(match[i].str()));
            }
            catch (...)
            {
                return false;
            }
        }
        
        return values.size() == m_tokens.size();
    }

    std::string DateTimeFormat::Format(const std::vector<int>& values) const
    {
        if (values.size() != m_tokens.size() || m_pattern.empty())
            return "";
        
        std::string result = m_pattern;
        
        for (size_t i = 0; i < m_tokens.size(); i++)
        {
            const auto& token = m_tokens[i];
            std::string valueStr = std::to_string(values[i]);
            
            if (valueStr.length() < token.length)
                valueStr = std::string(token.length - valueStr.length(), '0') + valueStr;
            
            result.replace(token.startPos, token.code.length(), valueStr);
        }
        
        return result;
    }

    // ============ DateTimeParser ============

    DateTimeParser::DateTimeParser(const std::string& defaultPattern)
    {
        SetDefaultFormat(defaultPattern);
    }

    void DateTimeParser::SetDefaultFormat(const std::string& pattern)
    {
        m_defaultFormat = DateTimeFormat(pattern);
    }

    std::string DateTimeParser::GetDefaultPattern() const
    {
        return m_defaultFormat.GetPattern();
    }

    void DateTimeParser::AddInputFormat(const std::string& pattern)
    {
        m_inputFormats.emplace_back(pattern);
    }

    void DateTimeParser::ClearInputFormats()
    {
        m_inputFormats.clear();
    }

    const std::vector<DateTimeFormat>& DateTimeParser::GetInputFormats() const
    {
        return m_inputFormats;
    }

    bool DateTimeParser::MapTokenValues(const DateTimeFormat& format,
                                        const std::vector<int>& parsedValues,
                                        int& year, int& month, int& day,
                                        int& hour, int& minute, int& second, int& millisecond)
    {
        if (parsedValues.size() != format.GetTokens().size())
            return false;
        
        const auto& tokens = format.GetTokens();
        
        for (size_t i = 0; i < tokens.size(); i++)
        {
            const std::string& code = tokens[i].code;
            int value = parsedValues[i];
            
            if (code.find('y') != std::string::npos)
            {
                year = value;
            }
            else if (code.find('M') != std::string::npos)
            {
                month = value;
            }
            else if (code.find('d') != std::string::npos)
            {
                day = value;
            }
            else if (code.find('h') != std::string::npos)
            {
                hour = value;
            }
            else if (code.find('m') != std::string::npos)
            {
                minute = value;
            }
            else if (code.find('s') != std::string::npos)
            {
                second = value;
            }
            else if (code.find('q') != std::string::npos)
            {
                millisecond = value;
            }
        }
        
        return true;
    }

    bool DateTimeParser::TryParse(const std::string& input,
                                  int& year, int& month, int& day,
                                  int& hour, int& minute, int& second, int& millisecond) const
    {
        year = month = day = hour = minute = second = millisecond = 0;
        
        std::vector<int> parsedValues;
        
        if (m_defaultFormat.IsValid() && m_defaultFormat.Parse(input, parsedValues))
        {
            return MapTokenValues(m_defaultFormat, parsedValues, 
                                   year, month, day, hour, minute, second, millisecond);
        }
        
        for (const auto& format : m_inputFormats)
        {
            if (format.Parse(input, parsedValues))
            {
                return MapTokenValues(format, parsedValues,
                                       year, month, day, hour, minute, second, millisecond);
            }
        }
        
        return false;
    }

    std::string DateTimeParser::Format(int year, int month, int day,
                                        int hour, int minute, int second, int millisecond) const
    {
        if (!m_defaultFormat.IsValid())
            return "";
        
        const auto& tokens = m_defaultFormat.GetTokens();
        std::vector<int> values;
        
        for (const auto& token : tokens)
        {
            const std::string& code = token.code;
            
            if (code.find('y') != std::string::npos)
                values.push_back(year);
            else if (code.find('M') != std::string::npos)
                values.push_back(month);
            else if (code.find('d') != std::string::npos)
                values.push_back(day);
            else if (code.find('h') != std::string::npos)
                values.push_back(hour);
            else if (code.find('m') != std::string::npos)
                values.push_back(minute);
            else if (code.find('s') != std::string::npos)
                values.push_back(second);
            else if (code.find('q') != std::string::npos)
                values.push_back(millisecond);
            else
                values.push_back(0);
        }
        
        return m_defaultFormat.Format(values);
    }

}