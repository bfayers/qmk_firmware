//Function to alternate holding shift for keypresses (mocking text joke)
bool enable_mocking = false;
bool mocking_shift_held = false;
void mocking_shift(void) {
    if (mocking_shift_held) {
        unregister_code(KC_LSFT);
        mocking_shift_held = false;
    } else {
        register_code(KC_LSFT);
        mocking_shift_held = true;
    }
}