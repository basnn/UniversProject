// FilteredTable.h
#pragma once
#include "Table.h"
#include "FilterOperation.h"
#include <vector>
#include <memory>

namespace DataEngine {

    struct FilterInfo
    {
        int columnIndex;
        std::unique_ptr<FilterOperation> operation;
        
        FilterInfo();
        FilterInfo(int col, std::unique_ptr<FilterOperation> op);
        FilterInfo(const FilterInfo& other);
        FilterInfo(FilterInfo&& other) noexcept;
        ~FilterInfo();
        
        FilterInfo& operator=(const FilterInfo& other);
        FilterInfo& operator=(FilterInfo&& other) noexcept;
        
        bool Evaluate(const DataObject& obj) const;
        std::string GetDescription() const;
    };

    class FilteredTable : public Table
    {
    private:
        Table* m_sourceTable;
        std::vector<size_t> m_filteredIndices;
        std::vector<FilterInfo> m_filters;
        
    public:
        explicit FilteredTable(Table* sourceTable);
        ~FilteredTable() override = default;
        
        void AddFilter(const FilterInfo& filter);
        void AddFilter(int columnIndex, std::unique_ptr<FilterOperation> operation);
        void RemoveFilter(size_t index);
        void ClearFilters();
        const std::vector<FilterInfo>& GetFilters() const { return m_filters; }
        
        void Update();
        
        Table* GetSourceTable() const { return m_sourceTable; }
        
        // Публичный метод для проверки фильтров (используется FilterHub)
        bool MatchesFilters(const DataObject& obj) const;
        
        // Методы для уведомлений от FilterHub
        void AddIndex(size_t sourceIndex);
        void RemoveIndex(size_t sourceIndex);
        void UpdateIndex(size_t sourceIndex);
        void ClearIndices();
        
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
        
    private:
        void RebuildIndices();
    };

}