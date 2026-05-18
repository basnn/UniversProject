// DataObject.cpp
#include "DataObject.h"
#include <stdexcept>
#include <algorithm>

namespace DataEngine {

    DataObject::DataObject(size_t size)
        : m_data(size)
    {}

    DataObject::DataObject(const DataObject& other)
        : m_data(other.m_data)
    {}

    DataObject::DataObject(DataObject&& other) noexcept
        : m_data(std::move(other.m_data))
    {}

    DataObject& DataObject::operator=(const DataObject& other)
    {
        if (this != &other)
        {
            m_data = other.m_data;
        }
        return *this;
    }

    DataObject& DataObject::operator=(DataObject&& other) noexcept
    {
        if (this != &other)
        {
            m_data = std::move(other.m_data);
        }
        return *this;
    }

    void DataObject::Set(int index, const Data& value)
    {
        ValidateIndex(index);
        m_data[index] = value;
    }

    void DataObject::Set(int index, Data&& value)
    {
        ValidateIndex(index);
        m_data[index] = std::move(value);
    }

    const Data& DataObject::Get(int index) const
    {
        ValidateIndex(index);
        return m_data[index];
    }

    Data& DataObject::Get(int index)
    {
        ValidateIndex(index);
        return m_data[index];
    }

    size_t DataObject::Size() const
    {
        return m_data.size();
    }

    void DataObject::Resize(size_t size)
    {
        m_data.resize(size);
    }

    void DataObject::Clear()
    {
        m_data.clear();
    }

    void DataObject::PushBack(const Data& value)
    {
        m_data.push_back(value);
    }

    void DataObject::PushBack(Data&& value)
    {
        m_data.push_back(std::move(value));
    }

    DataCompareInfo DataObject::Compare(const DataObject& other) const
    {
        size_t minSize = std::min(m_data.size(), other.m_data.size());
        
        bool fullEqual = true;
        bool onlyTypeEqual = true;
        bool onlyValueEqual = true;
        
        for (size_t i = 0; i < minSize; i++)
        {
            DataCompareInfo cmp = m_data[i].CompareDataInfo(other.m_data[i]);
            
            if (cmp != DataCompareInfo::FullEqual)
            {
                fullEqual = false;
                
                if (cmp != DataCompareInfo::OnlyTypeEqual)
                    onlyTypeEqual = false;
                
                if (cmp != DataCompareInfo::OnlyValueEqual)
                    onlyValueEqual = false;
            }
        }
        
        // Если размеры разные, это не полное совпадение
        if (m_data.size() != other.m_data.size())
        {
            fullEqual = false;
            onlyTypeEqual = false;
            onlyValueEqual = false;
        }
        
        if (fullEqual)
            return DataCompareInfo::FullEqual;
        
        if (onlyTypeEqual)
            return DataCompareInfo::OnlyTypeEqual;
        
        if (onlyValueEqual)
            return DataCompareInfo::OnlyValueEqual;
        
        return DataCompareInfo::None;
    }

    DataCompareInfo DataObject::CompareAt(int index, const Data& value) const
    {
        ValidateIndex(index);
        return m_data[index].CompareDataInfo(value);
    }

    bool DataObject::HasSameTypes(const DataObject& other) const
    {
        if (m_data.size() != other.m_data.size())
            return false;
        
        for (size_t i = 0; i < m_data.size(); i++)
        {
            if (m_data[i].GetType() != other.m_data[i].GetType())
                return false;
        }
        
        return true;
    }

    bool DataObject::HasSameValues(const DataObject& other) const
    {
        if (m_data.size() != other.m_data.size())
            return false;
        
        for (size_t i = 0; i < m_data.size(); i++)
        {
            if (m_data[i].ToString() != other.m_data[i].ToString())
                return false;
        }
        
        return true;
    }

    Data DataObject::AsData() const
    {
        if (m_data.empty())
            return Data();
        return m_data[0];
    }

    void DataObject::ValidateIndex(int index) const
    {
        if (index < 0 || index >= static_cast<int>(m_data.size()))
        {
            throw std::out_of_range("Index out of range");
        }
    }

}