/* Copyright 2022 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

// clang-format off
// List of Layers
enum layers{
  BASE,
  MAC_FN,
  WIN_FN
};

// Tap Dance Declarations
enum {
     TD_LSHIFT_CAPS,
};

// Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_LSHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
};

// This will be true when the switch is set to windows, and false when the switch is set to mac.
bool on_windows = false;

bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        on_windows = active;
    }
    return true;
}

// Macro Definitions
enum custom_keycodes {
    KM_SHOT = SAFE_RANGE,
    GUI_SPC,
    OS_FN,
    RGB_MODC,
    RGB_RMODC,
    KM_EMOJI
};

// Macro Processing
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
          case KM_SHOT:
               if (record->event.pressed) {
                    if (on_windows) {
                         SEND_STRING(SS_LGUI(SS_LSFT("S")));
                    } else {
                         SEND_STRING(SS_LCMD(SS_LOPT("8")));
                    }
                    return false;
               }
               break;
          case GUI_SPC:
               if (record->event.pressed) {
                    SEND_STRING(SS_LGUI(" "));
                    return false;
               }
               break;
          case OS_FN:
               if (record->event.pressed) {
                    //Move to the MAC_FN layer
                    layer_move(MAC_FN);
                    if (on_windows) {
                         //If the dip switch is set to windows, layer the WIN_FN layer on top of the MAC_FN layer.
                         layer_on(WIN_FN);
                    }
               } else {
                    //When we release the OS_FN key, move back to the BASE layer.
                    layer_move(BASE);
               }
               return false;
               break;
          case RGB_MODC:
          case RGB_RMODC:
               //Custom keycode to cycle through rgb modes, but skip the custom empty effect.
               if (record->event.pressed) {
                    keycode == RGB_MODC ? rgb_matrix_step() : rgb_matrix_step_reverse();
                    if ( rgb_matrix_get_mode() == RGB_MATRIX_CUSTOM_empty_effect ) {
                         keycode == RGB_MODC ? rgb_matrix_step() : rgb_matrix_step_reverse();
                    }
               }
               return false;
               break;
          case KM_EMOJI:
               if (record->event.pressed) {
                    if (on_windows) {
                         SEND_STRING(SS_LGUI("."));
                    } else {
                         SEND_STRING(SS_LCMD(SS_LCTL(" ")));
                    }
               }
               return false;
               break;
    }
    return true;
}

bool dynamic_recording = false;
bool DM_indicator_lit = false;
bool using_DM1 = true;
static uint16_t recording_timer;
// Dynamic Macro Hooks
void dynamic_macro_record_start_user(int8_t direction) {
     //Switch to custom defined empty RGB effect
     rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_empty_effect);
     //Turn off all keys
     rgb_matrix_set_color_all(0,0,0);
     if (direction == 1) {
          using_DM1 = true;
     } else {
          using_DM1 = false;
     }
     recording_timer = timer_read();
     dynamic_recording = true;
}
void dynamic_macro_record_end_user(int8_t direction) {
     dynamic_recording = false;
     //Restore previous RGB mode.
     rgb_matrix_reload_from_eeprom();
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
     //Handle the dynamic macro recording indicator(s).
     if (dynamic_recording) {
          if (DM_indicator_lit) {
               if (timer_elapsed(recording_timer) > 500) {
                    if (using_DM1) {
                         rgb_matrix_set_color(42, 0, 0, 0);
                    } else {
                         rgb_matrix_set_color(43, 0, 0, 0);
                    }
                    DM_indicator_lit = false;
                    recording_timer = timer_read();
               }
          } else {
               if (timer_elapsed(recording_timer) > 250) {
                    if (using_DM1) {
                         rgb_matrix_set_color(42, 255, 0, 0);
                    } else {
                         rgb_matrix_set_color(43, 255, 0, 0);
                    }
                    DM_indicator_lit = true;
                    recording_timer = timer_read();
               }
          }
     }
     return false;
}

//Definition of layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[BASE] = LAYOUT_iso_85(
     KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KM_SHOT,  KC_DEL,     RGB_MODC,
     KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
     KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_ENT,             KC_PGDN,
     GUI_SPC,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_BSLS,                      KC_HOME,
     TD(TD_LSHIFT_CAPS),  KC_INT1,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH, KC_RSFT,  KC_UP,    KC_END,
     KC_LCTL,  KC_LALT,  KC_LGUI,                                 KC_SPC,                                 KC_RGUI, OS_FN, KC_RCTL,     KC_LEFT,  KC_DOWN,  KC_RGHT),

[MAC_FN] = LAYOUT_iso_85(
     KC_TRNS,  KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LPAD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_TRNS,  KC_TRNS,  RGB_TOG,
     KC_TRNS,  BT_HST1,  BT_HST2,  BT_HST3,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  RGB_MODC,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  DM_REC1,  DM_REC2,  KC_TRNS,           KC_TRNS,
     KC_TRNS,  RGB_RMODC, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  DM_PLY1,  DM_PLY2,                     KC_TRNS,
     KC_TRNS,  KM_EMOJI,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  BAT_LVL,  NK_TOGG,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS),

[WIN_FN] = LAYOUT_iso_85(
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TASK,  KC_FILE,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,                      KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,            KC_TRNS,  KC_TRNS,  KC_TRNS,
     KC_TRNS,  KC_TRNS,  KC_TRNS,                                KC_TRNS,                                KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS)
};