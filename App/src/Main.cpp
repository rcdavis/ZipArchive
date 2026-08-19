
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "ZipArchive.h"

struct ZipSourceContents {
	std::string filename;
	std::string contents;
};

int main() {
	std::vector<std::pair<std::filesystem::path, std::string>> sources = {
		{ "Hello.txt", "Hello from libzip" },
		{ "Bye.txt", "Bye from libzip" },
		{ "Dir/First.txt", "Text within Dir" }
	};

	ZipArchive archive;
	archive.Open("Output.zip", true);
	archive.AddTextEntries(sources);

	// TODO: Why does GetText not work without closing and reopening the zip archive?
	// Can I not write and then read from the zip archive?
	// Or is it the flag I pass to Open?
	// This looks to be intentional. According to Copilot: When you add files to a ZIP archive
	// using libzip, the changes aren’t immediately committed to disk. Instead, libzip buffers
	// modifications and only writes them out when you call zip_close(). Until then,
	// the archive is in a sort of “pending” state. So if you try to read from the same
	// archive before closing it, you’re essentially accessing an incomplete or inconsistent
	// structure—which can lead to undefined behavior or outright errors.
	archive.Open("Output.zip");

	auto entries = archive.GetEntries();
	for (const auto &entry : entries) {
		auto contents = archive.GetText(entry);
		std::cout << "File" << std::endl;
		std::cout << "  Name: " << entry << std::endl;
		std::cout << "  Contents: " << contents << std::endl;
	}

	archive.Open("Output2.zip", true);
	archive.AddTextEntries(sources);

	std::vector<std::filesystem::path> files = {
		"res/textures/MenuPointer.png",
		"res/textures/MenuPlay.png",
		"res/textures/MenuOptions.png",
		"res/textures/MenuCredits.png",
		"res/textures/MenuExit.png"
	};

	archive.AddFiles(files);

	archive.Open("Output2.zip");

	entries = archive.GetEntries();
	for (const auto &entry : entries) {
		std::cout << "File" << std::endl;
		std::cout << "  Name: " << entry << std::endl;
	}

	std::filesystem::create_directory("Copies");
	for (const auto& textureFile : files) {
		auto contents = archive.GetData(textureFile);
		std::filesystem::path filename = "Copies";
		filename /= textureFile.filename();
		std::ofstream file(filename, std::ios::binary);
		if (file) {
			file.write(contents.data(), contents.size());
		}
	}

	return 0;
}
