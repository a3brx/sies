#include <lcom/lcf.h>
#include <machine/int86.h>
#include "graphics.h"
#include "macros/graphics.h"

uint32_t calcPtrDelta(uint32_t ptr, mmap_t *mmry);
uint32_t far2linear(uint32_t far);

uint8_t *moves_video_mem;
uint8_t *board_video_mem;

int init_video_mems() {
    board_video_mem = (uint8_t *) malloc(
            vbe_info.YResolution * vbe_info.XResolution * vbe_info.BitsPerPixel / 8 * sizeof(uint8_t));
    moves_video_mem = (uint8_t *) malloc(
            vbe_info.YResolution * vbe_info.XResolution * vbe_info.BitsPerPixel / 8 * sizeof(uint8_t));

    return 0;
}

int free_video_mems() {
    free(board_video_mem);
    free(moves_video_mem);
    return 0;
}

void reset_video_mems() {
    memset(board_video_mem, 0, BUFFER_SIZE);
    memset(moves_video_mem, 0, BUFFER_SIZE);
    memset(video_mem, 0, BUFFER_SIZE);
}

uint32_t calcPtrDelta(uint32_t ptr, mmap_t *mmry) {
    uint32_t physInt = (uint32_t) mmry->phys;
    return ptr - physInt;
}

int set_graphics_mode(uint16_t mode, reg86_t *reg86) {
    memset(reg86, 0, sizeof(*reg86));
    reg86->ax = 0x4F02;
    reg86->bx = LINEAR_FRAME_BUF | mode;
    reg86->intno = BIOS_INTERFACE;
    if (sys_int86(reg86) != OK) {
        panic("set_vbe_mode(%X): sys_int86() failed\n", mode);
        return 1;
    }
    return 0;
}

int allow_memory_mapping(uint16_t mode, struct minix_mem_range *mr) {
    int r;
    m_vbe_get_mode_info(mode, &vbe_info);
    unsigned int vram_base = vbe_info.PhysBasePtr;
    unsigned int vram_size = vbe_info.XResolution * vbe_info.YResolution * ((vbe_info.BitsPerPixel + 1) / 8);
    mr->mr_base = (phys_bytes) vram_base;
    mr->mr_limit = mr->mr_base + vram_size;
    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, mr))) {
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }
    video_mem = vm_map_phys(SELF, (void *) mr->mr_base, vram_size);
    if (video_mem == MAP_FAILED) {
        panic("Couldn't map video memory");
        return 2;
    }
    return 0;
}

int init_graphics(uint16_t mode) {
    reg86_t reg86;
    struct minix_mem_range mr;
    allow_memory_mapping(mode, &mr);
    set_graphics_mode(mode, &reg86);
    return 0;
}

int vbe_get_controller_info() {
    mmap_t memry;
    if (lm_alloc(sizeof(struct vbe_cntr_info_t), &memry) == NULL) {
        return 1;
    }
    struct vbe_cntr_info_t *aux =
            (struct vbe_cntr_info_t *) memry.virt;
    aux->VBESignature[0] = 'V';
    aux->VBESignature[1] = 'B';
    aux->VBESignature[2] = 'E';
    aux->VBESignature[3] = '2';
    reg86_t reg86;
    memset((void *) &reg86, 0, sizeof(struct reg86));
    reg86.ax = 0x4F00;
    reg86.es = PB2BASE(memry.phys);
    reg86.di = PB2OFF(memry.phys);
    reg86.intno = BIOS_INTERFACE;

    if (sys_int86(&reg86) != OK) {
        return 1;
    }

    struct vg_vbe_contr_info info;
    memset((void *) &info, 0, sizeof(struct vg_vbe_contr_info));
    for (int i = 0; i < 4; i++)
        info.VBESignature[i] = aux->VBESignature[i];
    for (int i = 0; i < 2; i++)
        info.VBEVersion[i] = aux->VbeVersion[i];

    uint32_t delta = calcPtrDelta(far2linear(aux->OemStringPtr), &memry);
    info.OEMString = (char *) (far2linear(aux->OemStringPtr) + delta);

    delta = calcPtrDelta(far2linear(aux->VideoModePtr), &memry);
    info.VideoModeList = (uint16_t * )(far2linear(aux->VideoModePtr) + delta);

    info.TotalMemory = aux->TotalMemory * 64 * BIT(10);

    delta = calcPtrDelta(far2linear(aux->OemVendorNamePtr), &memry);
    info.OEMVendorNamePtr = (char *) (far2linear(aux->OemVendorNamePtr) + delta);
    
    delta = calcPtrDelta(far2linear(aux->OemProductNamePtr), &memry);
    info.OEMProductNamePtr = (char *) (far2linear(aux->OemProductNamePtr) + delta);
    
    delta = calcPtrDelta(far2linear(aux->OemProductRevPtr), &memry);
    info.OEMProductRevPtr = (char *) (far2linear(aux->OemProductRevPtr) + delta);
    
    if (vg_display_vbe_contr_info(&info)) return 1;
        lm_free(&memry);
    return 0;
}
int m_vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vbe_info) {
    mmap_t memry;
    if (lm_alloc(sizeof(vbe_mode_info_t), &memry) == NULL) {
        return 1;
    }
    struct reg86 reg86;
    memset((void *) &reg86, 0, sizeof(struct reg86));

    reg86.ax = 0x4F01;
    reg86.cx = mode;
    reg86.es = PB2BASE(memry.phys);
    reg86.di = PB2OFF(memry.phys);
    reg86.intno = BIOS_INTERFACE;
    if (sys_int86(&reg86) != OK) {
        panic("vbe_get_controller_info(): sys_int86() failed\n");
        return 1;
    }
    memcpy(vbe_info, memry.virt, sizeof(vbe_mode_info_t));
    lm_free(&memry);
    return 0;
}
uint32_t far2linear(uint32_t far) {
    uint32_t farInt = (uint32_t) far;
    uint32_t msb, lsb, linear;
    msb = (farInt & (0xF0000)) >> 16;
    lsb = (farInt & (0x0FFFF));
    linear = (msb << 4) + lsb;
    return linear;
}
