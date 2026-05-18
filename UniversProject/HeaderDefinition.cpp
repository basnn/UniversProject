// HeaderDefinition.cpp
#include "HeaderDefinition.h"
#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace DataEngine {

    HeaderDefinition::HeaderDefinition(const std::vector<ColumnDefinition>& columns)
        : m_columns(columns)
    {}

    HeaderDefinition::HeaderDefinition(const HeaderDefinition& other)
        : m_columns(other.m_columns)
    {}

    HeaderDefinition::HeaderDefinition(HeaderDefinition&& other) noexcept
        : m_columns(std::move(other.m_columns))
    {}

    HeaderDefinition& HeaderDefinition::operator=(const HeaderDefinition& other)
    {
        if (this != &other)
        {
            m_columns = other.m_columns;
        }
        return *this;
    }

    HeaderDefinition& HeaderDefinition::operator=(HeaderDefinition&& other) noexcept
    {
        if (this != &other)
        {
            m_columns = std::move(other.m_columns);
        }
        return *this;
    }

    std::vector<ColumnDefinition>& HeaderDefinition::GetColumns()
    {
        return m_columns;
    }

    const std::vector<ColumnDefinition>& HeaderDefinition::GetColumns() const
    {
        return m_columns;
    }

    void HeaderDefinition::SetColumns(const std::vector<ColumnDefinition>& columns)
    {
        m_columns = columns;
    }

    ColumnDefinition& HeaderDefinition::GetColumn(size_t index)
    {
        ValidateIndex(index);
        return m_columns[index];
    }

    const ColumnDefinition& HeaderDefinition::GetColumn(size_t index) const
    {
        ValidateIndex(index);
        return m_columns[index];
    }

    void HeaderDefinition::AddColumn(const ColumnDefinition& column)
    {
        m_columns.push_back(column);
    }

    void HeaderDefinition::AddColumn(const std::string& name, DataType type, const Data& defaultValue)
    {
        m_columns.emplace_back(name, type, defaultValue);
    }

    void HeaderDefinition::RemoveColumn(size_t index)
    {
        ValidateIndex(index);
        m_columns.erase(m_columns.begin() + index);
    }

    void HeaderDefinition::UpdateColumn(size_t index, const ColumnDefinition& column)
    {
        ValidateIndex(index);
        m_columns[index] = column;
    }

    size_t HeaderDefinition::Size() const
    {
        return m_columns.size();
    }

    bool HeaderDefinition::IsEmpty() const
    {
        return m_columns.empty();
    }

    void HeaderDefinition::Clear()
    {
        m_columns.clear();
    }

    void HeaderDefinition::ApplyFromDataObject(const DataObject& sample)
    {
        std::vector<ColumnDefinition> newColumns;
        
        for (size_t i = 0; i < sample.Size(); i++)
        {
            const Data& data = sample.Get(i);
            ColumnDefinition newCol;
            
            const ColumnDefinition* existing = (i < m_columns.size()) ? &m_columns[i] : nullptr;
            
            newCol.ApplyFromData(data, existing);
            newColumns.push_back(newCol);
        }
        
        m_columns = newColumns;
    }

    DataObject HeaderDefinition::GetDefaultObject() const
    {
        DataObject result;
        result.Resize(m_columns.size());
        
        for (size_t i = 0; i < m_columns.size(); i++)
        {
            result.Set(i, m_columns[i].CreateData());
        }
        
        return result;
    }

    DataObject HeaderDefinition::ValidateDataObject(const DataObject& obj) const
    {
        if (m_columns.empty())
            return obj;
        
        DataObject result;
        result.Resize(m_columns.size());
        
        for (size_t i = 0; i < m_columns.size(); i++)
        {
            if (i < obj.Size())
                result.Set(i, m_columns[i].ValidateData(obj.Get(i)));
            else
                result.Set(i, m_columns[i].CreateData());
        }
        
        return result;
    }

    bool HeaderDefinition::HasSameTypes(const DataObject& obj) const
    {
        if (m_columns.size() != obj.Size())
            return false;
        
        for (size_t i = 0; i < m_columns.size(); i++)
        {
            if (m_columns[i].GetType() != obj.Get(i).GetType())
                return false;
        }
        
        return true;
    }

    int HeaderDefinition::FindColumnByName(const std::string& name) const
    {
        for (size_t i = 0; i < m_columns.size(); i++)
        {
            if (m_columns[i].GetName() == name)
                return static_cast<int>(i);
        }
        return -1;
    }

    std::string HeaderDefinition::ToString() const
    {
        std::ostringstream ss;
        ss << "HeaderDefinition [" << m_columns.size() << " columns]:\n";
        for (const auto& col : m_columns)
        {
            ss << "  " << col.ToString() << "\n";
        }
        return ss.str();
    }

    void HeaderDefinition::ValidateIndex(size_t index) const
    {
        if (index >= m_columns.size())
            throw std::out_of_range("Column index out of range");
    }

}