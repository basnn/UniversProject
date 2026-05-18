// Table.h
#pragma once
#include <vector>
#include <memory>
#include "DataObject.h"
#include "Loader.h"
#include "HeaderDefinition.h"

namespace DataEngine {

    class Table
    {
    private:
        std::vector<DataObject> m_objects;
        std::unique_ptr<Loader> m_loader;
        HeaderDefinition m_header;

    public:
        Table();
        explicit Table(std::unique_ptr<Loader> loader);
        explicit Table(const HeaderDefinition& header);
        Table(std::unique_ptr<Loader> loader, const HeaderDefinition& header);
        ~Table() = default;

        Table(const Table&) = delete;
        Table& operator=(const Table&) = delete;
        Table(Table&&) = default;
        Table& operator=(Table&&) = default;

        void SetLoader(std::unique_ptr<Loader> loader);
        Loader* GetLoader() const;
        void ClearLoader();

        HeaderDefinition& GetHeader();
        void SetHeader(const HeaderDefinition& header);
        
        void LoadAll();
        void Load(size_t count);
        void LoadRange(size_t startIndex, size_t count);
        
        void UnloadAll();
        void Unload(size_t count);
        void UnloadRange(size_t startIndex, size_t count);
        
        bool AddObject(const DataObject& obj);
        bool AddObject(DataObject&& obj);
        bool AddObjects(const std::vector<DataObject>& objects);
        
        DataObject& Get(size_t index);
        const DataObject& Get(size_t index) const;
        
        size_t IndexOf(const DataObject& obj) const;
        size_t IndexOf(const DataObject* obj) const;
        
        size_t Size() const;
        bool IsEmpty() const;
        void Clear();

        bool SaveAll();
        bool SaveRange(size_t startIndex, size_t count);
        bool SaveAt(size_t index);
        bool Save(const DataObject& obj);
        bool Save(const std::vector<DataObject>& objects);

        size_t GetTotalCount() const;
        size_t GetLoadedCount() const;

    private:
        DataObject ValidateWithDefault(const DataObject& obj) const;
        void LoadObjects(size_t start, size_t count);
        void ValidateAllObjects();
    };

}