#include <cstdio>
#include <vector>
#include <iostream>
#include <utility>

// RapidJSON headers
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "Town.h"
#include "Node.h"
#include "Routes.h"

// 64 KB buffer size aligns perfectly with Windows OS file system paging limits
constexpr size_t IO_BUFFER_SIZE = 65536;
using namespace std;
std::vector<std::tuple<std::string, int, int>> RT;



int LoadResource() {
    RT.emplace_back("Wheat", 2, 4);
	RT.emplace_back("Corn", 3, 5);
	RT.emplace_back("Rice", 1, 3);
	RT.emplace_back("Wood", 3, 5);
	RT.emplace_back("Stone", 3, 5);
	RT.emplace_back("Clay", 4, 6);
	RT.emplace_back("Fish", 4, 6);
	RT.emplace_back("Sheep", 5, 8);
	RT.emplace_back("Ore", 6);
	return 0;
}

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

int Actions(int i) {
    switch (i) {
    case 1:
        Travel;
    case 2:
        Buy;
    case 3:
        Sell;
    case 4:
        Craft;
    case 5:
        Build;
    case 6:
        Gather;
    case 7:
        Upkeep;
    default:
        return 0;
    }
}

int Travel() {
    // Implement travel logic here

    return 0;
}

int Buy() {
    // Implement buy logic here
    return 0;
}

int Sell() {
    // Implement sell logic here

    return 0;
}

int Craft() {
    // Implement craft logic here
    return 0;
}

int Build() {
    // Implement build logic here
    return 0;
}

int Gather() {
    // Implement gather logic here
    return 0;
}

int Upkeep() {
    // Implement upkeep logic here
    return 0;
}

vector<pair<string, Town>> TownsData = { { "Demensia",{} }, {"Noxus",{}}, {"Piltover",{}} };
vector<pair<string, Node>> NodeData = { {"N1",{}},{"N2",{}},{"N3",{}} };
vector<Routes> RouteData;

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
            for (pair<string, Town> pair : TownsData) {
                pair.second.rate = doc["towns"][pair.first.data()]["Production"]["rate"].GetInt();
                pair.second.ProductionRecources.emplace_back(doc["towns"][pair.first.data()]["Production"]["resources"].GetArray());
                pair.second.affinities.emplace_back(doc["towns"][pair.first.data()]["affinities"].GetArray());
                pair.second.ItemRates.emplace_back(doc["towns"][pair.first.data()]["item-rates"].GetArray());
                pair.second.Enteloot.first = doc["towns"][pair.first.data()]["enteloot"]["rate"].GetInt();
                pair.second.Enteloot.second = doc["towns"][pair.first.data()]["enteloot"]["amount"].GetInt();
            }
            for (pair<string, Node>pair : NodeData) {
                pair.second.type = doc["nodes"][pair.first.data()]["type"].GetString();
                pair.second.RecorceProduced.first = doc["nodes"][pair.first.data()]["recource"].GetString();
                pair.second.RecorceProduced.second = doc["nodes"][pair.first.data()]["yeild"].GetInt();
                pair.second.gatherTime = doc["nodes"][pair.first.data()]["gather-Time"].GetInt();
            }
            for (const auto& route : doc["routes"].GetArray()) {
                Routes var;
                var.Cost = route["toll"].GetInt();
                var.distance = route["weight"].GetInt();
                var.EndPoints.first = route["between"].GetArray()[0].GetString();
                var.EndPoints.second = route["between"].GetArray()[0].GetString();
          }
            doc.Clear; 

            
        }
            
        std::cout << "[Visual Studio 2022] Initialising fast write...\n";
        if (WriteJsonExtremelyFast(output_file, doc)) {
            std::cout << "Success! High-performance operations completed successfully.\n";
        }

    }

    return 0;
}