#include "fd_chacha_rng.h"
#include "../../util/simd/fd_avx512.h"
#include <assert.h>

#define wwu_rol16(a) wwb_exch_adj_pair( (a) )
#define wwu_rol12(a) wwu_rol( (a), 12 )
#define wwu_rol7(a)  wwu_rol( (a),  7 )

static inline __attribute__((always_inline)) wwu_t
wwu_rol8( wwu_t x ) {
  wwb_t const mask =
    wwb_bcast_hex( 3,0,1,2, 7,4,5,6, 11,8,9,10, 15,12,13,14 );
  return _mm512_shuffle_epi8( x, mask );
}

static inline void
fd_chacha_rng_quarter( wwu_t *zmm ) {
  zmm[0] = wwu_add( zmm[0], zmm[1] );
  zmm[3] = wwu_rol16( wwu_xor( zmm[3], zmm[0] ) );
  zmm[2] = wwu_add( zmm[2], zmm[3] );
  zmm[1] = wwu_rol12( wwu_xor( zmm[1], zmm[2] ) );

  zmm[0] = wwu_add( zmm[0], zmm[1] );
  zmm[3] = wwu_rol8( wwu_xor( zmm[3], zmm[0] ) );
  zmm[2] = wwu_add( zmm[2], zmm[3] );
  zmm[1] = wwu_rol7( wwu_xor( zmm[1], zmm[2] ) );
}

static inline void
fd_chacha_rng_kernel( wwu_t *zmm ) {
  fd_chacha_rng_quarter( zmm );
  // diagonalize
  zmm[1] = wwu_permute( wwu( 1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8, 13, 14, 15, 12 ), zmm[1] ); // (1, 2, 3, 0) 4x
  zmm[2] = wwu_permute( wwu( 2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13 ), zmm[2] ); // (2, 3, 0, 1) 4x
  zmm[3] = wwu_permute( wwu( 3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10, 15, 12, 13, 14 ), zmm[3] ); // (3, 0, 1, 2) 4x

  fd_chacha_rng_quarter( zmm );
  // undiagonalize
  zmm[3] = wwu_permute( wwu( 1, 2, 3, 0, 5, 6, 7, 4, 9, 10, 11, 8, 13, 14, 15, 12 ), zmm[3] ); // (1, 2, 3, 0) 4x
  zmm[2] = wwu_permute( wwu( 2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13 ), zmm[2] ); // (2, 3, 0, 1) 4x
  zmm[1] = wwu_permute( wwu( 3, 0, 1, 2, 7, 4, 5, 6, 11, 8, 9, 10, 15, 12, 13, 14 ), zmm[1] ); // (3, 0, 1, 2) 4x
}

static inline void
fd_chacha_rng_transpose( wwu_t *zmm ) {
  // v{chacha_instance}{row_in_instance}
  wwu_t v00_10_01_11 = _mm512_inserti64x4(   zmm[0], ((__m256i*)(zmm + 1))[0], 1   );
  wwu_t v20_30_21_31 = _mm512_shuffle_i64x2( zmm[0], zmm[1], 238                    );
  wwu_t v02_12_03_13 = _mm512_inserti64x4(   zmm[2], ((__m256i*)(zmm + 3))[0], 1   );
  wwu_t v22_32_23_33 = _mm512_shuffle_i64x2( zmm[2], zmm[3], 238                    );

  zmm[0] = _mm512_shuffle_i64x2( v00_10_01_11, v02_12_03_13, 136 );
  zmm[1] = _mm512_shuffle_i64x2( v00_10_01_11, v02_12_03_13, 221 );
  zmm[2] = _mm512_shuffle_i64x2( v20_30_21_31, v22_32_23_33, 136 );
  zmm[3] = _mm512_shuffle_i64x2( v20_30_21_31, v22_32_23_33, 221 );
}

static void
fd_chacha_rng_refill_avx512( fd_chacha_rng_t * rng,
                             ulong             rnd2_cnt ) {
  // /* This function should only be called if the buffer is empty. */
  // if( FD_UNLIKELY( rng->buf_off != rng->buf_fill ) ) {
  //   FD_LOG_CRIT(( "refill out of sync: buf_off=%lu buf_fill=%lu", rng->buf_off, rng->buf_fill ));
  // }

  // @bitCast(@as([4][16]u8, @splat("expand 32-byte k".*))),
  wwu_t iv = wwu( 1634760805, 857760878, 2036477234, 1797285236, 
                  1634760805, 857760878, 2036477234, 1797285236, 
                  1634760805, 857760878, 2036477234, 1797285236, 
                  1634760805, 857760878, 2036477234, 1797285236 );
  // @bitCast(@as([4][16]u8, @splat(self.key[0..16].*))),
  __m128i key_lo_v = _mm_load_si128( (__m128i const *)rng->key   );
  wwu_t key_lo = _mm512_broadcast_i32x4( key_lo_v );
  // @bitCast(@as([4][16]u8, @splat(self.key[16..32].*))),
  __m128i key_hi_v = _mm_load_si128( (__m128i const *)rng->key+1 );
  wwu_t key_hi = _mm512_broadcast_i32x4( key_hi_v );
  // @bitCast(@as([4]u128, @splat(self.counter))),
  wwu_t counter = _mm512_mask_set1_epi64( _mm512_setzero_si512(), 0x55, (long long)(rng->counter) );

  wwu_t offset = wwu( 0,0,0,0, 0,0,0,1, 0,0,0,2, 0,0,0,3 );
  wwu_t lanes[LANES][4];
  for (uint i = 0; i<LANES; i++) {
    lanes[i][0] = iv;
    lanes[i][1] = key_lo;
    lanes[i][2] = key_hi;
    lanes[i][3] = wwu_add( counter, offset );
    offset = wwu_add( offset, wwu_bcast( 4 ) );
  }

  wwu_t pre[LANES][4];
  memcpy( pre, lanes, sizeof(wwu_t) * LANES * 4);
  for (uint i = 0; i<rnd2_cnt; i++) {
    for (uint j = 0; j<LANES; j++) {
      fd_chacha_rng_kernel( lanes[j] );
    }
  }

  for (uint i = 0; i<LANES; i++) {
    for (uint j = 0; j<4; j++) {
      lanes[i][j] = wwu_add( lanes[i][j], pre[i][j] ); // add pre-round states into permuted-states.
    }
    fd_chacha_rng_transpose( lanes[i] );               // shuffle lane from vec-per-row to vec-per-instance
    memcpy( &rng->buf[i * 64 * 4], lanes[i], 4 * sizeof(__m512i) );  // write out instance to buffer
  }

  rng->counter += 4 * LANES;
  rng->read = 0;
}

void
fd_chacha8_rng_refill_avx512( fd_chacha_rng_t * rng ) {
  fd_chacha_rng_refill_avx512( rng, 4UL );
}

void
fd_chacha20_rng_refill_avx512( fd_chacha_rng_t * rng ) {
  fd_chacha_rng_refill_avx512( rng, 10UL );
}
