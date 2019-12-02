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

const int AUTO_TRACE_NONE 	= 0;
const int AUTO_TRACE_SINGLE_OBJ = 1;
const int AUTO_TRACE_ALL 	= 2;

int auto_trace_mode = AUTO_TRACE_NONE;
int trace_obj = -1;
int trace_obj_state = 0;

void select_auto_trace() {
	if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_X) == 1) {
		auto_trace_mode = AUTO_TRACE_NONE;
	}	
	if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_Y) == 1) {
		auto_trace_mode = AUTO_TRACE_SINGLE_OBJ;
		trace_obj = (trace_obj + 1) % 3;
	}	
	if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_A) == 1) {
		auto_trace_mode = AUTO_TRACE_ALL;
	}	
	if (master.get_digital_new_press(E_CONTROLLER_DIGITAL_B) == 1) {
		trace_obj_state = 0;
	}	
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
	while (true) {
		select_auto_trace();
		if (!chassis_control()) {
			char * trace_obj_str = "trace orange";
			switch (auto_trace_mode) {
				case AUTO_TRACE_SINGLE_OBJ:
					lcd::clear_line(1);
					if (trace_obj == 1) {
						trace_obj_str = "trace purple";
					} else if (trace_obj == 2) {
						trace_obj_str = "trace green";
					}
					lcd::set_text(1, trace_obj_str);
					follow_single_cube(trace_obj + 1, trace_obj_state);
					break;
				case AUTO_TRACE_ALL:
					break;
				case AUTO_TRACE_NONE:
					chassis.stop();
					break;
				default:
					break;
			}
		}
		pros::delay(50);
	}
}
