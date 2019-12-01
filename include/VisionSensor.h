/*vex-vision-config:begin*/
#include "vex.h"
vex::vision::signature ORANGE_CUBE = vex::vision::signature (1, 11265, 13055, 12160, -2609, -1855, -2232, 8.5, 0);
vex::vision::signature PURPULE_CUBE = vex::vision::signature (2, 949, 1999, 1474, 7669, 10889, 9279, 3, 0);
vex::vision::signature GREEN_CUBE = vex::vision::signature (3, -7501, -5897, -6700, -4311, -2729, -3520, 3, 0);
vex::vision::signature SIG_4 = vex::vision::signature (4, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_5 = vex::vision::signature (5, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_6 = vex::vision::signature (6, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision::signature SIG_7 = vex::vision::signature (7, 0, 0, 0, 0, 0, 0, 2.5, 0);
vex::vision camera = vex::vision (vex::PORT1, 50, ORANGE_CUBE, PURPULE_CUBE, GREEN_CUBE, SIG_4, SIG_5, SIG_6, SIG_7);
/*vex-vision-config:end*/