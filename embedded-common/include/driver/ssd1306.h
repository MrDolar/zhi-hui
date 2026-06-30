/**
 * @file ssd1306.h
 * @brief SSD1306 OLED 驱动
 */

#ifndef SSD1306_H
#define SSD1306_H

#include "common.h"

/* OLED 尺寸 */
#define SSD1306_WIDTH   128
#define SSD1306_HEIGHT  64

/* 颜色 */
#define SSD1306_BLACK   0
#define SSD1306_WHITE   1
#define SSD1306_INVERSE 2

/* OLED 设备 */
typedef struct {
    uint8_t i2c_id;
    uint8_t addr;       /* 0x3C 或 0x3D */
    uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
} ssd1306_t;

/* 初始化 */
int ssd1306_init(ssd1306_t *dev, uint8_t i2c_id, uint8_t addr);

/* 清屏 */
void ssd1306_clear(ssd1306_t *dev);

/* 刷新屏幕 */
void ssd1306_display(ssd1306_t *dev);

/* 像素操作 */
void ssd1306_set_pixel(ssd1306_t *dev, int16_t x, int16_t y, uint8_t color);
uint8_t ssd1306_get_pixel(ssd1306_t *dev, int16_t x, int16_t y);

/* 画线 */
void ssd1306_draw_line(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);

/* 画矩形 */
void ssd1306_draw_rect(ssd1306_t *dev, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void ssd1306_fill_rect(ssd1306_t *dev, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);

/* 画圆 */
void ssd1306_draw_circle(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t r, uint8_t color);
void ssd1306_fill_circle(ssd1306_t *dev, int16_t x0, int16_t y0, int16_t r, uint8_t color);

/* 显示字符 */
void ssd1306_draw_char(ssd1306_t *dev, int16_t x, int16_t y, char c, uint8_t color, uint8_t size);

/* 显示字符串 */
void ssd1306_draw_string(ssd1306_t *dev, int16_t x, int16_t y, const char *str, uint8_t color, uint8_t size);

/* 显示图片 */
void ssd1306_draw_bitmap(ssd1306_t *dev, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);

/* 滚动 */
void ssd1306_scroll_left(ssd1306_t *dev, uint8_t start, uint8_t stop);
void ssd1306_scroll_right(ssd1306_t *dev, uint8_t start, uint8_t stop);
void ssd1306_scroll_stop(ssd1306_t *dev);

/* 亮度 */
void ssd1306_set_brightness(ssd1306_t *dev, uint8_t brightness);

/* 反色 */
void ssd1306_invert(ssd1306_t *dev, bool invert);

#endif /* SSD1306_H */
