#include "Blimp.h"
/*
 * Init and run calls for follow flight mode
 */

bool ModeFollow::init(bool ignore_checks)
{
    if (!g2.follow.enabled()) {
        gcs().send_text(MAV_SEVERITY_WARNING, "Set FOLL_ENABLE = 1");
        return false;
    }

    // until a target is heard, hold the current position and heading
    target_pos = blimp.pos_ned;
    target_yaw = blimp.ahrs.get_yaw_rad();
    return true;
}

void ModeFollow::run()
{
    if (!blimp.motors->armed()) {
        target_pos = blimp.pos_ned;
        target_yaw = blimp.ahrs.get_yaw_rad();
    } else {
        g2.follow.init_offsets_if_required();

        Vector3p pos_ofs_ned_m;
        Vector3f vel_ofs_ned_ms;
        Vector3f accel_ofs_ned_mss;
        if (g2.follow.get_ofs_pos_vel_accel_NED_m(pos_ofs_ned_m, vel_ofs_ned_ms, accel_ofs_ned_mss)) {
            // target (plus offset) is valid: chase it
            target_pos = pos_ofs_ned_m.tofloat();
        }
        // otherwise the target is lost: keep the last target_pos, i.e. hold position
    }

    blimp.loiter->run(target_pos, target_yaw, Vector4b{false,false,false,false});
}