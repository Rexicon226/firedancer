#ifndef HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5s_ge_h
#define HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5s_ge_h

#include "fd_r43x6.h"

FD_PROTOTYPES_BEGIN

#define FD_R52X5_GE_ZERO(P) do {  \
    P##0 = wl( 0, 1, 1, 0 );      \
    P##1 = wl_zero();             \
    P##2 = wl_zero();             \
    P##3 = wl_zero();             \
    P##4 = wl_zero();             \
} while (0)

#define FD_R52X5_GE_DECODE2( Pa,sa, Pb,sb ) (__extension__({                                      \
    FD_R43X6_QUAD_DECL( _Pa );    FD_R43X6_QUAD_DECL( _Pb );                                      \
    int _err = fd_r43x6_ge_decode2( &_Pa03, &_Pa14, &_Pa25, (sa), &_Pb03, &_Pb14, &_Pb25, (sb) ); \
    fd_r43x6_t xa, xb, ya, yb, za, zb, ta, tb; \
    FD_R43X6_QUAD_UNPACK( xa,ya,za,ta, _Pa ); \
    FD_R43X6_QUAD_UNPACK( xb,yb,zb,tb, _Pb ); \
    FD_R52X5_QUAD_PACK( Pa, xa,ya,za,ta ); \
    FD_R52X5_QUAD_PACK( Pb, xb,yb,zb,tb ); \
    _err;                                                                                         \
  }))

#define FD_R52X5_GE_ADD(P3, P1, P2)                                            \
  do {                                                                         \
    FD_R52X5_QUAD_DECL(_ta);                                                   \
    FD_R52X5_QUAD_DECL(_tb);                                                   \
    FD_R52X5_QUAD_DECL(_alt);                                                  \
    FD_R52X5_QUAD_DIFF_SUM(_ta, P2);                                           \
    FD_R52X5_QUAD_LANE_IF(_ta, 1, 1, 0, 0, _ta, P2);                           \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                             \
    wv_t _1122d = wv(121666, 121666, 2 * 121666, 2 * 121665);                  \
    FD_R52X5_QUAD_MUL_CONSTANT(_ta, _ta, _1122d);                              \
    FD_R52X5_QUAD_NEGATE_LAZY(_tb, _ta);                                            \
    FD_R52X5_QUAD_LANE_IF(_ta, 0, 0, 0, 1, _tb, _ta);                          \
    FD_R52X5_QUAD_REDUCE(_alt, _ta);                                            \
    FD_R52X5_QUAD_DIFF_SUM(_ta, P1);                                           \
    FD_R52X5_QUAD_LANE_IF(_ta, 1, 1, 0, 0, _ta, P1);                           \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                             \
    FD_R52X5_QUAD_MUL_FAST(_ta, _ta, _alt);                                    \
    FD_R52X5_QUAD_PERMUTE(_ta, 0, 1, 3, 2, _ta);                               \
    FD_R52X5_QUAD_DIFF_SUM(_ta, _ta);                                          \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                             \
    FD_R52X5_QUAD_PERMUTE(_tb, 0, 3, 3, 0, _ta);                               \
    FD_R52X5_QUAD_PERMUTE(_ta, 2, 1, 2, 1, _ta);                               \
    FD_R52X5_QUAD_MUL_FAST(P3, _tb, _ta);                                      \
  } while (0)

#define FD_R52X5_GE_ADD_TABLE( P3, P1, T2 ) do {    \
    FD_R52X5_QUAD_DECL(_ta);                                                   \
    FD_R52X5_QUAD_DECL(_tb);                                                   \
    FD_R52X5_QUAD_DIFF_SUM(_ta, P1);                                           \
    FD_R52X5_QUAD_LANE_IF(_ta, 1, 1, 0, 0, _ta, P1);                           \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                             \
    FD_R52X5_QUAD_MUL_FAST(_ta, _ta, T2);                                      \
    FD_R52X5_QUAD_PERMUTE(_ta, 0, 1, 3, 2, _ta);                               \
    FD_R52X5_QUAD_DIFF_SUM(_ta, _ta);                                          \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                             \
    FD_R52X5_QUAD_PERMUTE(_tb, 0, 3, 3, 0, _ta);                               \
    FD_R52X5_QUAD_PERMUTE(_ta, 2, 1, 2, 1, _ta);                               \
    FD_R52X5_QUAD_MUL_FAST(P3, _tb, _ta);                                      \
  } while (0)

#define FD_R52X5_GE_DBL(P2, P1)                                                \
  do {                                                                         \
    FD_R52X5_QUAD_DECL(_ta);                                                   \
    FD_R52X5_QUAD_DECL(_tb);                                                   \
    FD_R52X5_QUAD_DECL(_1111);                                                 \
    FD_R52X5_QUAD_DECL(_2222);                                                 \
    FD_R52X5_QUAD_DECL(_2224);                                                 \
    FD_R52X5_QUAD_DECL(_zero);                                                 \
    _zero##0 = wl_zero();                                                      \
    _zero##1 = wl_zero();                                                      \
    _zero##2 = wl_zero();                                                      \
    _zero##3 = wl_zero();                                                      \
    _zero##4 = wl_zero();                                                      \
    FD_R52X5_QUAD_PERMUTE(_ta, 1, 0, 3, 2, P1);                                \
    FD_R52X5_QUAD_ADD_FAST(_ta, _ta, P1);                                      \
    FD_R52X5_QUAD_PERMUTE(_ta, 0, 1, 0, 1, _ta);                               \
    FD_R52X5_QUAD_LANE_IF(_ta, 0, 0, 0, 1, _ta, P1);                           \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                            \
    FD_R52X5_QUAD_SQR_FAST(_ta, _ta);                                          \
    FD_R52X5_QUAD_PERMUTE(_1111, 0, 0, 0, 0, _ta);                             \
    FD_R52X5_QUAD_PERMUTE(_2222, 1, 1, 1, 1, _ta);                             \
    FD_R52X5_QUAD_LANE_IF(_2224, 0, 0, 0, 1, _ta, _2222);                      \
    FD_R52X5_QUAD_NEGATE_LAZY(_2224, _2224);                                   \
    FD_R52X5_QUAD_ADD_FAST(_ta, _ta, _ta);                                     \
    FD_R52X5_QUAD_LANE_IF(_ta, 0, 0, 1, 0, _ta, _zero);                        \
    FD_R52X5_QUAD_ADD_FAST(_ta, _1111, _ta);                                   \
    FD_R52X5_QUAD_LANE_IF(_tb, 1, 0, 0, 1, _2222, _zero);                      \
    FD_R52X5_QUAD_ADD_FAST(_ta, _ta, _tb);                                     \
    FD_R52X5_QUAD_LANE_IF(_tb, 0, 1, 1, 1, _2224, _zero);                      \
    FD_R52X5_QUAD_ADD_FAST(_ta, _ta, _tb);                                     \
    FD_R52X5_QUAD_REDUCE(_ta, _ta);                                            \
    FD_R52X5_QUAD_PERMUTE(_tb, 3, 1, 1, 3, _ta);                               \
    FD_R52X5_QUAD_PERMUTE(_ta, 2, 0, 2, 0, _ta);                               \
    FD_R52X5_QUAD_MUL_FAST(P2, _ta, _tb);                                      \
  } while (0)

#define FD_R52X5_GE_IS_EQ( X, Y ) fd_r52x5_ge_is_eq( X##0, X##1, X##2, X##3, X##4, Y##0, Y##1, Y##2, Y##3, Y##4 )

FD_FN_UNUSED static int /* let compiler decide if worth inlining */
fd_r52x5_ge_is_eq( wl_t X0, wl_t X1, wl_t X2, wl_t X3, wl_t X4,
                   wl_t Y0, wl_t Y1, wl_t Y2, wl_t Y3, wl_t Y4 ) {
  fd_r43x6_t xn1, xn2, yn1, yn2;
  FD_R52X5_QUAD_PERMUTE ( X, 2,0,2,1, X );         /* X = XZ |XX |XZ |XY,  in u46|u46|u46|u46 */
  FD_R52X5_QUAD_PERMUTE ( Y, 0,2,1,2, Y );         /* Y = YX |YZ |YY |YZ , in u46|u46|u46|u46 */
  FD_R52X5_QUAD_MUL_FAST( X, X, Y );               /* X = xn2|xn1|yn2|yn1, in u62|u62|u62|u62 */
  FD_R52X5_QUAD_UNPACK  ( xn2, xn1, yn2, yn1, X );
  return (int)(!fd_r43x6_is_nonzero( fd_r43x6_sub_fast( xn1, xn2 ) /* in s62 */ )) &
         (int)(!fd_r43x6_is_nonzero( fd_r43x6_sub_fast( yn1, yn2 ) /* in s62 */ ));
}


FD_PROTOTYPES_END

#endif /* HEADER_fd_src_ballet_ed25519_avx512_fd_r52x5s_ge_h */
