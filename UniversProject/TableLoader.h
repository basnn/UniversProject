// TableLoader.h - бывший Table
#pragma once
#include <vector>
#include <memory>
#include "Table.h"
#include "Loader.h"

namespace DataEngine {

    class TableLoader : public Table
    {
    private:
        std::vector<DataObject> m_objects;
        std::unique_ptr<Loader> m_loader;

    public:
        TableLoader();
        explicit TableLoader(std::unique_ptr<Loader> loader);
        explicit TableLoader(const HeaderDefinition& header);
        TableLoader(std::unique_ptr<Loader> loader, const HeaderDefinition& header);
        ~TableLoader() = default;

        // Table implementation
        DataObject& Get(size_t index) override;
        const DataObject& Get(size_t index) const override;
        size_t Size() const override;
        bool IsEmpty() const override;
        
        void Add(const DataObject& obj) override;
        void Add(DataObject&& obj) override;
        void AddAll(const std::vector<DataObject>& objects) override;
        
        void Remove(size_t index) override;
        void RemoveRange(size_t start, size_t end) override;
        void Clear() override;
        
        size_t IndexOf(const DataObject& obj) const override;
        size_t IndexOf(const DataObject* obj) const override;
        
        std::vector<DataObject>::iterator begin() override;
        std::vector<DataObject>::iterator end() override;
        std::vector<DataObject>::const_iterator begin() const override;
        std::vector<DataObject>::const_iterator end() const override;
        
        void Load() override;
        void Unload() override;
        
        DataObject ValidateWithDefault(const DataObject& obj) const override;
       
        
        size_t GetTotalCount() const override;

        // Специфичные методы
        void SetLoader(std::unique_ptr<Loader> loader);
        Loader* GetLoader() const;
        
        void Load(size_t count);
        void LoadRange(size_t startIndex, size_t count);
        size_t GetLoadedCount() const;

    private:
        void ValidateAllObjects();
        DataObject CreateDefaultObject() const;
    };

}