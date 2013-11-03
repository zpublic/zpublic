#ifndef ENDIAN_H_
#define ENDIAN_H_

template <typename T16>
static T16 swap16(const T16& v)
{
    return ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff));
}

template <typename T32>
static T32 swap32(const T32& v)
{
    return (((v) >> 24) | \
           (((v) & 0x00ff0000) >> 8)  | \
           (((v) & 0x0000ff00) << 8)  | \
           ((v) << 24));
}

template <typename T64>
static T64 swap64(const T64& v)
{
    return (((v) >> 56) | \
            (((v) & 0x00ff000000000000) >> 40) | \
            (((v) & 0x0000ff0000000000) >> 24) | \
            (((v) & 0x000000ff00000000) >> 8)  | \
            (((v) & 0x00000000ff000000) << 8)  | \
            (((v) & 0x0000000000ff0000) << 24) | \
            (((v) & 0x000000000000ff00) << 40) | \
            (((v) << 56)));
}

static float swapfloat(const float& p)
{
    union { float asfloat; unsigned char asbytes[4]; } u1, u2;
	u1.asfloat = p;
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	//*p = u2.asfloat;
    return u2.asfloat;
}

static double swapdouble(const double& p)
{
	union { double asdouble; unsigned char asbytes[8]; } u1, u2;
	u1.asdouble = p;
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	//*p = u2.asfloat;
    return u2.asdouble;
}

#endif