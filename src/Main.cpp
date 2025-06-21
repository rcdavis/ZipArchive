
#include <iostream>
#include <string>
#include <vector>
#include <zip.h>

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
		"assets/textures/MenuPointer.png"
	};

	archive.AddFile("assets/textures/MenuPointer.png");
	archive.AddFiles(files);

	entries = archive.GetEntries();
	for (const auto &entry : entries) {
		std::cout << "File" << std::endl;
		std::cout << "  Name: " << entry << std::endl;
	}

	return 0;
}
