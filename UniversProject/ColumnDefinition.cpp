// ColumnDefinition.cpp
#include "ColumnDefinition.h"
#include <sstream>

namespace DataEngine {

    // Constructors
    ColumnDefinition::ColumnDefinition()
        : m_type(DataType::Text)
    {}

    ColumnDefinition::ColumnDefinition(const std::string& name, DataType type, const Data& defaultValue)
        : m_name(name)
        , m_type(type)
        , m_defaultValue(defaultValue)
    {}

    ColumnDefinition::ColumnDefinition(const ColumnDefinition& other)
        : m_name(other.m_name)
        , m_type(other.m_type)
        , m_defaultValue(other.m_defaultValue)
    {}

    ColumnDefinition::ColumnDefinition(ColumnDefinition&& other) noexcept
        : m_name(std::move(other.m_name))
        , m_type(other.m_type)
        , m_defaultValue(std::move(other.m_defaultValue))
    {
        other.m_type = DataType::Text;
    }

    // Assignment operators
    ColumnDefinition& ColumnDefinition::operator=(const ColumnDefinition& other)
    {
        if (this != &other)
        {
            m_name = other.m_name;
            m_type = other.m_type;
            m_defaultValue = other.m_defaultValue;
        }
        return *this;
    }

    ColumnDefinition& ColumnDefinition::operator=(ColumnDefinition&& other) noexcept
    {
        if (this != &other)
        {
            m_name = std::move(other.m_name);
            m_type = other.m_type;
            m_defaultValue = std::move(other.m_defaultValue);
            other.m_type = DataType::Text;
        }
        return *this;
    }

    // Getters
    const std::string& ColumnDefinition::GetName() const
    {
        return m_name;
    }

    DataType ColumnDefinition::GetType() const
    {
        return m_type;
    }

    const Data& ColumnDefinition::GetDefaultValue() const
    {
        return m_defaultValue;
    }

    // Setters
    void ColumnDefinition::SetName(const std::string& name)
    {
        m_name = name;
    }

    void ColumnDefinition::SetType(DataType type)
    {
        m_type = type;
    }

    void ColumnDefinition::SetDefaultValue(const Data& defaultValue)
    {
        m_defaultValue = defaultValue;
    }

    // Apply from Data with priorities using DataCompareInfo
    void ColumnDefinition::ApplyFromData(const Data& data, const ColumnDefinition* existing)
    {
        if (existing)
        {
            Data existingData = existing->CreateData();
            DataCompareInfo cmp = existingData.CompareDataInfo(data);
            
            switch (cmp)
            {
            case DataCompareInfo::FullEqual:
                // Приоритет 1: полное совпадение (тип и значение)
                m_name = existing->GetName();
                m_type = existing->GetType();
                m_defaultValue = existing->GetDefaultValue();
                return;
                
            case DataCompareInfo::OnlyTypeEqual:
                // Приоритет 2: совпадение только по типу
                m_name = existing->GetName();
                m_type = existing->GetType();
                m_defaultValue = existing->GetDefaultValue();
                return;
                
            case DataCompareInfo::OnlyValueEqual:
                // Приоритет 3: совпадение только по значению
                m_name = existing->GetName();
                m_type = data.GetType();
                m_defaultValue = data;
                return;
                
            case DataCompareInfo::None:
                // Приоритет 4: нет совпадений
                break;
            }
        }
        
        // Приоритет 4 (или default): подстановка значений из data
        m_name = "Column_" + std::to_string(static_cast<int>(data.GetType()));
        m_type = data.GetType();
        m_defaultValue = data;
    }

    // Match type only
    bool ColumnDefinition::MatchType(const Data& data) const
    {
        return m_type == data.GetType();
    }

    // Match both type and value
    bool ColumnDefinition::MatchTypeAndValue(const Data& data) const
    {
        return m_type == data.GetType() && 
               m_defaultValue.Compare(data) == CompareResult::Equal;
    }

    // Validate data against this column definition
    Data ColumnDefinition::ValidateData(const Data& data) const
    {
        if (data.IsNull())
            return m_defaultValue;
        
        // Try to convert to the required type
        Data converted = data.GetConvertedTo(m_type);
        if (!converted.IsNull())
            return converted;
        
        return m_defaultValue;
    }

    // Create Data from default value
    Data ColumnDefinition::CreateData() const
    {
        return m_defaultValue;
    }

    // String representation
    std::string ColumnDefinition::ToString() const
    {
        std::ostringstream ss;
        ss << "Column: " << m_name
            << ", Type: " << Data::GetTypeName(m_type)
           << ", Default: " << m_defaultValue.ToString();
        return ss.str();
    }

}