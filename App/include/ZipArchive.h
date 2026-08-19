
#pragma once

#include "zip.h"
#include <string>
#include <vector>
#include <filesystem>
#include <span>

class ZipArchive {
public:
	ZipArchive() = default;
	~ZipArchive();

	bool Open(const char* const filepath, bool createNew = false);
	void Close();

	std::vector<std::string> GetEntries();

	std::string GetText(const char* const filepath);
	std::vector<char> GetData(const char* const filepath);

	bool AddTextEntries(const std::vector<std::pair<std::filesystem::path, std::string>>& entries);
	bool AddFiles(std::span<const char* const> files);
	bool AddText(const char* const filepath, const std::string &text);
	bool AddFile(const char* const filepath);

private:
	zip_t* mArchive = nullptr;
};
