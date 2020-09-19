#pragma once

/// IBM's maximum kbc response time
#define DELAY_US                20000
#define delay(n)                tickdelay(micros_to_ticks(n))
#define MAX_TRIES               3

/// Kbc ports
#define STATUS_REGISTER             0x64
#define COMMAND_REGISTER            0x64
#define ARGUMENT_REGISTER           0x60
#define OUT_BUFFER                  0x60
#define IN_BUFFER                   0x60

/// Status byte
#define PARITY_ERROR                BIT(7)
#define TIMEOUT_ERROR               BIT(6)
#define MOUSE_DATA                  BIT(5)
#define INHIBIT_FLAG                BIT(4)
#define A2_INPUT_LINE               BIT(3)
#define SYSTEM_FLAG                 BIT(2)
#define FULL_IN_BUFFER              BIT(1)
#define FULL_OUT_BUFFER             BIT(0)

/// Kbc commands
#define CMD_BYTE_READ               0x20
#define CMD_BYTE_WRITE              0x60
#define DISABLE_MOUSE_IF            0xA7
#define ENABLE_MOUSE_IF             0xA8
#define CHECK_MOUSE_IF              0xA9
#define CHECK_KBC                   0xAA
#define CHECK_KBD_IF                0xAB
#define DISABLE_KBD_IF              0xAD
#define ENABLE_KBD_IF               0xAE
#define WRITE_MOUSE_BYTE            0xD4
    #define RESET_MOUSE             0xFF
    #define RESEND_MOUSE_DATA       0xFE
    #define SET_DEFAULTS            0xF6
    #define DISABLE_DATA_REPORTING  0xF5
    #define ENABLE_DATA_REPORTING   0xF4
    #define SET_SAMPLE_MODE         0xF3
    #define SET_REMOTE_MODE         0xF0
    #define READ_MOUSE_DATA         0xEB
    #define SET_STREAM_MODE         0xEA
    #define MOUSE_STATUS_REQUEST    0xE9
    #define SET_MOUSE_RESOLUTION    0xE8
    #define SET_MOUSE_SCALING_2     0xE7
    #define SET_MOUSE_SCALING_1     0xE6

    #define ACKNOWLEDGMENT          0xFA
    #define NOT_ACKNOWLEDGMENT      0xFE
    #define ACKNOWLEDGMENT_ERROR    0xFA

/// Kbc command byte
#define DISABLE_MOUSE_INT           BIT(5)
#define DISABLE_KBD_INT             BIT(4)
#define ENABLE_MOUSE_OUT_BUFFER     BIT(1)
#define ENABLE_KBD_OUT_BUFFER       BIT(0)
