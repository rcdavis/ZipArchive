
#include "ZipArchive.h"

#include <iostream>
#include <fstream>

ZipArchive::~ZipArchive() {
    Close();
}

bool ZipArchive::Open(const std::filesystem::path& filepath, bool createNew) {
    Close();

    int openFlags = 0;
    if (createNew)
        openFlags = ZIP_CREATE | ZIP_TRUNCATE;

    int errorCode = 0;
    mArchive = zip_open(filepath.c_str(), openFlags, &errorCode);
    if (!mArchive) {
        std::cerr << "Failed to open ZIP archive " << filepath << std::endl;
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

    zip_int64_t numEntries = zip_get_num_entries(mArchive, 0);
    for (zip_int64_t i = 0; i < numEntries; ++i) {
        const char* name = zip_get_name(mArchive, i, 0);
        if (!name)
            continue;

        entries.emplace_back(name);
    }

    return entries;
}

std::string ZipArchive::GetText(const std::filesystem::path& filepath) {
    zip_int64_t index = zip_name_locate(mArchive, filepath.c_str(), 0);
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

    // TODO: Why does this fail when index looks to be correct?
    zip_file_t* zf = zip_fopen_index(mArchive, index, 0);
    if (!zf) {
        std::cerr << "Failed to open file " << filepath << std::endl;
        return {};
    }

    std::string contents(st.size, '\0');
    zip_fread(zf, std::data(contents), st.size);
    zip_fclose(zf);

    return contents;
}

bool ZipArchive::AddText(const std::filesystem::path& filepath, const std::string& text) {
    zip_source_t* source = zip_source_buffer(mArchive, std::data(text), std::size(text), 0);
    if (!source) {
        std::cerr << "Failed to create zip source for " << filepath << std::endl;
        return false;
    }

    if (zip_file_add(mArchive, filepath.c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) < 0) {
        std::cerr << "Failed to add file " << filepath << " to ZIP archive" << std::endl;
        zip_source_free(source);
        return false;
    }

    return true;
}

bool ZipArchive::AddFile(const std::filesystem::path& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open " << filepath << std::endl;
        return false;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    zip_source_t* source = zip_source_buffer(mArchive, std::data(buffer), std::size(buffer), 0);
    if (!source) {
        std::cerr << "Failed to create zip source for " << filepath << std::endl;
        return false;
    }

    if (zip_file_add(mArchive, filepath.c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8) < 0) {
        std::cerr << "Failed to add file " << filepath << " to ZIP archive" << std::endl;
        zip_source_free(source);
        return false;
    }

    return true;
}
