#ifndef HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5_inl_h
#define HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5_inl_h

#ifndef HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5_h
#error "Do not include this directly; use fd_r52x5.h"
#endif

FD_PROTOTYPES_BEGIN

/* FD_R52X5_QUAD_DECL(Q) declars the wl_t Q0, Q1, Q2, Q3, Q4 in the
   local scope to represent X, Y, Z and T.

    Q0 = [ X0, Y0, Z0, T0 ]
    Q1 = [ X1, Y1, Z1, T1 ]
    Q2 = [ X2, Y2, Z2, T2 ]
    Q3 = [ X3, Y3, Z3, T3 ]
    Q4 = [ X4, Y4, Z4, T4 ]

   where Xi is the i-th limb of X. */
#define FD_R52X5_QUAD_DECL( Q ) wl_t Q##0, Q##1, Q##2, Q##3, Q##4

#define FD_R52X5_QUAD_MOV(D, S) do {                                           \
    D##0 = S##0;                                                               \
    D##1 = S##1;                                                               \
    D##2 = S##2;                                                               \
    D##3 = S##3;                                                               \
    D##4 = S##4;                                                               \
  } while (0)

/* TODO: comment */
#define fd_r43x6_to_r52x5(r, a) do {                                           \
    long _a0 = (a##0);                                                         \
    long _a1 = (a##1);                                                         \
    long _a2 = (a##2);                                                         \
    long _a3 = (a##3);                                                         \
    long _a4 = (a##4);                                                         \
    long _a5 = (a##5);                                                         \
    r##0 = (_a0 >> 0 ) | ((_a1 & ((1LL << 9 ) - 1)) << 43);                    \
    r##1 = (_a1 >> 9 ) | ((_a2 & ((1LL << 18) - 1)) << 34);                    \
    r##2 = (_a2 >> 18) | ((_a3 & ((1LL << 27) - 1)) << 25);                    \
    r##3 = (_a3 >> 27) | ((_a4 & ((1LL << 36) - 1)) << 16);                    \
    r##4 = (_a4 >> 36) | ((_a5 << 7));                                         \
  } while (0)

/* TODO: comment */
#define fd_r52x5_to_r43x6(r, a) do {                                           \
    long _a0 = (a##0);                                                         \
    long _a1 = (a##1);                                                         \
    long _a2 = (a##2);                                                         \
    long _a3 = (a##3);                                                         \
    long _a4 = (a##4);                                                         \
    r##0 = (_a0) & ((1LL << 43) - 1);                                          \
    r##1 = (_a0 >> 43) | ((_a1 & ((1LL << 34) - 1)) << 9 );                    \
    r##2 = (_a1 >> 34) | ((_a2 & ((1LL << 25) - 1)) << 18);                    \
    r##3 = (_a2 >> 25) | ((_a3 & ((1LL << 16) - 1)) << 27);                    \
    r##4 = (_a3 >> 16) | ((_a4 & ((1LL << 7 ) - 1)) << 36);                    \
    r##5 = (_a4 >> 7);                                                         \
  } while (0)

/* Packs 4 r43x6_t's into the r52x5 quad representation. */
#define FD_R52X5_QUAD_PACK( Q, x,y,z,t ) do {                                  \
    /* TODO: unpack with SIMD */                                               \
    wwl_t _r0 = (x);                                                           \
    wwl_t _r1 = (y);                                                           \
    wwl_t _r2 = (z);                                                           \
    wwl_t _r3 = (t);                                                           \
                                                                               \
    long _x0, _x1, _x2, _x3, _x4, _x5;                                         \
    long _y0, _y1, _y2, _y3, _y4, _y5;                                         \
    long _z0, _z1, _z2, _z3, _z4, _z5;                                         \
    long _t0, _t1, _t2, _t3, _t4, _t5;                                         \
    fd_r43x6_extract_limbs( _r0, _x );                                         \
    fd_r43x6_extract_limbs( _r1, _y );                                         \
    fd_r43x6_extract_limbs( _r2, _z );                                         \
    fd_r43x6_extract_limbs( _r3, _t );                                         \
                                                                               \
    fd_r43x6_to_r52x5( _x, _x );                                               \
    fd_r43x6_to_r52x5( _y, _y );                                               \
    fd_r43x6_to_r52x5( _z, _z );                                               \
    fd_r43x6_to_r52x5( _t, _t );                                               \
                                                                               \
    Q##0 = wl( _x0, _y0, _z0, _t0 );                                           \
    Q##1 = wl( _x1, _y1, _z1, _t1 );                                           \
    Q##2 = wl( _x2, _y2, _z2, _t2 );                                           \
    Q##3 = wl( _x3, _y3, _z3, _t3 );                                           \
    Q##4 = wl( _x4, _y4, _z4, _t4 );                                           \
  } while (0)

#define FD_R52X5_QUAD_UNPACK( x,y,z,t, Q ) do {                                \
    wl_t _r0 = Q##0;                                                           \
    wl_t _r1 = Q##1;                                                           \
    wl_t _r2 = Q##2;                                                           \
    wl_t _r3 = Q##3;                                                           \
    wl_t _r4 = Q##4;                                                           \
                                                                               \
    long _x0, _x1, _x2, _x3, _x4, _x5;                                         \
    long _y0, _y1, _y2, _y3, _y4, _y5;                                         \
    long _z0, _z1, _z2, _z3, _z4, _z5;                                         \
    long _t0, _t1, _t2, _t3, _t4, _t5;                                         \
    wl_unpack( _r0, _x0, _y0, _z0, _t0 );                                      \
    wl_unpack( _r1, _x1, _y1, _z1, _t1 );                                      \
    wl_unpack( _r2, _x2, _y2, _z2, _t2 );                                      \
    wl_unpack( _r3, _x3, _y3, _z3, _t3 );                                      \
    wl_unpack( _r4, _x4, _y4, _z4, _t4 );                                      \
                                                                               \
    fd_r52x5_to_r43x6(_x, _x);                                                 \
    fd_r52x5_to_r43x6(_y, _y);                                                 \
    fd_r52x5_to_r43x6(_z, _z);                                                 \
    fd_r52x5_to_r43x6(_t, _t);                                                 \
                                                                               \
    (x) = fd_r43x6( _x0, _x1, _x2, _x3, _x4, _x5 );                            \
    (y) = fd_r43x6( _y0, _y1, _y2, _y3, _y4, _y5 );                            \
    (z) = fd_r43x6( _z0, _z1, _z2, _z3, _z4, _z5 );                            \
    (t) = fd_r43x6( _t0, _t1, _t2, _t3, _t4, _t5 );                            \
  } while (0)

/* D = [ S(imm0) S(imm1) S(imm2) S(imm) ]
  where imm* are in [0, 3], mapping (0,1,2,3 -> X,Y,Z,T) */

#define FD_R52X5_QUAD_PERMUTE( D, imm0,imm1,imm2,imm3, S ) do { \
  D##0 = wl_permute( S##0, imm0,imm1,imm2,imm3 );               \
  D##1 = wl_permute( S##1, imm0,imm1,imm2,imm3 );               \
  D##2 = wl_permute( S##2, imm0,imm1,imm2,imm3 );               \
  D##3 = wl_permute( S##3, imm0,imm1,imm2,imm3 );               \
  D##4 = wl_permute( S##4, imm0,imm1,imm2,imm3 );               \
  } while (0)

/* D = [ imm0 ? SX : TX, imm1 ? SY : TY, imm2 ? SZ : TZ, imm3 ? ST : TT ] 
  imm* should be in [0,1]. */
#define FD_R52X5_QUAD_LANE_IF( D, imm0,imm1,imm2,imm3, S, T ) do { \
  wc_t c = wc_bcast_wide( imm0, imm1, imm2, imm3 );                \
  D##0 = wl_if( c, S##0, T##0 );                                   \
  D##1 = wl_if( c, S##1, T##1 );                                   \
  D##2 = wl_if( c, S##2, T##2 );                                   \
  D##3 = wl_if( c, S##3, T##3 );                                   \
  D##4 = wl_if( c, S##4, T##4 );                                   \
  } while (0)

#define FD_R52X5_QUAD_DIFF_SUM( D, S ) do { \
  FD_R52X5_QUAD_DECL( _tmp1 ); \
  FD_R52X5_QUAD_DECL( _tmp2 ); \
  FD_R52X5_QUAD_PERMUTE( _tmp1, 1,0,3,2, S ); \
  FD_R52X5_QUAD_NEGATE_LAZY( _tmp2, S ); \
  FD_R52X5_QUAD_LANE_IF( _tmp2, 1,0,1,0, _tmp2, S ); \
  FD_R52X5_QUAD_ADD_FAST( D, _tmp1, _tmp2 ); \
} while (0)

#define FD_R52X5_QUAD_REDUCE(D, S) do {                                         \
    wl_t _s0 = S##0;                                                           \
    wl_t _s1 = S##1;                                                           \
    wl_t _s2 = S##2;                                                           \
    wl_t _s3 = S##3;                                                           \
    wl_t _s4 = S##4;                                                           \
                                                                               \
    wl_t _mask = wl_bcast((1LL << 51) - 1);                                    \
    wl_t _r19 = wl_bcast(19);                                                  \
                                                                               \
    wl_t _c0 = wl_shru(_s0, 51);                                               \
    wl_t _c1 = wl_shru(_s1, 51);                                               \
    wl_t _c2 = wl_shru(_s2, 51);                                               \
    wl_t _c3 = wl_shru(_s3, 51);                                               \
    wl_t _c4 = wl_shru(_s4, 51);                                               \
                                                                               \
    D##0 = wl_madd52lo(wl_and(_s0, _mask), _c4, _r19);                         \
    D##1 = wl_add(wl_and(_s1, _mask), _c0);                                    \
    D##2 = wl_add(wl_and(_s2, _mask), _c1);                                    \
    D##3 = wl_add(wl_and(_s3, _mask), _c2);                                    \
    D##4 = wl_add(wl_and(_s4, _mask), _c3);                                    \
  } while (0)

#define FD_R52X5_QUAD_MUL_CONSTANT(D, S, y) do {                               \
    wl_t _s0 = S##0;                                                           \
    wl_t _s1 = S##1;                                                           \
    wl_t _s2 = S##2;                                                           \
    wl_t _s3 = S##3;                                                           \
    wl_t _s4 = S##4;                                                           \
    wl_t _y = (y);                                                             \
                                                                               \
    wl_t _r19 = wl_bcast(19);                                                  \
    wl_t _z0_1 = wl_zero();                                                    \
    wl_t _z1_1 = wl_zero();                                                    \
    wl_t _z2_1 = wl_zero();                                                    \
    wl_t _z3_1 = wl_zero();                                                    \
    wl_t _z4_1 = wl_zero();                                                    \
    wl_t _z1_2 = wl_zero();                                                    \
    wl_t _z2_2 = wl_zero();                                                    \
    wl_t _z3_2 = wl_zero();                                                    \
    wl_t _z4_2 = wl_zero();                                                    \
    wl_t _z5_2 = wl_zero();                                                    \
                                                                               \
    /* Wave 1 */                                                               \
    _z4_2 = wl_madd52hi(_z4_2, _y, _s3);                                       \
    _z5_2 = wl_madd52hi(_z5_2, _y, _s4);                                       \
    _z4_1 = wl_madd52lo(_z4_1, _y, _s4);                                       \
    _z0_1 = wl_madd52lo(_z0_1, _y, _s0);                                       \
    _z3_1 = wl_madd52lo(_z3_1, _y, _s3);                                       \
    _z2_1 = wl_madd52lo(_z2_1, _y, _s2);                                       \
    _z1_1 = wl_madd52lo(_z1_1, _y, _s1);                                       \
    _z3_2 = wl_madd52hi(_z3_2, _y, _s2);                                       \
                                                                               \
    /* Wave 2 */                                                               \
    _z2_2 = wl_madd52hi(_z2_2, _y, _s1);                                       \
    _z1_2 = wl_madd52hi(_z1_2, _y, _s0);                                       \
    _z0_1 = wl_madd52lo(_z0_1, wl_add(_z5_2, _z5_2), _r19);                    \
                                                                               \
    D##0 = _z0_1;                                                              \
    D##1 = wl_add(wl_add(_z1_1, _z1_2), _z1_2);                                \
    D##2 = wl_add(wl_add(_z2_1, _z2_2), _z2_2);                                \
    D##3 = wl_add(wl_add(_z3_1, _z3_2), _z3_2);                                \
    D##4 = wl_add(wl_add(_z4_1, _z4_2), _z4_2);                                \
  } while (0)

#define FD_R52X5_QUAD_NEGATE_LAZY( D, S ) do { \
  wl_t lo = wl_bcast( 0x7FFFFFFFFFFED0 ); \
  wl_t hi = wl_bcast( 0x7FFFFFFFFFFFF0 ); \
  D##0 = wl_sub( lo, S##0 );              \
  D##1 = wl_sub( hi, S##1 );              \
  D##2 = wl_sub( hi, S##2 );              \
  D##3 = wl_sub( hi, S##3 );              \
  D##4 = wl_sub( hi, S##4 );              \
  } while (0)

#define FD_R52X5_QUAD_ADD_FAST( R, P, Q ) do { \
  R##0 = wl_add( P##0, Q##0 ); \
  R##1 = wl_add( P##1, Q##1 ); \
  R##2 = wl_add( P##2, Q##2 ); \
  R##3 = wl_add( P##3, Q##3 ); \
  R##4 = wl_add( P##4, Q##4 ); \
  } while (0)

#define FD_R52X5_QUAD_MUL_FAST(R, P, Q) do {                                   \
    FD_R52X5_QUAD_DECL(_R);                                                    \
    fd_r52x5_quad_mul_fast(&_R0, &_R1, &_R2, &_R3, &_R4,                       \
                             P##0, P##1, P##2, P##3, P##4,                     \
                             Q##0, Q##1, Q##2, Q##3, Q##4);                    \
    FD_R52X5_QUAD_MOV(R, _R);                                                  \
  } while (0)

  
FD_FN_UNUSED static void /* let compiler decide if worth inlining */
fd_r52x5_quad_mul_fast( wl_t * _z0, wl_t * _z1, wl_t * _z2, wl_t * _z3, wl_t * _z4, 
                        wl_t    x0, wl_t    x1, wl_t    x2, wl_t    x3, wl_t    x4,
                        wl_t    y0, wl_t    y1, wl_t    y2, wl_t    y3, wl_t    y4 ) {
  wl_t z0_1 = wl_zero();
  wl_t z1_1 = wl_zero();
  wl_t z2_1 = wl_zero();
  wl_t z3_1 = wl_zero();
  wl_t z4_1 = wl_zero();
  wl_t z5_1 = wl_zero();
  wl_t z6_1 = wl_zero();
  wl_t z7_1 = wl_zero();
  wl_t z8_1 = wl_zero();

  wl_t z0_2 = wl_zero();
  wl_t z1_2 = wl_zero();
  wl_t z2_2 = wl_zero();
  wl_t z3_2 = wl_zero();
  wl_t z4_2 = wl_zero();
  wl_t z5_2 = wl_zero();
  wl_t z6_2 = wl_zero();
  wl_t z7_2 = wl_zero();
  wl_t z8_2 = wl_zero();
  wl_t z9_2 = wl_zero();

  /* Wave 0 */
  z4_1 = wl_madd52lo(z4_1, x2, y2);
  z5_2 = wl_madd52hi(z5_2, x2, y2);
  z5_1 = wl_madd52lo(z5_1, x4, y1);
  z6_2 = wl_madd52hi(z6_2, x4, y1);
  z6_1 = wl_madd52lo(z6_1, x4, y2);
  z7_2 = wl_madd52hi(z7_2, x4, y2);
  z7_1 = wl_madd52lo(z7_1, x4, y3);
  z8_2 = wl_madd52hi(z8_2, x4, y3);

  /* Wave 1 */
  z4_1 = wl_madd52lo(z4_1, x3, y1);
  z5_2 = wl_madd52hi(z5_2, x3, y1);
  z5_1 = wl_madd52lo(z5_1, x3, y2);
  z6_2 = wl_madd52hi(z6_2, x3, y2);
  z6_1 = wl_madd52lo(z6_1, x3, y3);
  z7_2 = wl_madd52hi(z7_2, x3, y3);
  z7_1 = wl_madd52lo(z7_1, x3, y4);
  z8_2 = wl_madd52hi(z8_2, x3, y4);

  /* Wave 2 */
  z8_1 = wl_madd52lo(z8_1, x4, y4);
  z9_2 = wl_madd52hi(z9_2, x4, y4);
  z4_1 = wl_madd52lo(z4_1, x4, y0);
  z5_2 = wl_madd52hi(z5_2, x4, y0);
  z5_1 = wl_madd52lo(z5_1, x2, y3);
  z6_2 = wl_madd52hi(z6_2, x2, y3);
  z6_1 = wl_madd52lo(z6_1, x2, y4);
  z7_2 = wl_madd52hi(z7_2, x2, y4);

  wl_t z8 = wl_add(wl_add(z8_1, z8_2), z8_2);
  wl_t z9 = wl_add(z9_2, z9_2);

  /* Wave 3 */
  z3_1 = wl_madd52lo(z3_1, x3, y0);
  z4_2 = wl_madd52hi(z4_2, x3, y0);
  z4_1 = wl_madd52lo(z4_1, x1, y3);
  z5_2 = wl_madd52hi(z5_2, x1, y3);
  z5_1 = wl_madd52lo(z5_1, x1, y4);
  z6_2 = wl_madd52hi(z6_2, x1, y4);
  z2_1 = wl_madd52lo(z2_1, x2, y0);
  z3_2 = wl_madd52hi(z3_2, x2, y0);

  wl_t z6 = wl_add(wl_add(z6_1, z6_2), z6_2);
  wl_t z7 = wl_add(wl_add(z7_1, z7_2), z7_2);

  /* Wave 4 */
  z3_1 = wl_madd52lo(z3_1, x2, y1);
  z4_2 = wl_madd52hi(z4_2, x2, y1);
  z4_1 = wl_madd52lo(z4_1, x0, y4);
  z5_2 = wl_madd52hi(z5_2, x0, y4);
  z1_1 = wl_madd52lo(z1_1, x1, y0);
  z2_2 = wl_madd52hi(z2_2, x1, y0);
  z2_1 = wl_madd52lo(z2_1, x1, y1);
  z3_2 = wl_madd52hi(z3_2, x1, y1);

  wl_t z5 = wl_add(wl_add(z5_1, z5_2), z5_2);

  /* Wave 5 */
  z3_1 = wl_madd52lo(z3_1, x1, y2);
  z4_2 = wl_madd52hi(z4_2, x1, y2);
  z0_1 = wl_madd52lo(z0_1, x0, y0);
  z1_2 = wl_madd52hi(z1_2, x0, y0);
  z1_1 = wl_madd52lo(z1_1, x0, y1);
  z2_1 = wl_madd52lo(z2_1, x0, y2);
  z2_2 = wl_madd52hi(z2_2, x0, y1);
  z3_2 = wl_madd52hi(z3_2, x0, y2);

  wl_t t0 = wl_zero();
  wl_t t1 = wl_zero();
  wl_t r19 = wl_bcast(19);

  /* Wave 6 */
  t0 = wl_madd52hi(t0, r19, z9);
  t1 = wl_madd52lo(t1, r19, wl_shru(z9, 52));
  z3_1 = wl_madd52lo(z3_1, x0, y3);
  z4_2 = wl_madd52hi(z4_2, x0, y3);
  z1_2 = wl_madd52lo(z1_2, r19, wl_shru(z5, 52));
  z2_2 = wl_madd52lo(z2_2, r19, wl_shru(z6, 52));
  z3_2 = wl_madd52lo(z3_2, r19, wl_shru(z7, 52));
  z0_1 = wl_madd52lo(z0_1, r19, z5);

  /* Wave 7 */
  z4_1 = wl_madd52lo(z4_1, r19, z9);
  z1_1 = wl_madd52lo(z1_1, r19, z6);
  z0_2 = wl_madd52lo(z0_2, r19, t0 + t1);
  z4_2 = wl_madd52hi(z4_2, r19, z8);
  z2_1 = wl_madd52lo(z2_1, r19, z7);
  z1_2 = wl_madd52hi(z1_2, r19, z5);
  z2_2 = wl_madd52hi(z2_2, r19, z6);
  z3_2 = wl_madd52hi(z3_2, r19, z7);

  /* Wave 8 */
  z3_1 = wl_madd52lo(z3_1, r19, z8);
  z4_2 = wl_madd52lo(z4_2, r19, wl_shru(z8, 52));

  wl_t r0 = wl_add(wl_add(z0_1, z0_2), z0_2);
  wl_t r1 = wl_add(wl_add(z1_1, z1_2), z1_2);
  wl_t r2 = wl_add(wl_add(z2_1, z2_2), z2_2);
  wl_t r3 = wl_add(wl_add(z3_1, z3_2), z3_2);
  wl_t r4 = wl_add(wl_add(z4_1, z4_2), z4_2);

  FD_R52X5_QUAD_MOV(*_z, r);
}

#define FD_R52X5_QUAD_SQR_FAST(R, P) do {                                      \
    FD_R52X5_QUAD_DECL(_R);                                                    \
    fd_r52x5_quad_sqr_fast(&_R0, &_R1, &_R2, &_R3, &_R4,                       \
                             P##0, P##1, P##2, P##3, P##4);                    \
    FD_R52X5_QUAD_MOV(R, _R);                                                  \
  } while (0)

FD_FN_UNUSED static void /* let compiler decide if worth inlining */
fd_r52x5_quad_sqr_fast( wl_t * _z0, wl_t * _z1, wl_t * _z2, wl_t * _z3, wl_t * _z4, 
                        wl_t    x0, wl_t    x1, wl_t    x2, wl_t    x3, wl_t    x4 ) {

  /* Represent values with coeff. 2 */
  wl_t z0_2 = wl_zero();
  wl_t z1_2 = wl_zero();
  wl_t z2_2 = wl_zero();
  wl_t z3_2 = wl_zero();
  wl_t z4_2 = wl_zero();
  wl_t z5_2 = wl_zero();
  wl_t z6_2 = wl_zero();
  wl_t z7_2 = wl_zero();
  wl_t z9_2 = wl_zero();

  /* Represent values with coeff. 4 */
  wl_t z2_4 = wl_zero();
  wl_t z3_4 = wl_zero();
  wl_t z4_4 = wl_zero();
  wl_t z5_4 = wl_zero();
  wl_t z6_4 = wl_zero();
  wl_t z7_4 = wl_zero();
  wl_t z8_4 = wl_zero();

  wl_t z0_1 = wl_zero();
  z0_1 = wl_madd52lo( z0_1, x0, x0 );

  wl_t z1_1 = wl_zero();
  z1_2 = wl_madd52lo( z1_2, x0, x1 );
  z1_2 = wl_madd52hi( z1_2, x0, x0 );
 
  z2_4 = wl_madd52hi(z2_4, x0, x1);
  wl_t z2_1 = wl_shl( z2_4, 2);
  z2_2 = wl_madd52lo(z2_2, x0, x2);
  z2_1 = wl_madd52lo(z2_1, x1, x1);

  z3_4 = wl_madd52hi(z3_4, x0, x2);
  wl_t z3_1 = wl_shl( z3_4, 2 );
  z3_2 = wl_madd52lo(z3_2, x1, x2);
  z3_2 = wl_madd52lo(z3_2, x0, x3);
  z3_2 = wl_madd52hi(z3_2, x1, x1);

  z4_4 = wl_madd52hi(z4_4, x1, x2);
  z4_4 = wl_madd52hi(z4_4, x0, x3);
  wl_t z4_1 = wl_shl( z4_4, 2 );
  z4_2 = wl_madd52lo(z4_2, x1, x3);
  z4_2 = wl_madd52lo(z4_2, x0, x4);
  z4_1 = wl_madd52lo(z4_1, x2, x2);

  z5_4 = wl_madd52hi(z5_4, x1, x3);
  z5_4 = wl_madd52hi(z5_4, x0, x4);
  wl_t z5_1 = wl_shl( z5_4, 2 );
  z5_2 = wl_madd52lo(z5_2, x2, x3);
  z5_2 = wl_madd52lo(z5_2, x1, x4);
  z5_2 = wl_madd52hi(z5_2, x2, x2);

  z6_4 = wl_madd52hi(z6_4, x2, x3);
  z6_4 = wl_madd52hi(z6_4, x1, x4);
  wl_t z6_1 = wl_shl( z6_4, 2 );
  z6_2 = wl_madd52lo(z6_2, x2, x4);
  z6_1 = wl_madd52lo(z6_1, x3, x3);

  z7_4 = wl_madd52hi(z7_4, x2, x4);
  wl_t z7_1 = wl_shl( z7_4, 2 );
  z7_2 = wl_madd52lo(z7_2, x3, x4);
  z7_2 = wl_madd52hi(z7_2, x3, x3);

  z8_4 = wl_madd52hi(z8_4, x3, x4);
  wl_t z8_1 = wl_shl( z8_4, 2 );
  z8_1 = wl_madd52lo(z8_1, x4, x4);

  wl_t z9_1 = wl_zero();
  z9_2 = wl_madd52hi(z9_2, x4, x4);

  z5_1 = wl_add( z5_1, wl_shl( z5_2, 1 ) );
  z6_1 = wl_add( z6_1, wl_shl( z6_2, 1 ) );
  z7_1 = wl_add( z7_1, wl_shl( z7_2, 1 ) );
  z9_1 = wl_add( z9_1, wl_shl( z9_2, 1 ) );

  wl_t t0 = wl_zero();
  wl_t t1 = wl_zero();
  wl_t r19 = wl_bcast( 19 );
  
  t0 = wl_madd52hi(t0, r19, z9_1);
  t1 = wl_madd52lo(t1, r19, wl_shru( z9_1, 52 ));

  z4_2 = wl_madd52lo(z4_2, r19, wl_shru( z8_1, 52 ));
  z3_2 = wl_madd52lo(z3_2, r19, wl_shru( z7_1, 52 ));
  z2_2 = wl_madd52lo(z2_2, r19, wl_shru( z6_1, 52 ));
  z1_2 = wl_madd52lo(z1_2, r19, wl_shru( z5_1, 52 ));

  z0_2 = wl_madd52lo(z0_2, r19, wl_add( t0, t1));
  z1_2 = wl_madd52hi(z1_2, r19, z5_1);
  z2_2 = wl_madd52hi(z2_2, r19, z6_1);
  z3_2 = wl_madd52hi(z3_2, r19, z7_1);
  z4_2 = wl_madd52hi(z4_2, r19, z8_1);

  z0_1 = wl_madd52lo(z0_1, r19, z5_1);
  z1_1 = wl_madd52lo(z1_1, r19, z6_1);
  z2_1 = wl_madd52lo(z2_1, r19, z7_1);
  z3_1 = wl_madd52lo(z3_1, r19, z8_1);
  z4_1 = wl_madd52lo(z4_1, r19, z9_1);

  wl_t r0 = wl_add(wl_add(z0_1, z0_2), z0_2);
  wl_t r1 = wl_add(wl_add(z1_1, z1_2), z1_2);
  wl_t r2 = wl_add(wl_add(z2_1, z2_2), z2_2);
  wl_t r3 = wl_add(wl_add(z3_1, z3_2), z3_2);
  wl_t r4 = wl_add(wl_add(z4_1, z4_2), z4_2);

  FD_R52X5_QUAD_MOV(*_z, r);
}

FD_PROTOTYPES_END

#endif /* HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5_inl_h */
