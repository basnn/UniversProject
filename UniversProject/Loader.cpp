// Loader.cpp
#include "Loader.h"
#include <sstream>
#include"MFP/MFWR.cpp"
#include"MFP/MFR.cpp"
#include"MFP/MFI.cpp"
#include<EDL/EDL.cpp>

namespace DataEngine {

    TextLoader::TextLoader()
        : m_flow(nullptr)
        , m_currentPosition(0)
        , m_objectCount(0)
        , m_isOwner(false)
    {}

    TextLoader::TextLoader(const TextImportConfig& config)
        : m_flow(nullptr)
        , m_config(config)
        , m_currentPosition(0)
        , m_objectCount(0)
        , m_isOwner(false)
    {}

    TextLoader::~TextLoader()
    {
        if (m_isOwner && m_flow)
            delete m_flow;
        m_flow = nullptr;
    }

    void TextLoader::SetConfig(const TextImportConfig& config)
    {
        m_config = config;
    }

    void TextLoader::SetFlow(MF::CFlow* flow, bool takeOwnership)
    {
        if (m_isOwner && m_flow)
            delete m_flow;

        m_flow = flow;
        m_isOwner = takeOwnership;
    }

    bool TextLoader::BuildIndex()
    {
        if (!m_flow)
            return false;

        m_flow->seek(0, MF::CFlow::beg);
        m_lineOffsets.clear();
        m_headers.clear();
        m_objectCount = 0;

        long long currentOffset = 0;

        // Пропускаем skipLines
        for (int i = 0; i < m_config.skipLines; i++)
        {
            char c;
            while (m_flow->read(c) && c != '\n') {}
            currentOffset = m_flow->getPos();
        }

        // Читаем заголовок
        if (m_config.hasHeader)
        {
            std::string headerLine;
            char c;
            while (m_flow->read(c) && c != '\n')
                headerLine += c;
            m_headers = SplitLine(headerLine);
            currentOffset = m_flow->getPos();
        }

        // Индексация строк
        while (true)
        {
            m_lineOffsets.push_back(currentOffset);

            char c;
            bool hasData = false;
            while (m_flow->read(c))
            {
                hasData = true;
                if (c == '\n')
                    break;
            }

            if (!hasData)
                break;

            currentOffset = m_flow->getPos();
            m_objectCount++;
        }

        Reset();
        return true;
    }

    DataObject TextLoader::ReadNext()
    {
        if (!HasNext())
            return DataObject();

        long long offset = m_lineOffsets[m_currentPosition];
        m_flow->seek(offset, MF::CFlow::beg);

        std::string line;
        char c;
        while (m_flow->read(c) && c != '\n')
            line += c;

        m_currentPosition++;
        return ParseLineToObject(line);
    }

    std::vector<DataObject> TextLoader::ReadNext(size_t count)
    {
        std::vector<DataObject> result;
        for (size_t i = 0; i < count && HasNext(); i++)
            result.push_back(ReadNext());
        return result;
    }

    bool TextLoader::HasNext() const
    {
        return m_flow && m_currentPosition < m_objectCount;
    }

    void TextLoader::Reset()
    {
        m_currentPosition = 0;
    }

    bool TextLoader::WriteNext(const DataObject& obj)
    {
        if (!m_flow)
            return false;

        std::string line = ObjectToLine(obj);
        if (line.empty())
            return false;

        line += '\n';

        if (m_flow->write(line.c_str()) == line.size())
        {
            m_lineOffsets.push_back(m_flow->getPos() - line.size());
            m_objectCount++;
            return true;
        }

        return false;
    }

    bool TextLoader::WriteNext(const std::vector<DataObject>& objects)
    {
        bool all = true;
        for (const auto& obj : objects)
        {
            if (!WriteNext(obj))
                all = false;
        }
        return all;
    }

    DataObject TextLoader::ReadAt(size_t index)
    {
        if (!m_flow || index >= m_objectCount)
            return DataObject();

        long long offset = m_lineOffsets[index];
        std::string line = ReadLineAtOffset(offset);
        return ParseLineToObject(line);
    }

    std::vector<DataObject> TextLoader::ReadRange(size_t start, size_t end)
    {
        std::vector<DataObject> result;
        if (!m_flow || end >= m_objectCount)
            return result;

        for (size_t i = start; i <= end; i++)
            result.push_back(ReadAt(i));

        return result;
    }

    bool TextLoader::WriteAt(size_t index, const DataObject& obj)
    {
        // Для текстового формата не поддерживается
        return false;
    }

    bool TextLoader::WriteRange(size_t start, const std::vector<DataObject>& objects)
    {
        // Для текстового формата не поддерживается
        return false;
    }

    bool TextLoader::DeleteAt(size_t index)
    {
        // Для текстового формата не поддерживается
        return false;
    }

    bool TextLoader::DeleteRange(size_t start, size_t end)
    {
        // Для текстового формата не поддерживается
        return false;
    }

    size_t TextLoader::GetObjectCount() const
    {
        return m_objectCount;
    }

    std::vector<std::string> TextLoader::SplitLine(const std::string& line) const
    {
        std::vector<std::string> result;
        std::stringstream ss(line);
        std::string field;

        while (std::getline(ss, field, m_config.delimiter))
            result.push_back(Trim(field));

        return result;
    }

    std::string TextLoader::Trim(const std::string& str) const
    {
        size_t start = str.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(" \t\r\n");
        return str.substr(start, end - start + 1);
    }

    Data TextLoader::ParseField(const std::string& value, DataType type) const
    {
        std::string v = Trim(value);
        if (v.empty()) return Data();

        try
        {
            switch (type)
            {
            case DataType::Integer: return Data(std::stoll(v));
            case DataType::UInteger: return Data(std::stoull(v));
            case DataType::Double: return Data(std::stod(v));
            default: return Data(v);
            }
        }
        catch (...)
        {
            return Data(v);
        }
    }

    std::string TextLoader::SerializeField(const Data& value) const
    {
        return value.ToString();
    }

    DataObject TextLoader::ParseLineToObject(const std::string& line)
    {
        std::vector<std::string> fields = SplitLine(line);
        DataObject obj;

        if (m_config.hasHeader && !m_headers.empty())
        {
            obj.Resize(m_headers.size());
            for (size_t i = 0; i < m_headers.size() && i < fields.size(); i++)
            {
                DataType type = (i < m_config.columnTypes.size()) ? m_config.columnTypes[i] : DataType::Text;
                obj.Set(i, ParseField(fields[i], type));
            }
        }
        else
        {
            obj.Resize(fields.size());
            for (size_t i = 0; i < fields.size(); i++)
            {
                DataType type = (i < m_config.columnTypes.size()) ? m_config.columnTypes[i] : DataType::Text;
                obj.Set(i, ParseField(fields[i], type));
            }
        }

        return obj;
    }

    std::string TextLoader::ObjectToLine(const DataObject& obj)
    {
        std::stringstream ss;
        for (size_t i = 0; i < obj.Size(); i++)
        {
            if (i > 0) ss << m_config.delimiter;
            ss << SerializeField(obj.Get(i));
        }
        return ss.str();
    }

    std::string TextLoader::ReadLineAtOffset(long long offset)
    {
        if (!m_flow || offset < 0)
            return "";

        m_flow->seek(offset, MF::CFlow::beg);

        std::string line;
        char c;
        while (m_flow->read(c) && c != '\n')
            line += c;

        return line;
    }

}