#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>
#include <cstdlib>
#include <vector>

namespace pipeline3D {

    struct Pixel {
        uint8_t r, g, b;
        uint8_t a;
    };

    struct Texture {
        std::vector<Pixel> data;
		std::size_t size = 0;
		int width, height;
		int channels;

        Texture(const char* filename);
        Texture(int w, int h, int channels);
        ~Texture();

        bool read(const char* filename);


        bool has_alpha_channel();

        float address_func_clamp(float x);
        float frac(float x);
        Pixel interpolate(Pixel p1, Pixel p2, float w);

        Pixel sample_pixel(const float u, const float v);

        static void write(const char* filename, int w, int h, int ch, std::vector<Pixel> data);

    };

}

#endif // TEXTURE_H