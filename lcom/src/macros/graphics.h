#ifndef _MACROS_H_
#define _MACROS_H_

#define MEGABYTE                  BIT(20)
// VBE AH returns
#define AH_SUCCESS                0x00
#define AH_FAIL                   0x01
#define AH_NOT_SUPPORTED          0x02
#define AH_INVALID                0x03
// VBE funcions
#define VBE_CONTR_FUNC            0x00
#define VBE_MODE_FUNC             0x01
    #define LINEAR_FRAME_BUF      BIT(14)
    #define NOT_CLR_DISPLAY_MEM   BIT(15)
// VBE interfaces
#define BIOS_INTERFACE            0x10
// VBE modes
#define CGA_MODE                  0x03
// Indexed color
#define INDEX_COLOR_MASK          0xFF
#define INDEX_MEMORY_MODEL        0x4
// Direct color
#define DIRECT_MEMORY_MODEL       0x6

#define BUFFER_SIZE               1024 * 768
#endif
