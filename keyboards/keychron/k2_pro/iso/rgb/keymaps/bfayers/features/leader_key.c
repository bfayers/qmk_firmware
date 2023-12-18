//Leader key definitions
void leader_end_user(void) {
    if (leader_sequence_two_keys(KC_N, KC_V)) {
        SEND_STRING("python -m venv .venv");
    } else if (leader_sequence_two_keys(KC_F, KC_C)) {
        SEND_STRING("-ss start_time -to end_time");
    } else if (leader_sequence_two_keys(KC_F, KC_G)) {
        SEND_STRING("-filter_complex \"[0:v] fps=25,scale=w=480:h=-1,split [a][b];[a] palettegen [p];[b][p] paletteuse\"")
    }
}