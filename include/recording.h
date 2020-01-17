#ifndef __RECORDING_H
#define __RECOORDING_H

namespace recording {
    struct RecordUnit {
        short tick;
        short left;
        short right;
        short lever;
        short left_intake;
        short right_intake;
        short arm;
    } ;

    void reset(int duration, int interval, void (*post_record_action)(void));
    void record();
    void printout();
    void replay();
    void set_replay_loader(std::vector<RecordUnit>& (*)(void));
    std::vector<RecordUnit>& dump();
}

#endif
