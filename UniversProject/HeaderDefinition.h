// HeaderDefinition.h
#pragma once
#include <vector>
#include <string>
#include "DataObject.h"
#include "ColumnDefinition.h"

namespace DataEngine {

    class HeaderDefinition
    {
    private:
        std::vector<ColumnDefinition> m_columns;

    public:
        HeaderDefinition() = default;
        explicit HeaderDefinition(const std::vector<ColumnDefinition>& columns);
        ~HeaderDefinition() = default;

        HeaderDefinition(const HeaderDefinition& other);
        HeaderDefinition(HeaderDefinition&& other) noexcept;
        HeaderDefinition& operator=(const HeaderDefinition& other);
        HeaderDefinition& operator=(HeaderDefinition&& other) noexcept;

        // Доступ к колонкам
        std::vector<ColumnDefinition>& GetColumns();
        const std::vector<ColumnDefinition>& GetColumns() const;
        void SetColumns(const std::vector<ColumnDefinition>& columns);
        
        ColumnDefinition& GetColumn(size_t index);
        const ColumnDefinition& GetColumn(size_t index) const;
        
        void AddColumn(const ColumnDefinition& column);
        void AddColumn(const std::string& name, DataType type, const Data& defaultValue = Data());
        void RemoveColumn(size_t index);
        void UpdateColumn(size_t index, const ColumnDefinition& column);
        
        size_t Size() const;
        bool IsEmpty() const;
        void Clear();

        // Применение из DataObject (сравнение и подстановка)
        void ApplyFromDataObject(const DataObject& sample);
        
        // Получение DataObject по образцу
        DataObject GetDefaultObject() const;
        
        // Валидация DataObject
        DataObject ValidateDataObject(const DataObject& obj) const;
        
        // Проверка соответствия типов
        bool HasSameTypes(const DataObject& obj) const;
        
        // Поиск колонки по имени
        int FindColumnByName(const std::string& name) const;
        
        std::string ToString() const;

    private:
        void ValidateIndex(size_t index) const;
    };

}