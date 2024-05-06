#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

// Structure to hold statistics for each symbol
struct SymbolStats {
    unsigned long maxTimeGap = 0;
    int totalVolume = 0;
    long long weightedPriceTotal = 0;
    int maxPrice = 0;
    unsigned long lastTimeStamp = 0;

    // Function to update the statistics with a new trade
    void updateStats(unsigned long timeStamp, int quantity, int price) {
        if (lastTimeStamp != 0) { // Check if it's not the first trade
            unsigned long timeGap = timeStamp - lastTimeStamp;
            if (timeGap > maxTimeGap) {
                maxTimeGap = timeGap;
            }
        }
        lastTimeStamp = timeStamp;
        totalVolume += quantity;
        weightedPriceTotal += (long long)quantity * price;
        if (price > maxPrice) {
            maxPrice = price;
        }
    }

    // Function to get weighted average price
    int getWeightedAveragePrice() const {
        return totalVolume == 0 ? 0 : weightedPriceTotal / totalVolume;
    }
};

int main() {
    std::ifstream infile("input.csv");
    std::ofstream outfile("output.csv");
    std::string line;
    std::map<std::string, SymbolStats> symbolMap;

    // Read each line of the input file
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        unsigned long timeStamp = std::stoul(tokens[0]);
        std::string symbol = tokens[1];
        int quantity = std::stoi(tokens[2]);
        int price = std::stoi(tokens[3]);

        // Update statistics for the symbol
        symbolMap[symbol].updateStats(timeStamp, quantity, price);
    }

    // Output the results to the file
    for (const auto& pair : symbolMap) {
        outfile << pair.first << ',' << pair.second.maxTimeGap << ',' << pair.second.totalVolume << ','
                << pair.second.getWeightedAveragePrice() << ',' << pair.second.maxPrice << std::endl;
    }

    return 0;
}