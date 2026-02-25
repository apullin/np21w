/**
 * @file  audio_test.c
 * @brief Correctness + performance tests for CT1741 resampling optimization.
 *
 * Tests that the Bresenham accumulator resampling produces identical
 * sample positions to the original per-sample division formula.
 *
 * Build:  make test
 * Run:    ./audio_test
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ------------------------------------------------------------------ */
/*  Test: CT1741 resampling — Bresenham vs original division          */
/* ------------------------------------------------------------------ */

/*
 * Original code (per-sample division):
 *   samppos = K * (i * samplen_src / samplen_dst);
 *
 * New code (Bresenham accumulator):
 *   frac += samplen_src;
 *   while (frac >= samplen_dst) { frac -= samplen_dst; samppos += K; }
 *
 * These MUST produce identical samppos values for every i.
 */

static int test_resampling(int K, const char *label, int samplen_src, int samplen_dst, int count) {
	int samppos_new = 0, frac = 0;
	int errors = 0;
	int i;

	for (i = 0; i < count; i++) {
		int samppos_old = K * (i * samplen_src / samplen_dst);
		if (samppos_new != samppos_old) {
			if (errors < 3) {
				fprintf(stderr, "  FAIL %s K=%d src=%d dst=%d i=%d: old=%d new=%d\n",
						label, K, samplen_src, samplen_dst, i, samppos_old, samppos_new);
			}
			errors++;
		}
		frac += samplen_src;
		while (frac >= samplen_dst) { frac -= samplen_dst; samppos_new += K; }
	}
	return errors;
}

/* ------------------------------------------------------------------ */
/*  Benchmark: division vs accumulator with memory access             */
/* ------------------------------------------------------------------ */

/*
 * Simulates the real CT1741 loop: compute samppos, read from a buffer,
 * write to output. This prevents the compiler from optimizing away the
 * division in ways it couldn't in the real code.
 */

static double bench_division_realistic(int samplen_src, int samplen_dst, int count, int iters, int K) {
	/* Allocate src/dst buffers to simulate real memory access */
	unsigned char *src = (unsigned char *)calloc(65536, 1);
	int *dst = (int *)calloc(count * 2, sizeof(int));
	clock_t start, end;
	int iter, i;
	int leng = 8192;

	/* Fill src with pattern */
	for (i = 0; i < 65536; i++) src[i] = (unsigned char)(i & 0xFF);

	start = clock();
	for (iter = 0; iter < iters; iter++) {
		memset(dst, 0, count * 2 * sizeof(int));
		for (i = 0; i < count; i++) {
			int samppos = K * (i * samplen_src / samplen_dst);
			if ((unsigned)samppos >= (unsigned)leng) break;
			dst[i*2]   += (int)src[samppos] * 100;
			dst[i*2+1] += (int)src[samppos] * 100;
		}
	}
	end = clock();

	free(src);
	free(dst);
	return (double)(end - start) / CLOCKS_PER_SEC;
}

static double bench_accumulator_realistic(int samplen_src, int samplen_dst, int count, int iters, int K) {
	unsigned char *src = (unsigned char *)calloc(65536, 1);
	int *dst = (int *)calloc(count * 2, sizeof(int));
	clock_t start, end;
	int iter, i;
	int leng = 8192;

	for (i = 0; i < 65536; i++) src[i] = (unsigned char)(i & 0xFF);

	start = clock();
	for (iter = 0; iter < iters; iter++) {
		int samppos = 0, frac = 0;
		memset(dst, 0, count * 2 * sizeof(int));
		for (i = 0; i < count; i++) {
			if ((unsigned)samppos >= (unsigned)leng) break;
			dst[i*2]   += (int)src[samppos] * 100;
			dst[i*2+1] += (int)src[samppos] * 100;
			frac += samplen_src;
			while (frac >= samplen_dst) { frac -= samplen_dst; samppos += K; }
		}
	}
	end = clock();

	free(src);
	free(dst);
	return (double)(end - start) / CLOCKS_PER_SEC;
}

/* ------------------------------------------------------------------ */
/*  Main                                                              */
/* ------------------------------------------------------------------ */

int main(void) {
	int total_errors = 0;
	int pass = 0, fail = 0;

	printf("=== CT1741 Audio Optimization Tests ===\n\n");

	/* --- Resampling correctness --- */
	printf("--- Resampling correctness (Bresenham vs division) ---\n");
	{
		int src_rates[] = {8000, 11025, 16000, 22050, 32000, 44100, 48000};
		int dst_rates[] = {22050, 44100, 48000};
		int counts[] = {64, 128, 256, 512, 1024, 2048, 4096};
		int Ks[] = {1, 2, 4}; /* mono8, stereo8/mono16, stereo16 */
		const char *Klabels[] = {"mono8", "stereo8/mono16", "stereo16"};
		int nsrc = sizeof(src_rates)/sizeof(src_rates[0]);
		int ndst = sizeof(dst_rates)/sizeof(dst_rates[0]);
		int ncnt = sizeof(counts)/sizeof(counts[0]);
		int nk = sizeof(Ks)/sizeof(Ks[0]);
		int si, di, ci, ki;

		for (si = 0; si < nsrc; si++) {
			for (di = 0; di < ndst; di++) {
				if (src_rates[si] == dst_rates[di]) continue;
				for (ci = 0; ci < ncnt; ci++) {
					for (ki = 0; ki < nk; ki++) {
						int e = test_resampling(Ks[ki], Klabels[ki],
								src_rates[si], dst_rates[di], counts[ci]);
						if (e) { fail++; total_errors += e; } else { pass++; }
					}
				}
			}
		}
		printf("  %d passed, %d failed\n", pass, fail);
	}

	/* --- Overflow safety test --- */
	printf("\n--- Overflow safety (large sample counts) ---\n");
	{
		/* With the original formula, i * samplen_src can overflow int
		 * for large i. Bresenham never computes that product. */
		int e;
		/* 48000 src, 44100 dst, 50000 samples: i*48000 overflows at i=44739 */
		e = test_resampling(1, "overflow_mono", 48000, 44100, 50000);
		if (e) {
			printf("  Large count (50000 samples): %d mismatches (expected for overflow)\n", e);
			/* Don't count as failure — this demonstrates Bresenham is MORE correct */
		} else {
			printf("  Large count (50000 samples): matched (no overflow in this range)\n");
		}

		/* Extreme: i=100000 with 48000/44100 - guaranteed overflow with int32 */
		e = test_resampling(1, "extreme_overflow", 48000, 44100, 100000);
		if (e) {
			printf("  Extreme count (100000 samples): %d mismatches from overflow\n", e);
			printf("  (This is expected - Bresenham avoids the overflow)\n");
		} else {
			printf("  Extreme count (100000 samples): matched\n");
		}
	}

	/* --- Performance benchmark --- */
	printf("\n--- Performance: division vs accumulator (with memory access) ---\n");
	{
		struct { int src; int dst; int K; const char *label; } cases[] = {
			{11025, 44100, 1, "11025->44100 mono8  (4:1 upsample)"},
			{22050, 44100, 2, "22050->44100 ster8  (2:1 upsample)"},
			{44100, 48000, 4, "44100->48000 ster16 (upsample)"},
			{48000, 44100, 1, "48000->44100 mono8  (downsample)"},
			{8000,  44100, 4, "8000->44100  ster16 (5.5:1 upsample)"},
		};
		int ncases = sizeof(cases)/sizeof(cases[0]);
		int count = 1024;
		int iters = 50000;
		int ci;

		for (ci = 0; ci < ncases; ci++) {
			double t_div = bench_division_realistic(cases[ci].src, cases[ci].dst, count, iters, cases[ci].K);
			double t_acc = bench_accumulator_realistic(cases[ci].src, cases[ci].dst, count, iters, cases[ci].K);
			printf("  %s\n", cases[ci].label);
			printf("    division:    %.3f s\n", t_div);
			printf("    accumulator: %.3f s\n", t_acc);
			if (t_acc > 0.001) {
				printf("    speedup:     %.2fx\n", t_div / t_acc);
			}
		}
	}

	/* --- Summary --- */
	printf("\n=== RESULT: %s (%d errors in %d correctness tests) ===\n",
		   total_errors == 0 ? "ALL PASSED" : "FAILURES DETECTED",
		   total_errors, pass + fail);

	return total_errors == 0 ? 0 : 1;
}
