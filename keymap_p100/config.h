/* Custom config for GMMK3 P100 ISO – German (AT/DE) Layout
 * Author: Custom keymap for Key Chatter Fix + RGB customization
 *
 * Ported from GMMK2 P96 ISO custom firmware.
 */

#pragma once

/* ─── Key Chatter / Debouncing ─────────────────────────────────────────
 * Some switches bounce up to ~20ms+. A value of 25 gives a safe margin
 * to eliminate all double-keypress artefacts.
 * QMK default is 5ms which is too low for chattering switches.
 */
#define DEBOUNCE 25

/* ─── EEPROM / VIA Dynamic Keymap ──────────────────────────────────────
 * The default wear-leveling backing (4096 bytes) may be too small for
 * VIA's dynamic keymaps on a 100% board. Increase to 8 KB.
 * Also limit dynamic keymap layers to 2 to fit within EEPROM.
 */
#undef WEAR_LEVELING_BACKING_SIZE
#define WEAR_LEVELING_BACKING_SIZE 8192
#define DYNAMIC_KEYMAP_LAYER_COUNT 2

/* ─── Encoder Configuration ───────────────────────────────────────────
 * The rotary encoder resolution controls sensitivity.
 */
#define ENCODER_RESOLUTION 4

/* ─── RGB Matrix Overrides ─────────────────────────────────────────────
 * We run a fully custom rgb_matrix_indicators_advanced_user() routine,
 * so we configure the matrix to start in a known state.
 */

/* Default mode: solid colour (we override it anyway) */
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR

/* Default HSV: dark blue, full saturation, moderate brightness */
#define RGB_MATRIX_DEFAULT_HUE  160   /* blue hue in QMK HSV (0-255 range) */
#define RGB_MATRIX_DEFAULT_SAT  255
#define RGB_MATRIX_DEFAULT_VAL  100   /* not too bright */

/* Enable the reactive effects we rely on internally */
#define RGB_MATRIX_KEYPRESSES         /* enables per-key reactive hooks */

/* Max number of reactive key events tracked at once */
#define RGB_MATRIX_MAX_KEY_EVENTS 16

/* Timeout for reactive animations (ms) – how long the flash lasts */
#define RGB_MATRIX_REACTIVE_TIMEOUT 600
