
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
	std::vector<ZipSourceContents> sources = {
		{ "Hello.txt", "Hello from libzip" },
		{ "Bye.txt", "Bye from libzip" },
		{ "Dir/First.txt", "Text within Dir" }
	};

	ZipArchive archive;
	archive.Open("Output.zip", true);
	for (const auto &source : sources) {
		archive.AddText(source.filename, source.contents);
	}

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
	for (const auto &source : sources) {
		archive.AddText(source.filename, source.contents);
	}
	archive.AddFile("MenuPointer.png");

	entries = archive.GetEntries();
	for (const auto &entry : entries) {
		std::cout << "File" << std::endl;
		std::cout << "  Name: " << entry << std::endl;
	}

	return 0;
}
