#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lv2/lv2.h>
#include <lv2/libc.h>
#include <lv2/memory.h>
#include <lv2/patch.h>
#include <lv2/syscall.h>
#include <lv2/usb.h>
#include <lv2/storage.h>
#include <lv2/thread.h>
#include <lv2/synchronization.h>
#include <lv2/modules.h>
#include <lv2/io.h>
#include <lv2/time.h>
#include <lv2/security.h>
#include <lv2/error.h>
#include <lv2/symbols.h>
#include <lv1/stor.h>
#include <lv1/patch.h>
#include "common.h"
#include "syscall8.h"
//#include "cobra.h"
#include "modulespatch.h"
#include "mappath.h"
#include "storage_ext.h"
#include "region.h"
#include "config.h"
#include "psp.h"
#include "sm_ext.h"
#include "laboratory.h"
#include "ps3mapi_core.h"

uint8_t p_fixed[20]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t a_fixed[20]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC};
uint8_t b_fixed[20]={0xA6,0x8B,0xED,0xC3,0x34,0x18,0x02,0x9C,0x1D,0x3C,0xE3,0x3B,0x9A,0x32,0x1F,0xCC,0xBB,0x9E,0x0F,0x0B};
uint8_t n_fixed[21]={0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xB5,0xAE,0x3C,0x52,0x3E,0x63,0x94,0x4F,0x21,0x27};
uint8_t gx_fixed[20]={0x12,0x8E,0xC4,0x25,0x64,0x87,0xFD,0x8F,0xDF,0x64,0xE2,0x43,0x7B,0xC0,0xA1,0xF6,0xD5,0xAF,0xDE,0x2C};
uint8_t gy_fixed[20]={0x59,0x58,0x55,0x7E,0xB1,0xDB,0x00,0x12,0x60,0x42,0x55,0x24,0xDB,0xC3,0x79,0xD5,0xAC,0x5F,0x4A,0xDF};

uint8_t ec_p_nm[20]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x4A, 0x51, 0xC3, 0xAD, 0xC1, 0x9C, 0x6B, 0xB0, 0xDE, 0xD8};
uint8_t ec_a_nm[20]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF ,0xFF ,0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t ec_b_nm[20]={0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};
uint8_t ec_G_nm[40]={0xED, 0x71, 0x3B, 0xDA, 0x9B, 0x78, 0x02, 0x70, 0x20, 0x9B, 0x1D, 0xBC, 0x84, 0x3F, 0x5E, 0x09, 0x2A, 0x50, 0x21, 0xD3, 0xA6, 0xA7, 0xAA, 0x81, 0x4E, 0x24, 0xFF, 0xED, 0x9F, 0xBD, 0xAA, 0xDB, 0x24, 0x3C, 0x86, 0x2A, 0x53, 0xA0, 0xB5, 0x20};
uint8_t ec_N_nm[20]={0x59, 0x74, 0x12, 0x3C, 0xCB, 0xE7, 0xFD, 0x63, 0xE2, 0xC3, 0x1C, 0xC4, 0x65, 0xCD, 0xE0, 0x33, 0x44, 0x61 ,0xF0, 0xF4};

//uint8_t ec_k[21]={0x00,0x42,0x75,0x72,0x6E,0x20,0x49,0x6E,0x20,0x48,0x65,0x6C,0x6C,0x20,0x53,0x6F,0x6E,0x79,0x20,0x21,0x21};
//uint8_t ec_Q_nm[40]={0x7C,0xFD,0x88,0x8E,0xE9,0xDD,0x5F,0x60,0x30,0xA5,0xC1,0xC0,0xA2,0xF9,0x08,0x4A,0x74,0xB1,0x32,0x87,0x33,0x52,0x68,0x11,0xCF,0xBA,0x95,0x35,0x06,0xE4,0x41,0x11,0xF0,0xE2,0xD6,0xF2,0xD1,0x91,0x3E,0x69};

uint8_t ec_k[21]={0x00,0xbf,0x21,0x22,0x4b,0x04,0x1f,0x29,0x54,0x9d,0xb2,0x5e,0x9a,0xad,0xe1,0x9e,0x72,0x0a,0x1f,0xe0,0xf1};
uint8_t ec_Q_nm[40]={0x94,0x8D,0xA1,0x3E,0x8C,0xAF,0xD5,0xBA,0x0E,0x90,0xCE,0x43,0x44,0x61,0xBB,0x32,0x7F,0xE7,0xE0,0x80,0x47,0x5E,0xAA,0x0A,0xD3,0xAD,0x4F,0x5B,0x62,0x47,0xA7,0xFD,0xA8,0x6D,0xF6,0x97,0x90,0x19,0x67,0x73};

struct point {
	u8 x[20];
	u8 y[20];
};


static uint8_t ec_p[20];
static uint8_t ec_a[20];
static uint8_t ec_b[20];
static struct point ec_G;
static struct point ec_Q;
static uint8_t ec_N[21];
/*
static void memcpy_inv(u8 *dst, u8 *src, u32 len)
{
	u32 j;

	for (j = 0; j < len; j++)
		dst[j] = ~src[j];
}
*/
void bn_print(char *name, u8 *a, u32 n)
{
	u32 i;

	printf("%s = ", name);

	for (i = 0; i < n; i++)
		printf("%02x", a[i]);

	printf("\n");
}

static void bn_zero(u8 *d, u32 n)
{
	memset(d, 0, n);
}

void bn_copy(u8 *d, u8 *a, u32 n)
{
	memcpy(d, a, n);
}

int bn_compare(u8 *a, u8 *b, u32 n)
{
	u32 i;

	for (i = 0; i < n; i++) {
		if (a[i] < b[i])
			return -1;
		if (a[i] > b[i])
			return 1;
	}

	return 0;
}

static u8 bn_add_1(u8 *d, u8 *a, u8 *b, u32 n)
{
	u32 i;
	u32 dig;
	u8 c;

	c = 0;
	for (i = n - 1; i < n; i--) {
		dig = a[i] + b[i] + c;
		c = dig >> 8;
		d[i] = dig;
	}

	return c;
}

static u8 bn_sub_1(u8 *d, u8 *a, u8 *b, u32 n)
{
	u32 i;
	u32 dig;
	u8 c;

	c = 1;
	for (i = n - 1; i < n; i--) {
		dig = a[i] + 255 - b[i] + c;
		c = dig >> 8;
		d[i] = dig;
	}

	return 1 - c;
}

void bn_reduce(u8 *d, u8 *N, u32 n)
{
	if (bn_compare(d, N, n) >= 0)
		bn_sub_1(d, d, N, n);
}

void bn_add(u8 *d, u8 *a, u8 *b, u8 *N, u32 n)
{
	if (bn_add_1(d, a, b, n))
		bn_sub_1(d, d, N, n);

	bn_reduce(d, N, n);
}

void bn_sub(u8 *d, u8 *a, u8 *b, u8 *N, u32 n)
{
	if (bn_sub_1(d, a, b, n))
		bn_add_1(d, d, N, n);
}

static const u8 inv256[0x80] = {
	0x01, 0xab, 0xcd, 0xb7, 0x39, 0xa3, 0xc5, 0xef,
	0xf1, 0x1b, 0x3d, 0xa7, 0x29, 0x13, 0x35, 0xdf,
	0xe1, 0x8b, 0xad, 0x97, 0x19, 0x83, 0xa5, 0xcf,
	0xd1, 0xfb, 0x1d, 0x87, 0x09, 0xf3, 0x15, 0xbf,
	0xc1, 0x6b, 0x8d, 0x77, 0xf9, 0x63, 0x85, 0xaf,
	0xb1, 0xdb, 0xfd, 0x67, 0xe9, 0xd3, 0xf5, 0x9f,
	0xa1, 0x4b, 0x6d, 0x57, 0xd9, 0x43, 0x65, 0x8f,
	0x91, 0xbb, 0xdd, 0x47, 0xc9, 0xb3, 0xd5, 0x7f,
	0x81, 0x2b, 0x4d, 0x37, 0xb9, 0x23, 0x45, 0x6f,
	0x71, 0x9b, 0xbd, 0x27, 0xa9, 0x93, 0xb5, 0x5f,
	0x61, 0x0b, 0x2d, 0x17, 0x99, 0x03, 0x25, 0x4f,
	0x51, 0x7b, 0x9d, 0x07, 0x89, 0x73, 0x95, 0x3f,
	0x41, 0xeb, 0x0d, 0xf7, 0x79, 0xe3, 0x05, 0x2f,
	0x31, 0x5b, 0x7d, 0xe7, 0x69, 0x53, 0x75, 0x1f,
	0x21, 0xcb, 0xed, 0xd7, 0x59, 0xc3, 0xe5, 0x0f,
	0x11, 0x3b, 0x5d, 0xc7, 0x49, 0x33, 0x55, 0xff,
};

static void bn_mon_muladd_dig(u8 *d, u8 *a, u8 b, u8 *N, u32 n)
{
	u32 dig;
	u32 i;

	u8 z = -(d[n-1] + a[n-1]*b) * inv256[N[n-1]/2];

	dig = d[n-1] + a[n-1]*b + N[n-1]*z;
	dig >>= 8;

	for (i = n - 2; i < n; i--) {
		dig += d[i] + a[i]*b + N[i]*z;
		d[i+1] = dig;
		dig >>= 8;
	}

	d[0] = dig;
	dig >>= 8;

	if (dig)
		bn_sub_1(d, d, N, n);

	bn_reduce(d, N, n);
}

void bn_mon_mul(u8 *d, u8 *a, u8 *b, u8 *N, u32 n)
{
	u8 t[512];
	u32 i;

	bn_zero(t, n);

	for (i = n - 1; i < n; i--)
		bn_mon_muladd_dig(t, a, b[i], N, n);

	bn_copy(d, t, n);
}

void bn_to_mon(u8 *d, u8 *N, u32 n)
{
	u32 i;

	for (i = 0; i < 8*n; i++)
		bn_add(d, d, d, N, n);
}

void bn_from_mon(u8 *d, u8 *N, u32 n)
{
	u8 t[512];

	bn_zero(t, n);
	t[n-1] = 1;
	bn_mon_mul(d, d, t, N, n);
}

static void bn_mon_exp(u8 *d, u8 *a, u8 *N, u32 n, u8 *e, u32 en)
{
	u8 t[512];
	u32 i;
	u8 mask;

	bn_zero(d, n);
	d[n-1] = 1;
	bn_to_mon(d, N, n);

	for (i = 0; i < en; i++)
		for (mask = 0x80; mask != 0; mask >>= 1) {
			bn_mon_mul(t, d, d, N, n);
			if ((e[i] & mask) != 0)
				bn_mon_mul(d, t, a, N, n);
			else
				bn_copy(d, t, n);
		}
}

static void elt_copy(u8 *d, u8 *a)
{
	memcpy(d, a, 20);
}

static void elt_zero(u8 *d)
{
	memset(d, 0, 20);
}

static int elt_is_zero(u8 *d)
{
	u32 i;

	for (i = 0; i < 20; i++)
		if (d[i] != 0)
			return 0;

	return 1;
}

static void elt_add(u8 *d, u8 *a, u8 *b)
{
	bn_add(d, a, b, ec_p, 20);
}

static void elt_sub(u8 *d, u8 *a, u8 *b)
{
	bn_sub(d, a, b, ec_p, 20);
}

static void elt_mul(u8 *d, u8 *a, u8 *b)
{
	bn_mon_mul(d, a, b, ec_p, 20);
}

static void elt_square(u8 *d, u8 *a)
{
	elt_mul(d, a, a);
}

void bn_mon_inv(u8 *d, u8 *a, u8 *N, u32 n)
{
	u8 t[512], s[512];

	bn_zero(s, n);
	s[n-1] = 2;
	bn_sub_1(t, N, s, n);
	bn_mon_exp(d, a, N, n, t, n);
}

static void elt_inv(u8 *d, u8 *a)
{
	u8 s[20];
	elt_copy(s, a);
	bn_mon_inv(d, s, ec_p, 20);
}

static void point_to_mon(struct point *p)
{
	bn_to_mon(p->x, ec_p, 20);
	bn_to_mon(p->y, ec_p, 20);
}

static void point_from_mon(struct point *p)
{
	bn_from_mon(p->x, ec_p, 20);
	bn_from_mon(p->y, ec_p, 20);
}

static void point_zero(struct point *p)
{
	elt_zero(p->x);
	elt_zero(p->y);
}

static int point_is_zero(struct point *p)
{
	return elt_is_zero(p->x) && elt_is_zero(p->y);
}

static void point_double(struct point *r, struct point *p)
{
	u8 s[20], t[20];
	struct point pp;
	u8 *px, *py, *rx, *ry;

	pp = *p;

	px = pp.x;
	py = pp.y;
	rx = r->x;
	ry = r->y;

	if (elt_is_zero(py)) {
		point_zero(r);
		return;
	}

	elt_square(t, px);	// t = px*px
	elt_add(s, t, t);	// s = 2*px*px
	elt_add(s, s, t);	// s = 3*px*px
	elt_add(s, s, ec_a);	// s = 3*px*px + a
	elt_add(t, py, py);	// t = 2*py
	elt_inv(t, t);		// t = 1/(2*py)
	elt_mul(s, s, t);	// s = (3*px*px+a)/(2*py)

	elt_square(rx, s);	// rx = s*s
	elt_add(t, px, px);	// t = 2*px
	elt_sub(rx, rx, t);	// rx = s*s - 2*px

	elt_sub(t, px, rx);	// t = -(rx-px)
	elt_mul(ry, s, t);	// ry = -s*(rx-px)
	elt_sub(ry, ry, py);	// ry = -s*(rx-px) - py
}

static void point_add(struct point *r, struct point *p, struct point *q)
{
	u8 s[20], t[20], u[20];
	u8 *px, *py, *qx, *qy, *rx, *ry;
	struct point pp, qq;

	pp = *p;
	qq = *q;

	px = pp.x;
	py = pp.y;
	qx = qq.x;
	qy = qq.y;
	rx = r->x;
	ry = r->y;

	if (point_is_zero(&pp)) {
		elt_copy(rx, qx);
		elt_copy(ry, qy);
		return;
	}

	if (point_is_zero(&qq)) {
		elt_copy(rx, px);
		elt_copy(ry, py);
		return;
	}

	elt_sub(u, qx, px);

	if (elt_is_zero(u)) {
		elt_sub(u, qy, py);
		if (elt_is_zero(u))
			point_double(r, &pp);
		else
			point_zero(r);

		return;
	}

	elt_inv(t, u);		// t = 1/(qx-px)
	elt_sub(u, qy, py);	// u = qy-py
	elt_mul(s, t, u);	// s = (qy-py)/(qx-px)

	elt_square(rx, s);	// rx = s*s
	elt_add(t, px, qx);	// t = px+qx
	elt_sub(rx, rx, t);	// rx = s*s - (px+qx)

	elt_sub(t, px, rx);	// t = -(rx-px)
	elt_mul(ry, s, t);	// ry = -s*(rx-px)
	elt_sub(ry, ry, py);	// ry = -s*(rx-px) - py
}

static void point_mul(struct point *d, u8 *a, struct point *b)	// a is bignum
{
	u32 i;
	u8 mask;

	point_zero(d);

	for (i = 0; i < 21; i++)
		for (mask = 0x80; mask != 0; mask >>= 1) {
			point_double(d, d);
			if ((a[i] & mask) != 0)
				point_add(d, d, b);
		}
}

static void generate_ecdsa(u8 *R, u8 *S, u8 *k, u8 *hash)
{
	u8 e[21];
	u8 kk[21];
	u8 m[21];
	u8 minv[21];
	struct point mG;

	e[0] = 0;
	memcpy(e + 1, hash, 20);
	bn_reduce(e, ec_N, 21);

try_again:
	get_pseudo_random_number(m,sizeof(m));
	m[0] = 0;
	if (bn_compare(m, ec_N, 21) >= 0)
		goto try_again;

	//	R = (mG).x
	point_mul(&mG, m, &ec_G);
	point_from_mon(&mG);
	R[0] = 0;
	elt_copy(R+1, mG.x);

	//	S = m**-1*(e + Rk) (mod N)
	bn_copy(kk, k, 21);
	bn_reduce(kk, ec_N, 21);
	bn_to_mon(m, ec_N, 21);
	bn_to_mon(e, ec_N, 21);
	bn_to_mon(R, ec_N, 21);
	bn_to_mon(kk, ec_N, 21);

	bn_mon_mul(S, R, kk, ec_N, 21);
	bn_add(kk, S, e, ec_N, 21);
	bn_mon_inv(minv, m, ec_N, 21);
	bn_mon_mul(S, minv, kk, ec_N, 21);

	bn_from_mon(R, ec_N, 21);
	bn_from_mon(S, ec_N, 21);
}

int set_vsh_curve(u8 *p, u8 *a, u8 *b, u8 *N, u8 *Gx, u8 *Gy)
{	
	memcpy(p,p_fixed, 20);
	memcpy(a, a_fixed, 20);
	memcpy(b, b_fixed, 20);
	memcpy(N, n_fixed, 21);
	memcpy(Gx, gx_fixed, 20);
	memcpy(Gy, gy_fixed, 20);

	return 0;
}

int ecdsa_set_curve()
{
	set_vsh_curve(ec_p, ec_a, ec_b, ec_N, ec_G.x, ec_G.y);
	bn_to_mon(ec_a, ec_p, 20);
	bn_to_mon(ec_b, ec_p, 20);

	point_to_mon(&ec_G);
	return 0;
}

void ecdsa_set_pub()
{
	memcpy(ec_Q.x, ec_Q_nm,20);
	memcpy(ec_Q.y, ec_Q_nm+20,20);
	point_to_mon(&ec_Q);
}

void ecdsa_set_priv()
{
	//ec_k already set
}
/*
static int check_ecdsa(struct point *Q, u8 *R, u8 *S, u8 *hash)
{
	u8 Sinv[21];
	u8 e[21];
	u8 w1[21], w2[21];
	struct point r1, r2;
	u8 rr[21];

	e[0] = 0;
	memcpy(e + 1, hash, 20);
	bn_reduce(e, ec_N, 21);

	bn_to_mon(R, ec_N, 21);
	bn_to_mon(S, ec_N, 21);
	bn_to_mon(e, ec_N, 21);

	bn_mon_inv(Sinv, S, ec_N, 21);

	bn_mon_mul(w1, e, Sinv, ec_N, 21);
	bn_mon_mul(w2, R, Sinv, ec_N, 21);

	bn_from_mon(w1, ec_N, 21);
	bn_from_mon(w2, ec_N, 21);

	point_mul(&r1, w1, &ec_G);
	point_mul(&r2, w2, Q);

	point_add(&r1, &r1, &r2);

	point_from_mon(&r1);

	rr[0] = 0;
	memcpy(rr + 1, r1.x, 20);
	bn_reduce(rr, ec_N, 21);

	bn_from_mon(R, ec_N, 21);
	bn_from_mon(S, ec_N, 21);

	return (bn_compare(rr, R, 21) == 0);
}
*/
void ecdsa_sign(u8 *hash, u8 *R, u8 *S)
{
	generate_ecdsa(R, S, ec_k, hash);
}

// Format of version:
// byte 0, 7 MS bits -> reserved
// byte 0, 1 LS bit -> 1 = CFW version, 0 = OFW/exploit version
// byte 1 and 2 -> ps3 fw version in BCD e.g 3.55 = 03 55. For legacy reasons, 00 00 means 3.41
// byte 3 is cobra firmware version,
// 1 = version 1.0-1.2,
// 2 = 2.0,
// 3 = 3.0
// 4 = 3.1
// 5 = 3.2
// 6 = 3.3
// 7 = 4.0
// 8 = 4.1
// 9 = 4.2
// A = 4.3
// B = 4.4
// C = 5.0
// D = 5.1
// E = 6.0
// F = 7.0

//#define CB_LOCATION "/dev_habib/rebug/cobra/stage2.cex"

#define COBRA_VERSION		0x0F
#define COBRA_VERSION_BCD	0x0810
#define HEN_REV				0x0232

#if defined(FIRMWARE_4_82)
	#define FIRMWARE_VERSION	0x0482
#elif defined(FIRMWARE_4_82DEX)
	#define FIRMWARE_VERSION	0x0482
#elif defined(FIRMWARE_4_84)
	#define FIRMWARE_VERSION	0x0484
#elif defined(FIRMWARE_4_84DEX)
	#define FIRMWARE_VERSION	0x0484  
#elif defined(FIRMWARE_4_85)
	#define FIRMWARE_VERSION	0x0485
#endif

#if defined(CFW)
	#define IS_CFW			1
#else
	#define IS_CFW			0
#endif

#define MAKE_VERSION(cobra, fw, type) ((cobra&0xFF) | ((fw&0xffff)<<8) | ((type&0x1)<<24))

typedef struct
{
	uint32_t address;
	uint32_t data;
} Patch;
/*
static Patch kernel_patches[] =
{
	{ patch_data1_offset, 0x01000000 },
	{ patch_func8_offset1, LI(R3, 0) }, // force lv2open return 0

	// disable calls in lv2open to lv1_send_event_locally which makes the system crash
	{ patch_func8_offset2, NOP },
	{ patch_func9_offset, NOP }, // 4.30 - watch: additional call after

	// psjailbreak, PL3, etc destroy this function to copy their code there.
	// We don't need that, but let's dummy the function just in case that patch is really necessary
	{ mem_base2, LI(R3, 1) },
	{ mem_base2 + 4, BLR },

	// sys_sm_shutdown, for ps2 let's pass to copy_from_user a fourth parameter
	{ shutdown_patch_offset, MR(R6, R31) },
	{ module_sdk_version_patch_offset, NOP },

	// User thread prio hack (needed for netiso)
	{ user_thread_prio_patch, NOP },
	{ user_thread_prio_patch2, NOP },

	// ODE Protection removal (needed for CFW 4.60+) 
	{ lic_patch, LI(R3, 1) },
	{ ode_patch, LI(R3, 0) },
	{ ode_patch + 4, STD(R3, 0, R9) },
};*/

#define N_KERNEL_PATCHES	(sizeof(kernel_patches) / sizeof(Patch))

int inst_and_run_kernel(uint8_t *payload, int size)
{
	if((!size) || (size>0x10000))
		return -1;
	
	if(!payload)
		return -2;
	
	memcpy((void *)0x80000000007f0000, get_secure_user_ptr(payload), size);

	f_desc_t f;
	int (* func)(void);
	f.addr = (void *)0x80000000007f0000;
	f.toc = (void *)MKA(TOC);
	func = (void *)&f;
	func();
	return 0;
}

int inst_and_run_kernel_dynamic(uint8_t *payload, int size, uint64_t *residence)
{
	if(!size)
		return -1;
	
	if(!payload)
		return -2;
	
	void *skprx=NULL;

	skprx=alloc(size,0x27);
	
	if(skprx)
	{
		memcpy(skprx, get_secure_user_ptr(payload), size);

		f_desc_t f;
		int (* func)(void);
		f.addr = skprx;
		f.toc = (void *)MKA(TOC);
		func = (void *)&f;
		func();
		uint64_t resident=(uint64_t)skprx;
		copy_to_user(&resident, get_secure_user_ptr(residence), 8);
		return 1;
	}
	
	return 0;
}

int rif_fd;
int act_fd;
int misc_fd;

LV2_HOOKED_FUNCTION_COND_POSTCALL_3(int,bnet_ioctl,(int socket,uint32_t flags, void* buffer))
{
	if(flags==0x10007300)
		return 0;
	else
		return DO_POSTCALL;
}

LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_6(int,sys_fs_open,(const char *path, int flags, int *fd, uint64_t mode, const void *arg, uint64_t size))
{
	if(!strstr(get_process_name(get_current_process_critical()),"vsh"))
	{
		rif_fd=0;
		act_fd=0;
		misc_fd=0;
		return 0;
	}
	
	int path_len=strlen(path);
	if(strstr(path,".rif"))
	{
		DPRINTF("RIF fd open called:%s\n",path);
		rif_fd=*fd;
	}
	else if(strstr(path,"act.dat"))
	{
		DPRINTF("act.dat fd open called:%s\n",path);
		act_fd=*fd;
	}
	else if((strstr(path,".edat")) || (strstr(path,".EDAT")) || (strstr(path,"ISO.BIN.ENC")) || (strstr(path+path_len-7,"CONFIG")))
	{
		misc_fd=*fd;
	}
	return 0;
}

int sha1(uint8_t *buf, uint64_t size, uint8_t *out)
{
	SHACtx *ctx;
	page_allocate_auto(NULL, 0x100, 0x2F, (void *)&ctx);
	sha1_init(ctx);
	sha1_update(ctx,buf,size);
	sha1_final(out,ctx);
	page_free(NULL, (void *)ctx, 0x2F);
	DPRINT_HEX(out, 20);
	return 0;
}

/////// 2.1.2 Encryption fix & hmac hash validation ///// START
#define SHA_BLOCKSIZE   (64)
#define SHA_DIGEST_LENGTH	(20)

/**
* Function to compute the digest
*
* @param k   Secret key
* @param lk  Length of the key in bytes
* @param d   Data
* @param ld  Length of data in bytes
* @param out Digest output
* @param t   Size of digest output
*/
void hmac_sha1(const uint8_t *k,  /* secret key */
        size_t lk,       /* length of the key in bytes */
        const uint8_t *d,  /* data */
        size_t ld,       /* length of data in bytes */
        uint8_t *out)      /* output buffer */ {
    SHACtx *ictx=alloc(0x100,0x27);
	SHACtx *octx=alloc(0x100,0x27);
    uint8_t isha[SHA_DIGEST_LENGTH];
    uint8_t key[SHA_DIGEST_LENGTH];
    uint8_t buf[SHA_BLOCKSIZE];
    size_t i;

    if (lk > SHA_BLOCKSIZE) {
        SHACtx *tctx=alloc(0x100,0x27);

        sha1_init(tctx);
        sha1_update(tctx, k, lk);
        sha1_final(key, tctx);
		dealloc(tctx,0x27);

        k = key;
        lk = SHA_DIGEST_LENGTH;
    }

    /**** Inner Digest ****/

    sha1_init(ictx);

    /* Pad the key for inner digest */
    for (i = 0; i < lk; ++i) {
        buf[i] = k[i] ^ 0x36;
    }
    for (i = lk; i < SHA_BLOCKSIZE; ++i) {
        buf[i] = 0x36;
    }

    sha1_update(ictx, buf, SHA_BLOCKSIZE);
    sha1_update(ictx, d, ld);

    sha1_final(isha, ictx);

    /**** Outer Digest ****/

    sha1_init(octx);

    /* Pad the key for outter digest */

    for (i = 0; i < lk; ++i) {
        buf[i] = k[i] ^ 0x5c;
    }
    for (i = lk; i < SHA_BLOCKSIZE; ++i) {
        buf[i] = 0x5c;
    }

    sha1_update(octx, buf, SHA_BLOCKSIZE);
    sha1_update(octx, isha, SHA_DIGEST_LENGTH);

    sha1_final(out, octx);

	dealloc(ictx,0x27);
	dealloc(octx,0x27);
}

uint8_t erk[0x20] = {
	0x34, 0x18, 0x12, 0x37, 0x62, 0x91, 0x37, 0x1c,
	0x8b, 0xc7, 0x56, 0xff, 0xfc, 0x61, 0x15, 0x25,
	0x40, 0x3f, 0x95, 0xa8, 0xef, 0x9d, 0x0c, 0x99,
	0x64, 0x82, 0xee, 0xc2, 0x16, 0xb5, 0x62, 0xed
};

uint8_t hmac[0x40] = {
	0xcc, 0x30, 0xc4, 0x22, 0x91, 0x13, 0xdb, 0x25,
	0x73, 0x35, 0x53, 0xaf, 0xd0, 0x6e, 0x87, 0x62,
	0xb3, 0x72, 0x9d, 0x9e, 0xfa, 0xa6, 0xd5, 0xf3,
	0x5a, 0x6f, 0x58, 0xbf, 0x38, 0xff, 0x8b, 0x5f,
	0x58, 0xa2, 0x5b, 0xd9, 0xc9, 0xb5, 0x0b, 0x01,
	0xd1, 0xab, 0x40, 0x28, 0x67, 0x69, 0x68, 0xea,
	0xc7, 0xf8, 0x88, 0x33, 0xb6, 0x62, 0x93, 0x5d,
	0x75, 0x06, 0xa6, 0xb5, 0xe0, 0xf9, 0xd9, 0x7a
};

uint8_t iv_qa[0x10] = {
	0xe8, 0x66, 0x3a, 0x69, 0xcd, 0x1a, 0x5c, 0x45,
	0x4a, 0x76, 0x1e, 0x72, 0x8c, 0x7c, 0x25, 0x4e
};
/////// 2.1.2 Encryption fix & hmac hash validation ///// END

LV2_HOOKED_FUNCTION_COND_POSTCALL_5(int,um_if_get_token,(uint8_t *token,uint32_t token_size,uint8_t *seed,uint32_t seed_size))
{
	if(seed!=0 && token!=0 && token_size==0x50 && seed_size==0x50)
	{
		memcpy(seed + 4, (void *)PS3MAPI_IDPS_2, 0x10);
		seed[3] = 1;
		seed[39] |= 0x1; /* QA_FLAG_EXAM_API_ENABLE */
		seed[39] |= 0x2; /* QA_FLAG_QA_MODE_ENABLE */
		seed[47] |= 0x2;
		seed[47] |= 0x4; /* checked by lv2_kernel.self and sys_init_osd.self */
				 /* can run sys_init_osd.self from /app_home ? */
		seed[51] |= 0x1; /* QA_FLAG_ALLOW_NON_QA */
		seed[51] |= 0x2; /* QA_FLAG_FORCE_UPDATE */
        /// 2.1.2 QA flag - hmac hash check - START
        hmac_sha1(hmac,64,seed,60,seed+60);
		aescbccfb_enc(token,seed,0x50,erk,256,iv_qa);
		DPRINT_HEX_C(seed,60);
         /// 2.1.2 QA flag - hmac hash check - END
		return 0;
	}

	return DO_POSTCALL;
}
	
LV2_HOOKED_FUNCTION_COND_POSTCALL_3(int,read_eeprom_by_offset,(uint32_t offset, uint8_t *value, uint64_t auth_id))
{
	if(offset==0x48c0a)
	{
		*value=0;
		return 0;
	}
	return DO_POSTCALL;
}

LV2_HOOKED_FUNCTION_PRECALL_SUCCESS_4(int,sys_fs_read,(int fd, void *buf, uint64_t nbytes, uint64_t *nread))
{	
	if(rif_fd==fd)
	{
		DPRINTF("RIF fd read called:%x %p %016lx %p\n",fd,buf,nbytes,nread);
		if(*nread==0x98)
		{
			DPRINTF("generating rif ECDSA\n");
			uint8_t *buf1;
			page_allocate_auto(NULL, 0x98, 0x2F, (void*)&buf1);
			memcpy(buf1,buf,0x98);
			uint8_t sha1_digest[20];
			sha1(buf1, 0x70,sha1_digest);
			uint8_t R[0x15];
			uint8_t S[0x15];
			ecdsa_sign(sha1_digest, R, S);
			memcpy(buf1+0x70, R+1, 0x14);
			memcpy(buf1+0x70+0x14, S+1, 0x14);
			memcpy(buf+0x70,buf1+0x70,0x28);
			page_free(NULL, buf1, 0x2F);
			rif_fd=0;
//			DPRINTF("R:%015x\nS:%015x\n",R,S);
		}
	}
	else if(act_fd==fd)
	{
		DPRINTF("act fd read called:%x %p %016lx %p\n\n",fd,buf,nbytes,nread);
		if(*nread==0x1038)
		{
			DPRINTF("generating act ECDSA\n");
			uint8_t *buf1;
			page_allocate_auto(NULL, 0x1038, 0x2F, (void*)&buf1);
			memcpy(buf1,buf,0x1038);
			uint8_t sha1_digest[20];
			sha1(buf1, 0x1010,sha1_digest);
			uint8_t R[0x15];
			uint8_t S[0x15];
			ecdsa_sign(sha1_digest, R, S);
			memcpy(buf1+0x1010, R+1, 0x14);
			memcpy(buf1+0x1010+0x14, S+1, 0x14);
			memcpy(buf+0x1010,buf1+0x1010,0x28);
			page_free(NULL, buf1, 0x2F);
			act_fd=0;
//			DPRINTF("R:%015x\nS:%015x\n",R,S);
		}
	}
	else if(misc_fd==fd)
	{
		if(*nread==0x100)
		{
			DPRINTF("generating misc ECDSA\n");
			uint8_t *buf1;
			page_allocate_auto(NULL, 0x100, 0x2F, (void*)&buf1);
			memcpy(buf1,buf,0x100);
			uint8_t sha1_digest[20];
			sha1(buf1, 0xd8,sha1_digest);
			uint8_t R[0x15];
			uint8_t S[0x15];
			ecdsa_sign(sha1_digest, R, S);
			memcpy(buf1+0xd8, R+1, 0x14);
			memcpy(buf1+0xd8+0x14, S+1, 0x14);
			memcpy(buf+0xd8,buf1+0xd8,0x28);
			page_free(NULL, buf1, 0x2F);
			misc_fd=0;
		}
	}
	return 0;
}

int unload_plugin_kernel(uint64_t residence)
{
	dealloc((void*)residence,0x27);
	return 0;
}

f_desc_t extended_syscall8;

int64_t syscall8(uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7);

LV2_SYSCALL2(uint64_t, sys_cfw_peek, (uint64_t *addr))
{
	/* if (block_peek)
		return (uint64_t)ENOSYS; */

	//DPRINTF("peek %p\n", addr);

	uint64_t ret = *addr;

	return ret;
}

//static void *current_813;

void _sys_cfw_poke(uint64_t *addr, uint64_t value);

LV2_HOOKED_FUNCTION(void, sys_cfw_new_poke, (uint64_t *addr, uint64_t value))
{
	DPRINTF("New poke called\n");

	_sys_cfw_poke(addr, value);
	asm volatile("icbi 0,%0; isync" :: "r"(addr));
}

LV2_HOOKED_FUNCTION(void *, sys_cfw_memcpy, (void *dst, void *src, uint64_t len))
{
	DPRINTF("sys_cfw_memcpy: %p %p 0x%lx\n", dst, src, len);

	if (len == 8)
	{
		_sys_cfw_poke(dst, *(uint64_t *)src);
		return dst;
	}

	return memcpy(dst, src, len);
}

/*
#define MAX_POKES	0x100
POKES pokes[MAX_POKES];
int poke_count;
*/

LV2_SYSCALL2(void, sys_cfw_poke, (uint64_t *ptr, uint64_t value))
{
	DPRINTF("LV2 poke %p %016lx\n", ptr, value);
	uint64_t addr=(uint64_t)ptr;
	if (addr >= MKA(syscall_table_symbol))
	{
		uint64_t syscall_num = (addr-MKA(syscall_table_symbol)) / 8;

		if ((syscall_num >= 6 && syscall_num <= 10) || syscall_num == 35)
		{
			uint64_t sc_null = *(uint64_t *)MKA(syscall_table_symbol);
			uint64_t syscall_not_impl = *(uint64_t *)sc_null;

			if (((value == sc_null) ||(value == syscall_not_impl)) && (syscall_num != 8)) //Allow removing protected syscall 6 7 9 10 35 NOT 8
			{
				DPRINTF("HB remove syscall %ld\n", syscall_num);
				*ptr=value;
				return;
			}
			else //Prevent syscall 6 7 9 10 and 35 from being re-written
			{
				DPRINTF("HB has been blocked from rewritting syscall %ld\n", syscall_num);
				return;
			}
		}
	}
	
	if(addr>MKA(hash_checked_area))
	{
		*ptr=value;
		return;
	}
	
/*	if(poke_count)
	{
		for(int i=0;i<poke_count;i++)
		{
			if(pokes[i].addr==addr)
			{
				pokes[i].poke_val=value;
				*ptr=value;
				return;
			}
		}
	}
	
	pokes[poke_count].addr=addr;
	pokes[poke_count].poke_val=value;
	pokes[poke_count].orig_val=*ptr;
	poke_count++;
	
	*ptr=value;*/
}

LV2_SYSCALL2(void, sys_cfw_lv1_poke, (uint64_t lv1_addr, uint64_t lv1_value))
{
	DPRINTF("LV1 poke %p %016lx\n", (void*)lv1_addr, lv1_value);	
	lv1_poked(lv1_addr, lv1_value);
}

LV2_SYSCALL2(void, sys_cfw_lv1_call, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 11, 10\n");
	asm("sc 1\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

LV2_SYSCALL2(void, sys_cfw_lv2_func, (uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t num))
{
	/* DO NOT modify */
	asm("mflr 0\n");
	asm("std 0, 16(1)\n");
	asm("mr 0, 10\n");
	asm("mtctr 0\n");
	asm("bctrl\n");
	asm("ld 0, 16(1)\n");
	asm("mtlr 0\n");
	asm("blr\n");
}

static INLINE int sys_get_version(uint32_t *version)
{
	uint32_t pv = MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW);
	return copy_to_user(&pv, get_secure_user_ptr(version), sizeof(uint32_t));
}

static INLINE int sys_get_version2(uint16_t *version)
{
	uint16_t cb = COBRA_VERSION_BCD;
	return copy_to_user(&cb, get_secure_user_ptr(version), sizeof(uint16_t));
}


static inline void ps3mapi_unhook_all(void)
{
	unhook_all_modules();
	unhook_all_region();
	unhook_all_map_path();
    unhook_all_storage_ext();
	//unhook_all_permissions();
}

static uint8_t photo_gui;

static uint64_t ps3mapi_key = 0;
static uint8_t ps3mapi_access_tries = 0;
static uint8_t ps3mapi_access_granted = 1;

static int ps3mapi_partial_disable_syscall8 = 0;
static uint8_t disable_cobra = 0;

LV2_SYSCALL2(int64_t, syscall8, (uint64_t function, uint64_t param1, uint64_t param2, uint64_t param3, uint64_t param4, uint64_t param5, uint64_t param6, uint64_t param7))
{
	extend_kstack(0);

	DPRINTF("Syscall 8 -> %lx\n", function);
	
	// -- AV: temporary disable cobra syscall (allow dumpers peek 0x1000 to 0x9800)
	static uint8_t tmp_lv1peek = 0;

	if(ps3mapi_partial_disable_syscall8 == 0 && extended_syscall8.addr == 0 && ps3mapi_access_granted)
	{
		if((function >= 0x9800) || (function & 3)) 
			tmp_lv1peek=0; else

		if(function <= 0x1000) 
		{
			tmp_lv1peek=1; 

			if(function <= SYSCALL8_OPCODE_ENABLE_COBRA) 
			{
				if(param1>=SYSCALL8_DISABLE_COBRA_CAPABILITY) 
					return (param1==SYSCALL8_DISABLE_COBRA_CAPABILITY) ? SYSCALL8_DISABLE_COBRA_OK : disable_cobra; 
				else 
					disable_cobra = (function==SYSCALL8_OPCODE_DISABLE_COBRA && param1==1);
			}
		}
	
	}
	else 
		tmp_lv1peek=0;
	// --


	if ((function == SYSCALL8_OPCODE_PS3MAPI) && ((int)param1 == PS3MAPI_OPCODE_REQUEST_ACCESS) && (param2 == ps3mapi_key) && (ps3mapi_access_tries < 3)) 
	{
		ps3mapi_access_tries = 0; 
		ps3mapi_access_granted = 1;
	}

	if((!ps3mapi_access_granted) && (ps3mapi_key != 0))
	{
		ps3mapi_access_tries += 1;

		if(ps3mapi_access_tries > 3) 
			ps3mapi_access_tries = 99;

		return ENOSYS;
	}

	if (3 <= ps3mapi_partial_disable_syscall8)
	{
		if (function == SYSCALL8_OPCODE_PS3MAPI && ps3mapi_access_granted)
		{
			if ((int)param1 == PS3MAPI_OPCODE_PDISABLE_SYSCALL8)
			{
				ps3mapi_partial_disable_syscall8 = (int)param2;
				return SUCCEEDED;
			}
			else if ((int)param1 == PS3MAPI_OPCODE_PCHECK_SYSCALL8)			
				return ps3mapi_partial_disable_syscall8;			
			else return ENOSYS;
		}
		else 
			return ENOSYS;
	}

	if ((function != SYSCALL8_OPCODE_PS3MAPI) && (2 <= ps3mapi_partial_disable_syscall8)) 
		return ENOSYS;

	// -- AV: disable cobra without reboot (use lv1 peek)
	if(disable_cobra) 
		return 0;

	switch (function)
	{
		case SYSCALL8_OPCODE_PS3MAPI:
			switch ((int)param1)
			{
				DPRINTF("syscall8: PS3M_API function 0x%x\n", (int)param1);

				//----------
				//CORE
				//----------
				case PS3MAPI_OPCODE_GET_CORE_VERSION:
					return PS3MAPI_CORE_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_CORE_MINVERSION:
					return PS3MAPI_CORE_MINVERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_VERSION:
					return PS3MAPI_FW_VERSION;
				break;
				case PS3MAPI_OPCODE_GET_FW_TYPE:
					return ps3mapi_get_fw_type((char *)param2);
				break;

				//----------------
				//PEEK & POKE (av)
				//----------------
				case PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE:
					return PS3MAPI_OPCODE_SUPPORT_SC8_PEEK_POKE_OK;
				break;
				case PS3MAPI_OPCODE_LV1_PEEK:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV1_POKE:
					return 0;
				break;
				case PS3MAPI_OPCODE_LV2_PEEK:
					return *(uint64_t *)param2;
				break;
				case PS3MAPI_OPCODE_LV2_POKE:
					if(param2>MKA(hash_checked_area))
					{
						*(uint64_t *)param2=param3;
					}
					return 0;
				break;

				//----------
				//SECURITY
				//----------
				case PS3MAPI_OPCODE_SET_ACCESS_KEY:
					ps3mapi_key = param2;
					ps3mapi_access_granted = 0;
					ps3mapi_access_tries = 0;
					return 0;

				//----------
				//PROCESS
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_PID:
					return ps3mapi_get_all_processes_pid((process_id_t *)param2);
				break;
				case PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID:
					return ps3mapi_get_process_name_by_pid((process_id_t)param2, (char *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_BY_PID:
					return ps3mapi_get_process_by_pid((process_id_t)param2, (process_t)param3);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC:
					return ps3mapi_get_current_process((process_t)param2);
				break;
				case PS3MAPI_OPCODE_GET_CURRENT_PROC_CRIT:
					return ps3mapi_get_current_process_critical((process_t)param2);
				break;
				//----------
				//MEMORY
				//----------
				case PS3MAPI_OPCODE_GET_PROC_MEM:
					return ps3mapi_get_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				case PS3MAPI_OPCODE_SET_PROC_MEM:
					return ps3mapi_set_process_mem((process_id_t)param2, param3, (char *)param4, (int)param5);
				break;
				//----------
				//MODULE
				//----------
				case PS3MAPI_OPCODE_GET_ALL_PROC_MODULE_PID:
					return ps3mapi_get_all_process_modules_prx_id((process_id_t)param2, (sys_prx_id_t *)param3);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_NAME:
					return ps3mapi_get_process_module_name_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_GET_PROC_MODULE_FILENAME:
					return ps3mapi_get_process_module_filename_by_prx_id((process_id_t)param2, (sys_prx_id_t)param3, (char *)param4);
				break;
				case PS3MAPI_OPCODE_LOAD_PROC_MODULE:
					return ps3mapi_load_process_modules((process_id_t)param2, (char *)param3, (void *)param4, (uint32_t)param5);
				break;
				case PS3MAPI_OPCODE_UNLOAD_PROC_MODULE:
					return ps3mapi_unload_process_modules((process_id_t)param2, (sys_prx_id_t)param3);
				break;
				//----------
				//VSH PLUGINS
				//----------
				case PS3MAPI_OPCODE_UNLOAD_VSH_PLUGIN:
					return ps3mapi_unload_vsh_plugin((char *)param2);
				break;
				case PS3MAPI_OPCODE_GET_VSH_PLUGIN_INFO:
					return ps3mapi_get_vsh_plugin_info((unsigned int)param2, (char *)param3, (char *)param4);
				break;
				//----------
				//SYSCALL
				//----------
				case PS3MAPI_OPCODE_DISABLE_SYSCALL:
					return ps3mapi_disable_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_CHECK_SYSCALL:
					return ps3mapi_check_syscall((int)param2);
				break;
				case PS3MAPI_OPCODE_PDISABLE_SYSCALL8:
					ps3mapi_partial_disable_syscall8 = (int)param2;
					return SUCCEEDED;
				break;
				case PS3MAPI_OPCODE_PCHECK_SYSCALL8:
					return ps3mapi_partial_disable_syscall8;
				break;
				//----------
				//REMOVE HOOK
				//----------
				case PS3MAPI_OPCODE_REMOVE_HOOK:
					ps3mapi_unhook_all(); //REMOVE ALL "MAMBA/COBRA" HOOK
					return SUCCEEDED;
				break;
				//-----------
				//PSID/IDPS
				//-----------
				case PS3MAPI_OPCODE_GET_IDPS:
					return ps3mapi_get_idps((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_IDPS:
					return ps3mapi_set_idps(param2, param3);
				break;
				case PS3MAPI_OPCODE_GET_PSID:
					return ps3mapi_get_psid((uint64_t *)param2);
				break;
				case PS3MAPI_OPCODE_SET_PSID:
					return ps3mapi_set_psid(param2, param3);
				break;
				//----------
				//MISC
				//----------
				case PS3MAPI_OPCODE_PHOTO_GUI:
					photo_gui = (uint8_t)param2;
					return photo_gui;
				break;
				//----------
				//DEFAULT
				//----------
				default:
					DPRINTF("syscall8: Unsupported PS3M_API opcode: 0x%lx\n", function);
					return ENOSYS;
				break;
			}
		break;

		case SYSCALL8_OPCODE_STEALTH_TEST:  //KW PSNPatch stealth extension compatibility
			return SYSCALL8_STEALTH_OK;
		break;

		case SYSCALL8_OPCODE_STEALTH_ACTIVATE: //KW PSNPatch stealth extension compatibility
		{
				uint64_t syscall_not_impl = *(uint64_t *)MKA(syscall_table_symbol);
				//*(uint64_t *)MKA(syscall_table_symbol+ 8* 8) = syscall_not_impl;
				ps3mapi_partial_disable_syscall8 = 2; //NzV Edit: Keep PS3M_API Features only.
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 9) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 10) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 11) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 15) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 35) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 36) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 38) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 6) = syscall_not_impl;
				*(uint64_t *)MKA(syscall_table_symbol + 8 * 7) = syscall_not_impl;
			return SYSCALL8_STEALTH_OK;
		}
		break;

		case SYSCALL8_OPCODE_GET_VERSION:
			return sys_get_version((uint32_t *)param1);
		break;
		
		case SYSCALL8_OPCODE_GET_VERSION2:
			return sys_get_version2((uint16_t *)param1);
		break;

		case SYSCALL8_OPCODE_GET_DISC_TYPE:
			return sys_storage_ext_get_disc_type((unsigned int *)param1, (unsigned int *)param2, (unsigned int *)param3);
		break;

		case SYSCALL8_OPCODE_READ_PS3_DISC:
			return sys_storage_ext_read_ps3_disc((void *)param1, param2, (uint32_t)param3);
		break;

		case SYSCALL8_OPCODE_FAKE_STORAGE_EVENT:
			return sys_storage_ext_fake_storage_event(param1, param2, param3);
		break;

		case SYSCALL8_OPCODE_GET_EMU_STATE:
			return sys_storage_ext_get_emu_state((sys_emu_state_t *)param1);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS3_DISCFILE:
			return sys_storage_ext_mount_ps3_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_DVD_DISCFILE:
			return sys_storage_ext_mount_dvd_discfile(param1, (char **)param2);
		break;

		case SYSCALL8_OPCODE_MOUNT_BD_DISCFILE:
			return sys_storage_ext_mount_bd_discfile(param1, (char **)param2);
		break;
		
		case SYSCALL8_OPCODE_RUN_PAYLOAD:
			return inst_and_run_kernel((uint8_t *)param1, param2);
		break;

		case SYSCALL8_OPCODE_RUN_PAYLOAD_DYNAMIC:
			return inst_and_run_kernel_dynamic((uint8_t *)param1, param2, (uint64_t *)param3);
		break;

		case SYSCALL8_OPCODE_UNLOAD_PAYLOAD_DYNAMIC:
			return unload_plugin_kernel(param1);
		break;

		case SYSCALL8_OPCODE_MOUNT_PSX_DISCFILE:
			return sys_storage_ext_mount_psx_discfile((char *)param1, param2, (ScsiTrackDescriptor *)param3);
		break;

		case SYSCALL8_OPCODE_MOUNT_PS2_DISCFILE:
			return sys_storage_ext_mount_ps2_discfile(param1, (char **)param2, param3, (ScsiTrackDescriptor *)param4);
		break;

		case SYSCALL8_OPCODE_MOUNT_DISCFILE_PROXY:
			return sys_storage_ext_mount_discfile_proxy(param1, param2, param3, param4, param5, param6, (ScsiTrackDescriptor *)param7);
		break;

		case SYSCALL8_OPCODE_UMOUNT_DISCFILE:
			return sys_storage_ext_umount_discfile();
		break;

		case SYSCALL8_OPCODE_GET_ACCESS:
		case SYSCALL8_OPCODE_REMOVE_ACCESS:
			return 0;//needed for mmCM

		case SYSCALL8_OPCODE_READ_COBRA_CONFIG:
			return sys_read_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_WRITE_COBRA_CONFIG:
			return sys_write_cobra_config((CobraConfig *)param1);
		break;

		case SYSCALL8_OPCODE_COBRA_USB_COMMAND:
			//return sys_cobra_usb_command(param1, param2, param3, (void *)param4, param5);
			return 0;
		break;
		
		case SYSCALL8_OPCODE_SET_PSP_UMDFILE:
			return sys_psp_set_umdfile((char *)param1, (char *)param2, param3);
		break;

		case SYSCALL8_OPCODE_SET_PSP_DECRYPT_OPTIONS:
			return sys_psp_set_decrypt_options(param1, param2, (uint8_t *)param3, param4, param5, (uint8_t *)param6, param7);
		break;

		case SYSCALL8_OPCODE_READ_PSP_HEADER:
			return sys_psp_read_header(param1, (char *)param2, param3, (uint64_t *)param4);
		break;

		case SYSCALL8_OPCODE_READ_PSP_UMD:
			return sys_psp_read_umd(param1, (void *)param2, param3, param4, param5);
		break;

		case SYSCALL8_OPCODE_PSP_PRX_PATCH:
			return sys_psp_prx_patch((uint32_t *)param1, (uint8_t *)param2, (void *)param3);
		break;

		case SYSCALL8_OPCODE_PSP_CHANGE_EMU:
			return sys_psp_set_emu_path((char *)param1);
		break;

		case SYSCALL8_OPCODE_PSP_POST_SAVEDATA_INITSTART:
			return sys_psp_post_savedata_initstart(param1, (void *)param2);
		break;

		case SYSCALL8_OPCODE_PSP_POST_SAVEDATA_SHUTDOWNSTART:
			return sys_psp_post_savedata_shutdownstart();
		break;

		case SYSCALL8_OPCODE_AIO_COPY_ROOT:
			return sys_aio_copy_root((char *)param1, (char *)param2);
		break;

		case SYSCALL8_OPCODE_MAP_PATHS:
			return sys_map_paths((char **)param1, (char **)param2, param3);
		break;

		case SYSCALL8_OPCODE_VSH_SPOOF_VERSION:
			return ENOSYS; //sys_vsh_spoof_version((char *)param1);
		break;

		case SYSCALL8_OPCODE_LOAD_VSH_PLUGIN:
			return sys_prx_load_vsh_plugin(param1, (char *)param2, (void *)param3, param4);
		break;

		case SYSCALL8_OPCODE_UNLOAD_VSH_PLUGIN:
			return sys_prx_unload_vsh_plugin(param1);
		break;

		case SYSCALL8_OPCODE_SEND_POWEROFF_EVENT:
			return sys_sm_ext_send_poweroff_event((int)param1);
		break;
		
		case SYSCALL8_OPCODE_GET_MAP_PATH:
			return get_map_path((unsigned int)param1, (char *)param2, (char *)param3);
		break;

#ifdef DEBUG
		case SYSCALL8_OPCODE_DUMP_STACK_TRACE:
			dump_stack_trace3((void *)param1, 16);
			return 0;
		break;
#endif		
		case SYSCALL8_OPCODE_POKE_LV2:
			*(uint64_t *)param1=param2;
			return 0;
		break;
		
		case SYSCALL8_OPCODE_IS_HEN:
			return 0x1337;
		break;
		
		case SYSCALL8_OPCODE_HEN_REV:
			return HEN_REV;
		break;

		/*case SYSCALL8_OPCODE_GENERIC_DEBUG:
			return sys_generic_debug(param1, (uint32_t *)param2, (void *)param3);
		break;*/


		default:
			if (1 <= ps3mapi_partial_disable_syscall8)	return ENOSYS;

	}

	DPRINTF("Unsupported syscall8 opcode: 0x%lx\n", function);
	return ENOSYS;
}

LV2_SYSCALL2(int, sm_set_fan_policy_sc,(uint8_t arg1, uint8_t arg2, uint8_t arg3))
{
	f_desc_t f;
	f.addr=(void*)MKA(sm_set_fan_policy);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t,uint8_t)=(void*)&f;
	return func(MKA(sysmem_obj),arg1,arg2,arg3);
}

LV2_SYSCALL2(int, sm_get_fan_policy_sc,(uint8_t id, uint8_t *st, uint8_t *policy, uint8_t * mode, uint8_t *duty))
{
	uint8_t st1;
	uint8_t policy1;
	uint8_t mode1;
	uint8_t duty1;
	f_desc_t f;
	f.addr=(void*)MKA(sm_get_fan_policy);
	f.toc=(void *)MKA(TOC);
	int(*func)(uint64_t, uint8_t,uint8_t *,uint8_t *,uint8_t *,uint8_t *,uint64_t)=(void*)&f;
	int ret=func(MKA(sysmem_obj),id,&st1,&policy1,&mode1,&duty1,10000000);
	if(ret==0)
	{
		ret=copy_to_user(&st1, st,1);
		if(ret==0)
		{
			ret=copy_to_user(&policy1, policy,1);
			if(ret==0)
			{
				ret=copy_to_user(&mode1, mode,1);
				if(ret==0)
				{
					ret=copy_to_user(&duty1, duty,1);
				}
			}
		}		
	}
	return ret;
}

static INLINE void apply_kernel_patches(void)
{
    /// Adding HEN patches on init for stability /// -- START
		#if defined (FIRMWARE_4_82DEX) ||  defined (FIRMWARE_4_84DEX)
		do_patch(MKA(vsh_patch),0x386000014E800020);
		#endif
	//do_patch32(MKA(patch_data1_offset), 0x01000000);	
	do_patch32(MKA(module_sdk_version_patch_offset), NOP);
	do_patch32(MKA(patch_func8_offset1),0x38600000); 
	do_patch32(MKA(patch_func8_offset2 ),0x60000000);
	do_patch32(MKA(user_thread_prio_patch),0x60000000);
	do_patch32(MKA(user_thread_prio_patch2),0x60000000);
	do_patch32(MKA(ECDSA_1),0x38600000);
	do_patch32(MKA(lic_patch),0x38600001); // ignore LIC.DAT check	
	do_patch32(MKA(patch_func9_offset),0x60000000);
	do_patch32(MKA(fix_80010009),0x60000000);
	do_patch(MKA(ode_patch),0x38600000F8690000);  // fix 0x8001002B / 80010017 errors  known as ODE patch
	do_patch(MKA(ECDSA_2),0x386000004e800020);
	do_patch(MKA(mem_base2),0x386000014e800020); // psjailbreak, PL3, etc destroy this function to copy their code there.
	do_patch(MKA(fix_8001003D),0x63FF003D60000000);
	do_patch(MKA(fix_8001003E),0x3FE080013BE00000);
	do_patch(MKA(PATCH_JUMP),0x2F84000448000098);
	*(uint64_t *)MKA(ECDSA_FLAG)=0;
	/// Adding HEN patches on init for stability ///	 -- END
	hook_function_with_precall(get_syscall_address(801),sys_fs_open,6);
	hook_function_with_cond_postcall(get_syscall_address(724),bnet_ioctl,3);
	hook_function_with_precall(get_syscall_address(802),sys_fs_read,4);
	#if defined (FIRMWARE_4_82) || defined (FIRMWARE_4_84) || defined (FIRMWARE_4_85)
	hook_function_with_cond_postcall(um_if_get_token_symbol,um_if_get_token,5);
	hook_function_with_cond_postcall(update_mgr_read_eeprom_symbol,read_eeprom_by_offset,3);
	#endif
	create_syscall2(8, syscall8);
	create_syscall2(6, sys_cfw_peek);
	create_syscall2(7, sys_cfw_poke);
//	create_syscall2(9, sys_cfw_lv1_poke);
	create_syscall2(10, sys_cfw_lv1_call);
//	create_syscall2(11, sys_cfw_lv1_peek);
	create_syscall2(15, sys_cfw_lv2_func);
	create_syscall2(389, sm_set_fan_policy_sc);
	create_syscall2(409, sm_get_fan_policy_sc);
}

/*void enable_ingame_screenshot()
{
	f_desc_t f;
	f.addr=(void*)0x19531c;
	f.toc=(void*)0x6F5558;
	int(*set_SSHT)(int)=(void*)&f;
	set_SSHT(1);
}*/

int main(void)
{
#ifdef DEBUG
	debug_init();
	debug_install();
	extern uint64_t _start;
	extern uint64_t __self_end;
	DPRINTF("PS3HEN loaded (load base = %p, end = %p) (version = %08X)\n", &_start, &__self_end, MAKE_VERSION(COBRA_VERSION, FIRMWARE_VERSION, IS_CFW));
#endif

//	poke_count=0;

			ecdsa_set_curve();
			ecdsa_set_pub();
			ecdsa_set_priv();
			cellFsUnlink("/dev_hdd0/hen/hfw_settings.xml"); // to avoid conflict for remap fix
	map_first_slot("/dev_hdd0/hen/hfw_settings.xml","/dev_hdd0/hen/xml/hfw_settings.xml");
	storage_ext_init();
	modules_patch_init();
//	drm_init();

	apply_kernel_patches();
	map_path_patches(1);
	storage_ext_patches();
	region_patches();
//	permissions_patches();
	init_mount_hdd0();
	do_hook_all_syscalls();
	memset((void *)MKA(0x7e0000),0,0x100);
	memset((void *)MKA(0x7f0000),0,0x1000);
	load_boot_plugins();
	load_boot_plugins_kernel();
//	enable_ingame_screenshot();
	
#ifdef DEBUG
	// "Laboratory"
	//do_dump_threads_info_test();
	//do_dump_processes_test();
	//do_hook_load_module();
	//do_hook_mutex_create();
	//do_ps2net_copy_test();
	//do_dump_modules_info_test();
	//do_pad_test();
#endif

	return 0;
}
