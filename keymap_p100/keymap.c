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
 *   - Lock-key pulse indicators: Caps Lock, Num Lock, Insert
 *   - FN-layer key highlighting (only mapped keys glow while FN held)
 *   - DIP switch for Win/Mac layer switching
 *   - VIA-compatible (4 layers: Win-Base, Win-FN, Mac-Base, Mac-FN)
 */

#include QMK_KEYBOARD_H
#include "keymap_german.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  Layer definitions
 * ═══════════════════════════════════════════════════════════════════════ */
enum layers {
    WIN_BL = 0,
    WIN_FL = 1,
    MACOS_BL = 2,
    MACOS_FL = 3
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Custom keycodes
 * ═══════════════════════════════════════════════════════════════════════ */
enum custom_keycodes {
    RGB_BASE_NEXT = SAFE_RANGE,
    RGB_BASE_PREV,
    RGB_SIDE_TOG,
    CUS_INS,       /* custom Insert handler for long-press tracking */
};

/* ═══════════════════════════════════════════════════════════════════════
 *  Base-colour presets
 * ═══════════════════════════════════════════════════════════════════════ */
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb_color_t;

static const rgb_color_t BASE_COLORS[] = {
    {  0,   0, 255},   /* 0 – Blau      */
    {255, 200,   0},   /* 1 – Gelb      */
    {148,   0, 211},   /* 2 – Violett   */
    {255,  20, 147},   /* 3 – Rosa      */
    {  0, 200, 200},   /* 4 – Türkis    */
    {255,  80,   0},   /* 5 – Orange    */
    { 50, 255,  50},   /* 6 – Hellgrün  */
    {255, 255, 255},   /* 7 – Weiß      */
    {255,   0,   0},   /* 8 – Rot       */
    {  0,   0,   0},   /* 9 – Aus       */
};
#define BASE_COLOR_COUNT ((uint8_t)(sizeof(BASE_COLORS) / sizeof(BASE_COLORS[0])))

/* ─── LED index ranges (from iso.c) ──────────────────────────────── */
#define SIDE_LED_START  105
#define SIDE_LED_END    125
#define LOGO_LED        125

/* ─── EEPROM persistence ─────────────────────────────────────────── */
typedef union {
    uint32_t raw;
    struct {
        uint8_t color_idx  : 8;
        bool    side_on    : 1;
    };
} user_config_t;

static user_config_t user_config;
static uint8_t       base_color_idx = 0;
static bool          side_leds_on   = true;

/* ═══════════════════════════════════════════════════════════════════════
 *  Lock-key pulse animation
 * ═══════════════════════════════════════════════════════════════════════ */
#define PULSE_DURATION_MS  3000
#define PULSE_COUNT        5
#define INS_LONG_PRESS_MS  2000

typedef struct {
    uint32_t start_time;
    bool     active;
    bool     is_green;
    uint8_t  led_idx;
} pulse_anim_t;

static pulse_anim_t caps_pulse = {0};
static pulse_anim_t num_pulse  = {0};
static pulse_anim_t ins_pulse  = {0};

/* Insert mode tracking (no OS feedback available) */
static bool     insert_mode_on = true;
static uint32_t ins_press_time = 0;
static bool     ins_key_held   = false;
static uint8_t  ins_row = 0, ins_col = 0;

/* Compute pulse brightness: triangle wave, 5 cycles in 3 seconds */
static uint8_t calc_pulse_brightness(uint32_t elapsed) {
    if (elapsed >= PULSE_DURATION_MS) return 0;
    uint32_t cycle_len = PULSE_DURATION_MS / PULSE_COUNT;
    uint32_t cycle_pos = elapsed % cycle_len;
    uint16_t half = cycle_len / 2;
    if (cycle_pos < half)
        return (uint8_t)((cycle_pos * 255) / half);
    else
        return (uint8_t)(((cycle_len - cycle_pos) * 255) / half);
}

/* Helper: start a pulse animation */
static void start_pulse(pulse_anim_t *p, uint8_t led, bool green) {
    p->start_time = timer_read32();
    p->active     = true;
    p->is_green   = green;
    p->led_idx    = led;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  EEPROM init / post-init
 * ═══════════════════════════════════════════════════════════════════════ */
void eeconfig_init_user(void) {
    user_config.raw       = 0;
    user_config.color_idx = 0;
    user_config.side_on   = true;
    eeconfig_update_user(user_config.raw);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    if (user_config.color_idx >= BASE_COLOR_COUNT) user_config.color_idx = 0;
    base_color_idx = user_config.color_idx;
    side_leds_on   = user_config.side_on;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  Keymap
 * ═══════════════════════════════════════════════════════════════════════ */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[WIN_BL] = LAYOUT(
    KC_ESC,            KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                              KC_MUTE,

    DE_CIRC, DE_1,     DE_2,    DE_3,    DE_4,    DE_5,    DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    DE_SS,   DE_ACUT, KC_BSPC,    CUS_INS, KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
    KC_TAB,  DE_Q,     DE_W,    DE_E,    DE_R,    DE_T,    DE_Z,    DE_U,    DE_I,    DE_O,    DE_P,    DE_UDIA, DE_PLUS,             KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
    KC_CAPS, DE_A,     DE_S,    DE_D,    DE_F,    DE_G,    DE_H,    DE_J,    DE_K,    DE_L,    DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,                                  KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, DE_LABK,  DE_Y,    DE_X,    DE_C,    DE_V,    DE_B,    DE_N,    DE_M,    DE_COMM, DE_DOT,  DE_MINS,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PENT,
    KC_LCTL, KC_LGUI,  KC_LALT,                            KC_SPC,                            KC_RALT, MO(WIN_FL), KC_APP, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT,    KC_P0,            KC_PDOT
),

[WIN_FL] = LAYOUT(
    QK_BOOT,           KC_MYCM, KC_WHOM, KC_CALC, KC_MSEL, KC_MPRV, KC_MRWD, KC_MPLY, KC_MSTP, KC_MUTE, KC_VOLU, KC_VOLD, _______,    _______, _______, _______,                              RGB_SIDE_TOG,

    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,    _______, _______, KC_HOME,    _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, KC_END,     _______, _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                                 _______, _______, _______,
    _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,             RM_VALU,             _______, _______, _______, _______,
    _______, GU_TOGG,  _______,                            _______,                            _______, _______, _______, _______,    RGB_BASE_PREV, RM_VALD, RGB_BASE_NEXT, _______,          _______
),

[MACOS_BL] = LAYOUT(
    KC_ESC,            KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_PSCR, KC_SCRL, KC_PAUS,                              KC_MUTE,

    DE_CIRC, DE_1,     DE_2,    DE_3,    DE_4,    DE_5,    DE_6,    DE_7,    DE_8,    DE_9,    DE_0,    DE_SS,   DE_ACUT, KC_BSPC,    CUS_INS, KC_HOME, KC_PGUP,    KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,
    KC_TAB,  DE_Q,     DE_W,    DE_E,    DE_R,    DE_T,    DE_Z,    DE_U,    DE_I,    DE_O,    DE_P,    DE_UDIA, DE_PLUS,             KC_DEL,  KC_END,  KC_PGDN,    KC_P7,   KC_P8,   KC_P9,   KC_PPLS,
    KC_CAPS, DE_A,     DE_S,    DE_D,    DE_F,    DE_G,    DE_H,    DE_J,    DE_K,    DE_L,    DE_ODIA, DE_ADIA, DE_HASH, KC_ENT,                                  KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, DE_LABK,  DE_Y,    DE_X,    DE_C,    DE_V,    DE_B,    DE_N,    DE_M,    DE_COMM, DE_DOT,  DE_MINS,          KC_RSFT,             KC_UP,               KC_P1,   KC_P2,   KC_P3,   KC_PENT,
    KC_LCTL, KC_LOPT,  KC_LCMD,                            KC_SPC,                            KC_RCMD, MO(MACOS_FL), KC_ROPT, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT, KC_P0,            KC_PDOT
),

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
 *  Rotary Encoder
 * ═══════════════════════════════════════════════════════════════════════ */
#if defined(ENCODER_ENABLE)
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        tap_code(clockwise ? KC_VOLU : KC_VOLD);
    }
    return false;
}
#endif

/* ═══════════════════════════════════════════════════════════════════════
 *  DIP Switch
 * ═══════════════════════════════════════════════════════════════════════ */
bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0) {
        if (active) { layer_off(MACOS_BL); layer_on(WIN_BL); }
        else        { layer_off(WIN_BL);   layer_on(MACOS_BL); }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  process_record_user
 * ═══════════════════════════════════════════════════════════════════════ */
#define MAX_EVENTS            16
#define REACTIVE_TIMEOUT_MS  500
#define WAVE_RADIUS           80

typedef struct {
    uint8_t  led_idx;
    uint32_t timestamp;
    bool     is_special;
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
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;
    uint8_t led = g_led_config.matrix_co[row][col];

    if (record->event.pressed) {
        switch (keycode) {
            case RGB_BASE_NEXT:
                base_color_idx = (base_color_idx + 1) % BASE_COLOR_COUNT;
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;

            case RGB_BASE_PREV:
                base_color_idx = (base_color_idx == 0) ? (BASE_COLOR_COUNT - 1) : (base_color_idx - 1);
                user_config.color_idx = base_color_idx;
                eeconfig_update_user(user_config.raw);
                return false;

            case RGB_SIDE_TOG:
                side_leds_on = !side_leds_on;
                user_config.side_on = side_leds_on;
                eeconfig_update_user(user_config.raw);
                return false;

            case KC_CAPS:
                if (led != NO_LED) {
                    bool will_be_on = !host_keyboard_led_state().caps_lock;
                    start_pulse(&caps_pulse, led, !will_be_on);
                }
                break;

            case KC_NUM:
                if (led != NO_LED) {
                    bool will_be_on = !host_keyboard_led_state().num_lock;
                    start_pulse(&num_pulse, led, will_be_on);
                }
                break;

            case CUS_INS:
                ins_press_time = timer_read32();
                ins_key_held   = true;
                ins_row = row;
                ins_col = col;
                if (led != NO_LED) record_event(led, false);
                return false;   /* we handle this key entirely */

            default:
                if (led != NO_LED) {
                    bool special = (keycode == KC_ESC || keycode == KC_ENT || keycode == KC_PENT);
                    record_event(led, special);
                }
                break;
        }
    } else {
        /* Key released */
        if (keycode == CUS_INS && ins_key_held) {
            uint8_t ins_led = g_led_config.matrix_co[ins_row][ins_col];
            uint32_t hold_time = timer_elapsed32(ins_press_time);
            if (hold_time >= INS_LONG_PRESS_MS) {
                /* Long press: reset insert tracking to ON, no key sent */
                insert_mode_on = true;
                if (ins_led != NO_LED) start_pulse(&ins_pulse, ins_led, true);
            } else {
                /* Short press: toggle insert mode and send key */
                insert_mode_on = !insert_mode_on;
                tap_code(KC_INS);
                if (ins_led != NO_LED) start_pulse(&ins_pulse, ins_led, insert_mode_on);
            }
            ins_key_held = false;
            return false;
        }
    }
    return true;
}

/* ═══════════════════════════════════════════════════════════════════════
 *  rgb_matrix_indicators_advanced_user – per-frame render loop
 * ═══════════════════════════════════════════════════════════════════════ */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t val = rgb_matrix_get_val();
    rgb_color_t bc = BASE_COLORS[base_color_idx];
    uint8_t base_r = (uint8_t)((uint16_t)bc.r * val / 255);
    uint8_t base_g = (uint8_t)((uint16_t)bc.g * val / 255);
    uint8_t base_b = (uint8_t)((uint16_t)bc.b * val / 255);
    bool backlight_on = (base_r > 0 || base_g > 0 || base_b > 0);

    /* ── FN-layer highlighting ────────────────────────────────────── */
    bool fn_active = layer_state_is(WIN_FL) || layer_state_is(MACOS_FL);
    if (fn_active) {
        uint8_t fn_layer = layer_state_is(MACOS_FL) ? MACOS_FL : WIN_FL;
        /* Turn off all LEDs first */
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        /* Light up only keys with FN bindings */
        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            for (uint8_t c = 0; c < MATRIX_COLS; c++) {
                uint8_t led_i = g_led_config.matrix_co[r][c];
                if (led_i == NO_LED || led_i < led_min || led_i >= led_max) continue;
                uint16_t fn_kc = keymap_key_to_keycode(fn_layer, (keypos_t){.row = r, .col = c});
                if (fn_kc != KC_TRNS && fn_kc != KC_NO) {
                    if (backlight_on)
                        rgb_matrix_set_color(led_i, base_r, base_g, base_b);
                    else
                        rgb_matrix_set_color(led_i, 0, 255, 0);
                }
            }
        }
        return false;  /* skip reactive effects while FN held */
    }

    /* ── 1. Paint every LED with the base colour ──────────────────── */
    for (uint8_t i = led_min; i < led_max; i++) {
        if (i >= SIDE_LED_START && i < SIDE_LED_END) {
            rgb_matrix_set_color(i, side_leds_on ? base_r : 0, side_leds_on ? base_g : 0, side_leds_on ? base_b : 0);
        } else {
            rgb_matrix_set_color(i, base_r, base_g, base_b);
        }
    }

    /* ── 1b. Lock-key persistent state (after pulse) ──────────────── */
    /* Caps Lock: abnormal = ON → dark */
    if (!caps_pulse.active && host_keyboard_led_state().caps_lock) {
        uint8_t cl = caps_pulse.led_idx;
        if (cl != NO_LED && cl >= led_min && cl < led_max)
            rgb_matrix_set_color(cl, 0, 0, 0);
    }
    /* Num Lock: abnormal = OFF → dark */
    {
        uint8_t nl = g_led_config.matrix_co[11][4];
        if (!num_pulse.active && nl != NO_LED && nl >= led_min && nl < led_max) {
            if (!host_keyboard_led_state().num_lock)
                rgb_matrix_set_color(nl, 0, 0, 0);
        }
    }
    /* Insert: abnormal = OFF → dark */
    if (!ins_pulse.active && !insert_mode_on) {
        uint8_t il = ins_pulse.led_idx;
        if (il != NO_LED && il >= led_min && il < led_max)
            rgb_matrix_set_color(il, 0, 0, 0);
    }

    /* ── 1c. Lock-key pulse animations ────────────────────────────── */
    pulse_anim_t *pulses[] = { &caps_pulse, &num_pulse, &ins_pulse };
    for (uint8_t p = 0; p < 3; p++) {
        if (!pulses[p]->active) continue;
        uint32_t elapsed = timer_elapsed32(pulses[p]->start_time);
        if (elapsed >= PULSE_DURATION_MS) { pulses[p]->active = false; continue; }
        uint8_t br = calc_pulse_brightness(elapsed);
        uint8_t pl = pulses[p]->led_idx;
        if (pl >= led_min && pl < led_max) {
            if (pulses[p]->is_green)
                rgb_matrix_set_color(pl, 0, br, 0);
            else
                rgb_matrix_set_color(pl, br, 0, 0);
        }
    }

    /* ── 2. Overlay reactive effects ──────────────────────────────── */
    for (uint8_t e = 0; e < MAX_EVENTS; e++) {
        if (!key_events[e].active) continue;
        uint32_t elapsed = timer_elapsed32(key_events[e].timestamp);
        if (elapsed > REACTIVE_TIMEOUT_MS) { key_events[e].active = false; continue; }
        uint8_t brightness = (uint8_t)(255 - (elapsed * 255) / REACTIVE_TIMEOUT_MS);
        uint8_t src_led = key_events[e].led_idx;

        if (key_events[e].is_special) {
            uint8_t src_x = g_led_config.point[src_led].x;
            uint8_t src_y = g_led_config.point[src_led].y;
            uint8_t wave_front = (uint8_t)((elapsed * WAVE_RADIUS) / REACTIVE_TIMEOUT_MS);
            uint8_t wave_width = 14;
            for (uint8_t i = led_min; i < led_max; i++) {
                uint8_t dx = (uint8_t)abs((int16_t)g_led_config.point[i].x - src_x);
                uint8_t dy = (uint8_t)abs((int16_t)g_led_config.point[i].y - src_y);
                uint16_t dist = dx + dy;
                if (dist <= (uint16_t)(wave_front + wave_width) && dist + wave_width >= wave_front) {
                    int16_t diff = (int16_t)wave_front - (int16_t)dist;
                    if (diff < 0) diff = -diff;
                    uint8_t lb = (diff < wave_width) ? (uint8_t)((uint16_t)brightness * (wave_width - diff) / wave_width) : 0;
                    if (lb > 0) rgb_matrix_set_color(i, lb, 0, 0);
                }
            }
        } else {
            if (src_led >= led_min && src_led < led_max)
                rgb_matrix_set_color(src_led, 0, brightness, 0);
        }
    }

    return false;
}
