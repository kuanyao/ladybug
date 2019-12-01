#include "main.h"
#include "vision_config.h"

// using namespace pros;

pros::Vision camera(VISION_PORT);
pros::vision_signature_s_t signature_orange_cube;
pros::vision_signature_s_t signature_purple_cube;
pros::vision_signature_s_t signature_green_cube;

void initialize_vision_sensor() {

  signature_orange_cube = pros::Vision::signature_from_utility(
    SIG_ORANGE_CUBE, //* @param id The signature id.
    11265, //* @param uMin The signature minimum value on the u axis.
    13055, //* @param uMax The signature maximum value on the u axis.
    12160, //* @param uMean The signature mean value on the u axis.
    -2609, //* @param vMin The signature minimum value on the v axis.
    -1855, //* @param vMax The signature maximum value on the v axis.
    -2232, //* @param vMean The signature mean value on the v axis.
    8.5,   //* @param range The signature range scale factor.
    0);      //* @param type The signature type, normal, or color code.
  camera.set_signature(SIG_ORANGE_CUBE, &signature_orange_cube);

  signature_purple_cube = pros::Vision::signature_from_utility(
    SIG_PURPLE_CUBE, //* @param id The signature id.
    949, //* @param uMin The signature minimum value on the u axis.
    1999, //* @param uMax The signature maximum value on the u axis.
    1474, //* @param uMean The signature mean value on the u axis.
    7669, //* @param vMin The signature minimum value on the v axis.
    10889, //* @param vMax The signature maximum value on the v axis.
    9279, //* @param vMean The signature mean value on the v axis.
    3,   //* @param range The signature range scale factor.
    0);      //* @param type The signature type, normal, or color code.
  camera.set_signature(SIG_PURPLE_CUBE, &signature_purple_cube);

  signature_green_cube = pros::Vision::signature_from_utility(
    SIG_GREEN_CUBE, //* @param id The signature id.
    -7501, //* @param uMin The signature minimum value on the u axis.
    -5897, //* @param uMax The signature maximum value on the u axis.
    -6700, //* @param uMean The signature mean value on the u axis.
    -4311, //* @param vMin The signature minimum value on the v axis.
    -2729, //* @param vMax The signature maximum value on the v axis.
    -3520, //* @param vMean The signature mean value on the v axis.
    3,   //* @param range The signature range scale factor.
    0);      //* @param type The signature type, normal, or color code.
  camera.set_signature(SIG_GREEN_CUBE, &signature_green_cube);
}