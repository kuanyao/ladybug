#include <cerrno>
#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <sstream>

using namespace std;


class Formatter
{
public:
    Formatter() {}
    ~Formatter() {}

    template <typename Type>
    Formatter & operator << (const Type & value)
    {
        stream_ << value;
        return *this;
    }

    std::string str() const         { return stream_.str(); }
    operator std::string () const   { return stream_.str(); }

    enum ConvertToString 
    {
        to_str
    };
    std::string operator >> (ConvertToString) { return stream_.str(); }

private:
    std::stringstream stream_;

    Formatter(const Formatter &);
    Formatter & operator = (Formatter &);
};

int load_program(const string file_name, const int base_tick) {
    string line;
    std::ifstream infile(file_name);
    if (!infile.is_open()) {
        throw std::runtime_error(Formatter() << "unable to open file " << file_name);
    }
    int tick;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> tick;
        cout << base_tick + tick;
        for (int num; iss >> num; ) {
            cout << " " << num;
        }
        cout << endl;
    }
    infile.close();
    return base_tick + tick;
}

int main(int argc, char* argv[]) {
    int base_tick = 0;
    for (int i=1; i<argc; ++i) {
        base_tick = load_program(argv[i], base_tick);
    }
}