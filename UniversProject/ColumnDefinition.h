// ColumnDefinition.h
#pragma once
#include <string>
#include "Data.h"

namespace DataEngine {

    class ColumnDefinition
    {
    private:
        std::string m_name;
        DataType m_type;
        Data m_defaultValue;

    public:
        ColumnDefinition();
        ColumnDefinition(const std::string& name, DataType type, const Data& defaultValue = Data());
        ColumnDefinition(const ColumnDefinition& other);
        ColumnDefinition(ColumnDefinition&& other) noexcept;
        ~ColumnDefinition() = default;

        ColumnDefinition& operator=(const ColumnDefinition& other);
        ColumnDefinition& operator=(ColumnDefinition&& other) noexcept;

        // Getters
        const std::string& GetName() const;
        DataType GetType() const;
        const Data& GetDefaultValue() const;

        // Setters
        void SetName(const std::string& name);
        void SetType(DataType type);
        void SetDefaultValue(const Data& defaultValue);

        // Применение из Data с приоритетами (использует DataCompareInfo из Data)
        void ApplyFromData(const Data& data, const ColumnDefinition* existing = nullptr);
        
        // Сравнение
        bool MatchType(const Data& data) const;
        bool MatchTypeAndValue(const Data& data) const;
        
        // Валидация данных
        Data ValidateData(const Data& data) const;
        
        // Создание Data из значения по умолчанию
        Data CreateData() const;
        
        // Строковое представление
        std::string ToString() const;
    };

}