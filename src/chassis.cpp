#include "main.h"
#include "api.h"
#include "okapi/api.hpp"
#include "vision_config.h"

using namespace std;
using namespace okapi;
using namespace pros;

const int WIDTH_WHEN_CUBE_IS_IN_FRONT = 60;
const int Y_POSITION_WHEN_CUBE_IS_IN_FRONT = 190;
const int HEIGHT_WHEN_CUBE_IS_IN_FRONT = 20;
const int X_POSITION_WHEN_CUBE_IS_IN_FRONT_CENTER = 62;
const int CENTER_POSITION_THRESHOLD = 10;
const int VISION_SENSOR_VIEW_CENTER = 158;


/* return boolean value to indicate if chassis is to be moved. */
bool chassis_control() {
    int left_power = master.get_analog(ANALOG_LEFT_Y);
	int right_power = master.get_analog(ANALOG_RIGHT_Y);
    const double chassis_movement_threshold = 0.05;

    bool isMoving = abs(left_power / 127.0) > chassis_movement_threshold || abs(right_power / 127.0) > chassis_movement_threshold;
    chassis.tank(left_power / 127.0, right_power / 127.0, chassis_movement_threshold);

    return isMoving;
}

vision_object_s_t & get_largest_obj(vision_object_s_t objs[]) {
	int max = 0;
	int index = 0;
	int arr_size = 3; //sizeof(objs) / sizeof(objs[0]);
	for (int i=0; i<arr_size; ++i) {
		if (objs[i].signature == 255) {
			continue;
		}
		int size = objs[i].width * objs[i].height;
		if (size > max) {
			max = size;
			index = i;
		}
	}
	return objs[index];
}

bool is_cube_close_enough(vision_object_s_t &vision_obj) {
    cout << "judging if close enough by width: " << vision_obj.width;
    cout << ", y: " << vision_obj.top_coord;
    cout << ", height: " << vision_obj.height << ". ";
    if (vision_obj.width > WIDTH_WHEN_CUBE_IS_IN_FRONT && 
        vision_obj.top_coord > Y_POSITION_WHEN_CUBE_IS_IN_FRONT && 
        vision_obj.height < HEIGHT_WHEN_CUBE_IS_IN_FRONT) {
        return true;
    }
    return false;
}

bool is_cube_front_centered(vision_object_s_t &vision_obj) {
    cout << "judging if front centered by x: " << vision_obj.left_coord << ". ";
    if (abs(vision_obj.left_coord - X_POSITION_WHEN_CUBE_IS_IN_FRONT_CENTER) < CENTER_POSITION_THRESHOLD) {
        return true;
    }
    return false;
}

bool is_cube_centered(vision_object_s_t &vision_obj) {
    cout << "judging if centered by x: " << vision_obj.x_middle_coord << ". "; 
    if (abs(vision_obj.x_middle_coord - VISION_SENSOR_VIEW_CENTER) < CENTER_POSITION_THRESHOLD) {
        return true;
    }
    return false;
}

void follow_single_cube(int signature_num, int& tracking_obj_state) {
    auto tracing_obj = camera.get_by_sig(0, signature_num); 

    if (tracing_obj.signature == signature_num) { 
        if (tracking_obj_state == 0) {
            cout << "chassis is too far, ";
            if (!is_cube_centered(tracing_obj)) {
                cout << "and not alighed to center. Adjusting angle now." << endl;
                chassis.setMaxVelocity(200);
                if (tracing_obj.x_middle_coord > VISION_SENSOR_VIEW_CENTER) {
                    chassis.turnAngle(5_deg);
                } else {
                    chassis.turnAngle(-5_deg);
                }
            } else {
                cout << "centered. Now moving closer." << endl;
                // chassis.setMaxVelocity(600);
                tracking_obj_state = 1;
            }
        }

        if (tracking_obj_state == 1) {
            if (is_cube_close_enough(tracing_obj)) {
                cout << "chassis is close enough, ";  
                chassis.stop();
                tracking_obj_state = 2;
            } else {
                cout << ", pulling closing." << endl;
                chassis.forward(0.2);
            }
        }

        if (tracking_obj_state == 2) {
            if (is_cube_front_centered(tracing_obj)) {
                cout << "and tracking cube is right in the front!!" << endl;
                tracking_obj_state = 3;
                // chassis.setMaxVelocity(600);
            } else {
                cout << "but not aligned to center. Adjusting angle now." << endl;
                chassis.setMaxVelocity(100);
                if (tracing_obj.left_coord > X_POSITION_WHEN_CUBE_IS_IN_FRONT_CENTER) {
                    chassis.turnAngle(5_deg);
                } else {
                    chassis.turnAngle(-5_deg);
                }
            }
        }
    } 
    // else {
    //     cout << "sig: " << tracing_obj.signature;
    //     cout << ", x: " << tracing_obj.left_coord;
    //     cout << ", y: " << tracing_obj.top_coord;
    //     cout << ", width: " << tracing_obj.width;
    //     cout << ", height: " << tracing_obj.height;
    //     cout << endl;
    // }
}