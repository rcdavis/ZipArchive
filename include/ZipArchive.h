
#pragma once

#include <zip.h>
#include <string>
#include <vector>
#include <filesystem>

class ZipArchive {
public:
	ZipArchive() = default;
	~ZipArchive();

	bool Open(const std::filesystem::path &filepath, bool createNew = false);
	void Close();

	std::vector<std::string> GetEntries();

	std::string GetText(const std::filesystem::path &filepath);
	std::vector<char> GetData(const std::filesystem::path& filepath);

	bool AddTextEntries(const std::vector<std::pair<std::filesystem::path, std::string>>& entries);
	bool AddFiles(const std::vector<std::filesystem::path>& files);
	bool AddText(const std::filesystem::path &filepath, const std::string &text);
	bool AddFile(const std::filesystem::path &filepath);

private:
	zip_t *mArchive = nullptr;
};
