#include "KiWadHandler.hpp"
#include "Reader.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstdio>
#include <zlib.h>
#include <utility>

using namespace std;

bool decompress_zlib(const std::vector<char>& compressed, std::vector<char>& decompressed)
{
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(compressed.data()));
    stream.avail_in = static_cast<uInt>(compressed.size());

    // initialize
    if (inflateInit2(&stream, MAX_WBITS) != Z_OK)
        return false;

    const size_t CHUNK_SIZE = 4096;
    std::vector<char> buffer(CHUNK_SIZE);

    int ret;
    do {
        stream.next_out = reinterpret_cast<Bytef*>(buffer.data());
        stream.avail_out = static_cast<uInt>(buffer.size());

        ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&stream);
            return false;
        }

        size_t bytes_decompressed = buffer.size() - stream.avail_out;
        decompressed.insert(decompressed.end(), buffer.begin(), buffer.begin() + bytes_decompressed);

    } while (ret != Z_STREAM_END);

    inflateEnd(&stream);
    return true;
}

KiWadHandler::KiWadHandler() {}

bool KiWadHandler::processFile(const string& path) {

    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file");
    }

    // Get file size
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw runtime_error("Error reading file");
    }

    // Check header
    if (size < HEADER.size())
        throw runtime_error("Size is smaller than header");

    fileData = move(buffer);

    Reader r(&fileData);
    vector<char> readHeader = r.read(HEADER.size());
	string headerStr(readHeader.begin(), readHeader.end());

	if (headerStr != HEADER)
        throw runtime_error(headerStr + " doesn't match: " + HEADER);

	KiWadFile _kiwadFile;
    _kiwadFile.extractInfo(r);
    kiwadFile = move(_kiwadFile);
    filePath = path;

    return true;
}

// KiWadFile implementation
void KiWadFile::extractInfo(Reader& reader) {
    version = reader.read<uint32_t>();
    fileCount = reader.read<uint32_t>();

    printf("WAD Version: %u\n", version);
    printf("Number of files: %u\n", fileCount);

    if (version >= 2) reader.read(1);

    for (int i = 0; i < fileCount; i++) {
        KiWadEntry entry;

		entry.offset = reader.read<uint32_t>();
        entry.size = reader.read<uint32_t>();
        entry.zipSize = reader.read<uint32_t>();
		entry.zipped = reader.read<bool>();
        entry.crc32 = reader.read<uint32_t>();
        entry.fileName = reader.readString(reader.read<uint32_t>());
        
        printf("File %d: %s, Offset: %u, Size: %u, Zipped: %s\n",
               i, entry.fileName.c_str(), entry.offset, entry.size,
			entry.zipped ? "Yes" : "No");

        entries.push_back(entry);   
    }
}

vector<char> KiWadHandler::getFileData(KiWadEntry entry) {
	vector<char> entryData = Reader(&fileData).readKiWadEntry(entry);
    
    if (!entry.zipped)
        return entryData;

    std::vector<char> decompressedData;
    if (!decompress_zlib(entryData, decompressedData))
        throw runtime_error("Failed to decompress data.");

    printf("Decompression successful for %s, size: %zu\n", entry.fileName.c_str(), decompressedData.size());
    return decompressedData;
}


