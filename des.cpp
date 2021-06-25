/***
 * Fast DES implementation by OLIET.
 * Full compatibility with CUDA.
 * See README.md
 * ***/

#include <iostream>
#include <cstdint>

#ifdef __CUDACC__
#define PREFIX __host__ __device__
#define CONST __constant__
#else
#define PREFIX
#define CONST const
#endif

using block_t = uint64_t;

namespace DES{

const int SBOX_HST_1[64]={14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13};
const int SBOX_HST_2[64]={15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9};
const int SBOX_HST_3[64]={10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10, 1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1, 13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7, 11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12};
const int SBOX_HST_4[64]={7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3, 1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9, 10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8, 15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14};
const int SBOX_HST_5[64]={2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1, 8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6, 4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13, 15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3};
const int SBOX_HST_6[64]={12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5, 0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8, 9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10, 7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13};
const int SBOX_HST_7[64]={4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10, 3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6, 1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7, 10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12};
const int SBOX_HST_8[64]={13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4, 10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2, 7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13, 0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11};

CONST int SBOX_DEV_1[64]={14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13};
CONST int SBOX_DEV_2[64]={15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9};
CONST int SBOX_DEV_3[64]={10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10, 1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1, 13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7, 11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12};
CONST int SBOX_DEV_4[64]={7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3, 1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9, 10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8, 15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14};
CONST int SBOX_DEV_5[64]={2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1, 8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6, 4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13, 15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3};
CONST int SBOX_DEV_6[64]={12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5, 0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8, 9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10, 7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13};
CONST int SBOX_DEV_7[64]={4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10, 3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6, 1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7, 10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12};
CONST int SBOX_DEV_8[64]={13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4, 10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2, 7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13, 0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11};

void print_bit(uint64_t x,int bit){
	for(int i=bit-1;i>=0;i--){
		std::cout << ((x>>i)&1);
	}
	std::cout << std::endl;
}

PREFIX uint32_t popcnt(uint32_t x)
{                       
    x = (x & 0x55555555) + (x>>1 & 0x55555555);
    x = (x & 0x33333333) + (x>>2 & 0x33333333);
    x = (x & 0x0F0F0F0F) + (x>>4 & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + (x>>8 & 0x00FF00FF);
    x = (x & 0x0000FFFF) + (x>>16 & 0x0000FFFF);
    return x;
}

PREFIX int parity(unsigned int x)
{
    return popcnt(x)%2;
}

PREFIX uint64_t translocate_PC1(const uint64_t k){
	return (k&0x0000000000000080)<<48
          |(k&0x0000000000000040)<<41
          |(k&0x0000000000008000)<<39
          |(k&0x0000000000000020)<<34
          |(k&0x0000000000004000)<<32
          |(k&0x0000000000800000)<<30
          |(k&0x0000000000000010)<<27
          |(k&0x0000000000000002)<<26
          |(k&0x0000000000002000)<<25
          |(k&0x0000000000400000)<<23
          |(k&0x0000000080000000)<<21
          |(k&0x0000000000001000)<<18
          |(k&0x0000000000000204)<<17
          |(k&0x0000000000200000)<<16
          |(k&0x0000000040000000)<<14
          |(k&0x0000008000000000)<<12
          |(k&0x0000000000100000)<<9
          |(k&0x0000000000020408)<<8
          |(k&0x0000000020000000)<<7
          |(k&0x0000004000000000)<<5
          |(k&0x0000800000000000)<<3
          |(k&0x0000000010000000)
          |(k&0x0000000002040800)>>1
          |(k&0x0000002000000000)>>2
          |(k&0x0000400000000000)>>4
          |(k&0x0080000000000000)>>6
          |(k&0x0000000204080000)>>10
          |(k&0x0000200000000000)>>11
          |(k&0x0040000000000000)>>13
          |(k&0x8000000000000000)>>15
          |(k&0x0000020408000000)>>19
          |(k&0x0020000000000000)>>20
          |(k&0x4000000000000000)>>22
          |(k&0x0002040800000000)>>28
          |(k&0x2000000000000000)>>29
          |(k&0x0000001000000000)>>33
          |(k&0x0204080000000000)>>37
          |(k&0x0000100000000000)>>42
          |(k&0x0408000000000000)>>46
          |(k&0x0010000000000000)>>51
          |(k&0x0800000000000000)>>55
          |(k&0x1000000000000000)>>60;
}

PREFIX block_t translocate_PC1I(const uint64_t kp){
    block_t k = (kp&0x00000000000001)<<60
               |(kp&0x00000000000010)<<55
               |(kp&0x00000000000002)<<51
               |(kp&0x00000000001020)<<46
               |(kp&0x00000000000004)<<42
               |(kp&0x00000000102040)<<37
               |(kp&0x00000000000008)<<33
               |(kp&0x00000100000000)<<29
               |(kp&0x00000000204080)<<28
               |(kp&0x00010000000000)<<22
               |(kp&0x00000200000000)<<20
               |(kp&0x00000000408100)<<19
               |(kp&0x01000000000000)<<15
               |(kp&0x00020000000000)<<13
               |(kp&0x00000400000000)<<11
               |(kp&0x00000000810200)<<10
               |(kp&0x02000000000000)<<6
               |(kp&0x00040000000000)<<4
               |(kp&0x00000800000000)<<2
               |(kp&0x00000001020400)<<1
               |(kp&0x00000010000000)
               |(kp&0x04000000000000)>>3
               |(kp&0x00080000000000)>>5
               |(kp&0x00001000000000)>>7
               |(kp&0x00000002040800)>>8
               |(kp&0x00000020000000)>>9
               |(kp&0x08000000000000)>>12
               |(kp&0x00100000000000)>>14
               |(kp&0x00002000000000)>>16
               |(kp&0x00000004080000)>>17
               |(kp&0x00000040000000)>>18
               |(kp&0x10000000000000)>>21
               |(kp&0x00200000000000)>>23
               |(kp&0x00004000000000)>>25
               |(kp&0x00000008000000)>>26
               |(kp&0x00000080000000)>>27
               |(kp&0x20000000000000)>>30
               |(kp&0x00400000000000)>>32
               |(kp&0x00008000000000)>>34
               |(kp&0x40000000000000)>>39
               |(kp&0x00800000000000)>>41
               |(kp&0x80000000000000)>>48;
#define P(i) k|=((uint64_t)(1-parity((k>>(i*8))&0xFE)))<<(i*8);
    P(0)
    P(1)
    P(2)
    P(3)
    P(4)
    P(5)
    P(6)
    P(7)
#undef P
    return k;
}

PREFIX uint64_t translocate_PC2(const uint64_t cd){
	return (cd&0x00000000000010)<<18
          |(cd&0x00000000000002)<<17
          |(cd&0x00000110000000)<<12
          |(cd&0x00000000000220)<<10
          |(cd&0x00000000008001)<<8
          |(cd&0x00008000000000)<<7
          |(cd&0x00040000000000)<<5
          |(cd&0x00000000000100)<<4
          |(cd&0x00000000000888)<<3
          |(cd&0x00000A00000000)<<2
          |(cd&0x00000040080000)<<1
          |(cd&0x00200000010000)
          |(cd&0x00000000001000)>>1
          |(cd&0x00020020000000)>>2
          |(cd&0x00002000000040)>>3
          |(cd&0x00000002000000)>>4
          |(cd&0x00000000000400)>>5
          |(cd&0x00000000020000)>>8
          |(cd&0x08000004000000)>>9
          |(cd&0x00401000804000)>>10
          |(cd&0x00110000000000)>>11
          |(cd&0xA4000000000000)>>12
          |(cd&0x00000000400000)>>15
          |(cd&0x01080000100000)>>18
          |(cd&0x10000000000000)>>20
          |(cd&0x02000000000000)>>21
          |(cd&0x00000001000000)>>24
          |(cd&0x00000008000000)>>26
          |(cd&0x40000000000000)>>30;
}

PREFIX uint64_t translocate_IP(const block_t a){
	return (a&0x0000000000000040)<<57
          |(a&0x0000000000000010)<<51
          |(a&0x0000000000004000)<<48
          |(a&0x0000000000000004)<<45
          |(a&0x0000000000001000)<<42
          |(a&0x0000000000400001)<<39
          |(a&0x0000000000000400)<<36
          |(a&0x0000000000100000)<<33
          |(a&0x0000000040000100)<<30
          |(a&0x0000000000040000)<<27
          |(a&0x0000000010000080)<<24
          |(a&0x0000004000010000)<<21
          |(a&0x0000000004000020)<<18
          |(a&0x0000001000008000)<<15
          |(a&0x0000400001000008)<<12
          |(a&0x0000000400002000)<<9
          |(a&0x0000100000800002)<<6
          |(a&0x0040000100000800)<<3
          |(a&0x0000040000200000)
          |(a&0x0010000080000200)>>3
          |(a&0x4000010000080000)>>6
          |(a&0x0004000020000000)>>9
          |(a&0x1000008000020000)>>12
          |(a&0x0001000008000000)>>15
          |(a&0x0400002000000000)>>18
          |(a&0x0000800002000000)>>21
          |(a&0x0100000800000000)>>24
          |(a&0x0000200000000000)>>27
          |(a&0x0080000200000000)>>30
          |(a&0x0000080000000000)>>33
          |(a&0x0020000000000000)>>36
          |(a&0x8000020000000000)>>39
          |(a&0x0008000000000000)>>42
          |(a&0x2000000000000000)>>45
          |(a&0x0002000000000000)>>48
          |(a&0x0800000000000000)>>51
          |(a&0x0200000000000000)>>57;
}

PREFIX uint64_t translocate_E(const uint64_t r){
	return (r&0x00000001)<<47
          |(r&0xF8000000)<<15
          |(r&0x1F800000)<<13
          |(r&0x01F80000)<<11
          |(r&0x001F8000)<<9
          |(r&0x0001F800)<<7
          |(r&0x00001F80)<<5
          |(r&0x000001F8)<<3
          |(r&0x0000001F)<<1
          |(r&0x80000000)>>31;
}

PREFIX uint32_t transform_SBOX(const uint64_t i){
	uint32_t j=0;
#ifndef __CUDA_ARCH__
#define T_SBOX(l) j|=SBOX_HST_##l[(i>>((8-l)*6))&0b111111]<<((8-l)*4);
#else
#define T_SBOX(l) j|=SBOX_DEV_##l[(i>>((8-l)*6))&0b111111]<<((8-l)*4);
#endif
	T_SBOX(1)
	T_SBOX(2)
	T_SBOX(3)
	T_SBOX(4)
	T_SBOX(5)
	T_SBOX(6)
	T_SBOX(7)
	T_SBOX(8)
#undef T_SBOX
	return j;
}

PREFIX uint32_t translocate_P(const uint32_t j){
	return (j&0x00000008)<<24
          |(j&0x00000010)<<21
          |(j&0x00001800)<<17
          |(j&0x00000002)<<16
          |(j&0x00010000)<<15
          |(j&0x00000040)<<14
          |(j&0x00000200)<<12
          |(j&0x00000001)<<11
          |(j&0x00008000)<<9
          |(j&0x00100000)<<6
          |(j&0x02020120)<<5
          |(j&0x00004000)<<4
          |(j&0x00000004)<<3
          |(j&0x00442000)>>6
          |(j&0x00000480)>>7
          |(j&0x88000000)>>8
          |(j&0x01000000)>>10
          |(j&0x00080000)>>13
          |(j&0x40800000)>>15
          |(j&0x00200000)>>19
          |(j&0x20000000)>>20
          |(j&0x04000000)>>22
          |(j&0x10000000)>>27;
}

PREFIX uint32_t f(const uint64_t r,const uint64_t sk){
	return translocate_P(transform_SBOX(translocate_E(r)^sk));
}

PREFIX block_t translocate_IPI(const uint64_t rl){
	return (rl&0x0000000000000001)<<57
          |(rl&0x0000000000000100)<<51
          |(rl&0x0000000000000002)<<48
          |(rl&0x0000000000010000)<<45
          |(rl&0x0000000000000200)<<42
          |(rl&0x0000000001000004)<<39
          |(rl&0x0000000000020000)<<36
          |(rl&0x0000000000000400)<<33
          |(rl&0x0000000002000008)<<30
          |(rl&0x0000000000040000)<<27
          |(rl&0x0000000100000800)<<24
          |(rl&0x0000000004000010)<<21
          |(rl&0x0000010000080000)<<18
          |(rl&0x0000000200001000)<<15
          |(rl&0x0001000008000020)<<12
          |(rl&0x0000020000100000)<<9
          |(rl&0x0100000400002000)<<6
          |(rl&0x0002000010000040)<<3
          |(rl&0x0000040000200000)
          |(rl&0x0200000800004000)>>3
          |(rl&0x0004000020000080)>>6
          |(rl&0x0000080000400000)>>9
          |(rl&0x0400001000008000)>>12
          |(rl&0x0008000040000000)>>15
          |(rl&0x0000100000800000)>>18
          |(rl&0x0800002000000000)>>21
          |(rl&0x0010000080000000)>>24
          |(rl&0x0000200000000000)>>27
          |(rl&0x1000004000000000)>>30
          |(rl&0x0020000000000000)>>33
          |(rl&0x0000400000000000)>>36
          |(rl&0x2000008000000000)>>39
          |(rl&0x0040000000000000)>>42
          |(rl&0x0000800000000000)>>45
          |(rl&0x4000000000000000)>>48
          |(rl&0x0080000000000000)>>51
          |(rl&0x8000000000000000)>>57;
}

PREFIX void generate_subkeys(uint64_t sk[16],const uint64_t kp){
	uint64_t c=kp&0xFFFFFFF0000000;
	uint64_t d=kp&0x0000000FFFFFFF;
	
#define G_S_1 c<<=1;c|=(c&0x100000000000000)>>28;c&=0xFFFFFFF0000000; \
              d<<=1;d|=(d&0x000000010000000)>>28;d&=0x0000000FFFFFFF; \
              *sk=translocate_PC2(c|d); \
              sk++;

#define G_S_2 c<<=2;c|=(c&0x300000000000000)>>28;c&=0xFFFFFFF0000000; \
              d<<=2;d|=(d&0x000000030000000)>>28;d&=0x0000000FFFFFFF; \
              *sk=translocate_PC2(c|d); \
              sk++;
	
	G_S_1
	G_S_1
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_1
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_2
	G_S_1
	
#undef G_S_1
#undef G_S_2
}

PREFIX block_t encrypt_block_with_subkeys_prime(const block_t pp,const uint64_t sk[16]){
    uint32_t l=(pp&0xFFFFFFFF00000000)>>32, r=pp&0x00000000FFFFFFFF, temp;
	
#define F(round) temp=r; \
                 r=l^f(r,sk[round-1]); \
                 l=temp;
	
	F(1)
	F(2)
	F(3)
	F(4)
	F(5)
	F(6)
	F(7)
	F(8)
	F(9)
	F(10)
	F(11)
	F(12)
	F(13)
	F(14)
	F(15)
	F(16)
	
#undef F
    return translocate_IPI((uint64_t)r<<32|l);
}

PREFIX block_t encrypt_block_with_subkeys(const block_t p,const uint64_t sk[16]){
	const uint64_t pp=translocate_IP(p);
    return encrypt_block_with_subkeys_prime(pp,sk);
}

PREFIX block_t encrypt_block(const block_t p,const block_t k){
    uint64_t sk[16];
	const uint64_t kp=translocate_PC1(k);
	generate_subkeys(sk,kp);
	return encrypt_block_with_subkeys(p,sk);
}

PREFIX block_t decrypt_block_with_subkeys_prime(const block_t cp,const uint64_t sk[16]){
    uint32_t l=(cp&0xFFFFFFFF00000000)>>32, r=cp&0x00000000FFFFFFFF, temp;
	
#define F(round) temp=r; \
                 r=l^f(r,sk[round-1]); \
                 l=temp;
	
	F(16)
	F(15)
	F(14)
	F(13)
	F(12)
	F(11)
	F(10)
	F(9)
	F(8)
	F(7)
	F(6)
	F(5)
	F(4)
	F(3)
	F(2)
	F(1)
	
#undef F
    return translocate_IPI((uint64_t)r<<32|l);
}

PREFIX block_t decrypt_block_with_subkeys(const block_t c,const uint64_t sk[16]){
	const uint64_t cp=translocate_IP(c);
    return decrypt_block_with_subkeys_prime(cp,sk);
}


PREFIX block_t decrypt_block(const block_t c,const block_t k){
    uint64_t sk[16];
	const uint64_t kp=translocate_PC1(k);
	generate_subkeys(sk,kp);
	return decrypt_block_with_subkeys(c,sk);
}

#ifndef __CUDACC__
void main_cpu(){
	// "ddaattaa"
	const block_t p = 0b0110010001100100011000010110000101110100011101000110000101100001;
	// "kkkeeyyy"
	const block_t k = 0b0110101101101011011010110110010101100101011110010111100101111001;
	
	const block_t c = encrypt_block(p,k);
	
	std::cout << "C: ";
	print_bit(c,64);
	
	std::cout << "P: ";
	print_bit(decrypt_block(c,k),64);
}
#else
__global__ void kernel(block_t *c,const block_t p,const block_t k){
    *c = encrypt_block(p,k);
}

void main_gpu(){
    block_t c,*c_dev;

    cudaMalloc((void**)&c_dev,sizeof(block_t));

    kernel<<<1,1>>>(c_dev,0,0);

    cudaDeviceSynchronize();

    cudaMemcpy(&c,c_dev,sizeof(block_t),cudaMemcpyDeviceToHost);
    
    std::cout << std::hex << c << std::endl;

    std::cout << decrypt_block(c,0) << " should be 0" << std::endl;

    cudaFree(c_dev);
}
#endif

void main(){
#ifndef __CUDACC__
    main_cpu();
#else
    main_gpu();
#endif
}

}
