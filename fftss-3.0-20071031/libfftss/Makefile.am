lib_LTLIBRARIES = libfftss.la

if ASM_IA64
ASM_SOURCES = r4_ia64_o.c r8_ia64_n.c r4_ia64_asm.s r8_ia64_asm.s
endif

if ASM_SPARC
ASM_SOURCES = 
endif

if ASM_BG
ASM_SOURCES = r4_bgl_asm_u1.s r4_bgl_asm_u4.s r4_bgl_asm_n.s
endif

if SSE2
SSE2_SOURCES = r4_sse2_o.c r4_sse2_u1.c r4_sse2_u4.c r4_sse2_1_n.c\
	r8_sse2_n.c r8_sse2_o.c r8_sse2_u1.c
endif

if SSE3
SSE3_SOURCES = r4_sse3_n.c r4_sse3_o.c r4_sse3_u1.c r4_sse3_u4.c \
	r4_sse3_h_n.c r4_sse3_h_o.c r4_sse3_h_u1.c 
#	r4_conroe_o.s r4_conroe_n.s r4_conroe_u1.s r4_conroe_u4.s \
#	r8_conroe_n.s
endif

if BG
BG_SOURCES = r4_bgl_n.c r4_bgl_o.c r4_bgl_u1.c \
	r8_bgl_n.c r8_bgl_o.c r8_bgl_u1.c \
	r4_bgl_pl_n.c
endif

if COMPLEX
COMPLEX_SOURCES = r4_cmplx_n.c
endif

EXTRA_SOURCES = $(ASM_SOURCES) $(SSE2_SOURCES) $(SSE3_SOURCES) \
		$(BG_SOURCES) $(COMPLEX_SOURCES)

BASIC_KERNELS = r4_n.c r4_o.c r4_u1.c r4_u4.c \
		r4_fma_n.c r4_fma_o.c r4_fma_u1.c r4_fma_u4.c \
		r8_n.c r8_o.c r8_u1.c \
		r8_fma_n.c r8_fma_o.c r8_fma_u1.c 
BASE = fftss.c fftss_test.c fftss_fma.c fftss_malloc.c fftss_table.c \
	fftss_kset.c fftss_set.c fftss_execute.c fftss_execute_dft.c \
	fftss_destroy_plan.c \
	fftss_version.c fftss_get_wtime.c fftss_counter.c fftss_cpuid.c \
	fftss_execute_dft_1d.c \
	fftss_execute_inplace_dft_1d.c \
	fftss_2d.c fftss_3d.c fftss_copy.c \
	fftss_threads.c fftss_fortran.c

libfftss_la_SOURCES = $(BASE) ../include/libfftss.h $(BASIC_KERNELS) $(EXTRA_SOURCES)
libfftss_la_LDFLAGS = -version-info 1:0:0
