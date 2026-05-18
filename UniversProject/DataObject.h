// DataObject.h
#pragma once
#include <vector>
#include "Data.h"

namespace DataEngine {

    class DataObject
    {
    private:
        std::vector<Data> m_data;

    public:
        DataObject() = default;
        explicit DataObject(size_t size);
        DataObject(const DataObject& other);
        DataObject(DataObject&& other) noexcept;
        ~DataObject() = default;

        DataObject& operator=(const DataObject& other);
        DataObject& operator=(DataObject&& other) noexcept;

        void Set(int index, const Data& value);
        void Set(int index, Data&& value);
        
        const Data& Get(int index) const;
        Data& Get(int index);

        size_t Size() const;
        void Resize(size_t size);
        void Clear();

        void PushBack(const Data& value);
        void PushBack(Data&& value);

        // Сравнение с другим DataObject
        DataCompareInfo Compare(const DataObject& other) const;
        
        // Сравнение по конкретному индексу с Data
        DataCompareInfo CompareAt(int index, const Data& value) const;
        
        // Проверка соответствия типам
        bool HasSameTypes(const DataObject& other) const;
        
        // Проверка соответствия значениям (без учета типа)
        bool HasSameValues(const DataObject& other) const;
        
        // Получение как Data (для случаев с одной колонкой)
        Data AsData() const;

    private:
        void ValidateIndex(int index) const;
    };

}