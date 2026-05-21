// FilterDefaultOperation.h
#pragma once
#include "FilterOperation.h"

namespace DataEngine {

    enum class FilterOperator
    {
        Equal, NotEqual, Greater, GreaterOrEqual,
        Less, LessOrEqual, Contains, StartsWith,
        EndsWith, IsNull, IsNotNull
    };

    class ValueCompareOperation : public FilterOperation
    {
    private:
        FilterOperator m_operator;
        Data m_value;
        
    public:
        ValueCompareOperation(FilterOperator op, const Data& value);
        
        bool Evaluate(const Data& value) const override;
        std::string GetDescription() const override;
        std::string GetName() const override;
        std::string GetDescriptionForOperator() const override;
        FilterOperation* Clone() const override;
    };

    class AndOperation : public FilterOperation
    {
    private:
        std::unique_ptr<FilterOperation> m_left;
        std::unique_ptr<FilterOperation> m_right;
        
    public:
        AndOperation(std::unique_ptr<FilterOperation> left, std::unique_ptr<FilterOperation> right);
        
        bool Evaluate(const Data& value) const override;
        std::string GetDescription() const override;
        std::string GetName() const override;
        std::string GetDescriptionForOperator() const override;
        FilterOperation* Clone() const override;
    };

    class OrOperation : public FilterOperation
    {
    private:
        std::unique_ptr<FilterOperation> m_left;
        std::unique_ptr<FilterOperation> m_right;
        
    public:
        OrOperation(std::unique_ptr<FilterOperation> left, std::unique_ptr<FilterOperation> right);
        
        bool Evaluate(const Data& value) const override;
        std::string GetDescription() const override;
        std::string GetName() const override;
        std::string GetDescriptionForOperator() const override;
        FilterOperation* Clone() const override;
    };

    class NotOperation : public FilterOperation
    {
    private:
        std::unique_ptr<FilterOperation> m_operation;
        
    public:
        explicit NotOperation(std::unique_ptr<FilterOperation> operation);
        
        bool Evaluate(const Data& value) const override;
        std::string GetDescription() const override;
        std::string GetName() const override;
        std::string GetDescriptionForOperator() const override;
        FilterOperation* Clone() const override;
    };

}