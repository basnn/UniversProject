// FilteredTable.cpp
#include "FilteredTable.h"
#include <algorithm>
#include <stdexcept>

namespace DataEngine {

    // ============ FilterInfo implementation ============
    
    FilterInfo::FilterInfo()
        : columnIndex(-1)
        , operation(nullptr)
    {}

    FilterInfo::FilterInfo(int col, std::unique_ptr<FilterOperation> op)
        : columnIndex(col)
        , operation(std::move(op))
    {}

    FilterInfo::FilterInfo(const FilterInfo& other)
        : columnIndex(other.columnIndex)
        , operation(other.operation ? std::unique_ptr<FilterOperation>(other.operation->Clone()) : nullptr)
    {}

    FilterInfo::FilterInfo(FilterInfo&& other) noexcept
        : columnIndex(other.columnIndex)
        , operation(std::move(other.operation))
    {
        other.columnIndex = -1;
    }

    FilterInfo::~FilterInfo() = default;

    FilterInfo& FilterInfo::operator=(const FilterInfo& other)
    {
        if (this != &other)
        {
            columnIndex = other.columnIndex;
            operation.reset(other.operation ? other.operation->Clone() : nullptr);
        }
        return *this;
    }

    FilterInfo& FilterInfo::operator=(FilterInfo&& other) noexcept
    {
        if (this != &other)
        {
            columnIndex = other.columnIndex;
            operation = std::move(other.operation);
            other.columnIndex = -1;
        }
        return *this;
    }

    bool FilterInfo::Evaluate(const DataObject& obj) const
    {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(obj.Size()))
            return false;
        if (!operation)
            return true;
        return operation->Evaluate(obj.Get(columnIndex));
    }

    std::string FilterInfo::GetDescription() const
    {
        if (!operation) return "no filter";
        return "col_" + std::to_string(columnIndex) + " " + operation->GetDescription();
    }

    // ============ FilteredTable implementation ============
    
    FilteredTable::FilteredTable(Table* sourceTable)
        : m_sourceTable(sourceTable)
    {
        if (m_sourceTable)
        {
            m_header = m_sourceTable->GetHeader();
            m_name = m_sourceTable->GetName() + "_filtered";
            Update();
        }
    }

    void FilteredTable::AddFilter(const FilterInfo& filter)
    {
        m_filters.push_back(filter);
        Update();
    }

    void FilteredTable::AddFilter(int columnIndex, std::unique_ptr<FilterOperation> operation)
    {
        m_filters.emplace_back(columnIndex, std::move(operation));
        Update();
    }

    void FilteredTable::RemoveFilter(size_t index)
    {
        if (index < m_filters.size())
        {
            m_filters.erase(m_filters.begin() + index);
            Update();
        }
    }

    void FilteredTable::ClearFilters()
    {
        m_filters.clear();
        Update();
    }

    void FilteredTable::Update()
    {
        if (!m_sourceTable)
        {
            m_filteredIndices.clear();
            m_header = HeaderDefinition();
            return;
        }
        
        m_header = m_sourceTable->GetHeader();
        RebuildIndices();
    }

    void FilteredTable::RebuildIndices()
    {
        m_filteredIndices.clear();
        
        if (!m_sourceTable)
            return;
        
        m_sourceTable->Load();
        
        for (size_t i = 0; i < m_sourceTable->Size(); i++)
        {
            const DataObject& obj = m_sourceTable->Get(i);
            if (MatchesFilters(obj))
            {
                m_filteredIndices.push_back(i);
            }
        }
    }

    bool FilteredTable::MatchesFilters(const DataObject& obj) const
    {
        if (m_filters.empty())
            return true;
        
        for (const auto& filter : m_filters)
        {
            if (!filter.Evaluate(obj))
                return false;
        }
        return true;
    }

    void FilteredTable::AddIndex(size_t sourceIndex)
    {
        if (!m_sourceTable)
            return;
        
        const DataObject& obj = m_sourceTable->Get(sourceIndex);
        if (MatchesFilters(obj))
        {
            auto it = std::lower_bound(m_filteredIndices.begin(), m_filteredIndices.end(), sourceIndex);
            m_filteredIndices.insert(it, sourceIndex);
        }
    }

    void FilteredTable::RemoveIndex(size_t sourceIndex)
    {
        auto it = std::find(m_filteredIndices.begin(), m_filteredIndices.end(), sourceIndex);
        if (it != m_filteredIndices.end())
        {
            m_filteredIndices.erase(it);
            
            for (auto& idx : m_filteredIndices)
            {
                if (idx > sourceIndex)
                    idx--;
            }
        }
    }

    void FilteredTable::UpdateIndex(size_t sourceIndex)
    {
        if (!m_sourceTable)
            return;
        
        const DataObject& obj = m_sourceTable->Get(sourceIndex);
        bool matches = MatchesFilters(obj);
        
        auto it = std::find(m_filteredIndices.begin(), m_filteredIndices.end(), sourceIndex);
        bool exists = (it != m_filteredIndices.end());
        
        if (matches && !exists)
        {
            AddIndex(sourceIndex);
        }
        else if (!matches && exists)
        {
            RemoveIndex(sourceIndex);
        }
    }

    void FilteredTable::ClearIndices()
    {
        m_filteredIndices.clear();
        RebuildIndices();
    }

    DataObject& FilteredTable::Get(size_t index)
    {
        if (index >= m_filteredIndices.size())
            throw std::out_of_range("Index out of range");
        
        return m_sourceTable->Get(m_filteredIndices[index]);
    }

    const DataObject& FilteredTable::Get(size_t index) const
    {
        if (index >= m_filteredIndices.size())
            throw std::out_of_range("Index out of range");
        
        return m_sourceTable->Get(m_filteredIndices[index]);
    }

    size_t FilteredTable::Size() const
    {
        return m_filteredIndices.size();
    }

    bool FilteredTable::IsEmpty() const
    {
        return m_filteredIndices.empty();
    }

    void FilteredTable::Add(const DataObject& obj)
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    void FilteredTable::Add(DataObject&& obj)
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    void FilteredTable::AddAll(const std::vector<DataObject>& objects)
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    void FilteredTable::Remove(size_t index)
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    void FilteredTable::RemoveRange(size_t start, size_t end)
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    void FilteredTable::Clear()
    {
        throw std::runtime_error("FilteredTable is read-only");
    }

    size_t FilteredTable::IndexOf(const DataObject& obj) const
    {
        for (size_t i = 0; i < m_filteredIndices.size(); i++)
        {
            const DataObject& current = m_sourceTable->Get(m_filteredIndices[i]);
            if (&current == &obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    size_t FilteredTable::IndexOf(const DataObject* obj) const
    {
        if (!obj) return static_cast<size_t>(-1);
        
        for (size_t i = 0; i < m_filteredIndices.size(); i++)
        {
            const DataObject& current = m_sourceTable->Get(m_filteredIndices[i]);
            if (&current == obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    std::vector<DataObject>::iterator FilteredTable::begin()
    {
        throw std::runtime_error("FilteredTable does not support direct iteration");
    }

    std::vector<DataObject>::iterator FilteredTable::end()
    {
        throw std::runtime_error("FilteredTable does not support direct iteration");
    }

    std::vector<DataObject>::const_iterator FilteredTable::begin() const
    {
        throw std::runtime_error("FilteredTable does not support direct iteration");
    }

    std::vector<DataObject>::const_iterator FilteredTable::end() const
    {
        throw std::runtime_error("FilteredTable does not support direct iteration");
    }

    void FilteredTable::Load()
    {
        if (m_sourceTable)
            m_sourceTable->Load();
        Update();
    }

    void FilteredTable::Unload()
    {
        if (m_sourceTable)
            m_sourceTable->Unload();
        m_filteredIndices.clear();
    }

    DataObject FilteredTable::ValidateWithDefault(const DataObject& obj) const
    {
        if (m_sourceTable)
            return m_sourceTable->ValidateWithDefault(obj);
        return obj;
    }

    size_t FilteredTable::GetTotalCount() const
    {
        return m_filteredIndices.size();
    }

}