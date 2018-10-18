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


std::string strip(std::string str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) {
        return str;
    } else {
        size_t end = str.find_last_not_of(" \t");
        return str.substr(start, end - start + 1);
    }
}


int main(int argc, char **argv) {
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        std::cout
            << "Usage: csvutil [OPTION]..." << std::endl
            << "Performs common operations on csv data." << std::endl
            << std::endl
            << "-h, --help    shows this information" << std::endl
            << "-i            read from specified file instead of stdin" << std::endl
            << "-o            write to specified file instead of stdout" << std::endl
            << "-id           specify input delimiter" << std::endl
            << "-od           specify output delimiter" << std::endl
            << "-rh           remove a header (ignore first line of input)" << std::endl
            << "-ah           add a specified header" << std::endl
            << "-t            trim whitespace" << std::endl
        ;
        return 0;
    }
    
    std::ifstream fin;
    std::ofstream fout;
    
    std::istream* in = &std::cin;
    std::ostream* out = &std::cout;
    
    std::string input_delimiter = ",";
    std::string output_delimiter = ",";
    
    bool remove_header = false;
    bool add_header = false;
    std::string header;
    
    bool trim = false;
    
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
        } else if (std::string(argv[i]) == "-t") {
            trim = true;
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
        if (trim) {
            for (int i = 0; i < tokens.size(); ++i) {
                tokens[i] = strip(tokens[i]);
            }
        }
        std::string joined = join(output_delimiter, tokens);
        *out << joined << std::endl;
    }
    
    return 0;
}