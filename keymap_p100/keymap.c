/* Copyright 2024 – Custom ISO-DE Keymap for GMMK3 P100 ISO
 *
 * Ported from GMMK2 P96 ISO custom firmware.
 *
 * Features:
 *   - German (DE/AT) ISO layout with Umlaute (Ö, Ä, Ü, ß)
 *   - Z/Y swapped for QWERTZ
 *   - Rotary encoder for volume control (rotate) and mute (press)
 *   - Selectable static base colour (10 presets incl. Off), cycled with
 *     FN + Left / FN + Right
 *   - Global brightness controlled by FN + Up / FN + Down (RM_VALU/RM_VALD)
 *   - ESC / ENTER trigger a reactive RED wave effect
 *   - Every other key flashes GREEN briefly on press
 *   - Side LEDs follow the selected base colour
 *   - FN + Encoder press toggles side LEDs on/off (persistent)
 *   - Num Lock LED indicator (dark when Num Lock is OFF)
 *   - VIA-compatible (2 layers: Base + Function)
 *   - DIP switch for Win/Mac layer switching
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  Layer definitions
 * ═══════════════════════════════════════════════════════════════════════ */
enum layers {
    WIN_BL = 0,   /* Windows Base Layer  */
    WIN_FL = 1,   /* Windows Function Layer */
    MACOS_BL = 2, /* Mac OS Base Layer */
    MACOS_FL = 3  /* Mac OS Function Layer */
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Custom keycodes  (must come before the keymap)
 * ═══════════════════════════════════════════════════════════════════════ */
enum custom_keycodes {
    RGB_BASE_NEXT = SAFE_RANGE,   /* FN + Right  →  next base colour  */
    RGB_BASE_PREV,                /* FN + Left   →  prev base colour  */
    RGB_SIDE_TOG,                 /* FN + Encoder press → toggle side LEDs */
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Base-colour presets
 *  Colours are stored at full (255) intensity; the global QMK brightness
 *  value (rgb_matrix_get_val()) scales them at render time so that
 *  FN+Up / FN+Down actually changes the perceived brightness.
 *  Last entry (0,0,0) = "Off" – reactive effects still work.
 * ═══════════════════════════════════════════════════════════════════════ */
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_color_t;

static const rgb_color_t BASE_COLORS[] = {
    {  0,   0, 255},   /* 0 – Blau      (Standard)  */
    {255, 200,   0},   /* 1 – Gelb                  */
    {148,   0, 211},   /* 2 – Violett               */
    {255,  20, 147},   /* 3 – Rosa                  */
    {  0, 200, 200},   /* 4 – Türkis                */
    {255,  80,   0},   /* 5 – Orange                */
    { 50, 255,  50},   /* 6 – Hellgrün              */
    {255, 255, 255},   /* 7 – Weiß                  */
    {255,   0,   0},   /* 8 – Rot                   */
    {  0,   0,   0},   /* 9 – Aus (nur Reaktiv-Effekte sichtbar) */
};
#define BASE_COLOR_COUNT ((uint8_t)(sizeof(BASE_COLORS) / sizeof(BASE_COLORS[0])))

/* ─── LED index ranges ──────────────────────────────────────────────
 * From iso.c:
 *   0-104  = Key LEDs (105 keys)
 *   105-114 = Left side LEDs  (SLED1-10)
 *   115-124 = Right side LEDs (SLED11-20)
 *   125     = Logo LED
 * ─────────────────────────────────────────────────────────────────── */
#define SIDE_LED_START  105
#define SIDE_LED_END    125   /* exclusive: 105..124 = 20 side LEDs */
#define LOGO_LED        125

/* ─── EEPROM persistence ─────────────────────────────────────────────
 * QMK provides one 32-bit "user" EEPROM slot free of charge.
 * We store the colour index in the lowest byte, and the side-LED
 * on/off state in the next bit.
 * ─────────────────────────────────────────────────────────────────── */
typedef union {
    uint32_t raw;
    struct {
        uint8_t color_idx  : 8;   /* which base colour is active */
        bool    side_on    : 1;   /* side LEDs enabled? */
    };
} user_config_t;

static user_config_t user_config;
static uint8_t       base_color_idx = 0;   /* set properly in keyboard_post_init_user */
static bool          side_leds_on   = true;

/* Called by QMK when the EEPROM is reset (e.g. after QK_BOOT flash) */
void eeconfig_init_user(void) {
    user_config.raw       = 0;
    user_config.color_idx = 0;       /* default: Blau */
    user_config.side_on   = true;    /* side LEDs on by default */
    eeconfig_update_user(user_config.raw);
}

/* Called once after the keyboard has fully initialised */
void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    /* Sanity-check: if the stored value is out of range, reset to 0 */
    if (user_config.color_idx >= BASE_COLOR_COUNT) {
        user_config.color_idx = 0;
    }
    base_color_idx = user_config.color_idx;
    side_leds_on   = user_config.side_on;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  Keymap
 * ═══════════════════════════════════════════════════════════════════════
 *
 *  GMMK3 P100 ISO – Full-size keyboard with rotary encoder (knob)
 *
 *  The LAYOUT macro maps keys to the physical matrix as defined in
 *  the keyboard's info.json.
 *
 *  Row 0 (Function):  ESC, F1-F12, PrtSc, ScrLk, Pause, Knob (encoder)
 *  Row 1 (Numbers):   `, 1-0, -, =, BS, Ins, Home, PgUp, NumLk, /, *, -
 *  Row 2 (QWERTY):    Tab, Q-], Del, End, PgDn, 7, 8, 9, +
 *  Row 3 (Home):      Caps, A-#, Enter, 4, 5, 6
 *  Row 4 (Shift):     LShft, \, Z-/, RShft, Up, 1, 2, 3, NumEnt
 *  Row 5 (Mods):      LCtrl, Win, LAlt, Space, RAlt, FN, App, RCtrl,
 *                     Left, Down, Right, 0, .
 *
 *  FN-Layer highlights:
 *    ESC          → QK_BOOT  (Bootloader)
 *    F1-F11       → Media keys
 *    FN + Up      → RM_VALU  (Helligkeit +)
 *    FN + Down    → RM_VALD  (Helligkeit –)
 *    FN + Right   → RGB_BASE_NEXT  (nächste Basisfarbe)
 *    FN + Left    → RGB_BASE_PREV  (vorherige Basisfarbe)
 *    FN + Knob    → RGB_SIDE_TOG   (Seiten-LEDs an/aus)
 *    Encoder      → Lautstärke drehen / Mute drücken
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* ──── Windows Base Layer (WIN_BL) ─── German QWERTZ ISO ────────────── */
[WIN_BL] = LAYOUT(
    KC_ESC,            KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                              KC_MUTE,

    DE_CIRC, DE_1,     DE_2,    DE_3,    DE_4,    DE_5,    DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    DE_SS,   DE_ACUT, KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
    KC_TAB,  DE_Q,     DE_W,    DE_E,    DE_R,    DE_T,    DE_Z,    DE_U,    DE_I,    DE_O,    DE_P,    DE_UDIA, DE_PLUS,             KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
    KC_CAPS, DE_A,     DE_S,    DE_D,    DE_F,    DE_G,    DE_H,    DE_J,    DE_K,    DE_L,    DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,                                  KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, DE_LABK,  DE_Y,    DE_X,    DE_C,    DE_V,    DE_B,    DE_N,    DE_M,    DE_COMM, DE_DOT,  DE_MINS,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PENT,
    KC_LCTL, KC_LGUI,  KC_LALT,                            KC_SPC,                            KC_RALT, MO(WIN_FL), KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT
),

/* ──── Windows Function Layer (WIN_FL) ──────────────────────────────── */
[WIN_FL] = LAYOUT(
    QK_BOOT,           KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MRWD, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLU, KC_VOLD, _______,    _______, _______, _______,                              RGB_SIDE_TOG,

    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, KC_HOME,    _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, KC_END,     _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                                 _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             RM_VALU,             _______, _______, _______, _______,
    _______, GU_TOGG,  _______,                            _______,                            _______, _______, _______, _______,    RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT, _______,          _______
),

/* ──── Mac OS Base Layer (MACOS_BL) ─── German QWERTZ ISO ───────────── */
[MACOS_BL] = LAYOUT(
    KC_ESC,            KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                              KC_MUTE,

    DE_CIRC, DE_1,     DE_2,    DE_3,    DE_4,    DE_5,    DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    DE_SS,   DE_ACUT, KC_BSPC,    KC_INS,  KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
    KC_TAB,  DE_Q,     DE_W,    DE_E,    DE_R,    DE_T,    DE_Z,    DE_U,    DE_I,    DE_O,    DE_P,    DE_UDIA, DE_PLUS,             KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
    KC_CAPS, DE_A,     DE_S,    DE_D,    DE_F,    DE_G,    DE_H,    DE_J,    DE_K,    DE_L,    DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,                                  KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, DE_LABK,  DE_Y,    DE_X,    DE_C,    DE_V,    DE_B,    DE_N,    DE_M,    DE_COMM, DE_DOT,  DE_MINS,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PENT,
    KC_LCTL, KC_LOPT,  KC_LCMD,                            KC_SPC,                            KC_RCMD, MO(MACOS_FL), KC_ROPT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0,            KC_PDOT
),

/* ──── Mac OS Function Layer (MACOS_FL) ─────────────────────────────── */
[MACOS_FL] = LAYOUT(
    QK_BOOT,           KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MRWD, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLU, KC_VOLD, _______,    _______, _______, _______,                              RGB_SIDE_TOG,

    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, KC_HOME,    _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, KC_END,     _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                                 _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             RM_VALU,             _______, _______, _______, _______,
    _______, _______,  _______,                            _______,                            _______, _______, _______, _______,    RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT, _______,          _______
),
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Rotary Encoder – Volume Control
 *  Rotate: Volume Up / Volume Down
 *  Press:  Mute (handled in keymap as KC_MUTE / RGB_SIDE_TOG via FN)
 * ═══════════════════════════════════════════════════════════════════════ */
#if defined(ENCODER_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    return false;  /* don't process further */
}
#endif

/* ═══════════════════════════════════════════════════════════════════════
 *  DIP Switch – Win/Mac Layer Switching
 *  Position 0: ON = Windows, OFF = Mac OS
 * ═══════════════════════════════════════════════════════════════════════ */
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        if (active) {
            layer_off(MACOS_BL);
            layer_on(WIN_BL);
        } else {
            layer_off(WIN_BL);
            layer_on(MACOS_BL);
        }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  process_record_user
 *  – cycles the base colour index on RGB_BASE_NEXT / RGB_BASE_PREV
 *  – toggles side LEDs on RGB_SIDE_TOG
 *  – records reactive flash events for all other keypresses
 * ═══════════════════════════════════════════════════════════════════════ */

/* ─── Reactive event ring-buffer ──────────────────────────────────── */
#define MAX_EVENTS            16
#define REACTIVE_TIMEOUT_MS  500
#define WAVE_RADIUS           80

typedef struct {
    uint8_t  led_idx;
    uint32_t timestamp;
    bool     is_special;   /* true = ESC/ENTER → red wave; false → green flash */
    bool     active;
} key_event_t;

static key_event_t key_events[MAX_EVENTS];
static uint8_t     event_head = 0;

static void record_event(uint8_t led, bool special) {
    key_events[event_head].led_idx    = led;
    key_events[event_head].timestamp  = timer_read32();
    key_events[event_head].is_special = special;
    key_events[event_head].active     = true;
    event_head = (event_head + 1) % MAX_EVENTS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case RGB_BASE_NEXT:
                base_color_idx = (base_color_idx + 1) % BASE_COLOR_COUNT;
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;   /* handled; don't pass through */

            case RGB_BASE_PREV:
                base_color_idx = (base_color_idx == 0)
                                 ? (BASE_COLOR_COUNT - 1)
                                 : (base_color_idx - 1);
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;

            case RGB_SIDE_TOG:
                side_leds_on = !side_leds_on;
                user_config.side_on = side_leds_on;
                eeconfig_update_user(user_config.raw);
                return false;

            default: {
                /* Record a reactive flash event for the pressed key */
                uint8_t row = record->event.key.row;
                uint8_t col = record->event.key.col;
                uint8_t led = g_led_config.matrix_co[row][col];
                if (led != NO_LED) {
                    bool special = (keycode == KC_ESC  ||
                                    keycode == KC_ENT  ||
                                    keycode == KC_PENT);
                    record_event(led, special);
                }
                break;
            }
        }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  rgb_matrix_indicators_advanced_user – per-frame render loop
 *
 *  1. Paint all key LEDs with the selected base colour, scaled by the
 *     global QMK brightness (rgb_matrix_get_val()).  This makes
 *     FN+Up / FN+Down actually change what you see.
 *  2. Paint side LEDs in the same base colour (if enabled).
 *  3. Paint the logo LED in the base colour.
 *  4. Overlay reactive effects from the ring-buffer on top.
 *  5. Num Lock indicator on the Num Lock key.
 * ═══════════════════════════════════════════════════════════════════════ */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    /* ── Compute brightness-scaled base colour ────────────────────── */
    uint8_t val = rgb_matrix_get_val();   /* 0-255, controlled by RM_VALU/D */
    rgb_color_t bc = BASE_COLORS[base_color_idx];
    uint8_t base_r = (uint8_t)((uint16_t)bc.r * val / 255);
    uint8_t base_g = (uint8_t)((uint16_t)bc.g * val / 255);
    uint8_t base_b = (uint8_t)((uint16_t)bc.b * val / 255);

    /* ── 1. Paint every LED with the base colour ──────────────────── */
    for (uint8_t i = led_min; i < led_max; i++) {
        /* Side LEDs: respect the toggle */
        if (i >= SIDE_LED_START && i < SIDE_LED_END) {
            if (side_leds_on) {
                rgb_matrix_set_color(i, base_r, base_g, base_b);
            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        /* Logo LED: always follows base colour */
        else if (i == LOGO_LED) {
            rgb_matrix_set_color(i, base_r, base_g, base_b);
        }
        /* Key LEDs: base colour */
        else {
            rgb_matrix_set_color(i, base_r, base_g, base_b);
        }
    }

    /* ── 1b. Num Lock indicator ────────────────────────────────────
     * Num Lock key LED goes dark when Num Lock is OFF – acts as a
     * subtle status indicator against any base colour.
     *
     * From info.json, Num Lock is at matrix position [11, 4].
     * ──────────────────────────────────────────────────────────── */
    {
        uint8_t num_led = g_led_config.matrix_co[11][4];
        if (num_led != NO_LED && num_led >= led_min && num_led < led_max) {
            if (!host_keyboard_led_state().num_lock) {
                rgb_matrix_set_color(num_led, 0, 0, 0);   /* off = Num Lock aus */
            }
        }
    }

    /* ── 2. Overlay reactive effects ──────────────────────────────── */
    for (uint8_t e = 0; e < MAX_EVENTS; e++) {
        if (!key_events[e].active) continue;

        uint32_t elapsed = timer_elapsed32(key_events[e].timestamp);
        if (elapsed > REACTIVE_TIMEOUT_MS) {
            key_events[e].active = false;
            continue;
        }

        /* Fade-out factor: full brightness at t=0, zero at t=TIMEOUT */
        uint8_t brightness = (uint8_t)(255 - (elapsed * 255) / REACTIVE_TIMEOUT_MS);
        uint8_t src_led    = key_events[e].led_idx;

        if (key_events[e].is_special) {
            /* ── Red wave radiating from ESC / ENTER ─────────────── */
            uint8_t src_x     = g_led_config.point[src_led].x;
            uint8_t src_y     = g_led_config.point[src_led].y;
            uint8_t wave_front = (uint8_t)((elapsed * WAVE_RADIUS) / REACTIVE_TIMEOUT_MS);
            uint8_t wave_width = 14;

            for (uint8_t i = led_min; i < led_max; i++) {
                uint8_t  dx   = (uint8_t)abs((int16_t)g_led_config.point[i].x - src_x);
                uint8_t  dy   = (uint8_t)abs((int16_t)g_led_config.point[i].y - src_y);
                uint16_t dist = dx + dy;

                if (dist <= (uint16_t)(wave_front + wave_width) &&
                    dist + wave_width >= wave_front) {
                    int16_t diff = (int16_t)wave_front - (int16_t)dist;
                    if (diff < 0) diff = -diff;
                    uint8_t lb = (diff < wave_width)
                        ? (uint8_t)((uint16_t)brightness * (wave_width - diff) / wave_width)
                        : 0;
                    if (lb > 0) rgb_matrix_set_color(i, lb, 0, 0);
                }
            }
        } else {
            /* ── Green flash on the single pressed key ───────────── */
            if (src_led >= led_min && src_led < led_max) {
                rgb_matrix_set_color(src_led, 0, brightness, 0);
            }
        }
    }

    return false;
}
