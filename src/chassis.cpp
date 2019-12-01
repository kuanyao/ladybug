#include "main.h"
#include "api.h"
#include "okapi/api.hpp"

/* return boolean value to indicate if chassis is to be moved. */
bool chassis_control() {
    int left_power = master.get_analog(ANALOG_LEFT_Y);
	int right_power = master.get_analog(ANALOG_RIGHT_Y);
    const double chassis_movement_threshold = 0.05;

    bool isMoving = abs(left_power / 127.0) > chassis_movement_threshold || abs(right_power / 127.0) > chassis_movement_threshold;
    chassis.tank(left_power / 127.0, right_power / 127.0, chassis_movement_threshold);

    return isMoving;
}
