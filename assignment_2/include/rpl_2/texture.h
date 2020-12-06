#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../3rdparty/include/stb_image.h"
#include "../../3rdparty/include/stb_image_write.h"


#include <cstdint>
#include <vector>

namespace rpl {

    struct Pixel {
        uint8_t r, g, b;
        uint8_t a;
    };

    struct Texture {
        std::vector<Pixel> data;
		std::size_t size = 0;
		int width, height;
		int channels;

        Texture(const char* filename) 
        {
            if (read(filename))
            {
                // ok
            } else
            {
                // fail
            }
        }
        Texture(int w, int h, int c) : width(w), height(h), channels(c)
        {
            size = width*height;
            data.clear();
            data.shrink_to_fit();
        }

        ~Texture() { }

        bool read(const char* filename)
        {
            uint8_t *tmp_data = stbi_load(filename, &width, &height, &channels, 0);
            if (tmp_data != NULL) {
                size = width*height;
                data.clear();
                // convert to Pixels
                for (size_t i=0; i<width*height*channels; i+=channels) {
                    Pixel p;
                    p.r = tmp_data[i]; p.g = tmp_data[i+1]; p.b = tmp_data[i+2];
                    if (has_alpha_channel()) {
                        p.a = tmp_data[i+3];
                    }
                    data.push_back(p);
                }
                data.shrink_to_fit();
            }
            stbi_image_free(tmp_data); // free temporary image data
            return !data.empty();
        }

        bool has_alpha_channel()
        {
            return channels == 4;
        }

        float address_func_clamp(float x)
        {
            if (x < 0) return 0;
            if (x > 1) return 1;
            return x;
        }
        float frac(float x)
        {
            return std::abs(x) - std::floor(std::abs(x));
        }

        Pixel interpolate(Pixel p1, Pixel p2, float w)
        {
            const float w2 = (1.0f-w);
            Pixel out = {
                static_cast<uint8_t>(p1.r*w + p2.r*w2),
                static_cast<uint8_t>(p1.g*w + p2.g*w2),
                static_cast<uint8_t>(p1.b*w + p2.b*w2),
            };

            if (has_alpha_channel()) {
                out.a = static_cast<uint8_t>(p1.a*w + p2.a*w2);
            }

            return out;
        }

        Pixel sample_pixel(const float u, const float v)
        {
            float x = address_func_clamp(u) * width;
            float y = address_func_clamp(v) * height;

            Pixel out;

            //
            // refer to screenshot
            int c0,c1,c2,c3;

            // c0 : top-left textile of (x,y)
            c0 = std::floor(y)*width+std::floor(x);
            // c1 : top-right textile of (x,y)
            c1 = std::floor(y)*width+std::ceil(x);
            // c2 : bottom-left textile of (x,y)
            c2 = std::ceil(y)*width+std::floor(x);
            // c3 : bottom-right textile of (x,y)
            c3 = std::ceil(y)*width+std::ceil(x);

            float a_ = frac(x);
            float b_ = frac(y);

            if (!(c0 > 0 && c1 > 0 && c2 > 0 && c3 > 0 && c0 < size && c1 < size && c2 < size && c3 < size)) return { 0, 0, 0, 0};

            Pixel p1 = interpolate(data[c0], data[c1], a_);
            Pixel p2 = interpolate(data[c2], data[c3], a_);
            out = interpolate(p1, p2, b_);

            return out;


        }


        static void write(const char* filename, int w, int h, int channels, std::vector<Pixel> data)
        {
            const bool has_alpha_channel = channels == 4;
            uint8_t *tmp_data = new uint8_t[w*h*channels];

            int idx;
            for (int i=0; i<data.size(); i++) {
                idx = i*channels;
                tmp_data[idx] = data[i].r;
                tmp_data[idx+1] = data[i].g;
                tmp_data[idx+2] = data[i].b;

                if (has_alpha_channel) {
                    tmp_data[idx+3] = data[i].a;
                }
            }

            const char* ext = strrchr(filename, '.');
            if(strcmp(ext, ".jpg") == 0) {
                stbi_write_jpg(filename, w, h, channels, tmp_data, 100);
            }
            else if(strcmp(ext, ".bmp") == 0) {
                stbi_write_bmp(filename, w, h, channels, tmp_data);
            }
            else if(strcmp(ext, ".tga") == 0) {
                stbi_write_tga(filename, w, h, channels, tmp_data);
            } 
            else {
                stbi_write_png(filename, w, h, channels, tmp_data, w*channels);
            }

            stbi_image_free(tmp_data); // free temporary image data
        }

    };

}