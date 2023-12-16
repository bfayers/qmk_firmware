RGB_MATRIX_EFFECT(empty_effect)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// Taken from https://github.com/tcteo/qmk_firmware/blob/28487fb8695f5afdc9594514174ffe9635d4eec0/keyboards/kbdcraft/adam0110/keymaps/tcteo/rgb_matrix_user.inc
// https://github.com/qmk/qmk_firmware/issues/14686
// [Bug] Cannot control RGB Matrix manually, colors repeatedly overwritten with solid color under RGB_MATRIX_NONE mode #14686
// As a workaround, define a custom effect that does nothing, set it as the init default in keymap.c: rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_empty_effect);
// When this effect is active, values written with rgb_matrix_set_color() aren't overwritten.

bool empty_effect(effect_params_t* params) {
    return false;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS