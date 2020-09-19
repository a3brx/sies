#pragma once

/// Mouse IRQ line
#define MOUSE_IRQ_LINE          12

/// First packet byte
#define MOUSE_Y_OVERFLOW        BIT(7)
#define MOUSE_X_OVERFLOW        BIT(6)
#define MOUSE_MSB_Y_DELTA       BIT(5)
#define MOUSE_MSB_X_DELTA       BIT(4)
#define SYNCHRONIZATION_BIT     BIT(3)
#define MOUSE_MIDDLE_BUTTON     BIT(2)
#define MOUSE_RIGHT_BUTTON      BIT(1)
#define MOUSE_LEFT_BUTTON       BIT(0)
