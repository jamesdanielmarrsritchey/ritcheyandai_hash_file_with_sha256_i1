#include <cryptopp/sha.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <iostream>
#include <string>
#include <getopt.h>
#include <fstream>

int main(int argc, char* argv[]) {
    CryptoPP::SHA256 hash;
    std::string digest;
    std::string filename;

    bool useUppercase = false;

    struct option long_options[] = {
        {"file", required_argument, 0, 'f'},
        {"letter_case", required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "f:l:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
            case 'l':
                if (std::string(optarg) == "upper") {
                    useUppercase = true;
                } else if (std::string(optarg) == "lower") {
                    useUppercase = false;
                } else {
                    std::cerr << "Invalid argument for --letter_case: " << optarg << std::endl;
                    return 1;
                }
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " --file <file to hash> [--letter_case upper|lower]" << std::endl;
                return 1;
        }
    }

    if (filename.empty()) {
        std::cerr << "No file provided to hash. Use --file option." << std::endl;
        return 1;
    }

    try {
        CryptoPP::FileSource f(filename.c_str(), true, 
            new CryptoPP::HashFilter(hash, 
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest), useUppercase)));
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Error reading file: " << e.what() << std::endl;
        return 1;
    }

    std::cout << digest << std::endl;

    return 0;
}