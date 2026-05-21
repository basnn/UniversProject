// FilterHub.h
#pragma once
#include "Table.h"
#include "FilteredTable.h"
#include <vector>
#include <memory>

namespace DataEngine {

    class FilterHub : public Table
    {
    private:
        std::unique_ptr<Table> m_sourceTable;              // оригинальная таблица
        std::vector<std::unique_ptr<FilteredTable>> m_filteredTables;  // все фильтрованные представления
        
    public:
        FilterHub();
        explicit FilterHub(std::unique_ptr<Table> sourceTable);
        ~FilterHub() override = default;
        
        // Управление исходной таблицей
        void SetSourceTable(std::unique_ptr<Table> sourceTable);
        Table* GetSourceTable() const { return m_sourceTable.get(); }
        
        // Создание фильтрованного представления
        FilteredTable* CreateFilteredTable();
        void RemoveFilteredTable(FilteredTable* table);
        size_t GetFilteredTableCount() const { return m_filteredTables.size(); }
        
        // Обновление всех фильтрованных таблиц
        void NotifyAllFilters();
        
        // Table implementation (прокси для m_sourceTable)
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
        
        HeaderDefinition& GetHeader() override;
        const HeaderDefinition& GetHeader() const override;
        void SetHeader(const HeaderDefinition& header) override;
        
    private:
        void NotifyAdd(size_t index, const DataObject& obj);
        void NotifyRemove(size_t index);
        void NotifyUpdate(size_t index, const DataObject& oldObj, const DataObject& newObj);
        void NotifyClear();
    };

}