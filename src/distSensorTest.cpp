#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <string>
#include <vector>
#include <math.h>

pros::Motor topRight(10, true); //port 13, clockwise
pros::Motor topLeft(13, false);
pros::Motor botLeft(9, true);
pros::Motor botRight(2, false);

int distRead;
pros::Distance dist(2); //distance censor 1
//pros::Distance dist2(7); //distance censor 2


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
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
void autonomous() {
	
	while (true) {
		dist.get(); //get distance
		distRead = dist.get();
		pros::delay(1);

		topRight.move_velocity(75); //forward
		topLeft.move_velocity(75);
		botLeft.move_velocity(-75);
		botRight.move_velocity(-75);
		pros::delay(1);

		while (distRead<=100) {
			topRight.move_velocity(-75); //backward
			topLeft.move_velocity(-75);
			botLeft.move_velocity(75);
			botRight.move_velocity(75);
			pros::delay(1000);
			
			topRight.move_velocity(0); //pause
			topLeft.move_velocity(0);
			botLeft.move_velocity(0);
			botRight.move_velocity(0);
			pros::delay(250);

			topRight.move_velocity(-75); //turn left
			topLeft.move_velocity(75);
			botLeft.move_velocity(-75);
			botRight.move_velocity(75);
			pros::delay(300);

			//velocity 15, delay .9 sec = 90 degrees

			dist.get(); //get distance
			distRead = dist.get();
			pros::delay(1);
		}	
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
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);

	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_mtr = left;
		right_mtr = right;

		pros::delay(20);
	}
}
