#include"Obj.h"
#include<iostream>
#include"Tools/Time.h"
#include<fstream>
#include"Tools/DebuggingTools.h"
#include"Tools/ErrorCodes.h"
#include"Maths/Vector3.h"

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

/*how algorithm works
1. calculate amount of vertexes,normals,connections
2. save vertexes,normals,connections
3. calculate face normals and smoothed normals
*/

//return packed data: Position,SmoothedNormal,FaceNormal,TextureCords
std::vector<float> ReadObjFileType(const char* filePath) {

	std::ifstream fileTextStream;
	fileTextStream.open(filePath);
	if (fileTextStream.fail()) {
		std::string errMsg;
		errMsg += "Failed to open file \"";
		errMsg += filePath;
		errMsg += '\"';
		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
	}
	std::string curLine;

	TimePoint loadStart = Time::GetTimePoint();
	std::cout << "Starting to load \"" << filePath << "\"'s data\n";

	unsigned int fileVertexesAmount = 0;
	unsigned int fileVertexConnectionsAmount = 0;
	unsigned int fileVertexConnectionsLensAmount = 0;
	std::vector<Vector3> fileVertexes;
	std::vector<unsigned int> fileVertexConnections;//how much is there "a/b/c"
	std::vector<unsigned int> fileVertexConnectionsLens;

	//first step
	{
		while (std::getline(fileTextStream, curLine)) {

			std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
			if (name == "v")//vertex
				fileVertexesAmount++;
			else if (name == "f") {//vertexes order
				fileVertexConnectionsLensAmount++;
				for (unsigned int i = 1; i < curLine.length() - 1; i++)
					if (curLine[i] == ' ') fileVertexConnectionsAmount++;
			}
		
		}

		fileVertexes.reserve(fileVertexesAmount);
		fileVertexConnections.reserve(fileVertexConnectionsAmount * 3);
		fileVertexConnectionsLens.reserve(fileVertexConnectionsLensAmount);

	}
	
	fileTextStream.clear();
	fileTextStream.seekg(0, fileTextStream.beg);

	unsigned int trianglesAmount = 0;

	//second step
	{
		while (std::getline(fileTextStream, curLine)) {

			std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
			if (name == "v") {//vertex
				//example of data: "v -0.276388 -0.447220 0.850649"
				unsigned int si = 2;
				float nums[3] = { 0,0,0 };
				unsigned int cordInd = 0;
				for (unsigned int ci = 3;ci < curLine.length()+1; ci++) {
					if (curLine[ci] == ' ' or ci == curLine.length()) {
						nums[cordInd] = std::stof(curLine.substr(si, ci - 1));
						cordInd++;
						si = ci + 1;
					}
				}
				fileVertexes.push_back({ nums[0],nums[1],nums[2] });
			}
			else if (name == "f") {

				/*example of data:
				f 98/2/80 86/4/80 99/18/80
				f 131/22/91 132/60/91 146/60/91 137/64/91
				note that indexes in .obj starts from 1 instead of 0
				*/

				unsigned int si = 1;
				unsigned int len = 0;
				bool waitingForNum = true;
				for (unsigned int ci = 2; ci < curLine.length(); ci++) {
					if (curLine[ci] == ' ') {
						si = ci;
						waitingForNum = true;
					}
					else if (curLine[ci] == '/' and waitingForNum) {
						fileVertexConnections.push_back(std::stoi(curLine.substr(si + 1, ci - si - 1)) - 1);
						waitingForNum = false;
						len++;
					}
				}
				fileVertexConnectionsLens.push_back(len);

				trianglesAmount += len - 2;

			}

		}
	}


	std::vector<float> preparedData;
	preparedData.reserve((3 + 3 + 3 + 2) * trianglesAmount);

	//third step
	{
		
		std::vector<Vector3> smoothedNormals;
		smoothedNormals.resize(fileVertexesAmount);

		unsigned int orderForThree[] = { 0,1,2 };
		unsigned int orderForFour[] = { 0,1,2,2,3,0 };

		unsigned int curConOff = 0;
		for (unsigned int coi = 0; coi < fileVertexConnectionsLens.size(); coi++) {
			unsigned int len = fileVertexConnectionsLens[coi];
			unsigned int curTrianglesAmount = len - 2;
			unsigned int* order = nullptr;
			if (len == 3) order = orderForThree;
			else if (len == 4) order = orderForFour;
			else DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,".OBJ FILE GOT MORE THEN FOUR OR LESS THEN 3 CONNECTIONS",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
			
			Vector3 normal = (fileVertexes[fileVertexConnections[curConOff + order[1]]] - fileVertexes[fileVertexConnections[curConOff + order[0]]]).Cross
			(fileVertexes[fileVertexConnections[curConOff + order[2]]] - fileVertexes[fileVertexConnections[curConOff + order[0]]]);
			Vector3 unitNormal = normal.Unit();

			for (unsigned int to = 0; to < curTrianglesAmount; to++) {
				unsigned int vi[] = {
					fileVertexConnections[curConOff + order[to * 3 + 0]],
					fileVertexConnections[curConOff + order[to * 3 + 1]],
					fileVertexConnections[curConOff + order[to * 3 + 2]],
				};

				for (unsigned int vii = 0; vii < 3; vii++) {
					//pos
					preparedData.push_back(fileVertexes[vi[vii]][0]);
					preparedData.push_back(fileVertexes[vi[vii]][1]);
					preparedData.push_back(fileVertexes[vi[vii]][2]);

					//keep smoothed normal empty for now, first element will be index of smoothed vec
					preparedData.push_back((float)vi[vii]);
					preparedData.push_back(0);
					preparedData.push_back(0);

					//face normal
					preparedData.push_back(unitNormal[0]);
					preparedData.push_back(unitNormal[1]);
					preparedData.push_back(unitNormal[2]);

					smoothedNormals[vi[vii]] += normal;

					//texture cords
					preparedData.push_back(0);
					preparedData.push_back(0);
				}
			}


			curConOff += len;
		}


		for (unsigned int pdi = 3; pdi < preparedData.size(); pdi += 11) {
			unsigned int smoothedNormaInd = (unsigned int)preparedData[pdi];
			Vector3 unitVec = smoothedNormals[smoothedNormaInd].Unit();
			preparedData[pdi + 0] = unitVec[0];
			preparedData[pdi + 1] = unitVec[1];
			preparedData[pdi + 2] = unitVec[2];
		}
	}

	std::cout << "Finished loading model \"" << filePath << "\" in " <<
		Time::GetDuration(loadStart, Time::GetTimePoint()) << " seconds" << std::endl;



	fileTextStream.close();

	return preparedData;
}


//std::vector<float> ReadObjFileType(const char* filePath) {
//	
//	std::ifstream fileTextStream;
//	fileTextStream.open(filePath);
//	if (fileTextStream.fail()) {
//		std::string errMsg;
//		errMsg += "Failed to open file \"";
//		errMsg += filePath;
//		errMsg += '\"';
//		DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,errMsg.c_str(),KURSAVAYAENGINE2_CORE_ERRORS::FAILED_THIRD_PARTY_FUNCTION });
//	}
//	std::string curLine;
//
//
//	std::vector<float> vertexes;
//	std::vector<float> smoothedNormals;
//
//	std::vector<float> filteredData;
//
//	std::cout << "Starting to calculate amount of vertexes/normals/triangles in \"" << filePath << '\"' << std::endl;
//	TimePoint calculatingStart = Time::GetTimePoint();
//
//	{//precalculate vector's lengths
//
//		unsigned int vertexesLen = 0;
//		unsigned int smoothedNormalsLen = 0;
//		unsigned int fileterDataLen = 0;
//
//		while (std::getline(fileTextStream, curLine)) {
//
//			std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
//			if (name == "v") {//vertex
//				vertexesLen += 3;
//			}
//			else if (name == "vn") {//normal
//				smoothedNormalsLen += 3;
//			}
//			else if (name == "f") {//vertexes order
//
//				unsigned int vertexesAmount = 0;
//
//				unsigned int i = 0;
//
//				while (true) {
//					i = findSymbolInd(curLine, ' ', i + 1);
//					if (i==curLine.size()) break;
//					vertexesAmount++;
//				}
//
//				fileterDataLen += (vertexesAmount - 2) * 3 * 11;
//			}
//
//		
//		}
//
//		vertexes.reserve(vertexesLen);
//		smoothedNormals.reserve(smoothedNormalsLen);
//		filteredData.reserve(fileterDataLen);
//	}
//	fileTextStream.clear();
//	fileTextStream.seekg(0, fileTextStream.beg);
//
//	TimePoint loadStart = Time::GetTimePoint();
//
//	std::cout << "Calculations done, model \"" << filePath << "\" have " <<
//		vertexes.capacity() / 3 << " vertexes, " << smoothedNormals.capacity() / 3 <<
//		" smoothed normals, and " << filteredData.capacity() / 11 / 3 <<
//		" triangles in result, calculations finished in " <<
//		Time::GetDuration(calculatingStart, loadStart) << " seconds" << std::endl;
//
//	std::cout << "Starting to load \"" << filePath << "\" data" << std::endl;
//
//	while (std::getline(fileTextStream, curLine)) {
//
//		unsigned int i = 0;
//		std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
//		if (name == "v") {//vertex
//			i = i + 2 - 2;
//			for (unsigned int mi = 0; mi < 3; mi++) {
//				//example of data: "v -0.276388 -0.447220 0.850649"
//				unsigned int si = i + 2;//move to beginning of num
//				unsigned int ei = mini(findSymbolInd(curLine,' ',si), findSymbolInd(curLine, '\n', si));
//				vertexes.push_back(std::stof(curLine.substr(si, ei - si)));
//				i = ei - 1;
//			}
//		}
//		else if (name == "vn") {//normal
//			i = i + 3 - 2;
//			for (unsigned int mi = 0; mi < 3; mi++) {
//				//example of data: "vn 0.7376 -0.4109 -0.5359"
//				unsigned int si = i + 2;//move to beginning of num
//				unsigned int ei = mini(findSymbolInd(curLine, ' ', si), findSymbolInd(curLine, '\n', si));
//
//				smoothedNormals.push_back(std::stof(curLine.substr(si, ei - si)));
//				i = ei - 1;
//			}
//
//		}
//		else if (name == "f") {//vertexes order
//
//			/*example of data:
//			f 98/2/80 86/4/80 99/18/80
//			f 131/22/91 132/60/91 146/60/91 137/64/91
//			note that indexes in .obj starts from 1 instead of 0
//			*/
//			std::vector<int> nums;
//			unsigned int lineEnd = findSymbolInd(curLine, '\n', i);
//			i++;
//			while (i != lineEnd) {
//				unsigned int ce = mini(findSymbolInd(curLine, ' ', i + 1), findSymbolInd(curLine, '/', i + 1));
//				if (ce > lineEnd) ce = lineEnd;
//				if ((ce - i - 1) == 0) nums.push_back(0);
//				else nums.push_back(std::stoi(curLine.substr(i + 1, ce - i - 1)) - 1);
//				i = ce;
//			}
//			
//			{//store results
//				std::vector<unsigned int> order;
//				if (nums.size() / 3 == 3) {
//					order = { 0,1,2 };
//				}
//				else if (nums.size() / 3 == 4) {
//					order = { 0,1,2,2,3,0 };
//				}
//				filteredData.reserve(filteredData.size() + order.size() * (3 + 3 + 2));
//
//				Vector3 p0(vertexes[nums[0] * 3 + 0], vertexes[nums[0] * 3 + 1], vertexes[nums[0] * 3 + 2]);
//				Vector3 p1(vertexes[nums[3] * 3 + 0], vertexes[nums[3] * 3 + 1], vertexes[nums[3] * 3 + 2]);
//				Vector3 p2(vertexes[nums[6] * 3 + 0], vertexes[nums[6] * 3 + 1], vertexes[nums[6] * 3 + 2]);
//				Vector3 normal = (p1 - p0).Cross(p2 - p0).Unit();
//				
//				for (unsigned int ni = 0; ni < order.size(); ni++) {
//
//					for (unsigned int vdi = 0; vdi < 3; vdi++) {
//						filteredData.push_back(vertexes[nums[order[ni] * 3 + 0] * 3 + vdi]);
//					}
//					for (unsigned int vdi = 0; vdi < 3; vdi++) {
//						filteredData.push_back(smoothedNormals[nums[order[ni] * 3 + 2] * 3 + vdi]);
//					}
//					for (unsigned int ndi = 0; ndi < 3; ndi++) {
//						filteredData.push_back(normal[ndi]);
//					}
//					filteredData.push_back(0);
//					filteredData.push_back(0);
//				}
//			}
//
//
//		}
//
//	}
//
//	std::cout << "Finished loading model \"" << filePath << "\" in " <<
//		Time::GetDuration(loadStart, Time::GetTimePoint()) << " seconds" << std::endl;
//
//
//
//	fileTextStream.close();
//
//	return filteredData;
//}