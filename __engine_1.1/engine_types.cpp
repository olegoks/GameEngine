#include "engine_types.hpp"
#include "engine_types.hpp"

const std::string Model::kDefaultFileName{ "Default file name." };
const std::string Model::kDefaultModelName{ "Default model name." };

const size_t AbstractFrame::kBitsPerPixel = sizeof(FramePixel) * std::numeric_limits<uint8_t>::digits;
const RgbColor AbstractFrame::kDefaultColor = { 0, 0, 0, 0 };
const size_t AbstractFrame::kDefaultWidth = 1920;
const size_t AbstractFrame::kDefaultHeight = 1080;

const Keystroke KeystrokesQueue::kDefaultKeystroke = Keystroke{};

std::wstring ConvertStringToWstring(const std::string& str) noexcept(true) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	return converter.from_bytes(str);
}
