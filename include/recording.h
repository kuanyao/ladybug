#ifndef __RECORDING_H
#define __RECOORDING_H

namespace recording {
    struct RecordUnit {
        int tick;
        int left_power;
        int right_power;
        // double left_position; 
        // double right_position;
    } ;

    void reset(int duration, int interval, void (*post_record_action)(void));
    void record();
    void printout();
    void replay();
    RecordUnit * dump();
}

#endif
