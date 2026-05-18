// Data.cpp
#include "Data.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

namespace DataEngine {

    Data::Data()
        : m_value(std::monostate{})
        , m_type(DataType::None)
    {}

    Data::Data(long long value)
        : m_value(value)
        , m_type(DataType::Integer)
    {}

    Data::Data(unsigned long long value)
        : m_value(value)
        , m_type(DataType::UInteger)
    {}

    Data::Data(double value)
        : m_value(value)
        , m_type(DataType::Double)
    {}

    Data::Data(const char* value)
        : m_value(std::string(value))
        , m_type(DataType::Text)
    {}

    Data::Data(const std::string& value)
        : m_value(value)
        , m_type(DataType::Text)
    {}

    Data::Data(const Date& value)
        : m_value(value)
        , m_type(DataType::Date)
    {}

    Data::Data(const Time& value)
        : m_value(value)
        , m_type(DataType::DateTime)
    {}

    Data::Data(const Data& other)
        : m_value(other.m_value)
        , m_type(other.m_type)
    {}

    Data::Data(Data&& other) noexcept
        : m_value(std::move(other.m_value))
        , m_type(other.m_type)
    {
        other.m_type = DataType::None;
        other.m_value = std::monostate{};
    }

    Data& Data::operator=(const Data& other)
    {
        if (this != &other)
        {
            m_value = other.m_value;
            m_type = other.m_type;
        }
        return *this;
    }

    Data& Data::operator=(Data&& other) noexcept
    {
        if (this != &other)
        {
            m_value = std::move(other.m_value);
            m_type = other.m_type;
            other.m_type = DataType::None;
            other.m_value = std::monostate{};
        }
        return *this;
    }

    DataType Data::GetType() const
    {
        return m_type;
    }

    const char* Data::GetTypeName(DataType type)
    {
        switch (type)
        {
        case DataType::None:     return "None";
        case DataType::Integer:  return "Integer";
        case DataType::UInteger: return "UInteger";
        case DataType::Double:   return "Double";
        case DataType::Text:     return "Text";
        case DataType::Date:     return "Date";
        case DataType::DateTime: return "DateTime";
        default:                 return "Unknown";
        }
    }

    const char* Data::GetTypeName() const
    {
        return GetTypeName(m_type);
    }

    long long Data::AsInteger() const
    {
        if (m_type == DataType::Integer)
            return std::get<long long>(m_value);
        if (m_type == DataType::UInteger)
            return static_cast<long long>(std::get<unsigned long long>(m_value));
        if (m_type == DataType::Double)
            return static_cast<long long>(std::get<double>(m_value));
        return 0;
    }

    unsigned long long Data::AsUInteger() const
    {
        if (m_type == DataType::UInteger)
            return std::get<unsigned long long>(m_value);
        if (m_type == DataType::Integer)
            return static_cast<unsigned long long>(std::get<long long>(m_value));
        if (m_type == DataType::Double)
            return static_cast<unsigned long long>(std::get<double>(m_value));
        return 0;
    }

    double Data::AsDouble() const
    {
        if (m_type == DataType::Double)
            return std::get<double>(m_value);
        if (m_type == DataType::Integer)
            return static_cast<double>(std::get<long long>(m_value));
        if (m_type == DataType::UInteger)
            return static_cast<double>(std::get<unsigned long long>(m_value));
        return 0.0;
    }

    std::string Data::AsText() const
    {
        if (m_type == DataType::Text)
            return std::get<std::string>(m_value);
        return ToString();
    }

    Date Data::AsDate() const
    {
        if (m_type == DataType::Date)
            return std::get<Date>(m_value);
        return Date();
    }

    Time Data::AsDateTime() const
    {
        if (m_type == DataType::DateTime)
            return std::get<Time>(m_value);
        return Time();
    }

    bool Data::IsNull() const { return m_type == DataType::None; }
    bool Data::IsInteger() const { return m_type == DataType::Integer; }
    bool Data::IsUInteger() const { return m_type == DataType::UInteger; }
    bool Data::IsDouble() const { return m_type == DataType::Double; }
    bool Data::IsNumber() const
    {
        return m_type == DataType::Integer || 
               m_type == DataType::UInteger || 
               m_type == DataType::Double;
    }
    bool Data::IsText() const { return m_type == DataType::Text; }
    bool Data::IsDate() const { return m_type == DataType::Date; }
    bool Data::IsDateTime() const { return m_type == DataType::DateTime; }

    void Data::ConvertTo(DataType newType)
    {
        if (m_type == newType) return;
        
        Data converted = GetConvertedTo(newType);
        if (!converted.IsNull())
        {
            *this = std::move(converted);
        }
    }

    Data Data::GetConvertedTo(DataType newType) const
    {
        if (m_type == newType)
            return *this;

        switch (newType)
        {
        case DataType::Integer:
            if (IsNumber()) return Data(AsInteger());
            if (IsText())
            {
                try { return Data((long long)std::stoll(AsText())); }
                catch (...) { return Data(); }
            }
            return Data();
            
        case DataType::UInteger:
            if (IsNumber()) return Data(AsUInteger());
            if (IsText())
            {
                try { return Data((unsigned long long)std::stoull(AsText())); }
                catch (...) { return Data(); }
            }
            return Data();
            
        case DataType::Double:
            if (IsNumber()) return Data(AsDouble());
            if (IsText())
            {
                try { return Data(std::stod(AsText())); }
                catch (...) { return Data(); }
            }
            return Data();
            
        case DataType::Text:
            return Data(ToString());
            
        case DataType::Date:
        {
            if (IsDate()) return Data(AsDate());
            if (IsDateTime()) return Data(AsDateTime());
            if (IsText())
            {
                Date d;
                if (Date::TryParse(AsText(), d))
                    return Data(d);
            }
            return Data();
        }
        
        case DataType::DateTime:
        {
            if (IsDateTime()) return Data(AsDateTime());
            if (IsDate())
            {
                Date d = AsDate();
                return Data(Time(d.GetYear(), d.GetMonth(), d.GetDay(), 0, 0, 0));
            }
            if (IsText())
            {
                Time t;
                if (Time::TryParse(AsText(), t))
                    return Data(t);
            }
            return Data();
        }
        
        default:
            return Data();
        }
    }

    Data Data::To(DataType newType) const
    {
        return GetConvertedTo(newType);
    }

    CompareResult Data::Compare(const Data& other) const
    {
        if (IsNumber() && other.IsNumber())
        {
            double a = AsDouble();
            double b = other.AsDouble();
            if (a < b) return CompareResult::Less;
            if (a > b) return CompareResult::Greater;
            return CompareResult::Equal;
        }
        
        if (IsText() && other.IsText())
        {
            int rez = AsText().compare(other.AsText());
            if (rez < 0) return CompareResult::Less;
            if (rez > 0) return CompareResult::Greater;
            return CompareResult::Equal;
        }

        if (IsDate() && other.IsDate())
        {
            Date a = AsDate();
            Date b = other.AsDate();
            if (a < b) return CompareResult::Less;
            if (a > b) return CompareResult::Greater;
            return CompareResult::Equal;
        }

        if (IsDateTime() && other.IsDateTime())
        {
            Time a = AsDateTime();
            Time b = other.AsDateTime();
            if (a < b) return CompareResult::Less;
            if (a > b) return CompareResult::Greater;
            return CompareResult::Equal;
        }

        return CompareResult::Invalid;
    }

    DataCompareInfo Data::CompareDataInfo(const Data& other) const
    {
        CompareResult cmp = Compare(other);
        
        if (cmp == CompareResult::Equal)
            return DataCompareInfo::FullEqual;
        
        if (m_type == other.m_type)
            return DataCompareInfo::OnlyTypeEqual;
        
        if (ToString() == other.ToString())
            return DataCompareInfo::OnlyValueEqual;
        
        return DataCompareInfo::None;
    }

    bool Data::operator==(const Data& other) const { return Compare(other) == CompareResult::Equal; }
    bool Data::operator!=(const Data& other) const { return Compare(other) != CompareResult::Equal; }
    bool Data::operator<(const Data& other) const { return Compare(other) == CompareResult::Less; }
    
    bool Data::operator<=(const Data& other) const
    {
        CompareResult rez = Compare(other);
        return rez == CompareResult::Less || rez == CompareResult::Equal;
    }
    
    bool Data::operator>(const Data& other) const { return Compare(other) == CompareResult::Greater; }
    
    bool Data::operator>=(const Data& other) const
    {
        CompareResult rez = Compare(other);
        return rez == CompareResult::Greater || rez == CompareResult::Equal;
    }

    Data Data::Add(const Data& other) const
    {
        if (IsNumber() && other.IsNumber())
            return Data(AsDouble() + other.AsDouble());
        if (IsText() && other.IsText())
            return Data(AsText() + other.AsText());
        return Data();
    }

    Data Data::Subtract(const Data& other) const
    {
        if (IsNumber() && other.IsNumber())
            return Data(AsDouble() - other.AsDouble());
        return Data();
    }

    Data Data::Multiply(const Data& other) const
    {
        if (IsNumber() && other.IsNumber())
            return Data(AsDouble() * other.AsDouble());
        return Data();
    }

    Data Data::Divide(const Data& other) const
    {
        if (IsNumber() && other.IsNumber() && other.AsDouble() != 0)
            return Data(AsDouble() / other.AsDouble());
        return Data();
    }

    std::string Data::ToString() const
    {
        switch (m_type)
        {
        case DataType::None:
            return "";
        case DataType::Integer:
            return std::to_string(std::get<long long>(m_value));
        case DataType::UInteger:
            return std::to_string(std::get<unsigned long long>(m_value));
        case DataType::Double:
        {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << std::get<double>(m_value);
            return ss.str();
        }
        case DataType::Text:
            return std::get<std::string>(m_value);
        case DataType::Date:
            return std::get<Date>(m_value).ToString();
        case DataType::DateTime:
            return std::get<Time>(m_value).ToString();
        }
        return "";
    }

}