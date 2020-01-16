#ifndef __STORAGE_H
#define __STORAGE_H

#include "recording.h"

namespace storage {
    bool is_slot_taken(const char * slot_name);
    void save_to_slot(recording::RecordUnit * recording, const char * slot_name);
    void clear_slot(const char * slot_name);
    void load_all_programs();
    recording::RecordUnit * get_program(const char *);
}

#endif
