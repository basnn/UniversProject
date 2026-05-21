// FilterDefaultOperation.cpp
#include "FilterDefaultOperation.h"
#include <sstream>

namespace DataEngine {

    // ============ ValueCompareOperation ============
    
    ValueCompareOperation::ValueCompareOperation(FilterOperator op, const Data& value)
        : m_operator(op), m_value(value) {}

    bool ValueCompareOperation::Evaluate(const Data& value) const
    {
        switch (m_operator)
        {
        case FilterOperator::Equal: return value == m_value;
        case FilterOperator::NotEqual: return value != m_value;
        case FilterOperator::Greater: return value > m_value;
        case FilterOperator::GreaterOrEqual: return value >= m_value;
        case FilterOperator::Less: return value < m_value;
        case FilterOperator::LessOrEqual: return value <= m_value;
        case FilterOperator::Contains:
        {
            std::string str = value.ToString();
            std::string substr = m_value.ToString();
            return str.find(substr) != std::string::npos;
        }
        case FilterOperator::StartsWith:
        {
            std::string str = value.ToString();
            std::string prefix = m_value.ToString();
            return str.find(prefix) == 0;
        }
        case FilterOperator::EndsWith:
        {
            std::string str = value.ToString();
            std::string suffix = m_value.ToString();
            if (suffix.length() > str.length()) return false;
            return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
        }
        case FilterOperator::IsNull: return value.IsNull();
        case FilterOperator::IsNotNull: return !value.IsNull();
        default: return false;
        }
    }

    std::string ValueCompareOperation::GetDescription() const
    {
        std::ostringstream ss;
        switch (m_operator)
        {
        case FilterOperator::Equal: ss << "="; break;
        case FilterOperator::NotEqual: ss << "!="; break;
        case FilterOperator::Greater: ss << ">"; break;
        case FilterOperator::GreaterOrEqual: ss << ">="; break;
        case FilterOperator::Less: ss << "<"; break;
        case FilterOperator::LessOrEqual: ss << "<="; break;
        case FilterOperator::Contains: ss << "CONTAINS"; break;
        case FilterOperator::StartsWith: ss << "STARTS WITH"; break;
        case FilterOperator::EndsWith: ss << "ENDS WITH"; break;
        case FilterOperator::IsNull: ss << "IS NULL"; break;
        case FilterOperator::IsNotNull: ss << "IS NOT NULL"; break;
        }
        ss << " " << m_value.ToString();
        return ss.str();
    }

    std::string ValueCompareOperation::GetName() const 
    { 
        return "ValueCompare"; 
    }

    std::string ValueCompareOperation::GetDescriptionForOperator() const
    {
        switch (m_operator)
        {
        case FilterOperator::Equal: return "Equals to the specified value";
        case FilterOperator::NotEqual: return "Not equals to the specified value";
        case FilterOperator::Greater: return "Greater than the specified value";
        case FilterOperator::GreaterOrEqual: return "Greater than or equal to the specified value";
        case FilterOperator::Less: return "Less than the specified value";
        case FilterOperator::LessOrEqual: return "Less than or equal to the specified value";
        case FilterOperator::Contains: return "Contains the specified substring";
        case FilterOperator::StartsWith: return "Starts with the specified prefix";
        case FilterOperator::EndsWith: return "Ends with the specified suffix";
        case FilterOperator::IsNull: return "Field value is null/empty";
        case FilterOperator::IsNotNull: return "Field value is not null/empty";
        default: return "";
        }
    }

    FilterOperation* ValueCompareOperation::Clone() const 
    { 
        return new ValueCompareOperation(m_operator, m_value); 
    }

    // ============ AndOperation ============
    
    AndOperation::AndOperation(std::unique_ptr<FilterOperation> left, std::unique_ptr<FilterOperation> right)
        : m_left(std::move(left)), m_right(std::move(right)) {}

    bool AndOperation::Evaluate(const Data& value) const
    {
        return m_left->Evaluate(value) && m_right->Evaluate(value);
    }

    std::string AndOperation::GetDescription() const
    {
        return "(" + m_left->GetDescription() + " AND " + m_right->GetDescription() + ")";
    }

    std::string AndOperation::GetName() const 
    { 
        return "AND"; 
    }

    std::string AndOperation::GetDescriptionForOperator() const
    {
        return "Logical AND between conditions";
    }

    FilterOperation* AndOperation::Clone() const 
    { 
        return new AndOperation(std::unique_ptr<FilterOperation>(m_left->Clone()), 
                                std::unique_ptr<FilterOperation>(m_right->Clone())); 
    }

    // ============ OrOperation ============
    
    OrOperation::OrOperation(std::unique_ptr<FilterOperation> left, std::unique_ptr<FilterOperation> right)
        : m_left(std::move(left)), m_right(std::move(right)) {}

    bool OrOperation::Evaluate(const Data& value) const
    {
        return m_left->Evaluate(value) || m_right->Evaluate(value);
    }

    std::string OrOperation::GetDescription() const
    {
        return "(" + m_left->GetDescription() + " OR " + m_right->GetDescription() + ")";
    }

    std::string OrOperation::GetName() const 
    { 
        return "OR"; 
    }

    std::string OrOperation::GetDescriptionForOperator() const
    {
        return "Logical OR between conditions";
    }

    FilterOperation* OrOperation::Clone() const 
    { 
        return new OrOperation(std::unique_ptr<FilterOperation>(m_left->Clone()), 
                               std::unique_ptr<FilterOperation>(m_right->Clone())); 
    }

    // ============ NotOperation ============
    
    NotOperation::NotOperation(std::unique_ptr<FilterOperation> operation)
        : m_operation(std::move(operation)) {}

    bool NotOperation::Evaluate(const Data& value) const
    {
        return !m_operation->Evaluate(value);
    }

    std::string NotOperation::GetDescription() const
    {
        return "NOT (" + m_operation->GetDescription() + ")";
    }

    std::string NotOperation::GetName() const 
    { 
        return "NOT"; 
    }

    std::string NotOperation::GetDescriptionForOperator() const
    {
        return "Logical NOT (negation) of a condition";
    }

    FilterOperation* NotOperation::Clone() const 
    { 
        return new NotOperation(std::unique_ptr<FilterOperation>(m_operation->Clone())); 
    }

    // ============ Глобальная регистрация ============
    
    namespace
    {
        struct Registrar
        {
            Registrar()
            {
                g_filterOperations.push_back(new ValueCompareOperation(FilterOperator::Equal, Data()));
                g_filterOperations.push_back(new AndOperation(nullptr, nullptr));
                g_filterOperations.push_back(new OrOperation(nullptr, nullptr));
                g_filterOperations.push_back(new NotOperation(nullptr));
            }
        };
        
        static Registrar s_registrar;
    }

}