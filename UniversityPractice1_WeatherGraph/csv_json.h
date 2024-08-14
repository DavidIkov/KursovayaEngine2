#pragma once
#include<string>
#include<vector>
namespace csv_json {
    struct DeviceDataStruct {
        std::wstring Name;
        struct DateWidthDataStruct {
            struct DateStruct {
                int Year = 0;
                int Month = 0;
                int Day = 0;
                int Hour = 0;
                int Minute = 0;
                int Second = 0;
                
                DateStruct operator-(const DateStruct& odate) const;
                bool operator>(const DateStruct& odate) const;
                bool operator<(const DateStruct& odate) const;
                long long unsigned int GetUniversalTime() const;
            };
            DateStruct Date;

            std::vector<float> Data;
        };
        std::vector<DateWidthDataStruct> Dates;
        struct DataNamesStruct {
            bool IsNumber;
            std::string DataName;
        };
        std::vector<DataNamesStruct> DataNames;
    };

    inline std::vector<DeviceDataStruct> Devices;

    void ReadFromCSV(const wchar_t* filepath);
    void ReadFromJSON(const wchar_t* filepath);
}