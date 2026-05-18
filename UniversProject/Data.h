// Data.h
#pragma once
#include <string>
#include <variant>
#include "Date.h"
#include "Time.h"

namespace DataEngine {

    enum class DataType
    {
        None = 0,
        Integer = 1,
        UInteger = 2,
        Double = 3,
        Text = 4,
        Date = 5,
        DateTime = 6
    };

    enum class CompareResult
    {
        Less = -1,
        Equal = 0,
        Greater = 1,
        Invalid = -2
    };

    enum class DataCompareInfo
    {
        FullEqual = 0,
        OnlyTypeEqual = 1,
        OnlyValueEqual = 2,
        None = 3
    };

    class Data
    {
    private:
        using VariantType = std::variant<
            std::monostate,
            long long,
            unsigned long long,
            double,
            std::string,
            Date,
            Time
        >;

        VariantType m_value;
        DataType m_type;

    public:
        Data();
        Data(long long value);
        Data(unsigned long long value);
        Data(double value);
        Data(const char* value);
        Data(const std::string& value);
        Data(const Date& value);
        Data(const Time& value);
        Data(const Data& other);
        Data(Data&& other) noexcept;

        Data& operator=(const Data& other);
        Data& operator=(Data&& other) noexcept;

        DataType GetType() const;
        static const char* GetTypeName(DataType type);
        const char* GetTypeName() const;

        long long AsInteger() const;
        unsigned long long AsUInteger() const;
        double AsDouble() const;
        std::string AsText() const;
        Date AsDate() const;
        Time AsDateTime() const;

        bool IsNull() const;
        bool IsInteger() const;
        bool IsUInteger() const;
        bool IsDouble() const;
        bool IsNumber() const;
        bool IsText() const;
        bool IsDate() const;
        bool IsDateTime() const;

        void ConvertTo(DataType newType);
        Data GetConvertedTo(DataType newType) const;
        Data To(DataType newType) const;

        CompareResult Compare(const Data& other) const;
        DataCompareInfo CompareDataInfo(const Data& other) const;

        bool operator==(const Data& other) const;
        bool operator!=(const Data& other) const;
        bool operator<(const Data& other) const;
        bool operator<=(const Data& other) const;
        bool operator>(const Data& other) const;
        bool operator>=(const Data& other) const;

        Data Add(const Data& other) const;
        Data Subtract(const Data& other) const;
        Data Multiply(const Data& other) const;
        Data Divide(const Data& other) const;

        std::string ToString() const;
    };

}