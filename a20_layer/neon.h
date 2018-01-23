#ifndef NEON_H
#define NEON_H

#ifdef __arm_A20__

#include <arm_neon.h>

class NV21toRGB_neon {

public:
	enum { bytes_per_pixel = 3 };
	typedef uint8x8x3_t PixelBlock;
	static PixelBlock const init_pixelblock(unsigned char /*fill_alpha*/) {
		return uint8x8x3_t();
	}
	static uint8x8_t const loadvu(unsigned char const* uv) {
		return vld1_u8(uv);
	}
	static void store_pixel_block(unsigned char* dst, PixelBlock& pblock, uint8x8_t const& r, uint8x8_t const& g, uint8x8_t const& b) {
		pblock.val[0] = r;
		pblock.val[1] = g;
		pblock.val[2] = b;
		vst3_u8(dst, pblock);
	}
};

bool nv21_to_rgb(unsigned char* rgb, unsigned char const* y, unsigned char const* uv, int width, int height);

#endif


#endif // NEON_H

