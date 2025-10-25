#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

class Reader;

using namespace std;

class KiWadEntry {
public:
    uint32_t offset;
    uint32_t size;
    uint32_t zipSize;
    bool zipped;
    uint32_t crc32;
    string fileName;
};

class KiWadFile {
public:
    explicit KiWadFile() {}
    void extractInfo(Reader& reader);
    uint32_t version;
    uint32_t fileCount;
    vector<KiWadEntry> entries;
};

class KiWadHandler {
private:
    vector<char> fileData;
    KiWadFile kiwadFile;

    inline static const string HEADER = "KIWAD";


public:
    string filePath;
    uint32_t getFileCount() const { return kiwadFile.fileCount; }
    uint32_t getVersion() const { return kiwadFile.version; }
    vector<KiWadEntry> getAllEntries() const { return kiwadFile.entries; }
    KiWadEntry getEntry(uint32_t index) const { return kiwadFile.entries[index]; }
    KiWadEntry getEntry(string fileName) const {
        for (const auto& entry : kiwadFile.entries) {
            if (entry.fileName == fileName)
                return entry;
        }

        throw runtime_error("File not found: " + fileName);
	}
    string getFileName() const { 
        size_t pos = filePath.find_last_of("/\\");
        if (pos == string::npos)
            return filePath;
        return filePath.substr(pos + 1); 
	}
    vector<char> getFileData(KiWadEntry entry);

    explicit KiWadHandler();
    bool processFile(const string& path);
};