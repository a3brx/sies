#pragma once

/// Constants
#define TIMER_FREQ          1193182

/// Timer 0 IRQ line
#define TIMER_IRQ_LINE      0

/// Timers ports
#define TIMER_0             0x40
#define TIMER_1             0x41
#define TIMER_2             0x42
#define TIMER_CTRL          0x43
#define SPEAKER_CTRL        0x61

/// Read-back command
#define RB_COMMAND          (BIT(7) | BIT(6))
#define RB_COUNT            BIT(5)
#define RB_STATUS           BIT(4)
#define RB_TIMER(n)         BIT((n) + 1)

/// Status byte
#define ST_OUTPUT           BIT(7)
#define ST_NULL_COUNT       BIT(6)
#define ST_INIT_MODE        (BIT(5) | BIT(4))
    #define LSB_MODE        BIT(4)
    #define MSB_MODE        BIT(5)
    #define LSB_MSB_MODE    (LSB_MODE | MSB_MODE)
#define ST_PROG_MODE        (BIT(3) | BIT(2) | BIT(1))
#define ST_BCD              BIT(0)
