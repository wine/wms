#include <png.h>

#include "pixel.h"

typedef struct {
    Pixel* pixels;
    size_t width, height;
} Bitmap;

Pixel* Bitmap_pixel_at(Bitmap* bitmap, size_t x, size_t y) {
    return bitmap->pixels + bitmap->width * y + x;
}

int Bitmap_save_to_file(Bitmap* bitmap, const char* file_path) {
    const int pixel_size = 3;
    const int depth = 8;

    FILE* file;

    png_structp png  = NULL;
    png_infop   info = NULL;
    png_byte**  rows = NULL;

    file = fopen(file_path, "w");
    png  = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (file == NULL || png == NULL) {
        fclose(file);
        return -1;
    }

    info = png_create_info_struct(png);

    if (info == NULL || setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        return -1;
    }

    png_set_IHDR(
            png,
            info,
            bitmap->width,
            bitmap->height,
            depth,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );

    rows = png_malloc(png, bitmap->height * sizeof(png_byte*));

    for (size_t y = 0; y < bitmap->height; ++y) {
        png_byte* row = png_malloc(png, sizeof(uint8_t) * bitmap->width * pixel_size);
        rows[y] = row;

        for (size_t x = 0; x < bitmap->width; ++x) {
            Pixel* pixel = Bitmap_pixel_at(bitmap, x, y);

            *row++ = pixel->red;
            *row++ = pixel->green;
            *row++ = pixel->blue;
        }
    }

    png_init_io(png, file);
    png_set_rows(png, info, rows);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);

    for (size_t y = 0; y < bitmap->height; ++y) {
        png_free(png, rows[y]);
    }

    png_free(png, rows);

    return 0;
}
