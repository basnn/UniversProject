// Table.cpp
#include "Table.h"
#include <stdexcept>
#include <algorithm>

namespace DataEngine {

    Table::Table() = default;

    Table::Table(std::unique_ptr<Loader> loader)
        : m_loader(std::move(loader))
    {}

    Table::Table(const HeaderDefinition& header)
        : m_header(header)
    {}

    Table::Table(std::unique_ptr<Loader> loader, const HeaderDefinition& header)
        : m_loader(std::move(loader))
        , m_header(header)
    {}

    void Table::SetLoader(std::unique_ptr<Loader> loader)
    {
        m_loader = std::move(loader);
        UnloadAll();
    }

    Loader* Table::GetLoader() const
    {
        return m_loader.get();
    }

    void Table::ClearLoader()
    {
        m_loader.reset();
        UnloadAll();
    }

    HeaderDefinition& Table::GetHeader()
    {
        return m_header;
    }

    void Table::SetHeader(const HeaderDefinition& header)
    {
        m_header = header;
        ValidateAllObjects();
    }

    void Table::LoadAll()
    {
        if (!m_loader)
            return;
        
        m_objects.clear();
        m_loader->Reset();
        
        while (m_loader->HasNext())
        {
            DataObject obj = m_loader->ReadNext();
            m_objects.push_back(ValidateWithDefault(obj));
        }
    }

    void Table::Load(size_t count)
    {
        if (!m_loader)
            return;
        
        size_t loaded = 0;
        while (m_loader->HasNext() && loaded < count)
        {
            DataObject obj = m_loader->ReadNext();
            m_objects.push_back(ValidateWithDefault(obj));
            loaded++;
        }
    }

    void Table::LoadRange(size_t startIndex, size_t count)
    {
        if (!m_loader)
            return;
        
        if (startIndex >= m_loader->GetObjectCount())
            return;
        
        size_t endIndex = startIndex + count - 1;
        if (endIndex >= m_loader->GetObjectCount())
            endIndex = m_loader->GetObjectCount() - 1;
        
        std::vector<DataObject> newObjects = m_loader->ReadRange(startIndex, endIndex);
        
        for (auto& obj : newObjects)
        {
            obj = ValidateWithDefault(obj);
        }
        
        if (startIndex >= m_objects.size())
        {
            m_objects.insert(m_objects.end(), newObjects.begin(), newObjects.end());
        }
        else
        {
            for (size_t i = 0; i < newObjects.size() && startIndex + i < m_objects.size(); i++)
            {
                m_objects[startIndex + i] = newObjects[i];
            }
            
            if (startIndex + newObjects.size() > m_objects.size())
            {
                m_objects.insert(m_objects.end(), 
                    newObjects.begin() + (m_objects.size() - startIndex), 
                    newObjects.end());
            }
        }
    }

    void Table::UnloadAll()
    {
        m_objects.clear();
        if (m_loader)
            m_loader->Reset();
    }

    void Table::Unload(size_t count)
    {
        if (count >= m_objects.size())
        {
            UnloadAll();
            return;
        }
        
        m_objects.erase(m_objects.begin(), m_objects.begin() + count);
    }

    void Table::UnloadRange(size_t startIndex, size_t count)
    {
        if (startIndex >= m_objects.size())
            return;
        
        size_t endIndex = startIndex + count;
        if (endIndex >= m_objects.size())
            endIndex = m_objects.size();
        
        m_objects.erase(m_objects.begin() + startIndex, m_objects.begin() + endIndex);
    }

    bool Table::AddObject(const DataObject& obj)
    {
        DataObject validated = ValidateWithDefault(obj);
        if (validated.Size() == 0 && !m_header.IsEmpty())
            return false;
        
        m_objects.push_back(std::move(validated));
        return true;
    }

    bool Table::AddObject(DataObject&& obj)
    {
        DataObject validated = ValidateWithDefault(obj);
        if (validated.Size() == 0 && !m_header.IsEmpty())
            return false;
        
        m_objects.push_back(std::move(validated));
        return true;
    }

    bool Table::AddObjects(const std::vector<DataObject>& objects)
    {
        bool allSuccess = true;
        for (const auto& obj : objects)
        {
            if (!AddObject(obj))
                allSuccess = false;
        }
        return allSuccess;
    }

    DataObject& Table::Get(size_t index)
    {
        if (index >= m_objects.size())
            throw std::out_of_range("Index out of range");
        
        return m_objects[index];
    }

    const DataObject& Table::Get(size_t index) const
    {
        if (index >= m_objects.size())
            throw std::out_of_range("Index out of range");
        
        return m_objects[index];
    }

    size_t Table::IndexOf(const DataObject& obj) const
    {
        for (size_t i = 0; i < m_objects.size(); i++)
        {
            if (&m_objects[i] == &obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    size_t Table::IndexOf(const DataObject* obj) const
    {
        if (!obj) return static_cast<size_t>(-1);
        
        for (size_t i = 0; i < m_objects.size(); i++)
        {
            if (&m_objects[i] == obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    size_t Table::Size() const
    {
        return m_objects.size();
    }

    bool Table::IsEmpty() const
    {
        return m_objects.empty();
    }

    void Table::Clear()
    {
        UnloadAll();
    }

    bool Table::SaveAll()
    {
        if (!m_loader || m_objects.empty())
            return false;
        
        m_loader->Reset();
        return m_loader->WriteNext(m_objects);
    }

    bool Table::SaveRange(size_t startIndex, size_t count)
    {
        if (!m_loader)
            return false;
        
        if (startIndex >= m_objects.size())
            return false;
        
        size_t endIndex = startIndex + count;
        if (endIndex > m_objects.size())
            endIndex = m_objects.size();
        
        std::vector<DataObject> toSave;
        for (size_t i = startIndex; i < endIndex; i++)
            toSave.push_back(m_objects[i]);
        
        return m_loader->WriteRange(startIndex, toSave);
    }

    bool Table::SaveAt(size_t index)
    {
        if (!m_loader || index >= m_objects.size())
            return false;
        
        return m_loader->WriteAt(index, m_objects[index]);
    }

    bool Table::Save(const DataObject& obj)
    {
        if (!m_loader)
            return false;
        
        DataObject validated = ValidateWithDefault(obj);
        return m_loader->WriteNext(validated);
    }

    bool Table::Save(const std::vector<DataObject>& objects)
    {
        if (!m_loader)
            return false;
        
        std::vector<DataObject> validated;
        for (const auto& obj : objects)
        {
            validated.push_back(ValidateWithDefault(obj));
        }
        
        return m_loader->WriteNext(validated);
    }

    size_t Table::GetTotalCount() const
    {
        if (m_loader)
            return m_loader->GetObjectCount();
        return m_objects.size();
    }

    size_t Table::GetLoadedCount() const
    {
        return m_objects.size();
    }

    DataObject Table::ValidateWithDefault(const DataObject& obj) const
    {
        return m_header.ValidateDataObject(obj);
    }

    void Table::LoadObjects(size_t start, size_t count)
    {
        if (!m_loader)
            return;
        
        std::vector<DataObject> newObjects = m_loader->ReadRange(start, start + count - 1);
        
        for (auto& obj : newObjects)
        {
            obj = ValidateWithDefault(obj);
        }
        
        m_objects.insert(m_objects.end(), newObjects.begin(), newObjects.end());
    }

    void Table::ValidateAllObjects()
    {
        if (m_header.IsEmpty())
            return;
        
        for (auto& obj : m_objects)
        {
            obj = m_header.ValidateDataObject(obj);
        }
    }

}