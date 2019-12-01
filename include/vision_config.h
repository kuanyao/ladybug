#ifndef __VISION_CONFIG_H
#define __VISION_CONFIG_H

#include "pros/vision.hpp"

#define SIG_ORANGE_CUBE 1
#define SIG_PURPLE_CUBE 2
#define SIG_GREEN_CUBE 3

#define VISION_PORT 21

extern pros::vision_signature_s_t signature_orange_cube;
extern pros::vision_signature_s_t signature_purple_cube;
extern pros::vision_signature_s_t signature_green_cube;
extern pros::vision_signature_s_t vision_sig_4;
extern pros::vision_signature_s_t vision_sig_5;
extern pros::vision_signature_s_t vision_sig_6;
extern pros::vision_signature_s_t vision_sig_7;

extern pros::Vision camera;

void initialize_vision_sensor();

#endif
