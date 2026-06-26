#include QMK_KEYBOARD_H
#include "tmk_core/protocol/nrf/keycode_str_converter.h"
#include "bmp_matrix.h"
#include "apidef.h"

bool isScrollMode;

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST };

// BMP CONFIG.JSN layout for cocot46:
//   rows=4, cols=14, device_rows=4, device_cols=7, diode_direction=4 (COL2ROW2COL)
//
// The stock matrix_func_col2row2col fills:
//   matrix_debouncing[0..3]: col2row data (bits 0-6)
//   matrix_debouncing[4..7]: row2col data (bits 0-6)   <- never debounced!
//
// bmp_debounce is called with device_row=4, device_col=14, so it expects
// bits 0-6 = col2row and bits 7-13 = row2col in the SAME rows 0-3.
//
// This override combines both phases correctly so all 49 keys reach the BMP.

extern const bmp_matrix_func_t matrix_func_row2col;
extern const bmp_matrix_func_t matrix_func_col2row;

static matrix_row_t cocot46_r2c_buf[8];

static void cocot46_matrix_init(void) {
    matrix_func_col2row.init();
}

static uint32_t cocot46_get_rows(void) {
    return BMPAPI->app.get_config()->matrix.device_rows;  // 4
}

static uint32_t cocot46_get_cols(void) {
    // col2row occupies bits 0-6, row2col occupies bits 7-13 → 14 total
    return BMPAPI->app.get_config()->matrix.device_cols * 2;  // 14
}

static uint32_t cocot46_scan(matrix_row_t *matrix_raw) {
    const bmp_api_config_t *config      = BMPAPI->app.get_config();
    const uint8_t           device_rows = config->matrix.device_rows;  // 4
    const uint8_t           device_cols = config->matrix.device_cols;  // 7
    const uint32_t          col_mask    = (1u << device_cols) - 1;     // 0x7F
    uint32_t                change      = 0;

    // Phase 1: col2row → fills matrix_raw[0..3] bits 0-6
    matrix_func_col2row.init();
    change |= matrix_func_col2row.scan(matrix_raw);

    // Phase 2: row2col → capture into temporary buffer[0..3] bits 0-6
    matrix_func_row2col.init();
    change |= matrix_func_row2col.scan(cocot46_r2c_buf);

    // Combine: row2col goes into bits 7-13 of the same rows
    for (uint8_t r = 0; r < device_rows; r++) {
        matrix_raw[r] = (matrix_raw[r] & col_mask) | ((cocot46_r2c_buf[r] & col_mask) << device_cols);
    }

    return change;
}

static const bmp_matrix_func_t cocot46_matrix_func = {
    cocot46_matrix_init,
    cocot46_get_rows,
    cocot46_get_cols,
    cocot46_scan
};

const bmp_matrix_func_t *get_matrix_func_user(void) {
    return &cocot46_matrix_func;
}

// LAYOUT macro: BMP returns logical (layout_row, layout_col) after remapping.
// CONFIG.JSN layout rows → logical positions:
//   Row 0: 12 keys → logical (0, 0..11)
//   Row 1: 12 keys → logical (1, 0..11)
//   Row 2: 14 keys → logical (2, 0..13)
//   Row 3: 11 keys → logical (3, 0..10)
#undef LAYOUT
#define LAYOUT( \
    k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, \
    k0c, k0d, k0e, k0f, k10, k11, k12, k13, k14, k15, k16, k17, \
    k18, k19, k1a, k1b, k1c, k1d, k1e, k1f, k20, k21, k22, k23, k24, k25, \
    k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, k2f, k30 \
) { \
    { k00, k01, k02, k03, k04, k05, k06, k07, k08, k09, k0a, k0b, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k0c, k0d, k0e, k0f, k10, k11, k12, k13, k14, k15, k16, k17, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k18, k19, k1a, k1b, k1c, k1d, k1e, k1f, k20, k21, k22, k23, k24, k25, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { k26, k27, k28, k29, k2a, k2b, k2c, k2d, k2e, k2f, k30, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
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

// AZ1UBALL via BMPAPI I2C
// GPIO 18 (SDA) and GPIO 16 (SCL) overlap with matrix col pins.
// TWIM must be released after each read so the matrix scanner can use those GPIOs.
#include "pointing_device.h"

#define AZ1UBALL_ADDR     0x0A
#define AZ1UBALL_REG_LEFT 0x04

// nRF52840: TWIM0 base=0x40003000, ENABLE register offset=0x500
// Writing 0 disables TWIM0, releasing GPIO 18/16 back to normal GPIO
#define NRF_TWIM0_ENABLE_REG (*((volatile uint32_t *)(0x40003000 + 0x500)))

static inline void az1uball_i2c_init(void) {
    const bmp_api_i2cm_config_t cfg = {.freq = I2C_FREQ_400K, .scl = CONFIG_PIN_SCL, .sda = CONFIG_PIN_SDA};
    BMPAPI->i2cm.init(&cfg);
}

static inline void az1uball_i2c_release(void) {
    NRF_TWIM0_ENABLE_REG = 0;
}

void pointing_device_init(void) {
    az1uball_i2c_init();
    // No release — TWIM holds GPIO 18/16 (phantom keys will appear, this is a diagnostic build)
}

void pointing_device_task(void) {
    static uint16_t last_read = 0;
    if (timer_elapsed(last_read) < 20) {
        pointing_device_send();
        return;
    }
    last_read = timer_read();

    // No re-init, no release — raw read with always-active TWIM
    uint8_t data[9] = {0};
    report_mouse_t rep = pointing_device_get_report();
    int8_t result = BMPAPI->i2cm.read_reg(0x7F, 0x00, data, 9, 100);  // invalid addr test

    if (result == 0) {
        // data[0] = chip ID (0x49 if pimoroni-compatible)
        // data[4..7] = left, right, up, down motion
        rep.x = (int8_t)data[0] + 1;
    } else {
        rep.x = 2;
    }

    pointing_device_set_report(rep);
    pointing_device_send();
}

const key_string_map_t custom_keys_user = {0, 0, ""};
