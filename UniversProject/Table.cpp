// Table.cpp
#include "Table.h"

namespace DataEngine {

    HeaderDefinition& Table::GetHeader()
    {
        return m_header;
    }

    const HeaderDefinition& Table::GetHeader() const
    {
        return m_header;
    }

    void Table::SetHeader(const HeaderDefinition& header)
    {
        m_header = header;
    }

    void Table::SetName(const std::string& name)
    {
        m_name = name;
    }

    const std::string& Table::GetName() const
    {
        return m_name;
    }

}