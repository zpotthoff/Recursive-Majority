#include <bitset>
#include <fstream>
#include <string>

std::pair<bool,int> isHardNine(std::string str) {
    int sec1, sec2, sec3;
    std::bitset<9> bitString(str);
    sec1 = (int)bitString[0] + (int)bitString[1] + (int)bitString[2];
    sec2 = (int)bitString[3] + (int)bitString[4] + (int)bitString[5];
    sec3 = (int)bitString[6] + (int)bitString[7] + (int)bitString[8];
    // good for hard dist
    if ((sec1 == 1 || sec1 == 2) && (sec2 == 1 || sec2 == 2) && (sec3 == 1 || sec3 == 2)) {
        // does not fall in hard dist
        if ((sec1 == 1 && sec2 == 1 && sec3 ==1) || (sec1 == 2 && sec2 == 2 && sec3 ==2)) 
            return {false,-1};
        else {
            if (sec1 == sec2) {
                return {true, sec1};
            }
            if (sec1 == sec3) {
                return {true, sec1};
            }
            if (sec2 == sec3) {
                return {true, sec2};
            }
        }
    }
    return {false,-1};
}

int main() {
    //Generates hard distribution on 9 vars
    // std::ofstream file;
    // file.open("hardDistribution9.txt");
    // int sec1, sec2, sec3;
    // for (std::size_t i = 0; i < 512; i++) {
    //     std::bitset<9> bitString(i);
    //     sec1 = (int)bitString[0] + (int)bitString[1] + (int)bitString[2];
    //     sec2 = (int)bitString[3] + (int)bitString[4] + (int)bitString[5];
    //     sec3 = (int)bitString[6] + (int)bitString[7] + (int)bitString[8];
    //     // good for hard dist
    //     if ((sec1 == 1 || sec1 == 2) && (sec2 == 1 || sec2 == 2) && (sec3 == 1 || sec3 == 2)) {
    //         // does not fall in hard dist
    //         if ((sec1 == 1 && sec2 == 1 && sec3 ==1) || (sec1 == 2 && sec2 == 2 && sec3 ==2)) 
    //             continue;
    //         else {
    //             file << bitString.to_string() << "\n";
    //         }
    //     }
    // }

    //Generates hard distribution on 27 vars
    // std::ofstream file;
    // file.open("hardDistribution27.txt");
    // std::pair<bool,int> sec1, sec2, sec3;
    // for (std::size_t i = 0; i < 134217728; i++) {
    //     std::bitset<27> bitString(i);
    //     std::string str = bitString.to_string();
    //     sec1 = isHardNine(str.substr(0,9));
    //     sec2 = isHardNine(str.substr(9,9));
    //     sec3 = isHardNine(str.substr(18,9));
    //     if (sec1.first && sec2.first && sec3.first) {
    //         if (sec1.second == sec2.second && sec2.second == sec3.second) {
    //             continue;
    //         }
    //         else
    //             file << str << "\n";
    //     }
    // }
    // file.close();

    // Generate hard distribution for COMPLETE function given first gate is 1, 9 bit string
    // std::ofstream file;
    // file.open("hardDistributionComplete6.txt");
    // int sec1, sec2;
    // for (std::size_t i = 0; i < 64; i++) {
    //     std::bitset<6> bitString(i);
    //     std::string str = bitString.to_string();
    //     sec1 = (int)bitString[0] + (int)bitString[1] + (int)bitString[2];
    //     sec2 = (int)bitString[3] + (int)bitString[4] + (int)bitString[5];
    //     if ((sec1 == 1 || sec1 == 2) && (sec2 == 1 || sec2 == 2)) {
    //         if (sec1 == 2 && sec2 == 2) { // both gate 1 and gate 2 are 1 in addition to gate 0 preset
    //             continue;
    //         }
    //         else {
    //             file << str << "111" << "\n";
    //         }
    //     }
    // }

    // Generate hard distribution for COMPLETE function given first gate is 1, 27 bit string
    std::ofstream file;
    file.open("hardDistributionComplete18.txt");
    std::pair<bool,int> sec1, sec2;
    for (std::size_t i = 0; i < 262144; i++) {
        std::bitset<18> bitString(i);
        std::string str = bitString.to_string();
        sec1 = isHardNine(str.substr(0,9));
        sec2 = isHardNine(str.substr(9,9));
        if (sec1.first && sec2.first) {
            if (sec1.second == 2 && sec2.second == 2) { // both gate 1 and gate 2 are 1 in addition to gate 0 preset
                continue;
            }
            else {
                file << str << "111111111" << "\n";
            }
        }
    }

    return 0;
}