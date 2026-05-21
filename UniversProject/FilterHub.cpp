// FilterHub.cpp
#include "FilterHub.h"
#include <algorithm>
#include <stdexcept>

namespace DataEngine {

    FilterHub::FilterHub() = default;

    FilterHub::FilterHub(std::unique_ptr<Table> sourceTable)
        : m_sourceTable(std::move(sourceTable))
    {}

    void FilterHub::SetSourceTable(std::unique_ptr<Table> sourceTable)
    {
        m_sourceTable = std::move(sourceTable);
        NotifyAllFilters();
    }

    FilteredTable* FilterHub::CreateFilteredTable()
    {
        if (!m_sourceTable)
            return nullptr;
        
        auto filtered = std::make_unique<FilteredTable>(m_sourceTable.get());
        FilteredTable* result = filtered.get();
        m_filteredTables.push_back(std::move(filtered));
        return result;
    }

    void FilterHub::RemoveFilteredTable(FilteredTable* table)
    {
        auto it = std::find_if(m_filteredTables.begin(), m_filteredTables.end(),
            [table](const std::unique_ptr<FilteredTable>& ptr) {
                return ptr.get() == table;
            });
        
        if (it != m_filteredTables.end())
            m_filteredTables.erase(it);
    }

    void FilterHub::NotifyAllFilters()
    {
        for (auto& filtered : m_filteredTables)
        {
            filtered->Update();
        }
    }

    void FilterHub::NotifyAdd(size_t index, const DataObject& obj)
    {
        for (auto& filtered : m_filteredTables)
        {
            if (filtered->MatchesFilters(obj))
            {
                // FilteredTable должен добавить индекс в свой список
                // Для этого нужен внутренний метод AddIndex
                // filtered->AddIndex(index);
            }
        }
    }

    void FilterHub::NotifyRemove(size_t index)
    {
        for (auto& filtered : m_filteredTables)
        {
            // FilteredTable должен удалить индекс из своего списка
            // filtered->RemoveIndex(index);
        }
    }

    void FilterHub::NotifyUpdate(size_t index, const DataObject& oldObj, const DataObject& newObj)
    {
        for (auto& filtered : m_filteredTables)
        {
            bool oldMatches = filtered->MatchesFilters(oldObj);
            bool newMatches = filtered->MatchesFilters(newObj);
            
            if (oldMatches && !newMatches)
            {
                // filtered->RemoveIndex(index);
            }
            else if (!oldMatches && newMatches)
            {
                // filtered->AddIndex(index);
            }
            else if (oldMatches && newMatches)
            {
                // filtered->UpdateIndex(index);
            }
        }
    }

    void FilterHub::NotifyClear()
    {
        for (auto& filtered : m_filteredTables)
        {
            filtered->ClearIndices();
        }
    }

    // ============ Table implementation (прокси) ============
    
    DataObject& FilterHub::Get(size_t index)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->Get(index);
    }

    const DataObject& FilterHub::Get(size_t index) const
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->Get(index);
    }

    size_t FilterHub::Size() const
    {
        if (!m_sourceTable)
            return 0;
        return m_sourceTable->Size();
    }

    bool FilterHub::IsEmpty() const
    {
        if (!m_sourceTable)
            return true;
        return m_sourceTable->IsEmpty();
    }

    void FilterHub::Add(const DataObject& obj)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        size_t newIndex = m_sourceTable->Size();
        m_sourceTable->Add(obj);
        NotifyAdd(newIndex, obj);
    }

    void FilterHub::Add(DataObject&& obj)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        size_t newIndex = m_sourceTable->Size();
        m_sourceTable->Add(std::move(obj));
        NotifyAdd(newIndex, obj);
    }

    void FilterHub::AddAll(const std::vector<DataObject>& objects)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        size_t startIndex = m_sourceTable->Size();
        m_sourceTable->AddAll(objects);
        
        for (size_t i = 0; i < objects.size(); i++)
        {
            NotifyAdd(startIndex + i, objects[i]);
        }
    }

    void FilterHub::Remove(size_t index)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        NotifyRemove(index);
        m_sourceTable->Remove(index);
    }

    void FilterHub::RemoveRange(size_t start, size_t end)
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        for (size_t i = start; i <= end; i++)
            NotifyRemove(i);
        
        m_sourceTable->RemoveRange(start, end);
    }

    void FilterHub::Clear()
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        
        NotifyClear();
        m_sourceTable->Clear();
    }

    size_t FilterHub::IndexOf(const DataObject& obj) const
    {
        if (!m_sourceTable)
            return static_cast<size_t>(-1);
        return m_sourceTable->IndexOf(obj);
    }

    size_t FilterHub::IndexOf(const DataObject* obj) const
    {
        if (!m_sourceTable)
            return static_cast<size_t>(-1);
        return m_sourceTable->IndexOf(obj);
    }

    std::vector<DataObject>::iterator FilterHub::begin()
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->begin();
    }

    std::vector<DataObject>::iterator FilterHub::end()
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->end();
    }

    std::vector<DataObject>::const_iterator FilterHub::begin() const
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->begin();
    }

    std::vector<DataObject>::const_iterator FilterHub::end() const
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->end();
    }

    void FilterHub::Load()
    {
        if (m_sourceTable)
            m_sourceTable->Load();
        NotifyAllFilters();
    }

    void FilterHub::Unload()
    {
        if (m_sourceTable)
            m_sourceTable->Unload();
        NotifyAllFilters();
    }

    DataObject FilterHub::ValidateWithDefault(const DataObject& obj) const
    {
        if (m_sourceTable)
            return m_sourceTable->ValidateWithDefault(obj);
        return obj;
    }

    size_t FilterHub::GetTotalCount() const
    {
        if (m_sourceTable)
            return m_sourceTable->GetTotalCount();
        return 0;
    }

    HeaderDefinition& FilterHub::GetHeader()
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->GetHeader();
    }

    const HeaderDefinition& FilterHub::GetHeader() const
    {
        if (!m_sourceTable)
            throw std::runtime_error("No source table");
        return m_sourceTable->GetHeader();
    }

    void FilterHub::SetHeader(const HeaderDefinition& header)
    {
        if (m_sourceTable)
            m_sourceTable->SetHeader(header);
        NotifyAllFilters();
    }

}