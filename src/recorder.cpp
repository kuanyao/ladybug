#include "main.h"
#include <exception>

#include "recording.h"

namespace recording {

    using namespace std;
    bool is_power_changed(int left, int right);
    void do_record(int left, int right);

    // private variables
    vector<RecordUnit> recording;
    vector<RecordUnit>& (*replay_loader)(void);
    const int threshold = 15;
    int recording_interval = 50;
    int recording_time = 15000;
    int tick = 0;
    void (*post_record_action)(void) = NULL;

    void reset(int duration, int interval, void (*action)(void)) {
        recording_interval = interval;
        recording_time = duration;
        tick = 0;
        post_record_action = action;
        
        recording.clear();
        int capacity = duration / interval;
    }

    void set_replay_loader(std::vector<RecordUnit>& (* loader)(void)) {
        replay_loader = loader;
    }

    void record() {
        int left_power = ((okapi::SkidSteerModel *)(&chassis->model()))->getLeftSideMotor()->getVoltage();
        int right_power = ((okapi::SkidSteerModel *)(&chassis->model()))->getRightSideMotor()->getVoltage();
        int capacity = recording_time/ recording_interval;

        if (tick == capacity - 1) {
            if (post_record_action != NULL) {
                post_record_action();
            }
            ++tick; 
            return;
        } else if (tick == capacity) {
            return;
        }

        ++ tick;

        if (is_power_changed(left_power, right_power)) {
            do_record(left_power, right_power);
        }
    }

    bool is_power_changed(int left_power, int right_power) {
        if (recording.size() == 0) {
            if (abs(left_power) > threshold || abs(right_power) > threshold) {
                cout << "initial position triggered" << endl;
                return true;
            }
        } else { 
            RecordUnit last_unit = recording.back();
            if (abs(last_unit.left - left_power) > threshold || abs(last_unit.right - right_power) > threshold) {
                cout << "new power change has detected." << endl;
                return true;
            } 
        }
        return false;
    }

    void do_record(int left_power, int right_power) {
        if (recording.size() == 0) {
            tick = 0; // reset tick position
        }
        RecordUnit new_unit; 
        cout << "saving a new record " << endl; 
        new_unit.tick = tick;
        new_unit.left = left_power;
        new_unit.right = right_power;
        recording.push_back(new_unit);
    }

    void printout() {
        for (int i=0; i<=recording.size(); ++i) {
            std::cout << recording[i].tick << ", " << recording[i].left << ", " << recording[i].right << std::endl;
        }
    }

    void replay() {
        if (replay_loader == NULL) {
            cout << "no replay loader found" << endl;
            return;
        }
        vector<RecordUnit> replay_recording = replay_loader();
        if (replay_recording.empty()) {
            cout << "load empty records, nothing to replay" << endl;
            return;
        }
        cout << "about to replay " << replay_recording.size() << " steps" << endl;
        int starting_tick = replay_recording.front().tick;
        int ending_tick = replay_recording.back().tick;
        cout << "starting at " << starting_tick << " and end at " << ending_tick << " for total " 
                << (ending_tick - starting_tick) * 50 / 1000 << " senconds" << endl;

        int r_index = 0;
        for (int t = starting_tick; t < ending_tick; ++t) {
            RecordUnit& unit = replay_recording[r_index];
            int tick = unit.tick;
            if (tick == t) {
                cout << "replaying " << unit.tick << ", " << unit.left << ", " << unit.right << endl;
                ((okapi::SkidSteerModel *)(&chassis->model()))->getLeftSideMotor()->moveVoltage(unit.left);
                ((okapi::SkidSteerModel *)(&chassis->model()))->getRightSideMotor()->moveVoltage(unit.right);
                r_index ++;
            }
            pros::delay(recording_interval);
        }
        cout << "replaying done !!" << endl;
    }

    vector<RecordUnit>& dump() {
        return recording;
    }
}