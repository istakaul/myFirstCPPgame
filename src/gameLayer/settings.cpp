#include "settings.h"
#include <fstream>
#include <filesystem>
#include <raymath.h>
#include <nlohmann/json.hpp>

Settings settings;

Settings settingsOld;

Settings &getSettings()
{
	return settings;
}

using Json = nlohmann::json;

void saveSettings()
{
	Json j;

	j["musicVolume"] = settings.musicVolume;
	j["masterVolume"] = settings.masterVolume;
	j["soundsVolume"] = settings.soundsVolume;

	std::error_code error;
	std::filesystem::create_directory(RESOURCES_PATH "../settings", error);

	/*--------------------- BINARY WAY ------------------------------------------*/
	//std::ofstream f(RESOURCES_PATH "../settings/settings.txt", std::ios::binary);
	//auto bytes = Json::to_cbor(j);
	//f.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
	/*----------------------------------------------------------------------------*/
	std::ofstream f(RESOURCES_PATH "../settings/settings.txt");

	f << j.dump(2); // 2 here says how many spaces to indent when writting the data
	f.close();
}

void loadSettings()
{
	// we reset all settings first
	// it's possible for some data to be missing
	settings = {};

	/*--------------------- BINARY WAY ------------------------------------------*/
	//std::ifstream f(RESOURCES_PATH "../settings/settings.txt", std::ios::binary);
	/*----------------------------------------------------------------------------*/
	std::ifstream f(RESOURCES_PATH "../settings/settings.txt");
	if (!f.is_open()) {
		return;
	}

	/*--------------------- BINARY WAY ------------------------------------------*/
	//std::vector<std::uint8_t> bytes((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	//Json j = Json::from_cbor(bytes, /*strict=*/false, /*allow_exceptions=*/false);
	/*----------------------------------------------------------------------------*/

	Json j;
	j = Json::parse(f, nullptr, /*allow_exceptions*/false);

	if (j["musicVolume"].is_number()) { settings.musicVolume = j["musicVolume"].get<float>(); }
	if (j["masterVolume"].is_number()) { settings.masterVolume = j["masterVolume"].get<float>(); }
	if (j["soundsVolume"].is_number()) { settings.soundsVolume = j["soundsVolume"].get<float>(); }

	settings.sanitize();

	f.close();
}

void updateSettings()
{
	if (settings != settingsOld) {
		saveSettings();
	}

	settingsOld = settings;
}

