// TableLoader.cpp
#include "TableLoader.h"
#include <stdexcept>
#include <algorithm>

namespace DataEngine {

    TableLoader::TableLoader() = default;

    TableLoader::TableLoader(std::unique_ptr<Loader> loader)
        : m_loader(std::move(loader))
    {}

    TableLoader::TableLoader(const HeaderDefinition& header)
    {
        m_header = header;
    }

    TableLoader::TableLoader(std::unique_ptr<Loader> loader, const HeaderDefinition& header)
        : m_loader(std::move(loader))
    {
        m_header = header;
    }

    DataObject& TableLoader::Get(size_t index)
    {
        if (index >= m_objects.size())
            throw std::out_of_range("Index out of range");
        return m_objects[index];
    }

    const DataObject& TableLoader::Get(size_t index) const
    {
        if (index >= m_objects.size())
            throw std::out_of_range("Index out of range");
        return m_objects[index];
    }

    size_t TableLoader::Size() const
    {
        return m_objects.size();
    }

    bool TableLoader::IsEmpty() const
    {
        return m_objects.empty();
    }

    void TableLoader::Add(const DataObject& obj)
    {
        DataObject validated = ValidateWithDefault(obj);
        if (validated.Size() == 0 && !m_header.IsEmpty())
            return;
        m_objects.push_back(validated);
    }

    void TableLoader::Add(DataObject&& obj)
    {
        DataObject validated = ValidateWithDefault(obj);
        if (validated.Size() == 0 && !m_header.IsEmpty())
            return;
        m_objects.push_back(std::move(validated));
    }

    void TableLoader::AddAll(const std::vector<DataObject>& objects)
    {
        for (const auto& obj : objects)
            Add(obj);
    }

    void TableLoader::Remove(size_t index)
    {
        if (index >= m_objects.size())
            throw std::out_of_range("Index out of range");
        m_objects.erase(m_objects.begin() + index);
    }

    void TableLoader::RemoveRange(size_t start, size_t end)
    {
        if (start >= m_objects.size())
            throw std::out_of_range("Start index out of range");
        if (end >= m_objects.size())
            end = m_objects.size() - 1;
        m_objects.erase(m_objects.begin() + start, m_objects.begin() + end + 1);
    }

    void TableLoader::Clear()
    {
        m_objects.clear();
    }

    size_t TableLoader::IndexOf(const DataObject& obj) const
    {
        for (size_t i = 0; i < m_objects.size(); i++)
        {
            if (&m_objects[i] == &obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    size_t TableLoader::IndexOf(const DataObject* obj) const
    {
        if (!obj) return static_cast<size_t>(-1);
        for (size_t i = 0; i < m_objects.size(); i++)
        {
            if (&m_objects[i] == obj)
                return i;
        }
        return static_cast<size_t>(-1);
    }

    std::vector<DataObject>::iterator TableLoader::begin()
    {
        return m_objects.begin();
    }

    std::vector<DataObject>::iterator TableLoader::end()
    {
        return m_objects.end();
    }

    std::vector<DataObject>::const_iterator TableLoader::begin() const
    {
        return m_objects.begin();
    }

    std::vector<DataObject>::const_iterator TableLoader::end() const
    {
        return m_objects.end();
    }

    void TableLoader::Load()
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

    void TableLoader::Unload()
    {
        m_objects.clear();
        if (m_loader)
            m_loader->Reset();
    }

    DataObject TableLoader::ValidateWithDefault(const DataObject& obj) const
    {
        return m_header.ValidateDataObject(obj);
    }

    size_t TableLoader::GetTotalCount() const
    {
        if (m_loader)
            return m_loader->GetObjectCount();
        return m_objects.size();
    }

    void TableLoader::SetLoader(std::unique_ptr<Loader> loader)
    {
        m_loader = std::move(loader);
        Unload();
    }

    Loader* TableLoader::GetLoader() const
    {
        return m_loader.get();
    }

    void TableLoader::Load(size_t count)
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

    void TableLoader::LoadRange(size_t startIndex, size_t count)
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
            obj = ValidateWithDefault(obj);
        
        if (startIndex >= m_objects.size())
        {
            m_objects.insert(m_objects.end(), newObjects.begin(), newObjects.end());
        }
        else
        {
            for (size_t i = 0; i < newObjects.size() && startIndex + i < m_objects.size(); i++)
                m_objects[startIndex + i] = newObjects[i];
            
            if (startIndex + newObjects.size() > m_objects.size())
            {
                m_objects.insert(m_objects.end(), 
                    newObjects.begin() + (m_objects.size() - startIndex), 
                    newObjects.end());
            }
        }
    }

    size_t TableLoader::GetLoadedCount() const
    {
        return m_objects.size();
    }

    void TableLoader::ValidateAllObjects()
    {
        if (m_header.IsEmpty())
            return;
        
        for (auto& obj : m_objects)
            obj = m_header.ValidateDataObject(obj);
    }

    DataObject TableLoader::CreateDefaultObject() const
    {
        return m_header.GetDefaultObject();
    }

}