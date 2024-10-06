#include"Texture.h"

using namespace Graphics::Abstractions;

TextureClass::TextureClass(DimensionsEnum dimensions, bool keepData, const char* filePath, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Texture(dimensions, filePath, &Size, keepData ? &Data : nullptr, sets, dataSets), Settings(sets), DataSettings(dataSets) {}
TextureClass::TextureClass(DimensionsEnum dimensions, Vector3U pixelsAmount, bool keepData, void* data, const SettingsStruct& sets, const DataSettingsStruct& dataSets) :
	Dimensions(dimensions), Size(pixelsAmount), Texture(dimensions, pixelsAmount, data, sets, dataSets), Settings(sets), DataSettings(dataSets), Data(keepData ? data : nullptr) {}
//TextureClass

TextureClass::TextureClass(RespConstrFlag, const TextureClass& toCopy):
	Texture(RespConstrFlag(), toCopy.Texture), Size(toCopy.Size), Settings(toCopy.Settings), DataSettings(toCopy.DataSettings), Dimensions(toCopy.Dimensions) {
	//if (toCopy.Data!=nullptr)
}
