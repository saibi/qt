#include "neon.h"

#ifdef __arm_A20__

template<typename trait>
bool decode_yuv_neon(unsigned char* out, unsigned char const* y, unsigned char const* uv, int width, int height, unsigned char fill_alpha=0xff)
{
	// pre-condition : width, height must be even
	if (0!=(width&1) || width<2 || 0!=(height&1) || height<2 || !out || !y || !uv)
		return false;

	// in & out pointers
	unsigned char* dst = out;

	// constants
	int const stride = width*trait::bytes_per_pixel;
	int const itHeight = height>>1;
	int const itWidth = width>>3;

	uint8x8_t const Yshift = vdup_n_u8(16);
	int32x4_t const tR = vdupq_n_s32(0);
	int32x4_t const tG = vdupq_n_s32(0);
	int32x4_t const tB = vdupq_n_s32(0);

	int32x4x2_t const R = vzipq_s32(tR, tR); // [tR0, tR0, tR1, tR1] [ tR2, tR2, tR3, tR3]
	int32x4x2_t const G = vzipq_s32(tG, tG); // [tG0, tG0, tG1, tG1] [ tG2, tG2, tG3, tG3]
	int32x4x2_t const B = vzipq_s32(tB, tB); // [tB0, tB0, tB1, tB1] [ tB2, tB2, tB3, tB3]

	// tmp variable
	uint16x8_t t;

// pixel block to temporary store 8 pixels
	typename trait::PixelBlock pblock = trait::init_pixelblock(fill_alpha);

	for (int j=0; j<itHeight; ++j, y+=width, dst+=stride) {
		for (int i=0; i<itWidth; ++i, y+=8, uv+=8, dst+=(8*trait::bytes_per_pixel)) {
			// Y00 = 298*(y-16) , Y01 = 298*(y-16)
						t = vmovl_u8(vqsub_u8(vld1_u8(y), Yshift));
			int32x4_t const Y00 = vmulq_n_u32(vmovl_u16(vget_low_u16(t)), 298);
			int32x4_t const Y01 = vmulq_n_u32(vmovl_u16(vget_high_u16(t)), 298);
			// Y10 = 298*(y-16) , Y11 = 298*(y-16)
			t = vmovl_u8(vqsub_u8(vld1_u8(y+width), Yshift));
			int32x4_t const Y10 = vmulq_n_u32(vmovl_u16(vget_low_u16(t)), 298);
			int32x4_t const Y11 = vmulq_n_u32(vmovl_u16(vget_high_u16(t)), 298);

			// upper 8 pixels
			trait::store_pixel_block(dst, pblock,
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(R.val[0], Y00)), vqmovun_s32(vaddq_s32(R.val[1], Y01))), 8),
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(G.val[0], Y00)), vqmovun_s32(vaddq_s32(G.val[1], Y01))), 8),
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(B.val[0], Y00)), vqmovun_s32(vaddq_s32(B.val[1], Y01))), 8));

			// lower 8 pixels
			trait::store_pixel_block(dst+stride, pblock,
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(R.val[0], Y10)), vqmovun_s32(vaddq_s32(R.val[1], Y11))), 8),
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(G.val[0], Y10)), vqmovun_s32(vaddq_s32(G.val[1], Y11))), 8),
			  vshrn_n_u16(vcombine_u16(vqmovun_s32(vaddq_s32(B.val[0], Y10)), vqmovun_s32(vaddq_s32(B.val[1], Y11))), 8));
		}
	}
	return true;
}

bool nv21_to_rgb(unsigned char* rgb, unsigned char const* y, unsigned char const* uv, int width, int height)
{
	return decode_yuv_neon<NV21toRGB_neon>(rgb, y, uv, width, height);
}

#endif
