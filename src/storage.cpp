#include "storage.h"
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace storage {
    using namespace std;
    using namespace recording;
    
    static const char * path = "/usd/tower2020/";
    map<string, RecordUnit*> all_programs;

    bool is_slot_taken(const char * slot_name) {
        return all_programs.count(slot_name) == 1;
    }

    void save_to_slot(recording::RecordUnit * recording, const char * slot_name) {
        string program_path = path;
        program_path = program_path + "/" + slot_name;
        ofstream program_file;
        program_file.open (program_path);
        int num_of_rec = sizeof(recording) / sizeof(recording[0]);
        for (int i=0; i<num_of_rec; ++i) {
            auto ru = recording[i];
            program_file << ru.tick << " ";
            program_file << ru.left << " ";
            program_file << ru.right<< " ";
            program_file << ru.lever << " ";
            program_file << ru.left_intake << " ";
            program_file << ru.right_intake << " ";
            program_file << ru.arm << " ";
            program_file << endl;
        }
        program_file.close();
        all_programs.insert_or_assign(slot_name, recording);
    }

    void clear_slot(const char * slot_name) {
        RecordUnit temp;
        save_to_slot(&temp, slot_name);
        all_programs.erase(slot_name);
    }

    RecordUnit* get_program(const char * program_name) {
        return all_programs[program_name];
    }

    void load_program(const filesystem::path& path) {
        string line;
        std::ifstream infile(path);
        vector<RecordUnit> rus;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            RecordUnit ru;
            if (!(iss >> ru.tick >> ru.left >> ru.right >> ru.lever >> ru.left_intake >> ru.right_intake >> ru.arm)) { break; } // error
            rus.push_back(ru);
        }
        if (rus.size() > 1) {
            RecordUnit * ru_array = new RecordUnit[rus.size()];
            copy(rus.begin(), rus.end(), ru_array);
            all_programs.insert_or_assign(path.filename(), ru_array);
        }
        infile.close();
    }

    void load_all_programs() {
        for (const auto & entry : filesystem::directory_iterator(path)) {
            load_program(entry.path());
        }
    }
}