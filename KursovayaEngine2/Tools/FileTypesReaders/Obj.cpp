#include"Obj.h"
#include<iostream>
#include"Tools/Time.h"

static void skipToNextLine(const std::string& txt, unsigned int& startInd) {
	while (txt[startInd] != '\n') {
		if (txt[startInd] == '\0') return;
		startInd++;
	}
	startInd++;
}
static unsigned int findSymbolInd(const std::string& txt, const char symbol, const unsigned int startInd) {
	unsigned int i = startInd;
	while (txt[i] != symbol) {
		if (txt[i] == '\0') return txt.size();
		i++;
	}
	return i;
}
static int mini(int a1, int a2) {
	return (a1 > a2) ? a2 : a1;
}

//TODO: add exceptions and some safety
std::vector<float> ReadObjFileType(const std::string& fileText) {

	unsigned int i = 0;
	std::vector<float> vertexes;
	std::vector<float> normals;

	std::vector<float> filteredData;


	{//precalculate vector's lengths
		unsigned int vertexesLen = 0;
		unsigned int normalsLen = 0;
		unsigned int fileterDataLen = 0;
		while (fileText[i] != '\0') {
			std::string name = fileText.substr(i, findSymbolInd(fileText, ' ', i) - i);
			if (name == "v") {//vertex
				i = i + 2 - 2;
				for (unsigned int mi = 0; mi < 3; mi++) {
					//example of data: "v -0.276388 -0.447220 0.850649"
					unsigned int si = i + 2;//move to beginning of num
					unsigned int ei = mini(findSymbolInd(fileText, ' ', si), findSymbolInd(fileText, '\n', si));
					vertexesLen++;
					i = ei - 1;
				}
				i += 2;
			}
			else if (name == "vn") {//normal
				i = i + 3 - 2;
				for (unsigned int mi = 0; mi < 3; mi++) {
					//example of data: "vn 0.7376 -0.4109 -0.5359"
					unsigned int si = i + 2;//move to beginning of num
					unsigned int ei = mini(findSymbolInd(fileText, ' ', si), findSymbolInd(fileText, '\n', si));
					normalsLen++;
					i = ei - 1;
				}
				i += 2;
			}
			else if (name == "f") {//vertexes order
				/*example of data:
				f 98/2/80 86/4/80 99/18/80
				f 131/22/91 132/60/91 146/60/91 137/64/91

				note that indexes in .obj starts from 1 instead of 0
				*/
				std::vector<int> nums;
				unsigned int lineEnd = findSymbolInd(fileText, '\n', i);

				i++;

				while (i != lineEnd) {
					unsigned int ce = mini(findSymbolInd(fileText, ' ', i + 1), findSymbolInd(fileText, '/', i + 1));
					if (ce > lineEnd) ce = lineEnd;
					nums.push_back(std::stoi(fileText.substr(i + 1, ce - i - 1)) - 1);
					i = ce;
				}

				i++;


				{//store results
					std::vector<unsigned int> order;
					if (nums.size() / 3 == 3) {
						order = { 0,1,2 };
					}
					else if (nums.size() / 3 == 4) {
						order = { 0,1,3,1,2,3 };
					}
					filteredData.reserve(filteredData.size() + order.size() * (3 + 3 + 2));
					for (unsigned int ni = 0; ni < order.size(); ni++) {
						for (unsigned int vdi = 0; vdi < 3; vdi++) {
							fileterDataLen++;
						}
						for (unsigned int ndi = 0; ndi < 3; ndi++) {
							fileterDataLen++;
						}
						fileterDataLen++;
						fileterDataLen++;
					}
				}

			}
			else {
				skipToNextLine(fileText, i);
			}
		}

		i = 0;
		vertexes.reserve(vertexesLen);
		normals.reserve(normalsLen);
		filteredData.reserve(fileterDataLen);
	}
	while (fileText[i]!='\0') {
		std::string name = fileText.substr(i, findSymbolInd(fileText, ' ', i) - i);
		if (name=="v") {//vertex
			i = i + 2 - 2;
			for (unsigned int mi = 0; mi < 3; mi++) {
				//example of data: "v -0.276388 -0.447220 0.850649"
				unsigned int si = i + 2;//move to beginning of num
				unsigned int ei = mini(findSymbolInd(fileText,' ',si), findSymbolInd(fileText, '\n', si));
				vertexes.push_back(std::stof(fileText.substr(si, ei - si)));
				i = ei - 1;
			}
			i += 2;
		}
		else if (name == "vn") {//normal
			i = i + 3 - 2;
			for (unsigned int mi = 0; mi < 3; mi++) {
				//example of data: "vn 0.7376 -0.4109 -0.5359"
				unsigned int si = i + 2;//move to beginning of num
				unsigned int ei = mini(findSymbolInd(fileText, ' ', si), findSymbolInd(fileText, '\n', si));
				normals.push_back(std::stof(fileText.substr(si, ei - si)));
				i = ei - 1;
			}
			i += 2;
		}
		else if (name == "f") {//vertexes order
			/*example of data:
			f 98/2/80 86/4/80 99/18/80
			f 131/22/91 132/60/91 146/60/91 137/64/91

			note that indexes in .obj starts from 1 instead of 0
			*/
			std::vector<int> nums;
			unsigned int lineEnd = findSymbolInd(fileText, '\n', i);

			i++;

			while (i != lineEnd) {
				unsigned int ce = mini(findSymbolInd(fileText, ' ', i + 1), findSymbolInd(fileText, '/', i + 1));
				if (ce > lineEnd) ce = lineEnd;
				nums.push_back(std::stoi(fileText.substr(i + 1, ce - i - 1)) - 1);
				i = ce;
			}
			
			i++;


			{//store results
				std::vector<unsigned int> order;
				if (nums.size() / 3 == 3) {
					order = { 0,1,2 };
				}
				else if (nums.size() / 3 == 4) {
					order = { 0,1,3,1,2,3 };
				}
				filteredData.reserve(filteredData.size() + order.size() * (3 + 3 + 2));
				for (unsigned int ni = 0; ni < order.size(); ni++) {
					for (unsigned int vdi = 0; vdi < 3; vdi++) {
						filteredData.push_back(vertexes[nums[order[ni] * 3 + 0] * 3 + vdi]);
					}
					for (unsigned int ndi = 0; ndi < 3; ndi++) {
						filteredData.push_back(normals[nums[order[ni] * 3 + 2] * 3 + ndi]);
					}
					filteredData.push_back(0);
					filteredData.push_back(0);
				}
			}

		}
		else {
			skipToNextLine(fileText, i);
		}
	}
	
	return filteredData;
}