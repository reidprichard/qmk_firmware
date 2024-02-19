/* Copyright 2021 Kyle McCreery
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

// Defines names for use in layer keycodes and the keymap

enum layer_names {
    _BASE,
    _FN1,
    _FN2,
    _NUMPAD,
    _NUM_FN1,
};

// keycodes: https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
                      KC_MEDIA_PLAY_PAUSE,  KC_MPRV,  KC_MNXT,    KC_MUTE,
                      KC_Q,                 KC_R,     KC_S,       KC_T,
                      KC_M,                 KC_N,     KC_O,       KC_P,
        KC_U,         KC_I,                 KC_J,     KC_K,       KC_L,
        MO(_FN2),     KC_E,                 KC_F,     KC_G,       KC_H,
        MO(_FN1),     KC_A,                 KC_B,     KC_C,       KC_D,

                      KC_F1,                KC_F2,    KC_F3
    ),
    [_FN1] = LAYOUT(
                      KC_MEDIA_PLAY_PAUSE,  KC_MPRV,    KC_MNXT,    KC_MUTE,
                      JS_16,                 JS_17,       JS_18,       JS_19,
                      JS_12,                 JS_13,       JS_14,       JS_15,
        LCA(KC_F24),  JS_8,                 JS_9,       JS_10,       JS_11,
        TO(_NUMPAD),  JS_4,                 JS_5,       JS_6,       JS_7,
        MO(_FN1),     JS_0,                 JS_1,       JS_2,       JS_3,

                      KC_F1,                 KC_F2,     KC_F3
    ),
    [_FN2] = LAYOUT(
                      _______,                 _______,     _______,       QK_BOOTLOADER,
                      _______,                 _______,     _______,       _______,
                      _______,                 _______,     _______,       _______,
        _______,      _______,                 _______,     _______,       _______,
        MO(_FN2),     _______,                 _______,     _______,       _______,
        TO(_NUMPAD),  _______,                 _______,     _______,       _______,

                      KC_F1,                KC_F2,    KC_F3
    ),
    [_NUMPAD] = LAYOUT(
                      _______,        _______,        _______,        _______,
                      KC_NUM_LOCK,    KC_KP_SLASH,    KC_KP_ASTERISK, KC_KP_MINUS,
                      KC_KP_7,        KC_KP_8,        KC_KP_9,        KC_KP_PLUS,
        _______,      KC_KP_4,        KC_KP_5,        KC_KP_6,        KC_BSPC,
        KC_LALT,      KC_KP_1,        KC_KP_2,        KC_KP_3,        KC_TAB,
        MO(_NUM_FN1), KC_KP_0,        KC_KP_EQUAL,    KC_KP_DOT,      KC_PENT,

                    _______,          _______,        _______
    ),
    [_NUM_FN1] = LAYOUT(
                    PB_1,  PB_2,  PB_3,  PB_4,
                    _______,  _______,  _______,  _______,
                    _______,  _______,  _______,  _______,
        _______,    KC_HOME,  KC_UP,    KC_PGUP,  _______,
        TO(_BASE),  KC_LEFT,  KC_DOWN,  KC_RIGHT, KC_ESC,
        _______,    KC_END,   KC_LCTL, KC_PGDN,   KC_CALC,

                    _______,  _______,  _______
    ),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE]    = { ENCODER_CCW_CW(LCA(KC_F22), LCA(KC_F23)),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_FN1]     = { ENCODER_CCW_CW(LSA(KC_F22), LSA(KC_F23)),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_FN2]     = { ENCODER_CCW_CW(RCS(KC_F22), RCS(KC_F23)),  ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_NUMPAD]  = { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT),         ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_NUM_FN1]  = { ENCODER_CCW_CW(KC_LEFT, KC_RIGHT),        ENCODER_CCW_CW(KC_TRNS, KC_TRNS) }
};
#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;       // flips the display 270 degrees
}

bool oled_task_user(void) {
    oled_write_ln_P(PSTR("Layer"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("FN 1"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("FN 2"), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("Num"), false);
            break;
        case 4:
            oled_write_ln_P(PSTR("#FN1"), false);
            break;
        default:
            oled_write_ln_P(PSTR("?"), false);
    }

    #define BLANK_LINE PSTR("    ")
    oled_write_ln_P(BLANK_LINE, false);
    oled_write_ln_P(BLANK_LINE, false);
    // oled_write_ln_P(PSTR(""), false);

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(led_state.num_lock ? PSTR("NUM ") : BLANK_LINE, false);
    oled_write_ln_P(led_state.caps_lock ? PSTR("CAP ") : BLANK_LINE, false);
    oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : BLANK_LINE, false);

    return false;
}
#endif

#ifdef RGBLIGHT_ENABLE

#ifdef RGBLIGHT_LAYERS
const rgblight_segment_t PROGMEM my_capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_RED}
);
const rgblight_segment_t PROGMEM my_default_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_TEAL}
);
const rgblight_segment_t PROGMEM my_fn1_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_ORANGE}
);
const rgblight_segment_t PROGMEM my_fn2_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_YELLOW}
);
const rgblight_segment_t PROGMEM my_numpad_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_MAGENTA}
);
const rgblight_segment_t PROGMEM my_numpadfn_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_PINK}
);
const rgblight_segment_t PROGMEM dummy_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 8, HSV_WHITE}
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    dummy_layer, // I'm not sure why, but I can't seem to do anything with layer 0
    my_default_layer,
    my_fn1_layer,
    my_fn2_layer,
    my_numpad_layer,
    my_numpadfn_layer,
    my_capslock_layer
);
#endif

#endif

void keyboard_post_init_user(void) {
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_LAYERS)
    rgblight_layers = my_rgb_layers;
#endif
}

bool led_update_user(led_t led_state) {
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_LAYERS)
    rgblight_set_layer_state(6, led_state.caps_lock);
#endif
    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_LAYERS)
    // rgblight_set_layer_state(1, layer_state_cmp(state, _BASE));
#endif
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_LAYERS)
    rgblight_set_layer_state(1, layer_state_cmp(state, _BASE));
    rgblight_set_layer_state(2, layer_state_cmp(state, _FN1));
    rgblight_set_layer_state(3, layer_state_cmp(state, _FN2));
    rgblight_set_layer_state(4, layer_state_cmp(state, _NUMPAD));
    rgblight_set_layer_state(5, layer_state_cmp(state, _NUM_FN1));

    // If in NUMPAD layer and numlock off, pulse LEDs. This gets overridden by lighting layers, so disabled for now.
    // if (!led_state.num_lock) {
    //     rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING);
    // }
    // else {
    //     rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    // }
#endif
    return state;
}
