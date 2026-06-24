#include QMK_KEYBOARD_H
#include "tmk_core/protocol/nrf/keycode_str_converter.h"

bool isScrollMode;

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST };

#undef LAYOUT
#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k0c, k0d, k0e, k0f, k10, k11, k12, k13, k14, k15, k16, k17, \
    k18, k19, k1a, k1b, k1c, k1d, k1e, k1f, k20, k21, k22, k23, k24, k25, \
    k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, k2f, k30 \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, k0c, k0d, k0e, k0f, k10, k11, k12, k13, k14, k15, k16, k17, k18, k19, k1a, k1b, k1c, k1d, k1e, k1f }, \
    { k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, k2f, k30, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GESC, KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,    KC_Y,    KC_U,   KC_I,   KC_O,    KC_P,    KC_GRV,
        KC_LCTL, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,    KC_H,    KC_J,   KC_K,   KC_L,    KC_SCLN, KC_QUOT,
        KC_LALT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,    KC_BTN1, KC_BSPC, KC_N,  KC_M,    KC_COMM, KC_DOT, KC_SLSH, KC_EQL,
        KC_RGUI, MO(1),  KC_SPC, KC_TAB, KC_PGUP,KC_BTN2, KC_PGDN, KC_ENT, KC_SPC, MO(2),   KC_DEL
    ),
    [_LOWER] = LAYOUT(
        KC_ESC,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,   KC_6,    KC_7,   KC_8,   KC_9,    KC_0,    KC_MINS,
        KC_RSFT, KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_LBRC, KC_NO,  KC_UP,  KC_NO,   KC_NO,   KC_RBRC,
        KC_INS,  KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_BTN1, KC_BTN2,KC_NO,  KC_LEFT, KC_DOWN, KC_RGHT, KC_BSLS, KC_NUHS,
        KC_NO,   KC_NO,  KC_SPC, KC_LGUI,KC_PGUP,KC_BTN3, KC_PGDN, KC_ENT, KC_SPC, KC_NO,   KC_NO
    ),
    [_RAISE] = LAYOUT(
        KC_ESC,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,    KC_F6,   KC_F7,  KC_F8,  KC_F9,   KC_F10,  KC_UNDO,
        KC_LSFT, KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_BTN1, KC_BTN2,KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_LGUI, KC_NO,  KC_SPC, KC_LGUI,KC_PGUP,KC_TRNS, KC_PGDN, KC_ENT, KC_SPC, KC_NO,   KC_DEL
    ),
    [_ADJUST] = LAYOUT(
        KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,   KC_NO,   KC_NO,  KC_NO,  KC_NO,   KC_NO
    )
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    isScrollMode = layer_state_is(_LOWER);
    if (isScrollMode) {
        mouse_report.h = mouse_report.x;
        mouse_report.v = -mouse_report.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

const key_string_map_t custom_keys_user = {0, 0, ""};
