#include <cstdio>
#include <vector>
#include <iostream>

// RapidJSON headers
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

// 64 KB buffer size aligns perfectly with Windows OS file system paging limits
constexpr size_t IO_BUFFER_SIZE = 65536;

bool ReadJsonExtremelyFast(const char* filepath, rapidjson::Document & doc) {
    // Windows low-level C file streaming
    FILE* fp = nullptr;
    fopen_s(&fp, filepath, "rb");
    if (!fp) {
        std::cerr << "Error: Could not open file for reading: " << filepath << "\n";
        return false;
    }

    std::vector<char> readBuffer(IO_BUFFER_SIZE);
    rapidjson::FileReadStream is(fp, readBuffer.data(), readBuffer.size());

    // Parse stream in-place using standard memory layouts
    doc.ParseStream(is);
    std::fclose(fp);

    if (doc.HasParseError()) {
        std::cerr << "JSON Parse Error: " << doc.GetParseError() << "\n";
        return false;
    }
    return true;
}

bool WriteJsonExtremelyFast(const char* filepath, const rapidjson::Document & doc) {
    FILE* fp = nullptr;
    fopen_s(&fp, filepath, "wb");
    if (!fp) {
        std::cerr << "Error: Could not open file for writing: " << filepath << "\n";
        return false;
    }

    std::vector<char> writeBuffer(IO_BUFFER_SIZE);
    rapidjson::FileWriteStream os(fp, writeBuffer.data(), writeBuffer.size());

    // Compact Writer avoids whitespace generation overhead
    rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
    bool success = doc.Accept(writer);

    std::fclose(fp);
    return success;
}

int main() {
    // Files will look inside your project execution directory
    const char* input_file = "input.json";
    const char* output_file = "output.json";

    rapidjson::Document doc;

    std::cout << "[Visual Studio 2022] Initialising fast read...\n";
    if (ReadJsonExtremelyFast(input_file, doc)) {

        // Quick high-speed DOM modification example
        auto& allocator = doc.GetAllocator();
        if (doc.IsObject()) {
            doc.AddMember("vs2022_optimized", true, allocator);
        }

        std::cout << "[Visual Studio 2022] Initialising fast write...\n";
        if (WriteJsonExtremelyFast(output_file, doc)) {
            std::cout << "Success! High-performance operations completed successfully.\n";
        }
    }

    return 0;
}