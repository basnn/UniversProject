// FilterOperation.h
#pragma once
#include <vector>
#include <string>
#include "Data.h"

namespace DataEngine {

    class FilterOperation
    {
    public:
        virtual ~FilterOperation() = default;
        
        virtual bool Evaluate(const Data& value) const = 0;
        virtual std::string GetDescription() const = 0;
        virtual std::string GetName() const = 0;
        virtual std::string GetDescriptionForOperator() const = 0;
        virtual FilterOperation* Clone() const = 0;
        
        static std::vector<FilterOperation*>& GetOperations();
        static FilterOperation* GetOperation(const std::string& name);
        
    protected:
        FilterOperation() = default;
    };

    extern std::vector<FilterOperation*> g_filterOperations;

}