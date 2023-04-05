#if defined(FIRMWARE_4_82DEX) || defined (FIRMWARE_4_84DEX)
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
#endif
