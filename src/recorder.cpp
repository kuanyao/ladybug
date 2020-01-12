#include "main.h"
#include <exception>

namespace recording {

    using namespace std;
    bool is_power_changed(int left, int right);
    void do_record(int tick, int left, int right);

    struct RecordUnit {
        int tick;
        int left_power;
        int right_power;
        // double left_position; 
        // double right_position;
    } ;

    // private variables
    RecordUnit *recording;
    int recording_position = -1;
    const int threshold = 15;
    int recording_interval = 50;
    int recording_time = 15000;
    int tick = 0;

    void reset(int duration, int interval) {
        recording_interval = interval;
        recording_time = duration;
        recording_position = -1;
        tick = 0;
        
        if (recording != NULL) {
            delete[] recording;
        }

        int capacity = duration / interval;
        recording = new RecordUnit[capacity];
        for (int i=0;i<capacity;++i) {
            recording[i].tick = 0;
            recording[i].left_power = 0;
            recording[i].right_power = 0;
        }
    }

    void record() {
        int left_power = ((okapi::SkidSteerModel *)(&chassis->model()))->getLeftSideMotor()->getVoltage();
        int right_power = ((okapi::SkidSteerModel *)(&chassis->model()))->getRightSideMotor()->getVoltage();
        int capacity = recording_time/ recording_interval;

        if (tick >= capacity - 1) {
            return;
        }

        ++ tick;

        if (is_power_changed(left_power, right_power)) {
            do_record(tick, left_power, right_power);
        }
    }

    bool is_power_changed(int left_power, int right_power) {
        if (recording_position == -1) {
            if (abs(left_power) > threshold || abs(right_power) > threshold) {
                cout << "initial position triggered" << endl;
                return true;
            }
        } else { 
            RecordUnit last_unit = recording[recording_position];
            if (abs(last_unit.left_power - left_power) > threshold || abs(last_unit.right_power - right_power) > threshold) {
                cout << "new power change has detected." << endl;
                return true;
            } 
        }
        return false;
    }

    void do_record(int tick, int left_power, int right_power) {
        if (recording == NULL) {
            return;
        }
        RecordUnit& new_unit = recording[++recording_position];
        cout << "maing a new record " << recording_position << endl;
        new_unit.tick = tick;
        new_unit.left_power = left_power;
        new_unit.right_power = right_power;
    }

    void printout() {
        for (int i=0; i<=recording_position; ++i) {
            std::cout << recording[i].tick << ", " << recording[i].left_power << ", " << recording[i].right_power << std::endl;
        }
    }

    void replay() {
        int capacity = recording_time / recording_interval;
        int r_index = 0;
        for (int t = 0; t < capacity; ++t) {
            RecordUnit& unit = recording[r_index];
            int tick = unit.tick;
            if (tick == t) {
                cout << "replaying " << unit.tick << ", " << unit.left_power << ", " << unit.right_power << endl;
                ((okapi::SkidSteerModel *)(&chassis->model()))->getLeftSideMotor()->moveVoltage(unit.left_power);
                ((okapi::SkidSteerModel *)(&chassis->model()))->getRightSideMotor()->moveVoltage(unit.right_power);
                r_index ++;
            }
            pros::delay(recording_interval);
        }
        cout << "replaying done !!" << endl;
    }
}