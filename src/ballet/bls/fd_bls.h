#ifndef HEADER_fd_src_ballet_bls_fd_bls_h
#define HEADER_fd_src_ballet_bls_fd_bls_h

/* Backend-neutral BLS12-381 API.  The point types below are finite affine
   points with canonical little-endian base-field limbs.  Only the opaque
   prepared representation is backend-local. */

#include "../fd_ballet_base.h"

#define FD_BLS_PAIR_MAX (64UL)

typedef struct {
  ulong x[6];
  ulong y[6];
} fd_bls_g1_t;

typedef struct {
  ulong x[2][6];
  ulong y[2][6];
} fd_bls_g2_t;

#if FD_HAS_AVX512
#include "avx512/fd_bls.h"
#else
#include "ref/fd_bls.h"
#endif

FD_PROTOTYPES_BEGIN

/* The byte-oriented operations use the Solana syscall encodings.  A G1
   point is x||y, a G2 point is x||y over Fp2, and big_endian selects the
   byte order of every field element and scalar.  Arithmetic accepts the
   same validation level as SIMD-0388: add/sub check curve encoding while
   mul additionally checks subgroup membership and scalar range.

   These operations return zero on success and -1 on failure.  Validation
   returns one for a valid subgroup point and zero otherwise. */

int fd_bls_g1_decompress( uchar       out[ 96 ],
                          uchar const in [ 48 ],
                          int         big_endian );
int fd_bls_g1_validate  ( uchar const in [ 96 ],
                          int         big_endian );
int fd_bls_g1_add       ( uchar       out[ 96 ],
                          uchar const a  [ 96 ],
                          uchar const b  [ 96 ],
                          int         big_endian );
int fd_bls_g1_sub       ( uchar       out[ 96 ],
                          uchar const a  [ 96 ],
                          uchar const b  [ 96 ],
                          int         big_endian );
int fd_bls_g1_mul       ( uchar       out[ 96 ],
                          uchar const scalar[ 32 ],
                          uchar const a     [ 96 ],
                          int         big_endian );

int fd_bls_g2_decompress( uchar       out[ 192 ],
                          uchar const in [  96 ],
                          int         big_endian );
int fd_bls_g2_validate  ( uchar const in [ 192 ],
                          int         big_endian );
int fd_bls_g2_add       ( uchar       out[ 192 ],
                          uchar const a  [ 192 ],
                          uchar const b  [ 192 ],
                          int         big_endian );
int fd_bls_g2_sub       ( uchar       out[ 192 ],
                          uchar const a  [ 192 ],
                          uchar const b  [ 192 ],
                          int         big_endian );
int fd_bls_g2_mul       ( uchar       out[ 192 ],
                          uchar const scalar[  32 ],
                          uchar const a     [ 192 ],
                          int         big_endian );

/* Computes prod_i e(g1[i],g2[i]) from byte-encoded subgroup points and
   writes the twelve Fp coefficients of GT in syscall wire order. */
int fd_bls_pairing_bytes( uchar       out[ 48*12 ],
                          uchar const g1 [],
                          uchar const g2 [],
                          ulong       cnt,
                          int         big_endian );

/* Verifies e(public_key,H(msg,domain))*e(-G1,signature)==1. */
int fd_bls_verify( uchar const  msg[],
                   ulong        msg_sz,
                   uchar const  signature[ 96 ],
                   uchar const  public_key[ 48 ],
                   char const * domain,
                   ulong        domain_len );

/* Preparation trusts that q is a validated subgroup point. */
int fd_bls_g2_prepare( fd_bls_g2_prepared_t * out,
                       fd_bls_g2_t const *     q );

/* Verifies e(p_prepared,q_prepared)*e(p_checked,q_checked)==1.  q_checked is
   subgroup checked; the prepared pair is trusted. */
int fd_bls_pairing_finalverify_prepared_checked(
    fd_bls_g1_t const *             p_prepared,
    fd_bls_g2_prepared_t const *    q_prepared,
    fd_bls_g1_t const *             p_checked,
    fd_bls_g2_t const *             q_checked );

/* Return one for the GT identity, zero otherwise, and -1 for invalid args. */
int fd_bls_pairing_finalverify( fd_bls_g1_t const * p,
                                fd_bls_g2_t const * q,
                                ulong               cnt );

/* q_subgroup_mask selects q inputs that are checked during the pairing. */
int fd_bls_pairing_finalverify_checked( fd_bls_g1_t const * p,
                                        fd_bls_g2_t const * q,
                                        ulong               cnt,
                                        ulong               q_subgroup_mask );

/* Computes prod_i e(p[i],q[i]) and exports its twelve Fp coefficients as
   canonical little-endian limbs.  The empty product is one. */
int fd_bls_pairing( ulong               out[12][6],
                    fd_bls_g1_t const * p,
                    fd_bls_g2_t const * q,
                    ulong               cnt );

FD_PROTOTYPES_END

#endif
