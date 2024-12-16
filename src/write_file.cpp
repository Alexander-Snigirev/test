#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

void debug_read_save_file(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open save file for reading.");
    }

    std::cout << "Reading save file: " << filename << std::endl;

    // 1. Читаем номер текущего раунда
    int roundNumber;
    inFile.read(reinterpret_cast<char*>(&roundNumber), sizeof(roundNumber));
    std::cout << "Round Number: " << roundNumber << std::endl;

    // 2. Читаем корабли игрока
    int numShips;
    inFile.read(reinterpret_cast<char*>(&numShips), sizeof(numShips));
    std::cout << "Player Ships Count: " << numShips << std::endl;

    for (int i = 0; i < numShips; ++i) {
        int id, length, orientation;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
        inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
        inFile.read(reinterpret_cast<char*>(&orientation), sizeof(orientation));
        std::cout << "  Ship ID: " << id
                  << ", Length: " << length
                  << ", Orientation: " << (orientation == 0 ? "Horizontal" : "Vertical") << std::endl;

        std::cout << "  Segments: ";
        for (int j = 0; j < length; ++j) {
            int status;
            inFile.read(reinterpret_cast<char*>(&status), sizeof(status));
            std::cout << (status == 0 ? "Intact" : "Damaged") << " ";
        }
        std::cout << std::endl;
    }

    // 3. Читаем игровое поле игрока
    int width, height;
    inFile.read(reinterpret_cast<char*>(&width), sizeof(width));
    inFile.read(reinterpret_cast<char*>(&height), sizeof(height));
    std::cout << "Player Field Size: " << width << "x" << height << std::endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellStatus, shipId, segmentIndex;
            inFile.read(reinterpret_cast<char*>(&cellStatus), sizeof(cellStatus));
            inFile.read(reinterpret_cast<char*>(&shipId), sizeof(shipId));
            inFile.read(reinterpret_cast<char*>(&segmentIndex), sizeof(segmentIndex));

            std::cout << "Cell (" << x << ", " << y << "): "
                      << "Status: " << cellStatus
                      << ", Ship ID: " << shipId
                      << ", Segment Index: " << segmentIndex << std::endl;
        }
    }

    // 4. Читаем корабли противника
    inFile.read(reinterpret_cast<char*>(&numShips), sizeof(numShips));
    std::cout << "Enemy Ships Count: " << numShips << std::endl;

    for (int i = 0; i < numShips; ++i) {
        int id, length, orientation;
        inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
        inFile.read(reinterpret_cast<char*>(&length), sizeof(length));
        inFile.read(reinterpret_cast<char*>(&orientation), sizeof(orientation));
        std::cout << "  Ship ID: " << id
                  << ", Length: " << length
                  << ", Orientation: " << (orientation == 0 ? "Horizontal" : "Vertical") << std::endl;

        std::cout << "  Segments: ";
        for (int j = 0; j < length; ++j) {
            int status;
            inFile.read(reinterpret_cast<char*>(&status), sizeof(status));
            std::cout << (status == 0 ? "Intact" : "Damaged") << " ";
        }
        std::cout << std::endl;
    }

    // 5. Читаем игровое поле противника
    inFile.read(reinterpret_cast<char*>(&width), sizeof(width));
    inFile.read(reinterpret_cast<char*>(&height), sizeof(height));
    std::cout << "Enemy Field Size: " << width << "x" << height << std::endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellStatus, shipId, segmentIndex;
            inFile.read(reinterpret_cast<char*>(&cellStatus), sizeof(cellStatus));
            inFile.read(reinterpret_cast<char*>(&shipId), sizeof(shipId));
            inFile.read(reinterpret_cast<char*>(&segmentIndex), sizeof(segmentIndex));

            std::cout << "Cell (" << x << ", " << y << "): "
                      << "Status: " << cellStatus
                      << ", Ship ID: " << shipId
                      << ", Segment Index: " << segmentIndex << std::endl;
        }
    }

    // 6. Читаем способности игрока
    int abilityCount;
    inFile.read(reinterpret_cast<char*>(&abilityCount), sizeof(abilityCount));
    std::cout << "Player Ability Count: " << abilityCount << std::endl;

    for (int i = 0; i < abilityCount; ++i) {
        char abilityType;
        inFile.read(&abilityType, sizeof(abilityType));
        std::string abilityName = (abilityType == 'D' ? "Double Damage" :
                                   (abilityType == 'S' ? "Scanner" : "Random Attack"));
        std::cout << "  Ability " << i + 1 << ": " << abilityName << std::endl;
    }

    inFile.close();
    std::cout << "Save file read completed." << std::endl;
}



int main(){
    debug_read_save_file("sv");
}


