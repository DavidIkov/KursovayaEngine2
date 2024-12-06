#include"ErrorsSystem.h"
#include<iostream>
#include<Windows.h>

static unsigned int GetAmountOfBytesForCharInUTF8(const char* charPtr) {
	
	const unsigned char* uCharPtr = (const unsigned char*)charPtr;
	unsigned char mask = 0b10000000;
	if (((uCharPtr[0] & mask) >> 7) == 0) return 1;
	else if (((uCharPtr[0] & (mask >> 1)) >> 6) == 0) return 0;
	else if (((uCharPtr[0] & (mask >> 2)) >> 5) == 0) return 2;
	else if (((uCharPtr[0] & (mask >> 3)) >> 4) == 0) return 3;
	else if (((uCharPtr[0] & (mask >> 4)) >> 3) == 0) return 4;

	return 0;
}
static unsigned int GetUnicodeIntFromCharUTF8(const char* charPtr) {

	const unsigned char* uCharPtr = (const unsigned char*)charPtr;

	unsigned char byteMask = 0xff;
	unsigned char mask = 0b10000000;

	unsigned int bytesAmount = GetAmountOfBytesForCharInUTF8(charPtr);
	unsigned int unicodeInd = 0;

	if (bytesAmount == 1) unicodeInd |= uCharPtr[0] & ~mask;
	else unicodeInd |= (byteMask & (uCharPtr[0] << (bytesAmount + 1))) >> (bytesAmount + 1);

	if (bytesAmount == 2) { unicodeInd <<= 6; unicodeInd |= (byteMask & (uCharPtr[1] << 2)) >> 2; }
	if (bytesAmount == 3) { unicodeInd <<= 6; unicodeInd |= (byteMask & (uCharPtr[2] << 2)) >> 2; }
	if (bytesAmount == 4) { unicodeInd <<= 6; unicodeInd |= (byteMask & (uCharPtr[2] << 2)) >> 2; }

	return unicodeInd;
}

KE2::ErrorsSystemNamespace::_MessageCreatingClass& KE2::ErrorsSystemNamespace::_MessageCreatingClass::operator<<(const char* msg) {
	unsigned int lenOfMsgInASCII = 0;
	for (unsigned int i = 0; msg[i] != 0; lenOfMsgInASCII++) {
		i += GetAmountOfBytesForCharInUTF8(msg + i);
	}
	std::string copyOfMsg(lenOfMsgInASCII, '?');
	for (unsigned int srcInd = 0, copyInd = 0; msg[srcInd] != 0; copyInd++) {
		unsigned int bytesAmount = GetAmountOfBytesForCharInUTF8(msg + srcInd);
		if (bytesAmount == 1) copyOfMsg[copyInd] = msg[srcInd];
		else copyOfMsg[copyInd] = '?';//windows console dont work with utf8 as far as i know
		srcInd += bytesAmount;
	}
	Msg += copyOfMsg;
	return *this;
}
KE2::ErrorsSystemNamespace::_MessageCreatingClass& KE2::ErrorsSystemNamespace::_MessageCreatingClass::operator<<(const std::string& msg) {
	operator<<(msg.c_str());
	return *this;
}
#ifdef KE2_Windows
KE2::ErrorsSystemNamespace::_MessageCreatingClass& KE2::ErrorsSystemNamespace::_MessageCreatingClass::operator<<(const wchar_t* msg) {
	unsigned int len = 0; for (; msg[len] != 0; len++);
	std::string newMsg(len, '?');
	for (unsigned int i = 0; i < len; i++)
		if (((unsigned int)msg[i] & (unsigned int)0xff00) != 0) newMsg[i] = '?';
		else newMsg[i] = (unsigned int)msg[i] & (unsigned int)0xff;
	operator<<(newMsg.c_str());
	return *this;
}
KE2::ErrorsSystemNamespace::_MessageCreatingClass& KE2::ErrorsSystemNamespace::_MessageCreatingClass::operator<<(const std::wstring& msg) {
	operator<<(msg.c_str());
	return *this;
}
#endif
void KE2::ErrorsSystemNamespace::_SendWarningClass::operator>>(_EndOfWarningStruct) {
	if (not IgnoreWarnings) {
		std::cout << "KE2 Warning: {" << (Msg.empty() ? "Empty warning" : Msg) << '}' << std::endl;
		Msg.clear();
		if (DebugBreakOnWarnings) {
			MessageBeep(MB_OK);
			__debugbreak();
		}
	}
}
void KE2::ErrorsSystemNamespace::_SendErrorClass::_ProcessError_NoThrow() noexcept {
	std::cout << "KE2 Error: {" << (Msg.empty() ? "Empty error" : Msg) << "}" << std::endl;
	Msg.clear();
	if (DebugBreakOnErrors) {
		MessageBeep(MB_OK);
		__debugbreak();
	}
	if (ThrowingError) 
		std::cout << "KE2 FATAL ERROR: {another unhandled throw happened when previous throw wasnt handled, program will crash after this message}" << std::endl;
}
