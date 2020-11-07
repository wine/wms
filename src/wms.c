#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "bitmap.h"

int main(int argc, char** argv) {
    // todo arguments (getopt or argp ?)
    const char* file = "todo.png";

    Display* display = XOpenDisplay(NULL);
    
    Window root = RootWindow(display, DefaultScreen(display));

    XWindowAttributes attributes;
    XGetWindowAttributes(display, root, &attributes);

    XImage* image = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

    const size_t width  = image->width;
    const size_t height = image->height;

    Bitmap bitmap;
    bitmap.width  = width;
    bitmap.height = height;
    bitmap.pixels = calloc(width * height, sizeof(Pixel));

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; x++) {
            unsigned long pixel = XGetPixel(image, x, y);
            
            unsigned char red   = (pixel & image->red_mask)   >> 16;
            unsigned char green = (pixel & image->green_mask) >> 8;
            unsigned char blue  = (pixel & image->blue_mask);
        
            Pixel* bitmap_pixel = &bitmap.pixels[y * width + x];
            bitmap_pixel->red   = red;
            bitmap_pixel->green = green;
            bitmap_pixel->blue  = blue;
        }
    }

    Bitmap_save_to_file(&bitmap, file);

    free(bitmap.pixels);
    XCloseDisplay(display);
}
