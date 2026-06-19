#include QMK_KEYBOARD_H

bool isScrollMode;

enum layer_names {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST
};

#undef LAYOUT
#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, \
    k10, k11, k12, k13, k14, k15, k16, \
    k20, k21, k22, k23, k24, k25, k26, \
    k30, k31, k32, k33, k34, k35, k36  \
) { \
    { k00, k01, k02, k03, k04, k05, k06, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k10, k11, k12, k13, k14, k15, k16, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k20, k21, k22, k23, k24, k25, k26, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k30, k31, k32, k33, k34, k35, k36, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }  \
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_BTN1, KC_BTN2,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_BTN3, KC_NO,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_NO,   KC_NO,
        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  MO(1),   KC_NO,   KC_NO
    ),
    [_LOWER] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_RSFT, KC_INS,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,   KC_NO
    ),
    [_RAISE] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LSFT, KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_TRNS, KC_NO,   KC_NO
    ),
    [_ADJUST] = LAYOUT(
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
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

const bmp_ex_keycode_t custom_keys_user[] = {};
