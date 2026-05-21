// Table.h
#pragma once
#include <vector>
#include <string>
#include "DataObject.h"
#include "HeaderDefinition.h"

namespace DataEngine {

    class Table
    {
    protected:
        HeaderDefinition m_header;
        std::string m_name;  // Название таблицы

    public:
        virtual ~Table() = default;

        // Основные операции с объектами
        virtual DataObject& Get(size_t index) = 0;
        virtual const DataObject& Get(size_t index) const = 0;
        virtual size_t Size() const = 0;
        virtual bool IsEmpty() const = 0;
        
        // Добавление объектов
        virtual void Add(const DataObject& obj) = 0;
        virtual void Add(DataObject&& obj) = 0;
        virtual void AddAll(const std::vector<DataObject>& objects) = 0;
        
        // Удаление объектов
        virtual void Remove(size_t index) = 0;
        virtual void RemoveRange(size_t start, size_t end) = 0;
        virtual void Clear() = 0;
        
        // Поиск
        virtual size_t IndexOf(const DataObject& obj) const = 0;
        virtual size_t IndexOf(const DataObject* obj) const = 0;
        
        // Итераторы
        virtual std::vector<DataObject>::iterator begin() = 0;
        virtual std::vector<DataObject>::iterator end() = 0;
        virtual std::vector<DataObject>::const_iterator begin() const = 0;
        virtual std::vector<DataObject>::const_iterator end() const = 0;
        
        // Загрузка/выгрузка
        virtual void Load() = 0;
        virtual void Unload() = 0;
        
        // Работа с Header
        virtual HeaderDefinition& GetHeader();
        virtual const HeaderDefinition& GetHeader() const;
        virtual void SetHeader(const HeaderDefinition& header);
        
        // Работа с названием таблицы
        virtual void SetName(const std::string& name);
        virtual const std::string& GetName() const;
        
        // Валидация
        virtual DataObject ValidateWithDefault(const DataObject& obj) const = 0;
        
        // Получение количества объектов в источнике
        virtual size_t GetTotalCount() const = 0;
        
    protected:
        Table() = default;
    };

}