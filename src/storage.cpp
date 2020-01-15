#include "storage.h"

namespace storage {
    bool is_slot_taken(const char * slot_name) {
        if (slot_name == "Alpha") {
            return true;
        }
        return false;
    }

    void save_to_slot(recording::RecordUnit * recording, const char * slot_name) {

    }

    void clear_slot(const char * slot_name) {

    }

    char ** get_occupied_slots() {
        char * slot_names[] = { "Alpha", "Lima"};
        return slot_names;
    }
}