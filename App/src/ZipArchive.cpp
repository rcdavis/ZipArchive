
#include "ZipArchive.h"

#include <iostream>

ZipArchive::~ZipArchive() {
	Close();
}

bool ZipArchive::Open(const char* const filepath, bool createNew) {
	Close();

	const int openFlags = (createNew) ? (ZIP_CREATE | ZIP_TRUNCATE) : 0;

	int errorCode = 0;
	mArchive = zip_open(filepath, openFlags, &errorCode);
	if (!mArchive) {
		zip_error_t error;
		zip_error_init_with_code(&error, errorCode);
		std::cerr << "Failed to open ZIP archive " << filepath << " with error: " << zip_error_strerror(&error) << std::endl;
		zip_error_fini(&error);
		return false;
	}

	return true;
}

void ZipArchive::Close() {
	if (mArchive) {
		zip_close(mArchive);
		mArchive = nullptr;
	}
}

std::vector<std::string> ZipArchive::GetEntries() {
	std::vector<std::string> entries;

	const zip_int64_t numEntries = zip_get_num_entries(mArchive, 0);
	entries.reserve(numEntries);
	for (zip_int64_t i = 0; i < numEntries; ++i) {
		const char* name = zip_get_name(mArchive, i, 0);
		if (!name)
			continue;

		entries.emplace_back(name);
	}

	return entries;
}

std::string ZipArchive::GetText(const char* const filepath) {
	const zip_int64_t index = zip_name_locate(mArchive, filepath, 0);
	if (index < 0) {
		std::cerr << "Failed to find " << filepath << " within the ZIP archive" << std::endl;
		return {};
	}

	zip_stat_t st;
	zip_stat_init(&st);
	if (zip_stat_index(mArchive, index, 0, &st) != 0) {
		std::cerr << "Failed to get info for file " << filepath << std::endl;
		return {};
	}

	zip_file_t* const zf = zip_fopen_index(mArchive, index, 0);
	if (!zf) {
		std::cerr << "Failed to open file " << filepath << std::endl;
		return {};
	}

	std::string contents(st.size, '\0');
	zip_fread(zf, std::data(contents), st.size);
	zip_fclose(zf);

	return contents;
}

std::vector<char> ZipArchive::GetData(const char* const filepath) {
	const zip_int64_t index = zip_name_locate(mArchive, filepath, 0);
	if (index < 0) {
		std::cerr << "Failed to find " << filepath << " within the ZIP archive" << std::endl;
		return {};
	}

	zip_stat_t st;
	zip_stat_init(&st);
	if (zip_stat_index(mArchive, index, 0, &st) != 0) {
		std::cerr << "Failed to get info for file " << filepath << std::endl;
		return {};
	}

	zip_file_t* const zf = zip_fopen_index(mArchive, index, 0);
	if (!zf) {
		std::cerr << "Failed to open file " << filepath << std::endl;
		return {};
	}

	std::vector<char> contents(st.size);
	zip_fread(zf, std::data(contents), st.size);
	zip_fclose(zf);

	return contents;
}

bool ZipArchive::AddTextEntries(const std::vector<std::pair<std::filesystem::path, std::string>>& entries) {
	for (const auto& entry : entries) {
		if (!AddText(entry.first.c_str(), entry.second))
			return false;
	}
	return true;
}

bool ZipArchive::AddFiles(const std::vector<std::filesystem::path>& files) {
	for (const auto& file : files) {
		if (!AddFile(file.c_str()))
			return false;
	}
	return true;
}

bool ZipArchive::AddText(const char* const filepath, const std::string& text) {
	zip_source_t* const source = zip_source_buffer(mArchive, std::data(text), std::size(text), 0);
	if (!source) {
		std::cerr << "Failed to create zip source for " << filepath << std::endl;
		return false;
	}

	if (zip_file_add(mArchive, filepath, source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) < 0) {
		std::cerr << "Failed to add file " << filepath << " to ZIP archive" << std::endl;
		zip_source_free(source);
		return false;
	}

	return true;
}

bool ZipArchive::AddFile(const char* const filepath) {
	if (!std::filesystem::exists(filepath)) {
		std::cerr << filepath << " could not be found" << std::endl;
		return false;
	}

	// TODO: passing -1 for len is deprecated but currently don't have
	// latest libzip for ZIP_LENGTH_TO_END.
	zip_source_t* const source = zip_source_file(mArchive, filepath, 0, -1);
	if (!source) {
		std::cerr << "Failed to create zip source for " << filepath << std::endl;
		return false;
	}

	if (zip_file_add(mArchive, filepath, source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) < 0) {
		std::cerr << "Failed to add file " << filepath << " to ZIP archive" << std::endl;
		zip_source_free(source);
		return false;
	}

	return true;
}
