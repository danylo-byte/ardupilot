#include "Blimp.h"
/*
 * Init and run calls for follow flight mode
 */

bool ModeFollow::init(bool ignore_checks)
{
    // start by holding the current position and heading
    target_pos = blimp.pos_ned;
    target_yaw = blimp.ahrs.get_yaw_rad();
    return true;
}

void ModeFollow::run()
{
    // TODO: replace target_pos with the followed target's position
    if (!blimp.motors->armed()) {
        target_pos = blimp.pos_ned;
        target_yaw = blimp.ahrs.get_yaw_rad();
    }

    blimp.loiter->run(target_pos, target_yaw, Vector4b{false,false,false,false});
}