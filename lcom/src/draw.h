#pragma once

xpm_image_t image;

/*
 * @brief Draws a pixel on screen.
 * @param x X coordinate where to draw.
 * @param y Y coordinate where to draw.
 * @param color Color of the pixel.
 * @param buffer Buffer to draw the pixel.
 * @return 0 if success, 1 otherwise.
 */
int draw_pixel(uint16_t x, uint16_t y, uint32_t color, void *buffer);

/*
 * @brief Draws an horizontal line on screen.
 * @param x X coordinate where to start drawing.
 * @param y Y coordinate where to start drawing.
 * @param width Length of the line.
 * @param color Color of the line.
 * @param buffer Buffer to draw the line.
 * @return 0 if success, 1 otherwise.
 */
int draw_hline(uint16_t x, uint16_t y, uint16_t width, uint32_t color, void *buffer);

/*
 * @brief Draws a filled rectangle on screen.
 * @param x X coordinate where to draw.
 * @param y Y coordinate where to draw.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @param color Color of the pixel.
 * @param buffer Buffer to draw the rectangle.
 * @return 0 if success, 1 otherwise.
 */
int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, void *buffer);

/*
 * @brief Draws a unfilled box on screen.
 * @param x X coordinate where to draw.
 * @param y Y coordinate where to draw.
 * @param width Width of the box.
 * @param height Height of the box.
 * @param border Thickness in pixels of the border.
 * @param color Color of the border.
 * @param buffer Buffer to draw the box.
 * @return 0 if success, 1 otherwise.
 */
int draw_box(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t border, uint32_t color, void *buffer);

/*
 * @brief Draws a pattern of rectangles on screen.
 * @param first First color of the pattern.
 * @param step How does the color change.
 * @param buffer Buffer to draw the pattern.
 * @return 0 if success, 1 otherwise.
 */
int draw_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step, void *buffer);

/*
 * @brief Draws a sprite on screen.
 * @param x X coordinate where to draw.
 * @param y Y coordinate where to draw.
 * @param buffer Buffer to draw the sprite.
 * @return 0 if success, 1 otherwise.
 */
int draw_sprite(xpm_map_t xpm, uint16_t x, uint16_t y, void *buffer);

/*
 * @brief Writes a string on screen.
 * @param x X coordinate where to draw.
 * @param y Y coordinate where to draw.
 * @param size The string size.
 * @param buffer Buffer to draw the sprite.
 * @return 0 if success, 1 otherwise.
 */
int write_string(char *t, uint16_t x, uint16_t y, int string_size, uint8_t *buffer);

