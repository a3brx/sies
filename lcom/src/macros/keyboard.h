#pragma once

/// Keyboard IRQ line
#define KEYBOARD_IRQ_LINE       1

/// Key codes
#define BREAKCODE               BIT(7)
#define LONG_SCANCODE           0xE0
#define ESC_MAKECODE            0x01

#define ESC_BREAKCODE           (BREAKCODE | ESC_MAKECODE)
#define BACKSPACE_BREAKCODE     (BREAKCODE | 0x0e)
#define ENTER_BREAKCODE         (BREAKCODE | 0x1c)
#define A_BREAKCODE             (BREAKCODE | 0x1e)
#define B_BREAKCODE             (BREAKCODE | 0x30)
#define C_BREAKCODE             (BREAKCODE | 0x2e)
#define D_BREAKCODE             (BREAKCODE | 0x20)
#define E_BREAKCODE             (BREAKCODE | 0x12)
#define F_BREAKCODE             (BREAKCODE | 0x21)
#define G_BREAKCODE             (BREAKCODE | 0x22)
#define H_BREAKCODE             (BREAKCODE | 0x23)
#define NUM1_BREAKCODE          (BREAKCODE | 0x02)
#define NUM2_BREAKCODE          (BREAKCODE | 0x03)
#define NUM3_BREAKCODE          (BREAKCODE | 0x04)
#define NUM4_BREAKCODE          (BREAKCODE | 0x05)
#define NUM5_BREAKCODE          (BREAKCODE | 0x06)
#define NUM6_BREAKCODE          (BREAKCODE | 0x07)
#define NUM7_BREAKCODE          (BREAKCODE | 0x08)
#define NUM8_BREAKCODE          (BREAKCODE | 0x09)
#define S_BREAKCODE             (BREAKCODE | 0x1f)
#define N_BREAKCODE             (BREAKCODE | 0x31)
#define P_BREAKCODE             (BREAKCODE | 0x19)
#define R_BREAKCODE             (BREAKCODE | 0x13)
#define W_BREAKCODE             (BREAKCODE | 0x11)

