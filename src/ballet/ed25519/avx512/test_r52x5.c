#include "../../fd_ballet.h"
#include "fd_r43x6.h"
#include "fd_r52x5_ge.h"
#include "fd_r43x6_ge.h"

#include <stdio.h>

static wv_t
uint256_rand( fd_rng_t * rng ) {
  ulong u0 = fd_rng_ulong( rng );
  ulong u1 = fd_rng_ulong( rng );
  ulong u2 = fd_rng_ulong( rng );
  ulong u3 = fd_rng_ulong( rng );
  return wv( u0, u1, u2, u3 );
}

int
main( int     argc,
      char ** argv ) {
  fd_boot( &argc, &argv );

  ulong iter_max = fd_env_strip_cmdline_ulong( &argc, &argv, "--iter-max", NULL, 10000000UL );
  ulong warm_max = fd_env_strip_cmdline_ulong( &argc, &argv, "--warm-max", NULL, 100UL      );

  FD_LOG_NOTICE(( "Testing with --iter-max %lu --warm-max %lu", iter_max, warm_max ));
  
  // {
  //   FD_R52X5_QUAD_DECL( X );
  //   FD_R52X5_GE_ZERO( X );

  //   // FD_R52X5_GE_ADD( X, X, X );
  //   FD_R52X5_GE_DBL( X, X );

  //   fd_r43x6_t x;
  //   fd_r43x6_t y;
  //   fd_r43x6_t z;
  //   fd_r43x6_t t;
  //   FD_R52X5_QUAD_UNPACK( x,y,z,t, X );

  //   long x0, x1, x2, x3, x4, x5;
  //   long y0, y1, y2, y3, y4, y5;
  //   long z0, z1, z2, z3, z4, z5;
  //   long t0, t1, t2, t3, t4, t5;
  //   fd_r43x6_extract_limbs(x, x);
  //   fd_r43x6_extract_limbs(y, y);
  //   fd_r43x6_extract_limbs(z, z);
  //   fd_r43x6_extract_limbs(t, t);

  //   printf("x: %ld, %ld, %ld, %ld, %ld, %ld\n", x0, x1, x2, x3, x4, x5);
  //   printf("y: %ld, %ld, %ld, %ld, %ld, %ld\n", y0, y1, y2, y3, y4, y5);
  //   printf("z: %ld, %ld, %ld, %ld, %ld, %ld\n", z0, z1, z2, z3, z4, z5);
  //   printf("t: %ld, %ld, %ld, %ld, %ld, %ld\n", t0, t1, t2, t3, t4, t5);
  // }


 
   fd_rng_t _rng[1]; fd_rng_t * rng = fd_rng_join( fd_rng_new( _rng, 0U, 0UL ) );

  do {
     wv_t u = uint256_rand( rng ); fd_r43x6_t x = fd_r43x6_unpack( u );

#   define BENCH(op) do {                                                     \
      for( ulong rem=warm_max; rem; rem-- ) op;                               \
      long dt = -fd_log_wallclock();                                          \
      for( ulong rem=iter_max; rem; rem-- ) op;                               \
      dt += fd_log_wallclock();                                               \
      FD_LOG_NOTICE(( "%-77s: %9.3f ns", #op, (double)dt/(double)iter_max )); \
    } while(0)

    fd_r43x6_t volatile dummy[1]; dummy[0] = x;
    fd_r43x6_t x0 = dummy[0]; fd_r43x6_t y0 = dummy[0];
    fd_r43x6_t x1 = dummy[0]; fd_r43x6_t y1 = dummy[0];
    fd_r43x6_t x2 = dummy[0]; fd_r43x6_t y2 = dummy[0];
    fd_r43x6_t x3 = dummy[0]; fd_r43x6_t y3 = dummy[0];
    
    FD_R52X5_QUAD_DECL( X ); FD_R52X5_QUAD_PACK( X, x0,x1,x2,x3 );
    FD_R52X5_QUAD_DECL( Y ); FD_R52X5_QUAD_PACK( Y, y0,y1,y2,y3 );

    FD_R43X6_QUAD_DECL( Z ); FD_R43X6_QUAD_PACK( Z, x0,x1,x2,x3 );
    FD_R43X6_QUAD_DECL( T ); FD_R43X6_QUAD_PACK( T, y0,y1,y2,y3 );

    BENCH( FD_R52X5_QUAD_PACK( X, x0,x1,x2,x3 ); FD_R52X5_QUAD_UNPACK( x0,x1,x2,x3, X ) );      
    BENCH( FD_R52X5_QUAD_PERMUTE( X, 1,2,3,0, X ) );
    BENCH( FD_R52X5_QUAD_LANE_IF( X, 0,0,1,1, Y, X ) );
    BENCH( FD_R52X5_QUAD_ADD_FAST( X, X, Y ) );
    BENCH( FD_R52X5_QUAD_MUL_FAST( X, X, Y ) );

    BENCH( FD_R52X5_GE_ADD( X, X, Y ) );
    BENCH( FD_R43X6_GE_ADD( Z, Z, T ) );

    BENCH( FD_R52X5_GE_DBL( X, X ) );
    BENCH( FD_R43X6_GE_DBL( Z, Z ) );


    /* Prevent compiler from optimizing away */
    dummy[0] = x0; dummy[0] = x1; dummy[0] = x2; dummy[0] = x3;
    dummy[0] = x;
    wv_t volatile dummy0[1]; dummy0[0] = u; u = dummy0[0];
  } while (0);

  FD_LOG_NOTICE(( "pass" ));
  fd_halt();
  return 0;
}
