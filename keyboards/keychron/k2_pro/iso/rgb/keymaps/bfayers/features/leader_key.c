//Leader key definitions
void leader_end_user(void) {
    if (leader_sequence_two_keys(KC_N, KC_V)) {
        SEND_STRING("python -m venv .venv");
    }
}