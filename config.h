#pragma once

// Matrix: 4 rows x 7 cols
#define MATRIX_ROWS 4
#define MATRIX_COLS 7
#define MATRIX_ROW_PINS { 9, 10, 47, 46 }
#define MATRIX_COL_PINS { 35, 32, 23, 22, 20, 19, 18 }
#define DIODE_DIRECTION COL2ROW

// AZ1UBALL I2C (SDA=D1=GPIO5, SCL=D0=GPIO6)
#define I2C1_SDA_PIN 5
#define I2C1_SCL_PIN 6

// pimoroni_trackball (AZ1UBALL)
#define PIMORONI_TRACKBALL_INTERVAL_MS 8
