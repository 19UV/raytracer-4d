#pragma once

#include <stddef.h>

struct Pixel {
	float r, g, b;
};
typedef struct Pixel Pixel;

struct Image {
	size_t width, height;

	Pixel* data;
};
typedef struct Image Image;

int image_create(Image* this, size_t width, size_t height);
void image_destroy(Image* this);

void image_set(Image* this, size_t x, size_t y, Pixel pixel);
Pixel image_get(const Image* this, size_t x, size_t y);

int image_save_png(const Image* this, const char* file_path);

