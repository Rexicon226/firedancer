$(call add-hdrs,fd_bls.h avx512/fd_bls.h ref/fd_bls.h)

ifdef FD_HAS_AVX512
$(call add-objs,fd_bls,fd_ballet)
FD_BLS_AVX512_CC:=clang-21
FD_BLS_AVX512_DEPS:=$(wildcard src/ballet/bls/avx512/*.h src/ballet/bls/avx512/*.inc) \
  src/ballet/bls/avx512/fd_bls_rns.c \
  src/ballet/bls/avx512/fd_bls_field.c \
  src/ballet/bls/avx512/fd_bls_final.c \
  src/ballet/bls/avx512/fd_bls_miller.c
$(OBJDIR)/obj/ballet/bls/avx512/fd_bls_backend.o: src/ballet/bls/avx512/fd_bls_backend.c $(FD_BLS_AVX512_DEPS)
	@echo -e "CC-IFMA\t$(notdir $@)"
	$(Q)$(MKDIR) $(dir $@) && \
$(FD_BLS_AVX512_CC) -std=c17 -O3 -g -fPIC -fno-semantic-interposition -fno-omit-frame-pointer -fwrapv -march=native -mavx512ifma \
  -DFD_HAS_AVX512=1 -DFD_HAS_INT128=1 -DFD_HAS_HOSTED=1 -DFD_HAS_X86=1 -DFD_IS_X86_64=1 \
  -Wno-unused-function -Wno-ignored-attributes -Wno-unknown-pragmas -I. -c $< -o $@
$(call add-objs,avx512/fd_bls_backend,fd_ballet)
ifdef FD_HAS_BLST
$(call make-unit-test,test_bls_avx512,avx512/test_bls_avx512,fd_ballet fd_util,$(BLST_LIBS))
$(call run-unit-test,test_bls_avx512)
endif
else
ifdef FD_HAS_BLST
$(call add-objs,fd_bls,fd_ballet)
else
$(warning fd_bls disabled due to lack of libblst on a non-AVX512 target)
endif
endif

ifdef FD_HAS_BLST

$(call add-hdrs,fd_bls12_381.h)
# Reference fallbacks for group/encoding/hash operations not yet native AVX-512.
$(call add-objs,ref/fd_bls_ops fd_bls12_381,fd_ballet)

$(call make-unit-test,test_bls12_381,test_bls12_381,fd_ballet fd_util,$(BLST_LIBS))

$(call run-unit-test,test_bls12_381)

else

$(warning bls12_381 disabled due to lack of libblst)

endif
