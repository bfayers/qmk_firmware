// This will be true when the switch is set to windows, and false when the switch is set to mac.
bool on_windows = false;

bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        on_windows = active;
        os_mode_indicating = true;
    }
    return true;
}