// FilterOperation.cpp
#include "FilterOperation.h"

namespace DataEngine {

    std::vector<FilterOperation*> g_filterOperations;

    std::vector<FilterOperation*>& FilterOperation::GetOperations()
    {
        return g_filterOperations;
    }

    FilterOperation* FilterOperation::GetOperation(const std::string& name)
    {
        for (auto* op : g_filterOperations)
        {
            if (op && op->GetName() == name)
                return op;
        }
        return nullptr;
    }

}