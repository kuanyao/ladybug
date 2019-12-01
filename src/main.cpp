#include "main.h"
#include "vision_config.h"

using namespace std;
using namespace pros;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	delay(100);
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	initialize_vision_sensor();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

bool is_position_changed(vision_object_s_t &old_pos, vision_object_s_t &new_pos) {
		if (old_pos.signature != new_pos.signature) {
			return true;
		}

		const int16_t threshold = 20;

		if (abs(old_pos.left_coord - new_pos.left_coord) > threshold ||
				abs(old_pos.top_coord - new_pos.top_coord) > threshold ||
				abs(old_pos.width - new_pos.width) > threshold ||
				abs(old_pos.height - new_pos.height) > threshold ||
				abs(old_pos.x_middle_coord - new_pos.x_middle_coord) > threshold ||
				abs(old_pos.y_middle_coord - new_pos.y_middle_coord) > threshold ||
				abs(old_pos.angle - new_pos.angle) > threshold) {
			return true;
		}

		return false;
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

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	vision_object_s_t old_pos;

	while (true) {
		// pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		// pros::vision_object_s_t rtn = camera.get_by_size(0);
		// if (rtn.signature != 255 && is_position_changed(old_pos, rtn)) {
		// 	cout << "sig_id: " << rtn.signature;
		// 	// cout << "left_coord: " << rtn.left_coord<< endl;
		// 	// cout << "top_coord: " << rtn.top_coord<< endl;
		// 	cout << ", width: " << rtn.width;
		// 	cout << ", height: " << rtn.height;
		// 	// cout << ", angle: " << rtn.angle;
		// 	cout << ", x_middle_coord: " << rtn.x_middle_coord;
		// 	cout << ", y_middle_coord: " << rtn.y_middle_coord;
		// 	cout << endl;
		// }
		// old_pos = rtn;

		// int orange_size = 0, green_size = 0, purple_size = 0;
		auto orange_obj = camera.get_by_sig(0, SIG_ORANGE_CUBE);
		// if (orange_obj.signature != 255) {
		// 	cout << "orange" << endl;
		// 	orange_size = orange_obj.width * orange_obj.height;
		// }
		auto green_obj = camera.get_by_sig(0, SIG_GREEN_CUBE);
		// if (green_obj.signature != 255) {
		// 	cout << "green" << endl;
		// 	green_size = green_obj.width * green_obj.height;
		// }
		auto purple_obj = camera.get_by_sig(0, SIG_PURPLE_CUBE);
		// if (purple_obj.signature != 255) {
		// 	cout << "purple" << endl;
		// 	purple_size = purple_obj.width * purple_obj.height;
		// }

		vision_object_s_t objs[] = {orange_obj, green_obj, purple_obj};
		auto largest_obj = get_largest_obj(objs);

		if (largest_obj.signature != 255 && old_pos.signature != largest_obj.signature) {
			cout << "largest obj present: " << largest_obj.signature << endl;
			old_pos = largest_obj;
		}
		pros::delay(50);
	}
}
