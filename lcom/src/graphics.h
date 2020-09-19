#pragma once

void *video_mem;
vbe_mode_info_t vbe_info;
struct vg_vbe_contr_info *vbe_controller_info;

#pragma pack(1)

struct vbe_cntr_info_t {
    uint8_t VBESignature[4];
    uint8_t VbeVersion[2];
    uint32_t OemStringPtr;
    uint8_t Capabilities[4];
    uint32_t VideoModePtr;
    uint16_t TotalMemory;
    uint16_t OemSoftwareRev;
    uint32_t OemVendorNamePtr;
    uint32_t OemProductNamePtr;
    uint32_t OemProductRevPtr;
    uint8_t Reserved[222];
    uint8_t OEMData[256];
};

#pragma options align = reset

/*
 * @brief Initializes the aditional video memories.
 * @return Returns 0.
 */
int init_video_mems();

/*
 * @brief Resets the video memories.
 */
void reset_video_mems();

/*
 * @brief Frees the aditional video memories.
 * @return Returns 0.
 */
int free_video_mems();

/*
 * @brief Sets graphics mode.
 * @param mode Mode to which the graphics will be set.
 * @return Returns 0 if successful, 1 otherwise.
 */
int set_graphics_mode(uint16_t mode, reg86_t *reg86);

/*
 * @brief Initializes the aditional video memories.
 * @param mode Mode to which the graphics will be set.
 * @return Returns 0 if successful, 1 and 2 otherwise.
 */
int init_graphics(uint16_t mode);

/*
 * @brief Our implementation of vbe_get_mode_info.
 * @param mode Mode to which the graphics will be set.
 * @param vbe_info Pointer to the struct of vbe_info to change
 * @return Returns 0 if successful, 1 and 2 otherwise.
 */
int m_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe_info);
