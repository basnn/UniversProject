// Loader.h
#pragma once
#include <vector>
#include "DataObject.h"
#include "MFP/MFI.h"
#include <memory>
#include <vector>
#include<MFP/MFWR.h>

namespace DataEngine {

    class Loader 
    {
    public:
        virtual ~Loader() = default;

        virtual DataObject ReadNext() = 0;
        virtual std::vector<DataObject> ReadNext(size_t count) = 0;
        virtual bool HasNext() const = 0;
        virtual void Reset() = 0;

        virtual bool WriteNext(const DataObject& obj) = 0;
        virtual bool WriteNext(const std::vector<DataObject>& objects) = 0;

        virtual DataObject ReadAt(size_t index) = 0;
        virtual std::vector<DataObject> ReadRange(size_t start, size_t end) = 0;

        virtual bool WriteAt(size_t index, const DataObject& obj) = 0;
        virtual bool WriteRange(size_t start, const std::vector<DataObject>& objects) = 0;

        virtual bool DeleteAt(size_t index) = 0;
        virtual bool DeleteRange(size_t start, size_t end) = 0;

        virtual size_t GetObjectCount() const = 0;

    protected:
        Loader() = default;
    };






    struct TextImportConfig
    {
        std::string source;              // путь к файлу
        char delimiter = ',';
        bool hasHeader = true;
        int skipLines = 0;
        std::vector<DataType> columnTypes;
    };

    class TextLoader : public Loader
    {
    private:
        MF::CFlow* m_flow;
        TextImportConfig m_config;
        std::vector<std::string> m_headers;
        std::vector<long long> m_lineOffsets;
        size_t m_currentPosition;
        size_t m_objectCount;
        bool m_isOwner;

    public:
        TextLoader();
        explicit TextLoader(const TextImportConfig& config);
        ~TextLoader();

        void SetConfig(const TextImportConfig& config);
        void SetFlow(MF::CFlow* flow, bool takeOwnership = false);

        // Инициализация
        bool BuildIndex();

        // Loader implementation
        DataObject ReadNext() override;
        std::vector<DataObject> ReadNext(size_t count) override;
        bool HasNext() const override;
        void Reset() override;

        bool WriteNext(const DataObject& obj) override;
        bool WriteNext(const std::vector<DataObject>& objects) override;

        DataObject ReadAt(size_t index) override;
        std::vector<DataObject> ReadRange(size_t start, size_t end) override;

        bool WriteAt(size_t index, const DataObject& obj) override;
        bool WriteRange(size_t start, const std::vector<DataObject>& objects) override;

        bool DeleteAt(size_t index) override;
        bool DeleteRange(size_t start, size_t end) override;

        size_t GetObjectCount() const override;

    private:
        std::vector<std::string> SplitLine(const std::string& line) const;
        std::string Trim(const std::string& str) const;
        Data ParseField(const std::string& value, DataType type) const;
        std::string SerializeField(const Data& value) const;
        DataObject ParseLineToObject(const std::string& line);
        std::string ObjectToLine(const DataObject& obj);
        std::string ReadLineAtOffset(long long offset);
    };









}








