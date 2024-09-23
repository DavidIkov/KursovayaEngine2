#include"csv_json.h"
#include"WinOS/FilesSystem.h"
#include"Tools/DebuggingTools.h"
#include<iostream>
#include"WinOS/wchar_t_Operations.h"


csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct::operator-(const DateStruct& odate) const {
	return { Year - odate.Year, Month - odate.Month, Day - odate.Day, Hour - odate.Hour, Minute - odate.Minute, Second - odate.Second };
}
#define dateCheckMacro(name,checkT,checkF,macr) if (name checkT odate.name)return true; else if (name checkF odate.name) return false; else {macr}
bool csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct::operator>(const DateStruct& odate) const {
	dateCheckMacro(Year, > , < , dateCheckMacro(Month, > , < , dateCheckMacro(Day, > , < , dateCheckMacro(Hour, > , < , dateCheckMacro(Minute, > , < , dateCheckMacro(Second, > , < , return true;))))))
}
bool csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct::operator<(const DateStruct& odate) const {
	dateCheckMacro(Year, <, >, dateCheckMacro(Month, <, >, dateCheckMacro(Day, <, >, dateCheckMacro(Hour, <, >, dateCheckMacro(Minute, <, >, dateCheckMacro(Second, <, >, return true;))))))
}
#undef dateCheckMacro

#define llu long long unsigned int
long long unsigned int csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct::GetUniversalTime() const {
	return (llu)Second + 60LLU * ((llu)Minute + 60LLU * ((llu)Hour + 24LLU * ((llu)Day + 30LLU * ((llu)Month + 12LLU * (llu)Year))));
}
#undef llu



static void ReadDate(std::string& text, unsigned int startInd, unsigned int endInd, csv_json::DeviceDataStruct::DateWidthDataStruct::DateStruct* dateStruct) {

	unsigned int minus1 = (unsigned int)text.find_first_of('-', startInd);
	dateStruct->Year = std::stoi(text.substr(startInd, minus1 - startInd));

	unsigned int minus2 = (unsigned int)text.find_first_of('-', minus1 + 1);
	dateStruct->Month = std::stoi(text.substr(minus1 + 1, minus2 - minus1 - 1));

	unsigned int space = (unsigned int)text.find_first_of(' ', minus2 + 1);
	dateStruct->Day = std::stoi(text.substr(minus2 + 1, space - minus2 - 1));

	unsigned int colon1 = (unsigned int)text.find_first_of(':', space + 1);
	dateStruct->Hour = std::stoi(text.substr(space + 1, colon1 - space - 1));

	unsigned int colon2 = (unsigned int)text.find_first_of(':', colon1 + 1);
	dateStruct->Minute = std::stoi(text.substr(colon1 + 1, colon2 - colon1 - 1));

	dateStruct->Second = std::stoi(text.substr(colon2 + 1, endInd - colon2));

}

void csv_json::ReadFromCSV(const wchar_t* filePath) {
	Devices.clear();
	Devices.resize(1);

	FilesSystemNamespace::OpenedFile file(filePath);

	std::string curLine;
	std::wstring curWideLine;


	{//first line
		file.GetNextLine(curLine);
		char_to_wchar(1251, curLine, curWideLine);

		unsigned int first = (unsigned int)curWideLine.find_first_of(L';');
		unsigned int second = (unsigned int)curWideLine.find_first_of(L';', first + 1);

		Devices[0].Name = curWideLine.substr(first + 1, second - first - 1);
	}
	{//second line
		file.GetNextLine(curLine);

		size_t start = curLine.find_first_of(L';');
		while (start != curLine.npos) {
			size_t end = curLine.find_first_of(L';', start + 1);
			if (end != curLine.npos) {
				Devices[0].DataNames.push_back({ true, curLine.substr(start + 1, end - start - 1) });
			}
			start = end;
		}
	}

	while (file.GetNextLine(curLine)) {
		if (Devices[0].Dates.size() == Devices[0].Dates.capacity()) Devices[0].Dates.reserve(Devices[0].Dates.size() + 500);

		DeviceDataStruct::DateWidthDataStruct obj;

		size_t startSemicolon = curLine.find_first_of(';');
		ReadDate(curLine, 0, (unsigned int)startSemicolon - 1, &obj.Date);
		if (obj.Date.Day > 30) {
			DebuggingTools::ManageTheError(DebuggingTools::Error{ DebuggingTools::ErrorTypes::Warning,
"дорогой алексей афанасьев, если вы при проверке достигли этой ошибки это значит что в введеном вами файле был день который > 30, вы сказали не заморачиваться по этому поводу так что вот... дата будет изменена на 30 день",
KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
			obj.Date.Day = 30;
		}

		obj.Data.reserve(Devices[0].DataNames.size());
		unsigned int iter = 0;
		while (startSemicolon != curLine.npos) {
			size_t endSemicolon = curLine.find_first_of(';', startSemicolon + 1);
			if (endSemicolon != curLine.npos) {
				if (Devices[0].DataNames[iter].IsNumber) {
					float num = 0;
					bool isNum = true;
					try
					{
						num = std::stof(curLine.substr(startSemicolon + 1, endSemicolon - startSemicolon - 1));
					}
					catch (std::invalid_argument&)
					{
						isNum = false;
					}
					Devices[0].DataNames[iter].IsNumber = isNum;
					obj.Data.push_back(num);
				}
				else obj.Data.push_back(0);
			}
			startSemicolon = endSemicolon;
			iter++;
		}

		Devices[0].Dates.push_back(obj);


	}

}



static void GetNextNameAndValue(std::string& text, unsigned int start, std::string& name, unsigned int& dataStart, unsigned int& dataEnd, unsigned int& endInd) {
	const char* textPtr = &text[0];
	unsigned int nameS = (unsigned int)text.find_first_of('"', start + 1);
	unsigned int nameE = (unsigned int)text.find_first_of('"', nameS + 1);
	name = text.substr(nameS + 1, nameE - nameS - 1);
	if (textPtr[nameE + 2] == '"') {
		dataStart = nameE + 2;
		dataEnd = (unsigned int)text.find_first_of('"', dataStart + 1);
		endInd = dataEnd + 1;
	}
	else if (textPtr[nameE + 2] == '{') {
		unsigned int openedBrackets = 1;
		unsigned int curInd = nameE + 3;
		while (curInd < text.length()) {
			if (textPtr[curInd] == '{') openedBrackets++;
			else if (textPtr[curInd] == '}') {
				openedBrackets--;
				if (openedBrackets==0) break;
			}
			curInd++;
		}
		dataStart = nameE + 2;
		dataEnd = curInd;
		endInd = dataEnd + 1;
	}
	else {
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,"FAILED TO READ VALUE IN JSON",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
	}

}
void csv_json::ReadFromJSON(const wchar_t* filePath) {
	Devices.clear();
	
	FilesSystemNamespace::OpenedFile file(filePath);


	std::string text;
	file.GetNextLine(text);

	
	std::string recordName; unsigned int dataStart; unsigned int dataEnd; unsigned int startInd = 0;
	while (startInd != text.length() - 1) {
		GetNextNameAndValue(text, startInd, recordName, dataStart, dataEnd, startInd);


		unsigned int dataInd = dataStart;
		std::string date; unsigned int dateStart; unsigned int dateEnd;
		GetNextNameAndValue(text, dataInd, date, dateStart, dateEnd, dataInd); date = text.substr(dateStart + 1, dateEnd - dateStart - 1);

		std::string serverName; unsigned int nameStart; unsigned int nameEnd;
		GetNextNameAndValue(text, dataInd, serverName, nameStart, nameEnd, dataInd); serverName = text.substr(nameStart + 1, nameEnd - nameStart - 1);
		std::wstring wideServerName; char_to_wchar(1251, serverName, wideServerName);

		std::string serverSerial; unsigned int serialStart; unsigned int serialEnd;
		GetNextNameAndValue(text, dataInd, serverSerial, serialStart, serialEnd, dataInd); serverSerial = text.substr(serialStart + 1, serialEnd - serialStart - 1);
		std::wstring wideServerSerial; char_to_wchar(1251, serverSerial, wideServerSerial);//just to concatenate serial and name

		wideServerName += L'(' + wideServerSerial + L')';


		unsigned int foundDeviceInd = (unsigned int)Devices.size();
		for (unsigned int i = 0; i < Devices.size(); i++) {
			if (Devices[i].Name == wideServerName) {
				foundDeviceInd = i;
				break;
			}
		}

		
		bool recordNames = false;
		if (foundDeviceInd == Devices.size()) {
			recordNames = true;
			if (Devices.size() == Devices.capacity()) Devices.reserve(Devices.size() + 20);
			Devices.emplace_back();
			Devices[foundDeviceInd].Name = wideServerName;
		}

		csv_json::DeviceDataStruct& curDevice = Devices[foundDeviceInd];




		std::string recordedDeviceData; unsigned int recordedDeviceDataStart; unsigned int recordedDeviceDataEnd;
		GetNextNameAndValue(text, dataInd, recordedDeviceData, recordedDeviceDataStart, recordedDeviceDataEnd, dataInd); 
		recordedDeviceData = text.substr(recordedDeviceDataStart + 1, recordedDeviceDataEnd - recordedDeviceDataStart - 1);

		
		if (curDevice.Dates.size() == curDevice.Dates.capacity()) curDevice.Dates.reserve(curDevice.Dates.size() + 500);

		csv_json::DeviceDataStruct::DateWidthDataStruct dateData;
		ReadDate(text, dateStart + 1, dateEnd - 1, &dateData.Date);
	
		if (dateData.Date.Day > 30) {
			DebuggingTools::ManageTheError(DebuggingTools::Error{ DebuggingTools::ErrorTypes::Warning,
"дорогой алексей афанасьев, если вы при проверке достигли этой ошибки это значит что в введеном вами файле был день который > 30, вы сказали не заморачиваться по этому поводу так что вот... дата будет изменена на 30 день",
KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
			dateData.Date.Day = 30;
		}

		if (not recordNames) dateData.Data.reserve(curDevice.DataNames.size());

		std::string recordedDeviceDataNameByDate; 
		unsigned int recordedDeviceDataByDateStart; unsigned int recordedDeviceDataByDateEnd;
		unsigned int repeat = 0;
		while (recordedDeviceDataStart != recordedDeviceDataEnd) {
			GetNextNameAndValue(text, recordedDeviceDataStart, recordedDeviceDataNameByDate, recordedDeviceDataByDateStart, recordedDeviceDataByDateEnd, recordedDeviceDataStart);

			if (recordNames) curDevice.DataNames.push_back({ true,recordedDeviceDataNameByDate });

			if (curDevice.DataNames[repeat].IsNumber) {
				float num = 0;
				bool isNum = true;
				try
				{
					num = std::stof(text.substr(recordedDeviceDataByDateStart + 1, recordedDeviceDataByDateEnd - recordedDeviceDataByDateStart - 1));
				}
				catch (std::invalid_argument&)
				{
					isNum = false;
				}
				curDevice.DataNames[repeat].IsNumber = isNum;
				dateData.Data.push_back(num);
			}
			else dateData.Data.push_back(0);
			
			repeat++;
		}

		curDevice.Dates.push_back(dateData);


	}
	
}