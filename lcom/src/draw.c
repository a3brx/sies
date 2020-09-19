#include <lcom/lcf.h>
#include "draw.h"
#include "macros/graphics.h"
#include "art/letters.h"
#include "art/numbers.h"

#define WHITE_COLOR (unsigned char) 63

extern void *video_mem;

extern vbe_mode_info_t vbe_info;

int draw_pixel(uint16_t x, uint16_t y, uint32_t color, void *buffer){
  if(x >= vbe_info.XResolution || y >= vbe_info.YResolution)
    return 1;
  uint16_t bytes = ((vbe_info.BitsPerPixel + 1) / 8);
  uint8_t * pixel = buffer;
  pixel += (x + vbe_info.XResolution * y) * bytes;
  for (char i = 0; i < bytes; i++)
    * (pixel + i) = color >> (i * 8);
  return 0;
}

int draw_hline(uint16_t x, uint16_t y, uint16_t width, uint32_t color, void *buffer) {
  for (int i = 0 ; i < width; i++)
    draw_pixel(x + i, y, color, buffer);
  return 0;
}

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void *buffer) {
  for (int i = 0 ; i < height; i++)
    draw_hline(x, y + i, width, color, buffer);
  return 0;
}

uint8_t get_color_bits(uint32_t color, uint8_t size, uint8_t pos) {
    uint8_t res;
    uint32_t mask;
    mask = BIT(size+1)-1;
    mask <<= pos;
    res = (color & mask) >> pos;
    return res;
}

void get_color_components(uint8_t *red, uint8_t *green, uint8_t *blue, uint32_t color) {
    *red = get_color_bits(color, vbe_info.RedMaskSize, vbe_info.RedFieldPosition);
    *green = get_color_bits(color, vbe_info.GreenMaskSize, vbe_info.GreenFieldPosition);
    *blue = get_color_bits(color, vbe_info.BlueMaskSize, vbe_info.BlueFieldPosition);
    return;
}

uint32_t get_rectangle_color(uint16_t row, uint16_t col, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    if (vbe_info.MemoryModel == INDEX_MEMORY_MODEL) {
        return (first + (row * no_rectangles + col) * step) % (1 << vbe_info.BitsPerPixel) & (INDEX_COLOR_MASK);
    } else if (vbe_info.MemoryModel == DIRECT_MEMORY_MODEL) {
        uint8_t red, green, blue;
        get_color_components(&red, &green, &blue, first);
        red = (red + col * step) % (1 << vbe_info.RedMaskSize);
        green = (green + row * step) % (1 << vbe_info.GreenMaskSize);
        blue = (blue + (col + row) * step) % (1 << vbe_info.BlueMaskSize);
        uint32_t joined_colors = (red << vbe_info.RedFieldPosition) | (green << vbe_info.GreenFieldPosition) | (blue << vbe_info.BlueFieldPosition);
        return joined_colors;
    }
    return 0;
}

int draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step, void *buffer){
  uint16_t height = vbe_info.YResolution / no_rectangles;
  uint16_t width = vbe_info.XResolution / no_rectangles;
  for(uint16_t row = 0; row < no_rectangles; row++){
    for(uint16_t col = 0; col < no_rectangles; col++){
      uint32_t color = get_rectangle_color(row, col, no_rectangles, first, step);
      draw_rectangle(col * width, row * height, width, height, color, buffer);
    }
  }
  return 0;
}

int draw_sprite(xpm_map_t xpm, uint16_t x, uint16_t y, void *buffer) {
    static xpm_image_t image;
    xpm_load(xpm, XPM_INDEXED, &image);
    uint8_t *byte = image.bytes;
    for (uint16_t row = 0; row < image.height; row++) {
        for (uint16_t col = 0; col < image.width; col++) {
            if (*byte != 0)
                draw_pixel(x + col, y + row, *byte, buffer);
            byte++;
        }
    }
    return 0;
}

int write_string(char *t, uint16_t x, uint16_t y, int string_size, uint8_t *buffer) {
    unsigned char colors[1];
    uint16_t x_his[80], y_his[80];
    colors[0] = WHITE_COLOR;
    for (int i = 0; i < string_size; i++) {
        x_his[i] = x; 
        y_his[i] = y;
        switch (t[i]) {
            case '0':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(zero, x, y, buffer);
                x += 20;
                break;
            case '1':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(one, x, y, buffer);
                x += 20;
                break;
            case '2':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(two, x, y, buffer);
                x += 20;
                break;
            case '3':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(three, x, y, buffer);
                x += 20;
                break;
            case '4':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(four, x, y, buffer);
                x += 20;
                break;
            case '5':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(five, x, y, buffer);
                x += 20;
                break;
            case '6':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(six, x, y, buffer);
                x += 20;
                break;
            case '7':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(seven, x, y, buffer);
                x += 20;
                break;
            case '8':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(eight, x, y, buffer);
                x += 20;
                break;
            case '9':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(nine, x, y, buffer);
                x += 20;
                break;
            case ':':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(double_point, x, y, buffer);
                x += 20;
                break;
            case 'a':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(A, x, y, buffer);
                x += 20;
                break; 
            case 'b':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(B, x, y, buffer);
                x += 20;
                break;              
            case 'c':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(C, x, y, buffer);
                x += 20;
                break;    
            case 'd':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(D, x, y, buffer);
                x += 20;
                break;  
            case 'e':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(E, x, y, buffer);
                x += 20;
                break;  
            case 'f':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(F, x, y, buffer);
                x += 20;
                break;              
            case 'g':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(G, x, y, buffer);
                x += 20;
                break;  
            case 'h':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(H, x, y, buffer);
                x += 20;
                break;        
            case 'i':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(I, x, y, buffer);
                x += 20;
                break;            
            case 'k':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(K, x, y, buffer);
                x += 20;
                break;      
            case 'l':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(L, x, y, buffer);
                x += 20;
                break;
            case 'm':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(M, x, y, buffer);
                x += 20;
                break;        
            case 'n':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(N, x, y, buffer);
                x += 20;
                break;        
            case 'o':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(O, x, y, buffer);
                x += 20;
                break;  
            case 'p':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(P, x, y, buffer);
                x += 20;
                break;                             
            case 'r':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(R, x, y, buffer);
                x += 20;
                break;                          
            case 's':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(S, x, y, buffer);
                x += 20;
                break;
            case 't':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(T, x, y, buffer);
                x += 20;
                break;             
            case 'u':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(U, x, y, buffer);
                x += 20;
                break;
            case 'v':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(V, x, y, buffer);
                x += 20;
                break;  
            case 'w':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(W, x, y, buffer);
                x += 20;
                break;  
            case 'x':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(X, x, y, buffer);
                x += 20;
                break;  
            case 'y':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(Y, x, y, buffer);
                x += 20;
                break;
            case '[':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(par_ab, x, y, buffer);
                x += 20;
                break;    
            case ']':
                draw_rectangle(x,y,16,32,0,buffer);
                draw_sprite(par_fech, x, y, buffer);
                x += 20;
                break;  
            case '\n':
                x = x_his[0];
                y += 36;
                break;
            case ' ':
                draw_rectangle(x,y,16,32,0,buffer);
                x += 20;
                break;
            case '\t':
                draw_rectangle(x,y,16,32,0,buffer);
                x += 40;
                break;
            case '\b':
                if (x != x_his[0])
                    x -= 20;
                draw_rectangle(x,y,16,32,0,buffer);
                break;
            default:
                draw_rectangle(x,y,16,32,0,buffer);
                x += 20;
                break;
        }

    }
    if (buffer != video_mem) {
        uint8_t *ptr = video_mem;
        for (int j = 0; j < string_size; j++) {
            for(int i = 0; i < 32; i++){
                uint32_t displ = (x_his[j]) + (y_his[j] + i) * vbe_info.XResolution;
                memcpy(ptr + displ, buffer + displ, 20 * string_size);
            }
        }
    }
    return 0;
}

int draw_box(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t border, uint32_t color, void *buffer) {
    draw_rectangle(x, y, border, height, color, buffer);
    draw_rectangle(x + border, y, width, border, color, buffer);
    draw_rectangle(x + width, y + border, border, height, color, buffer);
    draw_rectangle(x, y + height, width, border, color, buffer);
    return 0;
}


