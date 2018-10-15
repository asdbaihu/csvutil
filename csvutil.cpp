#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> tokenize(std::string delimiter, std::string line) {
    std::vector<std::string> tokens;
    
    size_t i = 0;
    std::string token;
    
    while ((i = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, i);
        tokens.push_back(token);
        line.erase(0, i + delimiter.length());
    }
    tokens.push_back(line);
    
    return tokens;
}

std::string join(std::string delimiter, std::vector<std::string> tokens) {
    std::string joined;
    
    for (std::vector<std::string>::const_iterator token = tokens.begin(); token != tokens.end(); ++token) {
        joined += *token;
        if (token != tokens.end() - 1) {
            joined += delimiter;
        }
    }
    
    return joined;
}

int main(int argc, char **argv) {
    std::ifstream fin;
    std::ofstream fout;
    
    std::istream* in = &std::cin;
    std::ostream* out = &std::cout;
    
    std::string input_delimiter = ",";
    std::string output_delimiter = ",";
    
    bool remove_header = false;
    bool add_header = false;
    std::string header;
    
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-i") {
            ++i;
            if (i < argc) {
                fin.open(argv[i]);
                if (!fin.is_open()) {
                    std::cerr << "Failed to open input file" << std::endl;
                    return -1;
                }
                in = &fin;
            } else {
                std::cerr << "Missing argument after '-i'" << std::endl;
                return -1;
            }
        }
        else if (std::string(argv[i]) == "-o") {
            ++i;
            if (i < argc) {
                fout.open(argv[i]);
                if (!fout.is_open()) {
                    std::cerr << "Failed to open output file" << std::endl;
                    return -1;
                }
                out = &fout;
            } else {
                std::cerr << "Missing argument after '-o'" << std::endl;
                return -1;
            }
        }
        else if (std::string(argv[i]) == "-id") {
            ++i;
            if (i < argc) {
                input_delimiter = argv[i];
            } else {
                std::cerr << "Missing argument after '-id'" << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-od") {
            ++i;
            if (i < argc) {
                output_delimiter = argv[i];
            } else {
                std::cerr << "Missing argument after '-od'" << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-rh") {
            remove_header = true;
        } else if (std::string(argv[i]) == "-ah") {
            add_header = true;
            ++i;
            if (i < argc) {
                header = argv[i];
            } else {
                std::cerr << "Missing argument after '-ah'" << std::endl;
                return -1;
            }
        } else {
            std::cerr << "Unrecognized argument: " << argv[i] << std::endl;
            return -1;
        }
    }
    
    std::string line;
    if (remove_header) {
        std::getline(*in, line);
    }
    if (add_header) {
        *out << header << std::endl;
    }
    while (std::getline(*in, line)) {
        std::vector<std::string> tokens = tokenize(input_delimiter, line);
        std::string joined = join(output_delimiter, tokens);
        *out << joined << std::endl;
    }
    
    return 0;
}