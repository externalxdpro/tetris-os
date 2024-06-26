// Includes
#include "keyboard.h"
#include "irq.h"
#include "util.h"

// Sets the keyboard layout
uint8_t kblayout_us[2][128] = {
    {KEY_NULL,
     KEY_ESC,
     '1',
     '2',
     '3',
     '4',
     '5',
     '6',
     '7',
     '8',
     '9',
     '0',
     '-',
     '=',
     KEY_BACKSPACE,
     KEY_TAB,
     'q',
     'w',
     'e',
     'r',
     't',
     'y',
     'u',
     'i',
     'o',
     'p',
     '[',
     ']',
     KEY_ENTER,
     0,
     'a',
     's',
     'd',
     'f',
     'g',
     'h',
     'j',
     'k',
     'l',
     ';',
     '\'',
     '`',
     0,
     '\\',
     'z',
     'x',
     'c',
     'v',
     'b',
     'n',
     'm',
     ',',
     '.',
     '/',
     0,
     0,
     0,
     ' ',
     0,
     KEY_F1,
     KEY_F2,
     KEY_F3,
     KEY_F4,
     KEY_F5,
     KEY_F6,
     KEY_F7,
     KEY_F8,
     KEY_F9,
     KEY_F10,
     0,
     0,
     KEY_HOME,
     KEY_UP,
     KEY_PAGE_UP,
     '-',
     KEY_LEFT,
     '5',
     KEY_RIGHT,
     '+',
     KEY_END,
     KEY_DOWN,
     KEY_PAGE_DOWN,
     KEY_INSERT,
     KEY_DELETE,
     0,
     0,
     0,
     KEY_F11,
     KEY_F12},
    {KEY_NULL,
     KEY_ESC,
     '!',
     '@',
     '#',
     '$',
     '%',
     '^',
     '&',
     '*',
     '(',
     ')',
     '_',
     '+',
     KEY_BACKSPACE,
     KEY_TAB,
     'Q',
     'W',
     'E',
     'R',
     'T',
     'Y',
     'U',
     'I',
     'O',
     'P',
     '{',
     '}',
     KEY_ENTER,
     0,
     'A',
     'S',
     'D',
     'F',
     'G',
     'H',
     'J',
     'K',
     'L',
     ':',
     '\"',
     '~',
     0,
     '|',
     'Z',
     'X',
     'C',
     'V',
     'B',
     'N',
     'M',
     '<',
     '>',
     '?',
     0,
     0,
     0,
     ' ',
     0,
     KEY_F1,
     KEY_F2,
     KEY_F3,
     KEY_F4,
     KEY_F5,
     KEY_F6,
     KEY_F7,
     KEY_F8,
     KEY_F9,
     KEY_F10,
     0,
     0,
     KEY_HOME,
     KEY_UP,
     KEY_PAGE_UP,
     '-',
     KEY_LEFT,
     '5',
     KEY_RIGHT,
     '+',
     KEY_END,
     KEY_DOWN,
     KEY_PAGE_DOWN,
     KEY_INSERT,
     KEY_DELETE,
     0,
     0,
     0,
     KEY_F11,
     KEY_F12},
};

// Initialize keyboard struct
struct Keyboard keyboard;

// Handles keyboard input and modifier keys
static void keyboard_handler(struct registers *regs) {
    uint16_t scan_code = (uint16_t)inportb(0x60);

    if (KEY_SCANCODE(scan_code) == KEY_LALT ||
        KEY_SCANCODE(scan_code) == KEY_RALT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_ALT),
                                KEY_IS_PRESSED(scan_code));
    } else if (KEY_SCANCODE(scan_code) == KEY_LCTRL ||
               KEY_SCANCODE(scan_code) == KEY_RCTRL) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CTRL),
                                KEY_IS_PRESSED(scan_code));
    } else if (KEY_SCANCODE(scan_code) == KEY_LSHIFT ||
               KEY_SCANCODE(scan_code) == KEY_RSHIFT) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SHIFT),
                                KEY_IS_PRESSED(scan_code));
    } else if (KEY_SCANCODE(scan_code) == KEY_CAPS_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_CAPS_LOCK),
                                KEY_IS_PRESSED(scan_code));
    } else if (KEY_SCANCODE(scan_code) == KEY_NUM_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_NUM_LOCK),
                                KEY_IS_PRESSED(scan_code));
    } else if (KEY_SCANCODE(scan_code) == KEY_SCROLL_LOCK) {
        keyboard.mods = BIT_SET(keyboard.mods, HIBIT(KEY_MOD_SCROLL_LOCK),
                                KEY_IS_PRESSED(scan_code));
    }

    keyboard.keys[(uint8_t)(scan_code & 0x7F)] = KEY_IS_PRESSED(scan_code);
    keyboard.chars[KEY_CHAR(scan_code)]        = KEY_IS_PRESSED(scan_code);
}

// Installs the keyboard handler into IRQ
void keyboard_install() { irq_install_handler(1, keyboard_handler); }
