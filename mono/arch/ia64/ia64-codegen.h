/*
 * ia64-codegen.h: Macros for generating ia64 code
 *
 * Authors:
 *   Zoltan Varga (vargaz@gmail.com)
 *
 * (C) 2005 Novell, Inc. 
 */

#ifndef _IA64_CODEGEN_H_
#define _IA64_CODEGEN_H_

#include <glib.h>

typedef enum {
	IA64_INS_TYPE_A,
	IA64_INS_TYPE_I,
	IA64_INS_TYPE_M,
	IA64_INS_TYPE_F,
	IA64_INS_TYPE_B,
	IA64_INS_TYPE_LX
} Ia64InsType;

typedef enum {
	IA64_TEMPLATE_MII     = 0x00,
	IA64_TEMPLATE_MIIS    = 0x01,
	IA64_TEMPLATE_MISI    = 0x02,
	IA64_TEMPLATE_MISIS   = 0x03,
	IA64_TEMPLATE_MLX     = 0x04,
	IA64_TEMPLATE_MLXS    = 0x05,
	IA64_TEMPLATE_UNUS1   = 0x06,
	IA64_TEMPLATE_UNUS2   = 0x07,
	IA64_TEMPLATE_MMI     = 0x08,
	IA64_TEMPLATE_MMIS    = 0x09,
	IA64_TEMPLATE_MSMI    = 0x0A,
	IA64_TEMPLATE_MSMIS   = 0x0B,
	IA64_TEMPLATE_MFI     = 0x0C,
	IA64_TEMPLATE_MFIS    = 0x0D,
	IA64_TEMPLATE_MMF     = 0x0E,
	IA64_TEMPLATE_MMFS    = 0x0F,
	IA64_TEMPLATE_MIB     = 0x10,
	IA64_TEMPLATE_MIBS    = 0x11,
	IA64_TEMPLATE_MBB     = 0x12,
	IA64_TEMPLATE_MBBS    = 0x13,
	IA64_TEMPLATE_UNUS3   = 0x14,
	IA64_TEMPLATE_UNUS4   = 0x15,
	IA64_TEMPLATE_BBB     = 0x16,
	IA64_TEMPLATE_BBBS    = 0x17,
	IA64_TEMPLATE_MMB     = 0x18,
	IA64_TEMPLATE_MMBS    = 0x19,
	IA64_TEMPLATE_UNUS5   = 0x1A,
	IA64_TEMPLATE_UNUS6   = 0x1B,
	IA64_TEMPLATE_MFB     = 0x1C,
	IA64_TEMPLATE_MFBS    = 0x1D,
	IA64_TEMPLATE_UNUS7   = 0x1E,
	IA64_TEMPLATE_UNUS8   = 0x1F,
} Ia64BundleTemplate;

typedef enum {
	IA64_R0 = 0,
	IA64_R1 = 1,
	IA64_R2 = 2,
	IA64_R3 = 3,
	IA64_R4 = 4,
	IA64_R5 = 5,
	IA64_R6 = 6,
	IA64_R7 = 7,
	IA64_R8 = 8,
	IA64_R9 = 9,
	IA64_R10 = 10,
	IA64_R11 = 11,
	IA64_R12 = 12,
	IA64_R13 = 13,
	IA64_R14 = 14,
	IA64_R15 = 15,
	IA64_R16 = 16,
	IA64_R17 = 17,
	IA64_R18 = 18,
	IA64_R19 = 19,
	IA64_R20 = 20,
	IA64_R21 = 21,
	IA64_R22 = 22,
	IA64_R23 = 23,
	IA64_R24 = 24,
	IA64_R25 = 25,
	IA64_R26 = 26,
	IA64_R27 = 27,
	IA64_R28 = 28,
	IA64_R29 = 29,
	IA64_R30 = 30,
	IA64_R31 = 31,

	/* Aliases */
	IA64_GP = IA64_R1,
	IA64_SP = IA64_R12,
	IA64_TP = IA64_R13
} Ia64GeneralRegister;

typedef enum {
	IA64_B0 = 0,
	IA64_B1 = 1,
	IA64_B2 = 2,
	IA64_B3 = 3,
	IA64_B4 = 4,
	IA64_B5 = 5,
	IA64_B6 = 6,
	IA64_B7 = 7
} Ia64BranchRegister;

typedef enum {
	IA64_PFS = 64
} Ia64ApplicationRegister;

/* disassembly */
#define ia64_bundle_template(code) ((*(guint64*)code) & 0x1f)
#define ia64_bundle_ins1(code) (((*(guint64*)code) >> 5) & 0x1ffffffffff)
#define ia64_bundle_ins2(code) (((*(guint64*)code) >> 46) | ((((guint64*)code)[1] & 0x3ffff) << 18))
#define ia64_bundle_ins3(code) ((((guint64*)code)[1]) >> 23)

#define ia64_ins_opcode(ins) (((guint64)(ins)) >> 37)
#define ia64_ins_qp(ins) (((guint64)(ins)) & 0x3f)
#define ia64_ins_r1(ins) ((((guint64)(ins)) >> 6) & 0x7f)
#define ia64_ins_btype(ins) ((((guint64)(ins)) >> 6) & 0x7)
#define ia64_ins_x3(ins) ((((guint64)(ins)) >> 33) & 0x7)
#define ia64_ins_x6(ins) ((((guint64)(ins)) >> 27) & 0x3f)
#define ia64_ins_vc(ins) ((((guint64)(ins)) >> 20) & 0x1)

#define IA64_NOP_I ((0x01 << 27))
#define IA64_NOP_M ((0x01 << 27))

/*
 * IA64 code cannot be emitted in the same way as code on other processors,
 * since 3 instructions are combined into a bundle. This structure keeps track
 * of already emitted instructions.
 *
 */

typedef struct {
	guint8 *buf;
	guint64 instructions [3];
	int itypes [3], stops [3];
	int nins;
} Ia64CodegenState;

static void ia64_emit_bundle (Ia64CodegenState *code, gboolean flush);

/*
 * FIXME:
 *
 *  In order to simplify things, we emit a stop after every instruction for
 * now. Also, we emit 1 ins + 2 nops.
 */

#define ia64_codegen_init(code, codegen_buf) do { \
    code.buf = codegen_buf; \
    code.nins = 0; \
} while (0)

#define ia64_codegen_close(code) do { \
    ia64_emit_bundle (&code, TRUE); \
} while (0)

#define ia64_begin_bundle(code) do { \
    ia64_emit_bundle (&code, TRUE); \
} while (0)

/* To ease debugging, we emit instructions immediately */
#define ia64_emit_ins(code, itype, ins) do { \
    code.instructions [code.nins] = ins; \
    code.itypes [code.nins] = itype; \
    code.stops [code.nins] = 1; \
    code.nins ++; \
    if ((itype != IA64_INS_TYPE_LX) || (code.nins == 2)) ia64_emit_bundle (&code, FALSE); \
    if (code.nins == 3) \
       ia64_emit_bundle (&code, FALSE); \
} while (0)

#if G_BYTE_ORDER != G_LITTLE_ENDIAN
#error "FIXME"
#endif

#define ia64_emit_bundle_template(code, template, i1, i2, i3) do { \
    guint64 dw1, dw2; \
    dw1 = (((guint64)(template)) & 0x1f) | ((guint64)(i1) << 5) | ((((guint64)(i2)) & 0x3ffff) << 46); \
    dw2 = (((guint64)(i2)) >> 18) | (((guint64)(i3)) << 23); \
    ((guint64*)(code)->buf)[0] = dw1; \
    ((guint64*)(code)->buf)[1] = dw2; \
    (code)->buf += 16; \
} while (0)

static void 
ia64_emit_bundle (Ia64CodegenState *code, gboolean flush)
{
	int i, template;
	guint64 i1, i2, i3;

	for (i = 0; i < code->nins; ++i) {
		switch (code->itypes [i]) {
		case IA64_INS_TYPE_A:
			ia64_emit_bundle_template (code, IA64_TEMPLATE_MIIS, code->instructions [i], IA64_NOP_I, IA64_NOP_I);
			break;
		case IA64_INS_TYPE_I:
			ia64_emit_bundle_template (code, IA64_TEMPLATE_MIIS, IA64_NOP_M, code->instructions [i], IA64_NOP_I);
			break;
		case IA64_INS_TYPE_M:
			ia64_emit_bundle_template (code, IA64_TEMPLATE_MIIS, code->instructions [i], IA64_NOP_I, IA64_NOP_I);
			break;
		case IA64_INS_TYPE_B:
			ia64_emit_bundle_template (code, IA64_TEMPLATE_MIBS, IA64_NOP_M, IA64_NOP_I, code->instructions [i]);
			break;
		case IA64_INS_TYPE_LX:
			ia64_emit_bundle_template (code, IA64_TEMPLATE_MLXS, IA64_NOP_M, code->instructions [i], code->instructions [i + 1]);
			i ++;
			break;
		default:
			g_assert_not_reached ();
		}
	}

	code->nins = 0;
}

#if 1

#define check_assert(cond) g_assert((cond))

#else

#define check_assert(cond)

#endif

#define check_greg(gr) check_assert ((guint64)(gr) < 128)

#define check_freg(fr) check_assert ((guint64)(fr) < 128)

#define check_preg(pr) check_assert ((guint64)(pr) < 64)

#define check_breg(pr) check_assert ((guint64)(pr) < 8)

#define check_count2(count) check_assert (((count) >= 1) && ((count) <= 4))

#define check_count5(count) check_assert (((count) >= 0) && ((count) < 32))

#define check_count6(count) check_assert (((count) >= 0) && ((count) < 64))

#define check_imm1(imm) check_assert (((gint64)(imm) >= -1) && ((gint64)(imm) <= 0))
#define check_imm3(imm) check_assert (((gint64)(imm) >= -4) && ((gint64)(imm) <= 3))
#define check_imm8(imm) check_assert (((gint64)(imm) >= -128) && ((gint64)(imm) <= 127))
#define check_imm9(imm) check_assert (((gint64)(imm) >= -256) && ((gint64)(imm) <= 255))
#define check_imm14(imm) check_assert (((gint64)(imm) >= -8192) && ((gint64)(imm) <= 8191))
#define check_imm21(imm) check_assert (((gint64)(imm) >= -0x200000) && ((gint64)(imm) <= (0x200000 - 1)))
#define check_imm22(imm) check_assert (((gint64)(imm) >= -0x400000) && ((gint64)(imm) <= (0x400000 - 1)))
#define check_imm62(imm) check_assert (((gint64)(imm) >= -0x2fffffffffffffffLL) && ((gint64)(imm) <= (0x2fffffffffffffffLL - 1)))

#define check_len4(len) check_assert (((gint64)(len) >= 1) && ((gint64)(len) <= 16))

#define check_bwh(bwh) check_assert ((bwh) >= 0 && (bwh) <= IA64_BWH_DPNT)

#define check_ph(ph) check_assert ((ph) >= 0 && (ph) <= IA64_PH_MANY)

#define check_dh(dh) check_assert ((dh) >= 0 && (dh) <= IA64_DH_CLR)

#define check_gregs(r1,r2,r3) do { check_greg ((r1)); check_greg ((r2)); check_greg ((r3)); } while (0)

#define check_pregs(p1,p2) do { check_preg ((p1)); check_preg ((p2)); } while (0)

#define sign_bit(imm) ((gint64)(imm) < 0 ? 1 : 0)

#define ia64_emit_ins_1(code,itype,f1,o1) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1))))

#define ia64_emit_ins_3(code,itype,f1,o1,f2,o2,f3,o3) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3))))

#define ia64_emit_ins_5(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5))))

#define ia64_emit_ins_6(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6))))

#define ia64_emit_ins_7(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6,f7,o7) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6)) | ((guint64)(f7) << (o7))))

#define ia64_emit_ins_8(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6,f7,o7,f8,o8) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6)) | ((guint64)(f7) << (o7)) | ((guint64)(f8) << (o8))))

#define ia64_emit_ins_9(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6,f7,o7,f8,o8,f9,o9) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6)) | ((guint64)(f7) << (o7)) | ((guint64)(f8) << (o8)) | ((guint64)(f9) << (o9))))

#define ia64_emit_ins_10(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6,f7,o7,f8,o8,f9,o9,f10,o10) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6)) | ((guint64)(f7) << (o7)) | ((guint64)(f8) << (o8)) | ((guint64)(f9) << (o9)) | ((guint64)(f10) << (o10))))

#define ia64_emit_ins_11(code,itype,f1,o1,f2,o2,f3,o3,f4,o4,f5,o5,f6,o6,f7,o7,f8,o8,f9,o9,f10,o10,f11,o11) ia64_emit_ins ((code), (itype), (((guint64)(f1) << (o1)) | ((guint64)(f2) << (o2)) | ((guint64)(f3) << (o3)) | ((guint64)(f4) << (o4)) | ((guint64)(f5) << (o5)) | ((guint64)(f6) << (o6)) | ((guint64)(f7) << (o7)) | ((guint64)(f8) << (o8)) | ((guint64)(f9) << (o9)) | ((guint64)(f10) << (o10)) | ((guint64)(f11) << (o11))))

#define ia64_a1(code, qp, r1, r2, r3, x2a, ve, x4, x2b) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 27, (x4), 29, (ve), 33, (x2a), 34, (8), 37); } while (0)

#define ia64_add_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 0, 0)
#define ia64_add1_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 0, 1)
#define ia64_sub_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 1, 1)
#define ia64_sub1_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 1, 0)
#define ia64_addp4_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 2, 0)
#define ia64_and_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 3, 0)
#define ia64_andcm_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 3, 1)
#define ia64_or_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 3, 2)
#define ia64_xor_pred(code, qp, r1, r2, r3) ia64_a1 ((code), (qp), r1, r2, r3, 0, 0, 3, 3)

#define ia64_a2(code, qp, r1, r2, r3, x2a, ve, x4, ct2d) do { check_gregs ((r1), (r2), (r3)); check_count2 (ct2d); ia64_emit_ins_9 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (ct2d - 1), 27, (x4), 29, (ve), 33, (x2a), 34, (8), 37); } while (0)

#define ia64_shladd_pred(code, qp, r1, r2, r3,count) ia64_a2 ((code), (qp), r1, r2, r3, 0, 0, 4, (count))
#define ia64_shladdp4_pred(code, qp, r1, r2, r3,count) ia64_a2 ((code), (qp), r1, r2, r3, 0, 0, 6, (count))

#define ia64_a3(code, qp, r1, imm8, r3, x2a, ve, x4, x2b) do { check_greg ((r1)); check_greg ((r3)); check_imm8 ((imm8)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, (imm8) & 0x7f, 13, (r3), 20, (x2b), 27, (x4), 29, (ve), 33, (x2a), 34, sign_bit((imm8)), 36, (8), 37); } while (0)

#define ia64_sub_imm_pred(code, qp,r1,imm8,r3) ia64_a3 ((code), (qp), (r1), (imm8), (r3), 0, 0, 9, 1)
#define ia64_and_imm_pred(code, qp,r1,imm8,r3) ia64_a3 ((code), (qp), (r1), (imm8), (r3), 0, 0, 0xb, 0)
#define ia64_andcm_imm_pred(code, qp,r1,imm8,r3) ia64_a3 ((code), (qp), (r1), (imm8), (r3), 0, 0, 0xb, 1)
#define ia64_or_imm_pred(code, qp,r1,imm8,r3) ia64_a3 ((code), (qp), (r1), (imm8), (r3), 0, 0, 0xb, 2)
#define ia64_xor_imm_pred(code, qp,r1,imm8,r3) ia64_a3 ((code), (qp), (r1), (imm8), (r3), 0, 0, 0xb, 3)

#define ia64_a4(code, qp, r1, imm14, r3, x2a, ve) do { check_greg ((r1)); check_greg ((r3)); check_imm14 ((imm14)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, ((imm14) & 0x7f), 13, (r3), 20, (((guint64)(imm14) >> 7) & 0x3f), 27, (ve), 33, (x2a), 34, sign_bit ((imm14)), 36, (8), 37); } while (0)

#define ia64_adds_imm_pred(code, qp,r1,imm14,r3) ia64_a4 ((code), (qp), (r1), (imm14), (r3), 2, 0)
#define ia64_addp4_imm_pred(code, qp,r1,imm14,r3) ia64_a4 ((code), (qp), (r1), (imm14), (r3), 3, 0)

#define ia64_a5(code, qp, r1, imm, r3) do { check_greg ((r1)); check_greg ((r3)); check_assert ((r3) < 4); check_imm22 ((imm)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, ((guint64)(imm) & 0x7f), 13, (r3), 20, (((guint64)(imm) >> 12) & 0x1f), 22, (((guint64)(imm) >> 7) & 0x1ff), 27, sign_bit ((imm)), 36, (9), 37); } while (0)

#define ia64_addl_imm_pred(code, qp,r1,imm22,r3) ia64_a5 ((code), (qp), (r1), (imm22), (r3))

#define ia64_a6(code, qp, p1, p2, r2, r3, opcode, x2, tb, ta, c) do { check_greg ((r2)); check_greg ((r3)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (p1), 6, (c), 12, (r2), 13, (r3), 20, (p2), 27, (ta), 33, (x2), 34, (tb), 36, (opcode), 37); } while (0)

#define ia64_cmp_lt_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 0, 0, 0)
#define ia64_cmp_ltu_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 0, 0, 0)
#define ia64_cmp_eq_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 0, 0, 0)
#define ia64_cmp_lt_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 0, 0, 1)
#define ia64_cmp_ltu_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 0, 0, 1)
#define ia64_cmp_eq_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 0, 0, 1)
#define ia64_cmp_eq_and_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 0, 1, 0)
#define ia64_cmp_eq_or_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 0, 1, 0)
#define ia64_cmp_eq_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 0, 1, 0)
#define ia64_cmp_ne_and_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 0, 1, 1)
#define ia64_cmp_ne_or_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 0, 1, 1)
#define ia64_cmp_ne_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 0, 1, 1)

#define ia64_cmp4_lt_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 0, 0, 0)
#define ia64_cmp4_ltu_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 0, 0, 0)
#define ia64_cmp4_eq_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 0, 0, 0)
#define ia64_cmp4_lt_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 0, 0, 1)
#define ia64_cmp4_ltu_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 0, 0, 1)
#define ia64_cmp4_eq_unc_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 0, 0, 1)
#define ia64_cmp4_eq_and_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 0, 1, 0)
#define ia64_cmp4_eq_or_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 0, 1, 0)
#define ia64_cmp4_eq_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 0, 1, 0)
#define ia64_cmp4_ne_and_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 0, 1, 1)
#define ia64_cmp4_ne_or_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 0, 1, 1)
#define ia64_cmp4_ne_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a6 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 0, 1, 1)

/* Pseudo ops */
#define ia64_cmp_ne_pred(code, qp, p1, p2, r2, r3) ia64_cmp_eq ((code), (p2), (p1), (r2), (r3))
#define ia64_cmp_le_pred(code, qp, p1, p2, r2, r3) ia64_cmp_lt ((code), (p2), (p1), (r3), (r2))
#define ia64_cmp_gt_pred(code, qp, p1, p2, r2, r3) ia64_cmp_lt ((code), (p1), (p2), (r3), (r2))
#define ia64_cmp_ge_pred(code, qp, p1, p2, r2, r3) ia64_cmp_lt ((code), (p2), (p1), (r2), (r3))
#define ia64_cmp_leu_pred(code, qp, p1, p2, r2, r3) ia64_cmp_ltu ((code), (p2), (p1), (r3), (r2))
#define ia64_cmp_gtu_pred(code, qp, p1, p2, r2, r3) ia64_cmp_ltu ((code), (p1), (p2), (r3), (r2))
#define ia64_cmp_geu_pred(code, qp, p1, p2, r2, r3) ia64_cmp_ltu ((code), (p2), (p1), (r2), (r3))

#define ia64_cmp4_ne_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_eq ((code), (p2), (p1), (r2), (r3))
#define ia64_cmp4_le_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_lt ((code), (p2), (p1), (r3), (r2))
#define ia64_cmp4_gt_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_lt ((code), (p1), (p2), (r3), (r2))
#define ia64_cmp4_ge_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_lt ((code), (p2), (p1), (r2), (r3))
#define ia64_cmp4_leu_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_ltu ((code), (p2), (p1), (r3), (r2))
#define ia64_cmp4_gtu_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_ltu ((code), (p1), (p2), (r3), (r2))
#define ia64_cmp4_geu_pred(code, qp, p1, p2, r2, r3) ia64_cmp4_ltu ((code), (p2), (p1), (r2), (r3))

#define ia64_a7(code, qp, p1, p2, r2, r3, opcode, x2, tb, ta, c) do { check_greg ((r2)); check_greg ((r3)); check_assert ((r2) == 0); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (p1), 6, (c), 12, (r2), 13, (r3), 20, (p2), 27, (ta), 33, (x2), 34, (tb), 36, (opcode), 37); } while (0)

#define ia64_cmp_gt_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 1, 0, 0)
#define ia64_cmp_gt_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 1, 0, 0)
#define ia64_cmp_gt_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 1, 0, 0)
#define ia64_cmp_le_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 1, 0, 1)
#define ia64_cmp_le_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 1, 0, 1)
#define ia64_cmp_le_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 1, 0, 1)
#define ia64_cmp_ge_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 1, 1, 0)
#define ia64_cmp_ge_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 1, 1, 0)
#define ia64_cmp_ge_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 1, 1, 0)
#define ia64_cmp_lt_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 0, 1, 1, 1)
#define ia64_cmp_lt_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 0, 1, 1, 1)
#define ia64_cmp_lt_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 0, 1, 1, 1)

#define ia64_cmp4_gt_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 1, 0, 0)
#define ia64_cmp4_gt_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 1, 0, 0)
#define ia64_cmp4_gt_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 1, 0, 0)
#define ia64_cmp4_le_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 1, 0, 1)
#define ia64_cmp4_le_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 1, 0, 1)
#define ia64_cmp4_le_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 1, 0, 1)
#define ia64_cmp4_ge_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 1, 1, 0)
#define ia64_cmp4_ge_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 1, 1, 0)
#define ia64_cmp4_ge_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 1, 1, 0)
#define ia64_cmp4_lt_and_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xc, 1, 1, 1, 1)
#define ia64_cmp4_lt_or_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xd, 1, 1, 1, 1)
#define ia64_cmp4_lt_or_andcm_pred(code, qp, p1, p2, r2, r3) ia64_a7 ((code), (qp), (p1), (p2), (r2), (r3), 0xe, 1, 1, 1, 1)

#define ia64_a8(code, qp, p1, p2, imm, r3, opcode, x2, ta, c) do { check_greg ((r3)); check_imm8 ((imm)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (p1), 6, (c), 12, ((guint64)(imm) & 0x7f), 13, (r3), 20, (p2), 27, (ta), 33, (x2), 34, sign_bit ((imm)), 36, (opcode), 37); } while (0)

#define ia64_cmp_lt_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 2, 0, 0)
#define ia64_cmp_ltu_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 2, 0, 0)
#define ia64_cmp_eq_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 2, 0, 0)
#define ia64_cmp_lt_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 2, 0, 1)
#define ia64_cmp_ltu_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 2, 0, 1)
#define ia64_cmp_eq_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 2, 0, 1)
#define ia64_cmp_eq_and_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 2, 1, 0)
#define ia64_cmp_eq_or_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 2, 1, 0)
#define ia64_cmp_eq_or_andcm_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 2, 1, 0)
#define ia64_cmp_ne_and_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 2, 1, 1)
#define ia64_cmp_ne_or_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 2, 1, 1)
#define ia64_cmp_ne_or_andcm_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 2, 1, 1)

#define ia64_cmp4_lt_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 3, 0, 0)
#define ia64_cmp4_ltu_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 3, 0, 0)
#define ia64_cmp4_eq_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 3, 0, 0)
#define ia64_cmp4_lt_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 3, 0, 1)
#define ia64_cmp4_ltu_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 3, 0, 1)
#define ia64_cmp4_eq_unc_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 3, 0, 1)
#define ia64_cmp4_eq_and_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 3, 1, 0)
#define ia64_cmp4_eq_or_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 3, 1, 0)
#define ia64_cmp4_eq_or_andcm_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 3, 1, 0)
#define ia64_cmp4_ne_and_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xc, 3, 1, 1)
#define ia64_cmp4_ne_or_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xd, 3, 1, 1)
#define ia64_cmp4_ne_or_andcm_imm_pred(code, qp, p1, p2, imm8, r3) ia64_a8 ((code), (qp), (p1), (p2), (imm8), (r3), 0xe, 3, 1, 1)

#define ia64_a9(code, qp, r1, r2, r3, x2a, za, zb, x4, x2b) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 27, (x4), 29, (zb), 33, (x2a), 34, (za), 36, (8), 37); } while (0)

#define ia64_padd1_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 0)
#define ia64_padd2_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 0, 0)
#define ia64_padd4_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 0, 0)
#define ia64_padd1_sss_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 1)
#define ia64_padd2_sss_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 0, 1)
#define ia64_padd1_uuu_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 2)
#define ia64_padd2_uuu_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 0, 2)
#define ia64_padd1_uus_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 3)
#define ia64_padd2_uus_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 0, 3)

#define ia64_psub1_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 1, 0)
#define ia64_psub2_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 1, 0)
#define ia64_psub4_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 1, 0)
#define ia64_psub1_sss_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 1, 1)
#define ia64_psub2_sss_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 1, 1)
#define ia64_psub1_uuu_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 1, 2)
#define ia64_psub2_uuu_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 1, 2)
#define ia64_psub1_uus_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 1, 3)
#define ia64_psub2_uus_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 1, 3)

#define ia64_pavg1_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 2)
#define ia64_pavg2_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 2, 2)
#define ia64_pavg1_raz_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 3)
#define ia64_pavg2_raz_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 2, 3)
#define ia64_pavgsub1_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 3, 2)
#define ia64_pavgsub2_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 3, 2)
#define ia64_pcmp1_eq_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 9, 0)
#define ia64_pcmp2_eq_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 9, 0)
#define ia64_pcmp4_eq_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 9, 0)
#define ia64_pcmp1_gt_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 9, 1)
#define ia64_pcmp2_gt_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 9, 1)
#define ia64_pcmp4_gt_pred(code, qp,r1,r2,r3) ia64_a9 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 9, 1)

#define ia64_a10(code, qp, r1, r2, r3, x2a, za, zb, x4, ct2d) do { check_gregs ((r1), (r2), (r3)); check_count2 ((ct2d)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_A, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (ct2d) - 1, 27, (x4), 29, (zb), 33, (x2a), 34, (za), 36, (8), 37); } while (0)

#define ia64_pshladd2_pred(code, qp, r1, r2, r3, count) ia64_a10 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 4, count);
#define ia64_pshradd2_pred(code, qp, r1, r2, r3, count) ia64_a10 ((code), (qp), (r1), (r2), (r3), 1, 0, 1, 6, count);

#define encode_pmpyshr_count(count) (((count) == 0) ? 0 : (((count) == 7) ? 1 : (((count) == 15) ? 2 : 3)))

#define ia64_i1(code, qp, r1, r2, r3, za, zb, ve, x2a, x2b, ct2d) do { check_gregs ((r1), (r2), (r3)); check_assert (((ct2d) == 0) | ((ct2d) == 7) | ((ct2d) == 15) | ((ct2d) == 16)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 28, encode_pmpyshr_count((ct2d)), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pmpyshr2_pred(code, qp, r1, r2, r3, count) ia64_i1 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 0, 3, (count));

#define ia64_pmpyshr2_u_pred(code, qp, r1, r2, r3, count) ia64_i1 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 0, 1, (count));

#define ia64_i2(code, qp, r1, r2, r3, za, zb, ve, x2a, x2b, x2c) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pmpy2_r_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 1, 3)
#define ia64_pmpy2_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 3, 3)
#define ia64_mix1_r_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 0, 2)
#define ia64_mix2_r_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 0, 2)
#define ia64_mix4_r_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 0, 2)
#define ia64_mix1_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 2, 2)
#define ia64_mix2_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 0, 2)
#define ia64_mix4_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 0, 2)
#define ia64_pack2_uss_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 0, 0)
#define ia64_pack2_sss_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 2, 0)
#define ia64_pack4_sss_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 2, 0)
#define ia64_unpack1_h_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 0, 1)
#define ia64_unpack2_h_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 0, 1)
#define ia64_unpack4_h_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 0, 1)
#define ia64_unpack1_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 2, 1)
#define ia64_unpack2_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 2, 1)
#define ia64_unpack4_l_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 2, 2, 1)
#define ia64_pmin1_u_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 1, 0)
#define ia64_pmax1_u_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 1, 1)
#define ia64_pmin2_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 3, 0)
#define ia64_pmax2_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 2, 3, 1)
#define ia64_psad1_pred(code, qp,  r1, r2, r3) ia64_i2 ((code), (qp), (r1), (r2), (r3), 0, 0, 0, 2, 3, 2)

typedef enum {
	IA64_MUX1_BRCST = 0x0,
	IA64_MUX1_MIX   = 0x8,
	IA64_MUX1_SHUF  = 0x9,
	IA64_MUX1_ALT   = 0xa,
	IA64_MUX1_REV   = 0xb
} Ia64Mux1Permutation;
 
#define ia64_i3(code, qp, r1, r2, mbtype, opcode, za, zb, ve, x2a, x2b, x2c) do { check_greg ((r1)); check_greg ((r2)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (mbtype), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (opcode), 37); } while (0)

#define ia64_mux1_pred(code, qp, r1, r2, mbtype) ia64_i3 ((code), (qp), (r1), (r2), (mbtype), 7, 0, 0, 0, 3, 2, 2)

#define ia64_i4(code, qp, r1, r2, mhtype, opcode, za, zb, ve, x2a, x2b, x2c) do { check_greg ((r1)); check_greg ((r2)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (mhtype), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (opcode), 37); } while (0)

#define ia64_mux2_pred(code, qp, r1, r2, mhtype) ia64_i4 ((code), (qp), (r1), (r2), (mhtype), 7, 0, 1, 0, 3, 2, 2)

#define ia64_i5(code, qp, r1, r2, r3, za, zb, ve, x2a, x2b, x2c) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pshr2_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 0, 2, 0)
#define ia64_pshr4_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 2, 0)
#define ia64_shr_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 0, 2, 0)
#define ia64_pshr2_u_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 0, 0, 0)
#define ia64_pshr4_u_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 0, 0)
#define ia64_shr_u_pred(code, qp, r1, r3, r2) ia64_i5 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 0, 0, 0)

#define ia64_i6(code, qp, r1, count, r3, za, zb, ve, x2a, x2b, x2c) do { check_greg ((r1)); check_greg ((r3)); check_count5 ((count)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (count), 14, (r3), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pshr2_imm_pred(code, qp, r1, r3, count) ia64_i6 ((code), (qp), (r1), (count), (r3), 0, 1, 0, 1, 3, 0)
#define ia64_pshr4_imm_pred(code, qp, r1, r3, count) ia64_i6 ((code), (qp), (r1), (count), (r3), 1, 0, 0, 1, 3, 0)
#define ia64_pshr2_u_imm_pred(code, qp, r1, r3, count) ia64_i6 ((code), (qp), (r1), (count), (r3), 0, 1, 0, 1, 1, 0)
#define ia64_pshr4_u_imm_pred(code, qp, r1, r3, count) ia64_i6 ((code), (qp), (r1), (count), (r3), 1, 0, 0, 1, 1, 0)

#define ia64_i7(code, qp, r1, r2, r3, za, zb, ve, x2a, x2b, x2c) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pshl2_pred(code, qp, r1, r2, r3) ia64_i7 ((code), (qp), (r1), (r2), (r3), 0, 1, 0, 0, 0, 1)
#define ia64_pshl4_pred(code, qp, r1, r2, r3) ia64_i7 ((code), (qp), (r1), (r2), (r3), 1, 0, 0, 0, 0, 1)
#define ia64_shl_pred(code, qp, r1, r2, r3) ia64_i7 ((code), (qp), (r1), (r2), (r3), 1, 1, 0, 0, 0, 1)

#define ia64_i8(code, qp, r1, r2, count, za, zb, ve, x2a, x2b, x2c) do { check_gregs ((r1), (r2), 0); check_count5 ((count)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, 31 - (count), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_pshl2_imm_pred(code, qp, r1, r2, count) ia64_i8 ((code), (qp), (r1), (r2), (count), 0, 1, 0, 3, 1, 1)
#define ia64_pshl4_imm_pred(code, qp, r1, r2, count) ia64_i8 ((code), (qp), (r1), (r2), (count), 1, 0, 0, 3, 1, 1)

#define ia64_i9(code, qp, r1, r3, za, zb, ve, x2a, x2b, x2c) do { check_gregs ((r1), 0, (r3)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, 0, 13, (r3), 20, (x2b), 28, (x2c), 30, (ve), 32, (zb), 33, (x2a), 34, (za), 36, (7), 37); } while (0)

#define ia64_popcnt_pred(code, qp, r1, r3) ia64_i9 ((code), (qp), (r1), (r3), 0, 1, 0, 1, 1, 2)

#define ia64_i10(code, qp, r1, r2, r3, count, opcode, x2, x) do { check_gregs ((r1), (r2), (r3)); check_count6 ((count)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (count), 27, (x), 33, (x2), 34, (opcode), 37); } while (0)

#define ia64_shrp_pred(code, qp, r1, r2, r3, count) ia64_i10 ((code), (qp), (r1), (r2), ( r3), (count), 5, 3, 0)

#define ia64_i11(code, qp, r1, r3, pos, len, x2, x, y) do { ia64_emit_ins_8 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, ((pos) << 1) | (y), 13, (r3), 20, (len) - 1, 27, (x), 33, (x2), 34, (5), 37); } while (0)

#define ia64_extr_u_pred(code, qp, r1, r3, pos, len) ia64_i11 ((code), (qp), (r1), (r3), (pos), (len), 1, 0, 0)
#define ia64_extr_pred(code, qp, r1, r3, pos, len) ia64_i11 ((code), (qp), (r1), (r3), (pos), (len), 1, 0, 1)

#define ia64_i12(code, qp, r1, r2, pos, len, x2, x, y) do { ia64_emit_ins_8 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (63 - (pos)) | ((y) << 6), 20, (len) - 1, 27, (x), 33, (x2), 34, (5), 37); } while (0)

#define ia64_dep_z_pred(code, qp, r1, r2, pos, len) ia64_i12 ((code), (qp), (r1), (r2), (pos), (len), 1, 1, 0)

#define ia64_i13(code, qp, r1, imm, pos, len, x2, x, y) do { ia64_emit_ins_9 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, ((guint64)(imm) & 0x7f), 13, (63 - (pos)) | ((y) << 6), 20, (len) - 1, 27, (x), 33, (x2), 34, sign_bit ((imm)), 36, (5), 37); } while (0)

#define ia64_dep_z_imm_pred(code, qp, r1, imm, pos, len) ia64_i13 ((code), (qp), (r1), (imm), (pos), (len), 1, 1, 1)

#define ia64_i14(code, qp, r1, imm, r3, pos, len, x2, x) do { check_imm1 (imm); ia64_emit_ins_9 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (63 - (pos)) << 1, 13, (r3), 20, (len), 27, (x), 33, (x2), 34, sign_bit ((imm)), 36, (5), 37); } while (0)

#define ia64_dep_imm_pred(code, qp, r1, imm, r3, pos, len) ia64_i14 ((code), (qp), (r1), (imm), (r3), (pos), (len), 3, 1)

#define ia64_i15(code, qp, r1, r2, r3, pos, len) do { check_len4 ((len)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (len) - 1, 27, (63 - (pos)), 31, (4), 37); } while (0)

#define ia64_dep_pred(code, qp, r1, r2, r3, pos, len) ia64_i15 ((code), (qp), (r1), (r2), (r3), (pos), (len))

#define ia64_i16(code, qp, p1, p2, r3, pos, x2, ta, tb, y, c) do { check_pregs ((p1), (p2)); ia64_emit_ins_11 ((code), IA64_INS_TYPE_I, (qp), 0, (p1), 6, (c), 12, (y), 13, (pos), 14, (r3), 20, (p2), 27, (ta), 33, (x2), 34, (tb), 36, (5), 37); } while (0)

#define ia64_tbit_z_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 0, 0, 0, 0)
#define ia64_tbit_z_unc_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 0, 0, 0, 1)
#define ia64_tbit_z_and_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 0, 1, 0, 0)
#define ia64_tbit_nz_and_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 0, 1, 0, 1)
#define ia64_tbit_z_or_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 1, 0, 0, 0)
#define ia64_tbit_nz_or_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 1, 0, 0, 1)
#define ia64_tbit_z_or_andcm_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 1, 1, 0, 0)
#define ia64_tbit_nz_or_andcm_pred(code, qp, p1, p2, r3, pos) ia64_i16 ((code), (qp), (p1), (p2), (r3), (pos), 0, 1, 1, 0, 1)

#define ia64_i17(code, qp, p1, p2, r3, x2, ta, tb, y, c) do { check_pregs ((p1), (p2)); ia64_emit_ins_10 ((code), IA64_INS_TYPE_I, (qp), 0, (p1), 6, (c), 12, (y), 13, (r3), 20, (p2), 27, (ta), 33, (x2), 34, (tb), 36, (5), 37); } while (0)

#define ia64_tnat_z_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 0, 0, 1, 0)
#define ia64_tnat_z_unc_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 0, 0, 1, 1)
#define ia64_tnat_z_and_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 0, 1, 1, 0)
#define ia64_tnat_nz_and_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 0, 1, 1, 1)
#define ia64_tnat_z_or_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 1, 0, 1, 0)
#define ia64_tnat_nz_or_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 1, 0, 1, 1)
#define ia64_tnat_z_or_andcm_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 1, 1, 1, 0)
#define ia64_tnat_nz_or_andcm_pred(code, qp, p1, p2, r3) ia64_i17 ((code), (qp), (p1), (p2), (r3), 0, 1, 1, 1, 1)

#define ia64_i18(code, qp, imm, x3, x6, y) do { ia64_emit_ins_7 ((code), IA64_INS_TYPE_I, (qp), 0, (imm) & 0xfffff, 6, (y), 26, (x6), 27, (x3), 33, ((imm) >> 20) & 0x1, 36, (0), 37); } while (0)

#define ia64_nop_i_pred(code, qp, imm) ia64_i18 ((code), (qp), (imm), 0, 1, 0)
#define ia64_hint_i_pred(code, qp, imm) ia64_i18 ((code), (qp), (imm), 0, 1, 1)

#define ia64_i19(code, qp, imm, x3, x6) do { check_imm21 ((imm)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_I, (qp), 0, (imm) & 0xfffff, 6, (x6), 27, (x3), 33, ((imm) >> 20) & 0x1, 36, (0), 37); } while (0)

#define ia64_break_i_pred(code, qp, imm) ia64_i19 ((code), (qp), (imm), 0, 0)

#define ia64_i20(code, qp, r2, imm, x3) do { check_imm21 ((imm)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_I, (qp), 0, (imm) & 0x7f, 6, (r2), 13, ((imm) >> 7) & 0x1fff, 20, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_chk_s_i_pred(code, qp,r2,disp) ia64_i20 ((code), (qp), (r2), (disp), 1)

#define ia64_i21(code, qp, b1, r2, tag13, x3, x, ih, wh) do { check_imm8 (tag13); check_gregs (0, (r2), 0); check_breg ((b1)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_I, (qp), 0, (b1), 6, (r2), 13, (wh), 20, (x), 22, (ih), 23, (tag13) & 0x1ff, 24, (x3), 33, (0), 37); } while (0)

typedef enum {
	IA64_MOV_TO_BR_WH_SPTK = 0,
	IA64_MOV_TO_BR_WH_NONE = 1,
	IA64_MOV_TO_BR_WH_DPTK = 2
} Ia64MovToBrWhetherHint;

typedef enum {
	IA64_BR_IH_NONE = 0,
	IA64_BR_IH_IMP = 1
} Ia64BranchImportanceHint;

#define ia64_mov_to_br_pred(code, qp, b1, r2, disp, wh, ih) ia64_i21 ((code), (qp), (b1), (r2), (disp), 7, 0, ih, wh)
#define ia64_mov_ret_to_br_pred(code, qp, b1, r2, disp, wh, ih) ia64_i21 ((code), (qp), (b1), (r2), (disp), 7, 1, ih, wh)

#define ia64_i22(code, qp, r1, b2, x3, x6) do { check_gregs ((r1), 0, 0); check_breg ((b2)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (b2), 13, (x6), 27, (x3), 33, (0), 37); } while (0)

#define ia64_mov_from_br_pred(code, qp, r1, b2) ia64_i22 ((code), (qp), (r1), (b2), 0, 0x31);

#define ia64_i23(code, qp, r2, mask, x3) do { check_greg ((r2)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_I, (qp), 0, (mask) & 0x7f, 6, (r2), 13, ((mask) >> 7) & 0xff, 24, (x3), 33, sign_bit ((mask)), 36, (0), 37); } while (0)

#define ia64_mov_to_pred_pred(code, qp, r2, mask) ia64_i23 ((code), (qp), (r2), (mask) >> 1, 3)

#define ia64_i24(code, qp, imm, x3) do { ia64_emit_ins_5 ((code), IA64_INS_TYPE_I, (qp), 0, (imm) & 0x7ffffff, 6, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_mov_to_pred_rot_imm_pred(code, qp,imm) ia64_i24 ((code), (qp), (imm) >> 16, 2)

#define ia64_i25(code, qp, r1, x3, x6) do { check_greg ((r1)); ia64_emit_ins_5 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (x6), 27, (x3), 33, (0), 37); } while (0)

#define ia64_mov_from_ip_pred(code, qp, r1) ia64_i25 ((code), (qp), (r1), 0, 0x30)
#define ia64_mov_from_pred_pred(code, qp, r1) ia64_i25 ((code), (qp), (r1), 0, 0x33)

#define ia64_i26(code, qp, ar3, r2, x3, x6) do { check_greg ((r2)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_I, (qp), 0, (r2), 13, (ar3), 20, (x6), 27, (x3), 33, (0), 37); } while (0)

#define ia64_mov_to_ar_i_pred(code, qp, ar3, r2) ia64_i26 ((code), (qp), (ar3), (r2), 0, 0x2a)

#define ia64_i27(code, qp, ar3, imm, x3, x6) do { check_imm8 ((imm)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_I, (qp), 0, (imm) & 0x7f, 13, (ar3), 20, (x6), 27, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_mov_to_ar_imm_i_pred(code, qp, ar3, imm) ia64_i27 ((code), (qp), (ar3), (imm), 0, 0x0a)

#define ia64_i28(code, qp, r1, ar3, x3, x6) do { check_greg ((r1)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (ar3), 20, (x6), 27, (x3), 33, (0), 37); } while (0)

#define ia64_mov_from_ar_i_pred(code, qp, r1, ar3) ia64_i28 ((code), (qp), (r1), (ar3), 0, 0x32)

#define ia64_i29(code, qp, r1, r3, x3, x6) do { check_gregs ((r1), 0, (r3)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_I, (qp), 0, (r1), 6, (r3), 20, (x6), 27, (x3), 33, (0), 37); } while (0)

#define ia64_zxt1_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x10)
#define ia64_zxt2_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x11)
#define ia64_zxt4_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x12)
#define ia64_sxt1_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x14)
#define ia64_sxt2_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x15)
#define ia64_sxt4_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x16)
#define ia64_czx1_l_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x18)
#define ia64_czx2_l_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x19)
#define ia64_czx1_r_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x1C)
#define ia64_czx2_r_pred(code, qp, r1, r3) ia64_i29 ((code), (qp), (r1), (r3), 0, 0x1D)

/*
 * M Instruction Type
 */

typedef enum {
	IA64_LD_HINT_NONE = 0,
	IA64_LD_HINT_NT1  = 1,
	IA64_LD_HINT_NTA  = 3
} Ia64LoadHint;

typedef enum {
	IA64_ST_HINT_NONE = 0,
	IA64_ST_HINT_NTA  = 3
} Ia64StoreHint;

#define ia64_m1(code, qp, r1, r3, hint, m, x, x6) do { check_gregs ((r1), 0, (r3)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_ld1_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x00)
#define ia64_ld2_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x01)
#define ia64_ld4_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x02)
#define ia64_ld8_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x03)

#define ia64_ld1_s_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x04)
#define ia64_ld2_s_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x05)
#define ia64_ld4_s_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x06)
#define ia64_ld8_s_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x07)

#define ia64_ld1_a_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x08)
#define ia64_ld2_a_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x09)
#define ia64_ld4_a_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0A)
#define ia64_ld8_a_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0B)

#define ia64_ld1_sa_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0C)
#define ia64_ld2_sa_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0D)
#define ia64_ld4_sa_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0E)
#define ia64_ld8_sa_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x0F)

#define ia64_ld1_bias_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x10)
#define ia64_ld2_bias_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x11)
#define ia64_ld4_bias_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x12)
#define ia64_ld8_bias_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x13)

#define ia64_ld1_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x14)
#define ia64_ld2_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x15)
#define ia64_ld4_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x16)
#define ia64_ld8_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x17)

#define ia64_ld8_fill_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x1B)

#define ia64_ld1_c_clr_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x20)
#define ia64_ld2_c_clr_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x21)
#define ia64_ld4_c_clr_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x22)
#define ia64_ld8_c_clr_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x23)

#define ia64_ld1_c_nc_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x24)
#define ia64_ld2_c_nc_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x25)
#define ia64_ld4_c_nc_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x26)
#define ia64_ld8_c_nc_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x27)

#define ia64_ld1_c_clr_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x28)
#define ia64_ld2_c_clr_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x29)
#define ia64_ld4_c_clr_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x2A)
#define ia64_ld8_c_clr_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 0, 0x2B)

#define ia64_ld16_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 1, 0x28)
#define ia64_ld16_acq_hint_pred(code, qp, r1, r3, hint) ia64_m1 ((code), (qp), (r1), (r3), (hint), 0, 1, 0x2C)

#define ia64_m2(code, qp, r1, r2, r3, hint, m, x, x6) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_ld1_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x00)
#define ia64_ld2_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x01)
#define ia64_ld4_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x02)
#define ia64_ld8_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x03)

#define ia64_ld1_s_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x04)
#define ia64_ld2_s_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x05)
#define ia64_ld4_s_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x06)
#define ia64_ld8_s_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x07)

#define ia64_ld1_a_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x08)
#define ia64_ld2_a_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x09)
#define ia64_ld4_a_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0A)
#define ia64_ld8_a_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0B)

#define ia64_ld1_sa_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0C)
#define ia64_ld2_sa_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0D)
#define ia64_ld4_sa_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0E)
#define ia64_ld8_sa_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x0F)

#define ia64_ld1_bias_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x10)
#define ia64_ld2_bias_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x11)
#define ia64_ld4_bias_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x12)
#define ia64_ld8_bias_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x13)

#define ia64_ld1_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x14)
#define ia64_ld2_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x15)
#define ia64_ld4_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x16)
#define ia64_ld8_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x17)

#define ia64_ld8_fill_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x1B)

#define ia64_ld1_c_clr_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x20)
#define ia64_ld2_c_clr_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x21)
#define ia64_ld4_c_clr_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x22)
#define ia64_ld8_c_clr_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x23)

#define ia64_ld1_c_nc_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x24)
#define ia64_ld2_c_nc_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x25)
#define ia64_ld4_c_nc_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x26)
#define ia64_ld8_c_nc_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x27)

#define ia64_ld1_c_clr_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x28)
#define ia64_ld2_c_clr_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x29)
#define ia64_ld4_c_clr_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x2A)
#define ia64_ld8_c_clr_acq_inc_hint_pred(code, qp, r1, r2, r3, hint) ia64_m2 ((code), (qp), (r1), (r2), (r3), (hint), 1, 0, 0x2B)

#define ia64_m3(code, qp, r1, r3, imm, hint, m, x, x6) do { check_gregs ((r1), 0, (r3)); check_imm9 ((imm)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (imm) & 0x7f, 13, (r3), 20, ((imm) >> 7) & 0x1, 27, (hint), 28, (x6), 30, sign_bit ((imm)), 36, (5), 37); } while (0)

#define ia64_ld1_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x00)
#define ia64_ld2_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x01)
#define ia64_ld4_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x02)
#define ia64_ld8_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x03)

#define ia64_ld1_s_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x04)
#define ia64_ld2_s_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x05)
#define ia64_ld4_s_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x06)
#define ia64_ld8_s_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x07)

#define ia64_ld1_a_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x08)
#define ia64_ld2_a_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x09)
#define ia64_ld4_a_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0A)
#define ia64_ld8_a_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0B)

#define ia64_ld1_sa_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0C)
#define ia64_ld2_sa_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0D)
#define ia64_ld4_sa_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0E)
#define ia64_ld8_sa_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x0F)

#define ia64_ld1_bias_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x10)
#define ia64_ld2_bias_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x11)
#define ia64_ld4_bias_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x12)
#define ia64_ld8_bias_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x13)

#define ia64_ld1_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x14)
#define ia64_ld2_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x15)
#define ia64_ld4_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x16)
#define ia64_ld8_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x17)

#define ia64_ld8_fill_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x1B)

#define ia64_ld1_c_clr_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x20)
#define ia64_ld2_c_clr_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x21)
#define ia64_ld4_c_clr_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x22)
#define ia64_ld8_c_clr_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x23)

#define ia64_ld1_c_nc_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x24)
#define ia64_ld2_c_nc_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x25)
#define ia64_ld4_c_nc_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x26)
#define ia64_ld8_c_nc_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x27)

#define ia64_ld1_c_clr_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x28)
#define ia64_ld2_c_clr_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x29)
#define ia64_ld4_c_clr_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x2A)
#define ia64_ld8_c_clr_acq_inc_imm_hint_pred(code, qp, r1, r3, imm, hint) ia64_m3 ((code), (qp), (r1), (r3), (imm), (hint), 1, 0, 0x2B)

#define ia64_m4(code, qp, r3, r2, hint, m, x, x6) do { check_gregs (0, (r2), (r3)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (r2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_st1_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x30)
#define ia64_st2_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x31)
#define ia64_st4_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x32)
#define ia64_st8_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x33)

#define ia64_st1_rel_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x34)
#define ia64_st2_rel_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x35)
#define ia64_st4_rel_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x36)
#define ia64_st8_rel_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x37)

#define ia64_st8_spill_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 0, 0x3B)

#define ia64_st16_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 1, 0x30)
#define ia64_st16_rel_hint_pred(code, qp, r3, r2, hint) ia64_m4 ((code), (qp), (r3), (r2), (hint), 0, 1, 0x34)

#define ia64_m5(code, qp, r3, r2, imm, hint, m, x, x6) do { check_gregs (0, (r2), (r3)); check_imm9 ((imm)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 6, (r2), 13, (r3), 20, ((imm) >> 7) & 0x1, 27, (hint), 28, (x6), 30, sign_bit ((imm)), 36, (5), 37); } while (0)

#define ia64_st1_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x30)
#define ia64_st2_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x31)
#define ia64_st4_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x32)
#define ia64_st8_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x33)

#define ia64_st1_rel_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x34)
#define ia64_st2_rel_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x35)
#define ia64_st4_rel_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x36)
#define ia64_st8_rel_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x37)

#define ia64_st8_spill_inc_imm_hint_pred(code, qp, r3, r2, imm, hint) ia64_m5 ((code), (qp), (r3), (r2), (imm), (hint), 0, 0, 0x3B)

#define ia64_m6(code, qp, f1, r3, hint, m, x, x6) do { check_greg ((r3)); check_freg ((f1)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_ldfs_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x02)
#define ia64_ldfd_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x03)
#define ia64_ldf8_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x01)
#define ia64_ldfe_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x00)

#define ia64_ldfs_s_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x06)
#define ia64_ldfd_s_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x07)
#define ia64_ldf8_s_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x05)
#define ia64_ldfe_s_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x04)

#define ia64_ldfs_a_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0A)
#define ia64_ldfd_a_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0B)
#define ia64_ldf8_a_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x09)
#define ia64_ldfe_a_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x08)

#define ia64_ldfs_sa_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0E)
#define ia64_ldfd_sa_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0F)
#define ia64_ldf8_sa_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0D)
#define ia64_ldfe_sa_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x0C)

#define ia64_ldfs_c_clr_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x22)
#define ia64_ldfd_c_clr_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x23)
#define ia64_ldf8_c_clr_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x21)
#define ia64_ldfe_c_clr_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x20)

#define ia64_ldfs_c_nc_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x26)
#define ia64_ldfd_c_nc_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x27)
#define ia64_ldf8_c_nc_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x25)
#define ia64_ldfe_c_nc_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x24)

#define ia64_ldf_fill_hint_pred(code, qp, f1, r3, hint) ia64_m6 ((code), (qp), (f1), (r3), (hint), 0, 0, 0x1B)

#define ia64_m7(code, qp, f1, r3, r2, hint, m, x, x6) do { check_greg ((r3)); check_freg ((f1)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (r2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_ldfs_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x02)
#define ia64_ldfd_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x03)
#define ia64_ldf8_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x01)
#define ia64_ldfe_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x00)

#define ia64_ldfs_s_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x06)
#define ia64_ldfd_s_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x07)
#define ia64_ldf8_s_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x05)
#define ia64_ldfe_s_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x04)

#define ia64_ldfs_a_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0A)
#define ia64_ldfd_a_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0B)
#define ia64_ldf8_a_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x09)
#define ia64_ldfe_a_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x08)

#define ia64_ldfs_sa_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0E)
#define ia64_ldfd_sa_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0F)
#define ia64_ldf8_sa_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0D)
#define ia64_ldfe_sa_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x0C)

#define ia64_ldfs_c_clr_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x22)
#define ia64_ldfd_c_clr_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x23)
#define ia64_ldf8_c_clr_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x21)
#define ia64_ldfe_c_clr_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x20)

#define ia64_ldfs_c_nc_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x26)
#define ia64_ldfd_c_nc_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x27)
#define ia64_ldf8_c_nc_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x25)
#define ia64_ldfe_c_nc_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x24)

#define ia64_ldf_fill_inc_hint_pred(code, qp, f1, r3, r2, hint) ia64_m7 ((code), (qp), (f1), (r3), (r2), (hint), 1, 0, 0x1B)

#define ia64_m8(code, qp, f1, r3, imm, hint, x6) do { check_greg ((r3)); check_imm9 ((imm)); check_freg ((f1)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (imm) & 0x7f, 13, (r3), 20, ((imm) >> 7) & 0x1, 27, (hint), 28, (x6), 30, sign_bit ((imm)), 36, (7), 37); } while (0)

#define ia64_ldfs_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x02)
#define ia64_ldfd_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x03)
#define ia64_ldf8_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x01)
#define ia64_ldfe_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x00)

#define ia64_ldfs_s_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x06)
#define ia64_ldfd_s_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x07)
#define ia64_ldf8_s_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x05)
#define ia64_ldfe_s_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x04)

#define ia64_ldfs_a_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0A)
#define ia64_ldfd_a_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0B)
#define ia64_ldf8_a_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x09)
#define ia64_ldfe_a_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x08)

#define ia64_ldfs_sa_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0E)
#define ia64_ldfd_sa_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0F)
#define ia64_ldf8_sa_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0D)
#define ia64_ldfe_sa_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x0C)

#define ia64_ldfs_c_clr_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x22)
#define ia64_ldfd_c_clr_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x23)
#define ia64_ldf8_c_clr_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x21)
#define ia64_ldfe_c_clr_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x20)

#define ia64_ldfs_c_nc_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x26)
#define ia64_ldfd_c_nc_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x27)
#define ia64_ldf8_c_nc_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x25)
#define ia64_ldfe_c_nc_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x24)

#define ia64_ldf_fill_inc_imm_hint_pred(code, qp, f1, r3, imm, hint) ia64_m8 ((code), (qp), (f1), (r3), (imm), (hint), 0x1B)

#define ia64_m9(code, qp, r3, f2, hint, m, x, x6) do { check_greg ((r3)); check_freg ((f2)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (f2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_stfs_hint_pred(code, qp, r3, f2, hint) ia64_m9 ((code), (qp), (r3), (f2), (hint), 0, 0, 0x32)
#define ia64_stfd_hint_pred(code, qp, r3, f2, hint) ia64_m9 ((code), (qp), (r3), (f2), (hint), 0, 0, 0x33)
#define ia64_stf8_hint_pred(code, qp, r3, f2, hint) ia64_m9 ((code), (qp), (r3), (f2), (hint), 0, 0, 0x31)
#define ia64_stfe_hint_pred(code, qp, r3, f2, hint) ia64_m9 ((code), (qp), (r3), (f2), (hint), 0, 0, 0x30)
#define ia64_stf_spill_hint_pred(code, qp, r3, f2, hint) ia64_m9 ((code), (qp), (r3), (f2), (hint), 0, 0, 0x3B)

#define ia64_m10(code, qp, r3, f2, imm, hint, x6) do { check_greg ((r3)); check_freg ((f2)); check_imm9 ((imm)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 6, (f2), 13, (r3), 20, ((imm) >> 7) & 0x1, 27, (hint), 28, (x6), 30, sign_bit ((imm)), 36, (7), 37); } while (0)

#define ia64_stfs_inc_imm_hint_pred(code, qp, r3, f2, imm, hint) ia64_m10 ((code), (qp), (r3), (f2), (imm), (hint), 0x32)
#define ia64_stfd_inc_imm_hint_pred(code, qp, r3, f2, imm, hint) ia64_m10 ((code), (qp), (r3), (f2), (imm), (hint), 0x33)
#define ia64_stf8_inc_imm_hint_pred(code, qp, r3, f2, imm, hint) ia64_m10 ((code), (qp), (r3), (f2), (imm), (hint), 0x31)
#define ia64_stfe_inc_imm_hint_pred(code, qp, r3, f2, imm, hint) ia64_m10 ((code), (qp), (r3), (f2), (imm), (hint), 0x30)
#define ia64_stf_spill_inc_imm_hint_pred(code, qp, r3, f2, imm, hint) ia64_m10 ((code), (qp), (r3), (f2), (imm), (hint), 0x3B)

#define ia64_m11(code, qp, f1, f2, r3, hint, m, x, x6) do { check_greg ((r3)); check_freg ((f1)); check_freg ((f2)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (f2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_ldfps_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x02)
#define ia64_ldfpd_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x03)
#define ia64_ldfp8_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x01)

#define ia64_ldfps_s_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x06)
#define ia64_ldfpd_s_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x07)
#define ia64_ldfp8_s_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x05)

#define ia64_ldfps_a_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x0A)
#define ia64_ldfpd_a_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x0B)
#define ia64_ldfp8_a_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x09)

#define ia64_ldfps_sa_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x0E)
#define ia64_ldfpd_sa_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x0F)
#define ia64_ldfp8_sa_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x0D)

#define ia64_ldfps_c_clr_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x22)
#define ia64_ldfpd_c_clr_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x23)
#define ia64_ldfp8_c_clr_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x21)

#define ia64_ldfps_c_nc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x26)
#define ia64_ldfpd_c_nc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x27)
#define ia64_ldfp8_c_nc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m11 ((code), (qp), (f1), (f2), (r3), (hint), 0, 1, 0x25)

#define ia64_m12(code, qp, f1, f2, r3, hint, m, x, x6) do { check_greg ((r3)); check_freg ((f1)); check_freg ((f2)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (f2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_ldfps_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x02)
#define ia64_ldfpd_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x03)
#define ia64_ldfp8_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x01)

#define ia64_ldfps_s_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x06)
#define ia64_ldfpd_s_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x07)
#define ia64_ldfp8_s_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x05)

#define ia64_ldfps_a_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x0A)
#define ia64_ldfpd_a_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x0B)
#define ia64_ldfp8_a_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x09)

#define ia64_ldfps_sa_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x0E)
#define ia64_ldfpd_sa_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x0F)
#define ia64_ldfp8_sa_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x0D)

#define ia64_ldfps_c_clr_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x22)
#define ia64_ldfpd_c_clr_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x23)
#define ia64_ldfp8_c_clr_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x21)

#define ia64_ldfps_c_nc_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x26)
#define ia64_ldfpd_c_nc_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x27)
#define ia64_ldfp8_c_nc_inc_hint_pred(code, qp, f1, f2, r3, hint) ia64_m12 ((code), (qp), (f1), (f2), (r3), (hint), 1, 1, 0x25)

typedef enum {
	IA64_LFHINT_NONE = 0,
	IA64_LFHINT_NT1 = 1,
	IA64_LFHINT_NT2 = 2,
	IA64_LFHINT_NTA = 3
} Ia64LinePrefetchHint;

#define ia64_m13(code, qp, r3, hint, m, x, x6) do { check_greg ((r3)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_lfetch_hint_pred(code, qp, r3, hint) ia64_m13 ((code), (qp), (r3), (hint), 0, 0, 0x2C)
#define ia64_lfetch_excl_hint_pred(code, qp, r3, hint) ia64_m13 ((code), (qp), (r3), (hint), 0, 0, 0x2D)
#define ia64_lfetch_fault_hint_pred(code, qp, r3, hint) ia64_m13 ((code), (qp), (r3), (hint), 0, 0, 0x2E)
#define ia64_lfetch_fault_excl_hint_pred(code, qp, r3, hint) ia64_m13 ((code), (qp), (r3), (hint), 0, 0, 0x2F)

#define ia64_m14(code, qp, r3, r2, hint, m, x, x6) do { check_greg ((r3)); check_greg ((r2)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (r2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_lfetch_inc_hint_pred(code, qp, r3, r2, hint) ia64_m14 ((code), (qp), (r3), (r2), (hint), 1, 0, 0x2C)
#define ia64_lfetch_excl_inc_hint_pred(code, qp, r3, r2, hint) ia64_m14 ((code), (qp), (r3), (r2), (hint), 1, 0, 0x2D)
#define ia64_lfetch_fault_inc_hint_pred(code, qp, r3, r2, hint) ia64_m14 ((code), (qp), (r3), (r2), (hint), 1, 0, 0x2E)
#define ia64_lfetch_fault_excl_inc_hint_pred(code, qp, r3, r2, hint) ia64_m14 ((code), (qp), (r3), (r2), (hint), 1, 0, 0x2F)

#define ia64_m15(code, qp, r3, imm, hint, x6) do { check_greg ((r3)); check_imm9 ((imm)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 13, (r3), 20, ((imm) >> 7) & 0x1, 27, (hint), 28, (x6), 30, sign_bit ((imm)), 36, (7), 37); } while (0)

#define ia64_lfetch_inc_imm_hint_pred(code, qp, r3, imm, hint) ia64_m15 ((code), (qp), (r3), (imm), (hint), 0x2C)
#define ia64_lfetch_excl_inc_imm_hint_pred(code, qp, r3, imm, hint) ia64_m15 ((code), (qp), (r3), (imm), (hint), 0x2D)
#define ia64_lfetch_fault_inc_imm_hint_pred(code, qp, r3, imm, hint) ia64_m15 ((code), (qp), (r3), (imm), (hint), 0x2E)
#define ia64_lfetch_fault_excl_inc_imm_hint_pred(code, qp, r3, imm, hint) ia64_m15 ((code), (qp), (r3), (imm), (hint), 0x2F)

#define ia64_m16(code, qp, r1, r3, r2, hint, m, x, x6) do { check_gregs ((r1), (r2), (r3)); ia64_emit_ins_9 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (r2), 13, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_cmpxchg1_acq_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x00)
#define ia64_cmpxchg2_acq_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x01)
#define ia64_cmpxchg4_acq_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x02)
#define ia64_cmpxchg8_acq_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x03)
#define ia64_cmpxchg1_rel_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x04)
#define ia64_cmpxchg2_rel_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x05)
#define ia64_cmpxchg4_rel_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x06)
#define ia64_cmpxchg8_rel_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x07)
#define ia64_cmpxchg16_acq_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x20)
#define ia64_cmpxchg16_rel_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x24)
#define ia64_xchg1_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x08)
#define ia64_xchg2_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x09)
#define ia64_xchg4_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x0A)
#define ia64_xchg8_hint_pred(code, qp, r1, r3, r2, hint) ia64_m16 ((code), (qp), (r1), (r3), (r2), (hint), 0, 1, 0x0B)

#define encode_inc3(inc3) ((inc3) == 16 ? 0 : ((inc3) == 8 ? 1 : ((inc3) == 4 ? 2 : 3)))

#define ia64_m17(code, qp, r1, r3, imm, hint, m, x, x6) do { int aimm = (imm) < 0 ? - (imm) : (imm); check_gregs ((r1), 0, (r3)); check_assert ((aimm) == 16 || (aimm) == 8 || (aimm) == 4 || (aimm) == 1); ia64_emit_ins_10 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, encode_inc3 (aimm), 13, sign_bit ((imm)), 15, (r3), 20, (x), 27, (hint), 28, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_fetchadd4_acq_hint_pred(code, qp, r1, r3, inc, hint) ia64_m17 ((code), (qp), (r1), (r3), (inc), (hint), 0, 1, 0x12)
#define ia64_fetchadd8_acq_hint_pred(code, qp, r1, r3, inc, hint) ia64_m17 ((code), (qp), (r1), (r3), (inc), (hint), 0, 1, 0x12)
#define ia64_fetchadd4_rel_hint_pred(code, qp, r1, r3, inc, hint) ia64_m17 ((code), (qp), (r1), (r3), (inc), (hint), 0, 1, 0x16)
#define ia64_fetchadd8_rel_hint_pred(code, qp, r1, r3, inc, hint) ia64_m17 ((code), (qp), (r1), (r3), (inc), (hint), 0, 1, 0x17)

#define ia64_m18(code, qp, f1, r2, m, x, x6) do { check_greg ((r2)); check_freg ((f1)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (r2), 13, (x), 27, (x6), 30, (m), 36, (6), 37); } while (0)

#define ia64_setf_sig_pred(code, qp, f1, r2) ia64_m18 ((code), (qp), (f1), (r2), 0, 1, 0x1C)
#define ia64_setf_exp_pred(code, qp, f1, r2) ia64_m18 ((code), (qp), (f1), (r2), 0, 1, 0x1D)
#define ia64_setf_s_pred(code, qp, f1, r2) ia64_m18 ((code), (qp), (f1), (r2), 0, 1, 0x1E)
#define ia64_setf_d_pred(code, qp, f1, r2) ia64_m18 ((code), (qp), (f1), (r2), 0, 1, 0x1F)

#define ia64_m19(code, qp, r1, f2, m, x, x6) do { check_greg ((r1)); check_freg ((f2)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (f2), 13, (x), 27, (x6), 30, (m), 36, (4), 37); } while (0)

#define ia64_getf_sig_pred(code, qp, r1, f2) ia64_m19 ((code), (qp), (r1), (f2), 0, 1, 0x1C)
#define ia64_getf_exp_pred(code, qp, r1, f2) ia64_m19 ((code), (qp), (r1), (f2), 0, 1, 0x1D)
#define ia64_getf_s_pred(code, qp, r1, f2) ia64_m19 ((code), (qp), (r1), (f2), 0, 1, 0x1E)
#define ia64_getf_d_pred(code, qp, r1, f2) ia64_m19 ((code), (qp), (r1), (f2), 0, 1, 0x1F)

#define ia64_m20(code, qp, r2, imm, x3) do { check_greg ((r2)); check_imm21 ((imm)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 6, (r2), 13, ((imm) >> 7) & 0x1fff, 20, (x3), 33, sign_bit ((imm)), 36, (1), 37); } while (0)

#define ia64_chk_s_m_pred(code, qp,r2,disp) ia64_m20 ((code), (qp), (r2), (disp), 1)

#define ia64_m21(code, qp, f2, imm, x3) do { check_freg ((f2)); check_imm21 ((imm)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 6, (f2), 13, ((imm) >> 7) & 0x1fff, 20, (x3), 33, sign_bit ((imm)), 36, (1), 37); } while (0)

#define ia64_chk_s_float_m_pred(code, qp,f2,disp) ia64_m21 ((code), (qp), (f2), (disp), 3)

#define ia64_m22(code, qp, r1, imm, x3) do { check_greg ((r1)); check_imm21 ((imm)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (imm) & 0xfffff, 13, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_chk_a_nc_pred(code, qp,r1,disp) ia64_m22 ((code), (qp), (r1), (disp), 4)
#define ia64_chk_a_clr_pred(code, qp,r1,disp) ia64_m22 ((code), (qp), (r1), (disp), 5)

#define ia64_m23(code, qp, f1, imm, x3) do { check_freg ((f1)); check_imm21 ((imm)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (imm) & 0xfffff, 13, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_chk_a_nc_float_pred(code, qp,f1,disp) ia64_m23 ((code), (qp), (f1), (disp), 6)
#define ia64_chk_a_clr_float_pred(code, qp,f1,disp) ia64_m23 ((code), (qp), (f1), (disp), 7)

#define ia64_m24(code, qp, x3, x4, x2) do { ia64_emit_ins_5 ((code), IA64_INS_TYPE_M, (qp), 0, (x4), 27, (x2), 31, (x3), 33, (0), 37); } while (0)

#define ia64_invala_pred(code, qp) ia64_m24 ((code), (qp), 0, 0, 1)
#define ia64_fwb_pred(code, qp) ia64_m24 ((code), (qp), 0, 0, 2)
#define ia64_mf_pred(code, qp) ia64_m24 ((code), (qp), 0, 2, 2)
#define ia64_mf_a_pred(code, qp) ia64_m24 ((code), (qp), 0, 3, 2)
#define ia64_srlz_d_pred(code, qp) ia64_m24 ((code), (qp), 0, 0, 3)
#define ia64_stlz_i_pred(code, qp) ia64_m24 ((code), (qp), 0, 1, 3)
#define ia64_sync_i_pred(code, qp) ia64_m24 ((code), (qp), 0, 3, 3)

#define ia64_m25(code, qp, x3, x4, x2) do { ia64_emit_ins_5 ((code), IA64_INS_TYPE_M, (qp), 0, (x4), 27, (x2), 31, (x3), 33, (0), 37); } while (0)

#define ia64_flushrs_pred(code, qp) ia64_m24 ((code), (qp), 0, 0xC, 0)
#define ia64_loadrs_pred(code, qp) ia64_m24 ((code), (qp), 0, 0XA, 0)

#define ia64_m26(code, qp, r1, x3, x4, x2) do { check_greg ((r1)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (x4), 27, (x2), 31, (x3), 33, (0), 37); } while (0)

#define ia64_invala_e_pred(code, qp, r1) ia64_m26 ((code), (qp), (r1), 0, 2, 1)

#define ia64_m27(code, qp, f1, x3, x4, x2) do { check_freg ((f1)); ia64_emit_ins_5 ((code), IA64_INS_TYPE_M, (qp), 0, (f1), 6, (x4), 27, (x2), 31, (x3), 33, (0), 37); } while (0)

#define ia64_invala_e_float_pred(code, qp, f1) ia64_m26 ((code), (qp), (f1), 0, 3, 1)

#define ia64_m28(code, qp, r3, x3, x6, x) do { check_greg ((r3)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r3), 20, (x6), 27, (x3), 33, (x), 36, (1), 37); } while (0)

#define ia64_fc_pred(code, qp, r3) ia64_m28 ((code), (qp), (r3), 0, 0x30, 0)
#define ia64_fc_i_pred(code, qp, r3) ia64_m28 ((code), (qp), (r3), 0, 0x30, 1)

#define ia64_m29(code, qp, ar3, r2, x3, x6) do { check_greg ((r2)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r2), 13, (ar3), 20, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_to_ar_m_pred(code, qp, ar3, r2) ia64_m29 ((code), (qp), (ar3), (r2), 0, 0x2a)

#define ia64_m30(code, qp, ar3, imm, x3, x4, x2) do { check_imm8 ((imm)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0x7f, 13, (ar3), 20, (x4), 27, (x2), 31, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_mov_to_ar_imm_m_pred(code, qp, ar3, imm) ia64_m30 ((code), (qp), (ar3), (imm), 0, 8, 2)

#define ia64_m31(code, qp, r1, ar3, x3, x6) do { check_greg ((r1)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (ar3), 20, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_from_ar_m_pred(code, qp, r1, ar3) ia64_m31 ((code), (qp), (r1), (ar3), 0, 0x22)
#define ia64_m32(code, qp, cr3, r2, x3, x6) do { check_greg ((r2)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r2), 13, (cr3), 20, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_to_cr_pred(code, qp, cr3, r2) ia64_m32 ((code), (qp), (cr3), (r2), 0, 0x2C)

#define ia64_m33(code, qp, r1, cr3, x3, x6) do { check_greg ((r1)); ia64_emit_ins_6 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (cr3), 20, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_from_cr_pred(code, qp, r1, cr3) ia64_m33 ((code), (qp), (r1), (cr3), 0, 0x24)

#define ia64_m34(code, qp, r1, sor, sol, sof, x3) do { check_greg ((r1)); check_assert ((guint64)(sor) <= 0xf); check_assert ((guint64)(sol) <= 0x7f); check_assert ((guint64)(sof) <= 96); ia64_begin_bundle ((code)); check_assert ((code).nins == 0); check_assert ((qp) == 0); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (sof), 13, (sol), 20, (sor), 27, (x3), 33, (1), 37); } while (0)

#define ia64_alloc_pred(code, qp, r1, i, l, o, r) do { check_assert (((r) % 8) == 0); check_assert ((r) <= (i) + (l) + (o)); ia64_m34 ((code), (qp), (r1), (r) >> 3, (i) + (l), (i) + (l) + (o), 6); } while (0)

#define ia64_m35(code, qp, r2, x3, x6) do { check_greg ((r2)); ia64_emit_ins_5 ((code), IA64_INS_TYPE_M, (qp), 0, (r2), 13, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_to_psr_l_pred(code, qp, r2) ia64_m35 ((code), (qp), (r2), 0, 0x2D)
#define ia64_mov_to_psr_um_pred(code, qp, r2) ia64_m35 ((code), (qp), (r2), 0, 0x29)

#define ia64_m36(code, qp, r1, x3, x6) do { check_greg ((r1)); ia64_emit_ins_5 ((code), IA64_INS_TYPE_M, (qp), 0, (r1), 6, (x6), 27, (x3), 33, (1), 37); } while (0)

#define ia64_mov_from_psr_pred(code, qp, r1) ia64_m36 ((code), (qp), (r1), 0, 0x25)
#define ia64_mov_from_psr_um_pred(code, qp, r1) ia64_m36 ((code), (qp), (r1), 0, 0x21)

#define ia64_m37(code, qp, imm, x3, x2, x4) do { check_imm21 ((imm)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0xfffff, 6, (x4), 27, (x2), 31, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_break_m_pred(code, qp, imm) ia64_m37 ((code), (qp), (imm), 0, 0, 0)

/* The System/Memory Management instruction encodings (M38-M47) are missing */

#define ia64_m48(code, qp, imm, x3, x4, x2, y) do { check_imm21 ((imm)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_M, (qp), 0, (imm) & 0xfffff, 6, (y), 26, (x4), 27, (x2), 31, (x3), 33, sign_bit ((imm)), 36, (0), 37); } while (0)

#define ia64_nop_m_pred(code, qp, imm) ia64_m48 ((code), (qp), (imm), 0, 1, 0, 0)
#define ia64_hint_m_pred(code, qp, imm) ia64_m48 ((code), (qp), (imm), 0, 1, 0, 1)

typedef enum {
	IA64_BWH_SPTK = 0,
	IA64_BWH_SPNT = 1,
	IA64_BWH_DPTK = 2,
	IA64_BWH_DPNT = 3
} Ia64BranchWhetherHint;

typedef enum {
	IA64_PH_FEW = 0,
	IA64_PH_MANY = 1
} Ia64SeqPrefetchHint;

typedef enum {
	IA64_DH_NONE = 0,
	IA64_DH_CLR = 1
} Ia64BranchCacheDeallocHint;

#define ia64_b1(code, qp, imm, bwh, ph, dh, btype) do { check_imm21 ((imm)); check_bwh ((bwh)); check_ph ((ph)); check_dh ((dh)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_B, (qp), 0, (btype), 6, (ph), 12, (imm) & 0xfffff, 13, (bwh), 33, (dh), 35, sign_bit ((imm)), 36, (4), 37); } while (0)

#define ia64_br_cond_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b1 ((code), (qp), (disp), (bwh), (ph), (dh), 0)
#define ia64_br_wexit_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b1 ((code), (qp), (disp), (bwh), (ph), (dh), 2)
#define ia64_br_wtop_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b1 ((code), (qp), (disp), (bwh), (ph), (dh), 3)

#define ia64_b2(code, qp, imm, bwh, ph, dh, btype) do { check_imm21 ((imm)); check_assert ((qp) == 0); check_bwh ((bwh)); check_ph ((ph)); check_dh ((dh)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_B, (qp), 0, (btype), 6, (ph), 12, (imm) & 0xfffff, 13, (bwh), 33, (dh), 35, sign_bit ((imm)), 36, (4), 37); } while (0)

#define ia64_br_cloop_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b2 ((code), (qp), (disp), (bwh), (ph), (dh), 5)
#define ia64_br_cexit_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b2 ((code), (qp), (disp), (bwh), (ph), (dh), 6)
#define ia64_br_ctop_hint_pred(code, qp, disp, bwh, ph, dh) ia64_b2 ((code), (qp), (disp), (bwh), (ph), (dh), 7)

#define ia64_b3(code, qp, b1, imm, bwh, ph, dh) do { check_imm21 ((imm)); check_bwh ((bwh)); check_ph ((ph)); check_dh ((dh)); check_breg ((b1)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_B, (qp), 0, (b1), 6, (ph), 12, (imm) & 0xfffff, 13, (bwh), 33, (dh), 35, sign_bit ((imm)), 36, (5), 37); } while (0)

#define ia64_br_call_hint_pred(code, qp, b1, disp, bwh, ph, dh) ia64_b3 ((code), (qp), (b1), (disp), (bwh), (ph), (dh))

#define ia64_b4(code, qp, b2, bwh, ph, dh, x6, btype) do { check_breg ((b2)); check_bwh ((bwh)); check_ph ((ph)); check_dh ((dh)); ia64_emit_ins_8 ((code), IA64_INS_TYPE_B, (qp), 0, (btype), 6, (ph), 12, (b2), 13, (x6), 27, (bwh), 33, (dh), 35, (0), 37); } while (0)

#define ia64_br_cond_reg_hint_pred(code, qp, b1, bwh, ph, dh) ia64_b4 ((code), (qp), (b1), (bwh), (ph), (dh), 0x20, 0)
#define ia64_br_ia_reg_hint_pred(code, qp, b1, bwh, ph, dh) ia64_b4 ((code), (qp), (b1), (bwh), (ph), (dh), 0x20, 1)
#define ia64_br_ret_reg_hint_pred(code, qp, b1, bwh, ph, dh) ia64_b4 ((code), (qp), (b1), (bwh), (ph), (dh), 0x21, 4)

#define ia64_b5(code, qp, b1, b2, bwh, ph, dh) do { check_breg ((b1)); check_breg ((b2)); check_bwh ((bwh)); check_ph ((ph)); check_dh ((dh)); ia64_emit_ins_7 ((code), IA64_INS_TYPE_B, (qp), 0, (b1), 6, (ph), 12, (b2), 13, ((bwh) * 2) + 1, 32, (dh), 35, (1), 37); } while (0)

#define ia64_br_call_reg_hint_pred(code, qp, b1, b2, bwh, ph, dh) ia64_b5 ((code), (qp), (b1), (b2), (bwh), (ph), (dh))

typedef enum {
	IA64_IPWH_SPTK = 0,
	IA64_IPWH_LOOP = 1,
	IA64_IPWH_DPTK = 2,
	IA64_IPWH_EXIT = 3
} Ia64IPRelativeBranchWhetherHint;

/* B6 and B7 is missing */

#define ia64_b8(code, qp, x6) do { ia64_emit_ins_3 ((code), IA64_INS_TYPE_B, (qp), 0, (x6), 27, (0), 37); } while (0)

#define ia64_cover_pred(code, qp) ia64_b8 ((code), (qp), 0x02)
#define ia64_clrrrb_pred(code, qp) ia64_b8 ((code), (qp), 0x04)
#define ia64_clrrrb_pr_pred(code, qp) ia64_b8 ((code), (qp), 0x05)
#define ia64_rfi_pred(code, qp) ia64_b8 ((code), (qp), 0x08)
#define ia64_bsw_0_pred(code, qp) ia64_b8 ((code), (qp), 0x0C)
#define ia64_bsw_1_pred(code, qp) ia64_b8 ((code), (qp), 0x0D)
#define ia64_epc_pred(code, qp) ia64_b8 ((code), (qp), 0x10)

#define ia64_b9(code, qp, imm, opcode, x6) do { check_imm21 ((imm)); ia64_emit_ins_5 ((code), IA64_INS_TYPE_B, (qp), 0, (imm) & 0xfffff, 6, (x6), 27, ((imm) >> 20) & 0x1, 36, (opcode), 37); } while (0)

#define ia64_break_b_pred(code, qp, imm) ia64_b9 ((code), (qp), (imm), 0, 0x00)
#define ia64_nop_b_pred(code, qp, imm) ia64_b9 ((code), (qp), (imm), 2, 0x00)
#define ia64_hint_b_pred(code, qp, imm) ia64_b9 ((code), (qp), (imm), 2, 0x01)

#define ia64_x1(code, qp, imm, x3, x6) do { check_imm62 ((imm)); ia64_begin_bundle (code); ia64_emit_ins_1 ((code), IA64_INS_TYPE_LX, ((guint64)(imm) >> 21) & 0x1ffffffffffULL, 0); ia64_emit_ins_6 ((code), IA64_INS_TYPE_LX, (qp), 0, (guint64)(imm) & 0xfffff, (6), (x6), 27, (x3), 33, ((guint64)(imm) >> 20) & 0x1, 36, (0), 37); } while (0)

#define ia64_break_x_pred(code, qp, imm) ia64_x1 ((code), (qp), (imm), 0, 0x00)

#define ia64_x2(code, qp, r1, imm, vc) do { check_greg ((r1)); ia64_begin_bundle (code); ia64_emit_ins_1 ((code), IA64_INS_TYPE_LX, ((guint64)(imm) >> 22) & 0x1ffffffffffULL, 0); ia64_emit_ins_9 ((code), IA64_INS_TYPE_LX, (qp), 0, (r1), 6, (guint64)(imm) & 0x7f, (13), (vc), 20, ((guint64)(imm) >> 21) & 0x1, 21, ((guint64)(imm) >> 16) & 0x1f, 22, ((guint64)(imm) >> 7) & 0x1ff, 27, ((guint64)(imm) >> 63) & 0x1, 36, (6), 37); } while (0)

#define ia64_movl_pred(code, qp, r1, imm) ia64_x2 ((code), (qp), (r1), (imm), 0)

#define ia64_x3(code, qp, imm, bwh, ph, dh, btype) do { ia64_begin_bundle (code); ia64_emit_ins_1 ((code), IA64_INS_TYPE_LX, ((guint64)(imm) >> 20) & 0x1ffffffffffULL, 0); ia64_emit_ins_8 ((code), IA64_INS_TYPE_LX, (qp), 0, (btype), 6, (ph), 12, (guint64)(imm) & 0xfffff, (13), (bwh), 33, (dh), 35, ((guint64)(imm) >> 59) & 0x1, 36, (0xC), 37); } while (0)

#define ia64_brl_cond_hint_pred(code, qp, disp, bwh, ph, dh) ia64_x3 ((code), (qp), (disp), (bwh), (ph), (dh), 0)

#define ia64_x4(code, qp, b1, imm, bwh, ph, dh) do { check_breg ((b1)); ia64_begin_bundle (code); ia64_emit_ins_1 ((code), IA64_INS_TYPE_LX, ((guint64)(imm) >> 20) & 0x1ffffffffffULL, 0); ia64_emit_ins_8 ((code), IA64_INS_TYPE_LX, (qp), 0, (b1), 6, (ph), 12, (guint64)(imm) & 0xfffff, (13), (bwh), 33, (dh), 35, ((guint64)(imm) >> 59) & 0x1, 36, (0xD), 37); } while (0)

#define ia64_brl_call_hint_pred(code, qp, b1, disp, bwh, ph, dh) ia64_x4 ((code), (qp), (b1), (disp), (bwh), (ph), (dh))

#define ia64_x5(code, qp, imm, x3, x6, y) do { check_imm62 ((imm)); ia64_begin_bundle (code); ia64_emit_ins_1 ((code), IA64_INS_TYPE_LX, ((guint64)(imm) >> 21) & 0x1ffffffffffULL, 0); ia64_emit_ins_7 ((code), IA64_INS_TYPE_LX, (qp), 0, (guint64)(imm) & 0xfffff, (6), (y), 26, (x6), 27, (x3), 33, ((guint64)(imm) >> 20) & 0x1, 36, (0), 37); } while (0)

#define ia64_nop_x_pred(code, qp, imm) ia64_x5 ((code), (qp), (imm), 0, 0x01, 0)
#define ia64_hint_x_pred(code, qp, imm) ia64_x5 ((code), (qp), (imm), 0, 0x01, 1)

/*
 * Non predicated instruction variants
 */


#define ia64_add(code, r1, r2, r3) ia64_add_pred ((code), 0, r1, r2, r3)
#define ia64_add1(code, r1, r2, r3) ia64_add1_pred ((code), 0, r1, r2, r3)
#define ia64_sub(code, r1, r2, r3) ia64_sub_pred ((code), 0, r1, r2, r3)
#define ia64_sub1(code, r1, r2, r3) ia64_sub1_pred ((code), 0, r1, r2, r3)
#define ia64_addp4(code, r1, r2, r3) ia64_addp4_pred ((code), 0, r1, r2, r3)
#define ia64_and(code, r1, r2, r3) ia64_and_pred ((code), 0, r1, r2, r3)
#define ia64_andcm(code, r1, r2, r3) ia64_andcm_pred ((code), 0, r1, r2, r3)
#define ia64_or(code, r1, r2, r3) ia64_or_pred ((code), 0, r1, r2, r3)
#define ia64_xor(code, r1, r2, r3) ia64_xor_pred ((code), 0, r1, r2, r3)


#define ia64_shladd(code, r1, r2, r3,count) ia64_shladd_pred ((code), 0, r1, r2, r3,count)
#define ia64_shladdp4(code, r1, r2, r3,count) ia64_shladdp4_pred ((code), 0, r1, r2, r3,count)


#define ia64_sub_imm(code, r1,imm8,r3) ia64_sub_imm_pred ((code), 0, r1,imm8,r3)
#define ia64_and_imm(code, r1,imm8,r3) ia64_and_imm_pred ((code), 0, r1,imm8,r3)
#define ia64_andcm_imm(code, r1,imm8,r3) ia64_andcm_imm_pred ((code), 0, r1,imm8,r3)
#define ia64_or_imm(code, r1,imm8,r3) ia64_or_imm_pred ((code), 0, r1,imm8,r3)
#define ia64_xor_imm(code, r1,imm8,r3) ia64_xor_imm_pred ((code), 0, r1,imm8,r3)


#define ia64_adds_imm(code, r1,imm14,r3) ia64_adds_imm_pred ((code), 0, r1,imm14,r3)
#define ia64_addp4_imm(code, r1,imm14,r3) ia64_addp4_imm_pred ((code), 0, r1,imm14,r3)


#define ia64_addl_imm(code, r1,imm22,r3) ia64_addl_imm_pred ((code), 0, r1,imm22,r3)


#define ia64_cmp_lt(code, p1, p2, r2, r3) ia64_cmp_lt_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ltu(code, p1, p2, r2, r3) ia64_cmp_ltu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_eq(code, p1, p2, r2, r3) ia64_cmp_eq_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_lt_unc(code, p1, p2, r2, r3) ia64_cmp_lt_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ltu_unc(code, p1, p2, r2, r3) ia64_cmp_ltu_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_eq_unc(code, p1, p2, r2, r3) ia64_cmp_eq_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_eq_and(code, p1, p2, r2, r3) ia64_cmp_eq_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_eq_or(code, p1, p2, r2, r3) ia64_cmp_eq_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_eq_or_andcm(code, p1, p2, r2, r3) ia64_cmp_eq_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ne_and(code, p1, p2, r2, r3) ia64_cmp_ne_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ne_or(code, p1, p2, r2, r3) ia64_cmp_ne_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ne_or_andcm(code, p1, p2, r2, r3) ia64_cmp_ne_or_andcm_pred ((code), 0, p1, p2, r2, r3)

#define ia64_cmp4_lt(code, p1, p2, r2, r3) ia64_cmp4_lt_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ltu(code, p1, p2, r2, r3) ia64_cmp4_ltu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_eq(code, p1, p2, r2, r3) ia64_cmp4_eq_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_lt_unc(code, p1, p2, r2, r3) ia64_cmp4_lt_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ltu_unc(code, p1, p2, r2, r3) ia64_cmp4_ltu_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_eq_unc(code, p1, p2, r2, r3) ia64_cmp4_eq_unc_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_eq_and(code, p1, p2, r2, r3) ia64_cmp4_eq_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_eq_or(code, p1, p2, r2, r3) ia64_cmp4_eq_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_eq_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_eq_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ne_and(code, p1, p2, r2, r3) ia64_cmp4_ne_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ne_or(code, p1, p2, r2, r3) ia64_cmp4_ne_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ne_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_ne_or_andcm_pred ((code), 0, p1, p2, r2, r3)

/* Pseudo ops */
#define ia64_cmp_ne(code, p1, p2, r2, r3) ia64_cmp_ne_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_le(code, p1, p2, r2, r3) ia64_cmp_le_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_gt(code, p1, p2, r2, r3) ia64_cmp_gt_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ge(code, p1, p2, r2, r3) ia64_cmp_ge_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_leu(code, p1, p2, r2, r3) ia64_cmp_leu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_gtu(code, p1, p2, r2, r3) ia64_cmp_gtu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_geu(code, p1, p2, r2, r3) ia64_cmp_geu_pred ((code), 0, p1, p2, r2, r3)

#define ia64_cmp4_ne(code, p1, p2, r2, r3) ia64_cmp4_ne_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_le(code, p1, p2, r2, r3) ia64_cmp4_le_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_gt(code, p1, p2, r2, r3) ia64_cmp4_gt_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ge(code, p1, p2, r2, r3) ia64_cmp4_ge_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_leu(code, p1, p2, r2, r3) ia64_cmp4_leu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_gtu(code, p1, p2, r2, r3) ia64_cmp4_gtu_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_geu(code, p1, p2, r2, r3) ia64_cmp4_geu_pred ((code), 0, p1, p2, r2, r3)


#define ia64_cmp_gt_and(code, p1, p2, r2, r3) ia64_cmp_gt_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_gt_or(code, p1, p2, r2, r3) ia64_cmp_gt_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_gt_or_andcm(code, p1, p2, r2, r3) ia64_cmp_gt_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_le_and(code, p1, p2, r2, r3) ia64_cmp_le_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_le_or(code, p1, p2, r2, r3) ia64_cmp_le_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_le_or_andcm(code, p1, p2, r2, r3) ia64_cmp_le_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ge_and(code, p1, p2, r2, r3) ia64_cmp_ge_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ge_or(code, p1, p2, r2, r3) ia64_cmp_ge_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_ge_or_andcm(code, p1, p2, r2, r3) ia64_cmp_ge_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_lt_and(code, p1, p2, r2, r3) ia64_cmp_lt_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_lt_or(code, p1, p2, r2, r3) ia64_cmp_lt_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp_lt_or_andcm(code, p1, p2, r2, r3) ia64_cmp_lt_or_andcm_pred ((code), 0, p1, p2, r2, r3)

#define ia64_cmp4_gt_and(code, p1, p2, r2, r3) ia64_cmp4_gt_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_gt_or(code, p1, p2, r2, r3) ia64_cmp4_gt_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_gt_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_gt_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_le_and(code, p1, p2, r2, r3) ia64_cmp4_le_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_le_or(code, p1, p2, r2, r3) ia64_cmp4_le_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_le_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_le_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ge_and(code, p1, p2, r2, r3) ia64_cmp4_ge_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ge_or(code, p1, p2, r2, r3) ia64_cmp4_ge_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_ge_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_ge_or_andcm_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_lt_and(code, p1, p2, r2, r3) ia64_cmp4_lt_and_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_lt_or(code, p1, p2, r2, r3) ia64_cmp4_lt_or_pred ((code), 0, p1, p2, r2, r3)
#define ia64_cmp4_lt_or_andcm(code, p1, p2, r2, r3) ia64_cmp4_lt_or_andcm_pred ((code), 0, p1, p2, r2, r3)


#define ia64_cmp_lt_imm(code, p1, p2, imm8, r3) ia64_cmp_lt_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_ltu_imm(code, p1, p2, imm8, r3) ia64_cmp_ltu_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_eq_imm(code, p1, p2, imm8, r3) ia64_cmp_eq_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_lt_unc_imm(code, p1, p2, imm8, r3) ia64_cmp_lt_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_ltu_unc_imm(code, p1, p2, imm8, r3) ia64_cmp_ltu_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_eq_unc_imm(code, p1, p2, imm8, r3) ia64_cmp_eq_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_eq_and_imm(code, p1, p2, imm8, r3) ia64_cmp_eq_and_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_eq_or_imm(code, p1, p2, imm8, r3) ia64_cmp_eq_or_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_eq_or_andcm_imm(code, p1, p2, imm8, r3) ia64_cmp_eq_or_andcm_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_ne_and_imm(code, p1, p2, imm8, r3) ia64_cmp_ne_and_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_ne_or_imm(code, p1, p2, imm8, r3) ia64_cmp_ne_or_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp_ne_or_andcm_imm(code, p1, p2, imm8, r3) ia64_cmp_ne_or_andcm_imm_pred ((code), 0, p1, p2, imm8, r3)

#define ia64_cmp4_lt_imm(code, p1, p2, imm8, r3) ia64_cmp4_lt_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_ltu_imm(code, p1, p2, imm8, r3) ia64_cmp4_ltu_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_eq_imm(code, p1, p2, imm8, r3) ia64_cmp4_eq_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_lt_unc_imm(code, p1, p2, imm8, r3) ia64_cmp4_lt_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_ltu_unc_imm(code, p1, p2, imm8, r3) ia64_cmp4_ltu_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_eq_unc_imm(code, p1, p2, imm8, r3) ia64_cmp4_eq_unc_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_eq_and_imm(code, p1, p2, imm8, r3) ia64_cmp4_eq_and_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_eq_or_imm(code, p1, p2, imm8, r3) ia64_cmp4_eq_or_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_eq_or_andcm_imm(code, p1, p2, imm8, r3) ia64_cmp4_eq_or_andcm_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_ne_and_imm(code, p1, p2, imm8, r3) ia64_cmp4_ne_and_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_ne_or_imm(code, p1, p2, imm8, r3) ia64_cmp4_ne_or_imm_pred ((code), 0, p1, p2, imm8, r3)
#define ia64_cmp4_ne_or_andcm_imm(code, p1, p2, imm8, r3) ia64_cmp4_ne_or_andcm_imm_pred ((code), 0, p1, p2, imm8, r3)


#define ia64_padd1(code, r1,r2,r3) ia64_padd1_pred ((code), 0, r1,r2,r3)
#define ia64_padd2(code, r1,r2,r3) ia64_padd2_pred ((code), 0, r1,r2,r3)
#define ia64_padd4(code, r1,r2,r3) ia64_padd4_pred ((code), 0, r1,r2,r3)
#define ia64_padd1_sss(code, r1,r2,r3) ia64_padd1_sss_pred ((code), 0, r1,r2,r3)
#define ia64_padd2_sss(code, r1,r2,r3) ia64_padd2_sss_pred ((code), 0, r1,r2,r3)
#define ia64_padd1_uuu(code, r1,r2,r3) ia64_padd1_uuu_pred ((code), 0, r1,r2,r3)
#define ia64_padd2_uuu(code, r1,r2,r3) ia64_padd2_uuu_pred ((code), 0, r1,r2,r3)
#define ia64_padd1_uus(code, r1,r2,r3) ia64_padd1_uus_pred ((code), 0, r1,r2,r3)
#define ia64_padd2_uus(code, r1,r2,r3) ia64_padd2_uus_pred ((code), 0, r1,r2,r3)

#define ia64_psub1(code, r1,r2,r3) ia64_psub1_pred ((code), 0, r1,r2,r3)
#define ia64_psub2(code, r1,r2,r3) ia64_psub2_pred ((code), 0, r1,r2,r3)
#define ia64_psub4(code, r1,r2,r3) ia64_psub4_pred ((code), 0, r1,r2,r3)
#define ia64_psub1_sss(code, r1,r2,r3) ia64_psub1_sss_pred ((code), 0, r1,r2,r3)
#define ia64_psub2_sss(code, r1,r2,r3) ia64_psub2_sss_pred ((code), 0, r1,r2,r3)
#define ia64_psub1_uuu(code, r1,r2,r3) ia64_psub1_uuu_pred ((code), 0, r1,r2,r3)
#define ia64_psub2_uuu(code, r1,r2,r3) ia64_psub2_uuu_pred ((code), 0, r1,r2,r3)
#define ia64_psub1_uus(code, r1,r2,r3) ia64_psub1_uus_pred ((code), 0, r1,r2,r3)
#define ia64_psub2_uus(code, r1,r2,r3) ia64_psub2_uus_pred ((code), 0, r1,r2,r3)

#define ia64_pavg1(code, r1,r2,r3) ia64_pavg1_pred ((code), 0, r1,r2,r3)
#define ia64_pavg2(code, r1,r2,r3) ia64_pavg2_pred ((code), 0, r1,r2,r3)
#define ia64_pavg1_raz(code, r1,r2,r3) ia64_pavg1_raz_pred ((code), 0, r1,r2,r3)
#define ia64_pavg2_raz(code, r1,r2,r3) ia64_pavg2_raz_pred ((code), 0, r1,r2,r3)
#define ia64_pavgsub1(code, r1,r2,r3) ia64_pavgsub1_pred ((code), 0, r1,r2,r3)
#define ia64_pavgsub2(code, r1,r2,r3) ia64_pavgsub2_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp1_eq(code, r1,r2,r3) ia64_pcmp1_eq_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp2_eq(code, r1,r2,r3) ia64_pcmp2_eq_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp4_eq(code, r1,r2,r3) ia64_pcmp4_eq_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp1_gt(code, r1,r2,r3) ia64_pcmp1_gt_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp2_gt(code, r1,r2,r3) ia64_pcmp2_gt_pred ((code), 0, r1,r2,r3)
#define ia64_pcmp4_gt(code, r1,r2,r3) ia64_pcmp4_gt_pred ((code), 0, r1,r2,r3)


#define ia64_pshladd2(code, r1, r2, r3, count) ia64_pshladd2_pred ((code), 0, r1, r2, r3, count)
#define ia64_pshradd2(code, r1, r2, r3, count) ia64_pshradd2_pred ((code), 0, r1, r2, r3, count)

#define ia64_pmpyshr2(code, r1, r2, r3, count) ia64_pmpyshr2_pred ((code), 0, r1, r2, r3, count)

#define ia64_pmpyshr2_u(code, r1, r2, r3, count) ia64_pmpyshr2_u_pred ((code), 0, r1, r2, r3, count)


#define ia64_pmpy2_r(code,  r1, r2, r3) ia64_pmpy2_r_pred ((code), 0,  r1, r2, r3)
#define ia64_pmpy2_l(code,  r1, r2, r3) ia64_pmpy2_l_pred ((code), 0,  r1, r2, r3)
#define ia64_mix1_r(code,  r1, r2, r3) ia64_mix1_r_pred ((code), 0,  r1, r2, r3)
#define ia64_mix2_r(code,  r1, r2, r3) ia64_mix2_r_pred ((code), 0,  r1, r2, r3)
#define ia64_mix4_r(code,  r1, r2, r3) ia64_mix4_r_pred ((code), 0,  r1, r2, r3)
#define ia64_mix1_l(code,  r1, r2, r3) ia64_mix1_l_pred ((code), 0,  r1, r2, r3)
#define ia64_mix2_l(code,  r1, r2, r3) ia64_mix2_l_pred ((code), 0,  r1, r2, r3)
#define ia64_mix4_l(code,  r1, r2, r3) ia64_mix4_l_pred ((code), 0,  r1, r2, r3)
#define ia64_pack2_uss(code,  r1, r2, r3) ia64_pack2_uss_pred ((code), 0,  r1, r2, r3)
#define ia64_pack2_sss(code,  r1, r2, r3) ia64_pack2_sss_pred ((code), 0,  r1, r2, r3)
#define ia64_pack4_sss(code,  r1, r2, r3) ia64_pack4_sss_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack1_h(code,  r1, r2, r3) ia64_unpack1_h_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack2_h(code,  r1, r2, r3) ia64_unpack2_h_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack4_h(code,  r1, r2, r3) ia64_unpack4_h_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack1_l(code,  r1, r2, r3) ia64_unpack1_l_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack2_l(code,  r1, r2, r3) ia64_unpack2_l_pred ((code), 0,  r1, r2, r3)
#define ia64_unpack4_l(code,  r1, r2, r3) ia64_unpack4_l_pred ((code), 0,  r1, r2, r3)
#define ia64_pmin1_u(code,  r1, r2, r3) ia64_pmin1_u_pred ((code), 0,  r1, r2, r3)
#define ia64_pmax1_u(code,  r1, r2, r3) ia64_pmax1_u_pred ((code), 0,  r1, r2, r3)
#define ia64_pmin2(code,  r1, r2, r3) ia64_pmin2_pred ((code), 0,  r1, r2, r3)
#define ia64_pmax2(code,  r1, r2, r3) ia64_pmax2_pred ((code), 0,  r1, r2, r3)
#define ia64_psad1(code,  r1, r2, r3) ia64_psad1_pred ((code), 0,  r1, r2, r3)

#define ia64_mux1(code, r1, r2, mbtype) ia64_mux1_pred ((code), 0, r1, r2, mbtype)


#define ia64_mux2(code, r1, r2, mhtype) ia64_mux2_pred ((code), 0, r1, r2, mhtype)


#define ia64_pshr2(code, r1, r3, r2) ia64_pshr2_pred ((code), 0, r1, r3, r2)
#define ia64_pshr4(code, r1, r3, r2) ia64_pshr4_pred ((code), 0, r1, r3, r2)
#define ia64_shr(code, r1, r3, r2) ia64_shr_pred ((code), 0, r1, r3, r2)
#define ia64_pshr2_u(code, r1, r3, r2) ia64_pshr2_u_pred ((code), 0, r1, r3, r2)
#define ia64_pshr4_u(code, r1, r3, r2) ia64_pshr4_u_pred ((code), 0, r1, r3, r2)
#define ia64_shr_u(code, r1, r3, r2) ia64_shr_u_pred ((code), 0, r1, r3, r2)


#define ia64_pshr2_imm(code, r1, r3, count) ia64_pshr2_imm_pred ((code), 0, r1, r3, count)
#define ia64_pshr4_imm(code, r1, r3, count) ia64_pshr4_imm_pred ((code), 0, r1, r3, count)
#define ia64_pshr2_u_imm(code, r1, r3, count) ia64_pshr2_u_imm_pred ((code), 0, r1, r3, count)
#define ia64_pshr4_u_imm(code, r1, r3, count) ia64_pshr4_u_imm_pred ((code), 0, r1, r3, count)


#define ia64_pshl2(code, r1, r3, r2) ia64_pshl2_pred ((code), 0, r1, r3, r2)
#define ia64_pshl4(code, r1, r3, r2) ia64_pshl4_pred ((code), 0, r1, r3, r2)
#define ia64_shl(code, r1, r3, r2) ia64_shl_pred ((code), 0, r1, r3, r2)


#define ia64_pshl2_imm(code, r1, r2, count) ia64_pshl2_imm_pred ((code), 0, r1, r2, count)
#define ia64_pshl4_imm(code, r1, r2, count) ia64_pshl4_imm_pred ((code), 0, r1, r2, count)


#define ia64_popcnt(code, r1, r3) ia64_popcnt_pred ((code), 0, r1, r3)


#define ia64_shrp(code, r1, r2, r3, count) ia64_shrp_pred ((code), 0, r1, r2, r3, count)


#define ia64_extr_u(code, r1, r3, pos, len) ia64_extr_u_pred ((code), 0, r1, r3, pos, len)
#define ia64_extr(code, r1, r3, pos, len) ia64_extr_pred ((code), 0, r1, r3, pos, len)


#define ia64_dep_z(code, r1, r2, pos, len) ia64_dep_z_pred ((code), 0, r1, r2, pos, len)


#define ia64_dep_z_imm(code, r1, imm, pos, len) ia64_dep_z_imm_pred ((code), 0, r1, imm, pos, len)


#define ia64_dep_imm(code, r1, imm, r3, pos, len) ia64_dep_imm_pred ((code), 0, r1, imm, r3, pos, len)


#define ia64_dep(code, r1, r2, r3, pos, len) ia64_dep_pred ((code), 0, r1, r2, r3, pos, len)


#define ia64_tbit_z(code, p1, p2, r3, pos) ia64_tbit_z_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_z_unc(code, p1, p2, r3, pos) ia64_tbit_z_unc_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_z_and(code, p1, p2, r3, pos) ia64_tbit_z_and_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_nz_and(code, p1, p2, r3, pos) ia64_tbit_nz_and_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_z_or(code, p1, p2, r3, pos) ia64_tbit_z_or_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_nz_or(code, p1, p2, r3, pos) ia64_tbit_nz_or_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_z_or_andcm(code, p1, p2, r3, pos) ia64_tbit_z_or_andcm_pred ((code), 0, p1, p2, r3, pos)
#define ia64_tbit_nz_or_andcm(code, p1, p2, r3, pos) ia64_tbit_nz_or_andcm_pred ((code), 0, p1, p2, r3, pos)


#define ia64_tnat_z(code, p1, p2, r3) ia64_tnat_z_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_z_unc(code, p1, p2, r3) ia64_tnat_z_unc_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_z_and(code, p1, p2, r3) ia64_tnat_z_and_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_nz_and(code, p1, p2, r3) ia64_tnat_nz_and_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_z_or(code, p1, p2, r3) ia64_tnat_z_or_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_nz_or(code, p1, p2, r3) ia64_tnat_nz_or_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_z_or_andcm(code, p1, p2, r3) ia64_tnat_z_or_andcm_pred ((code), 0, p1, p2, r3)
#define ia64_tnat_nz_or_andcm(code, p1, p2, r3) ia64_tnat_nz_or_andcm_pred ((code), 0, p1, p2, r3)


#define ia64_nop_i(code, imm) ia64_nop_i_pred ((code), 0, imm)
#define ia64_hint_i(code, imm) ia64_hint_i_pred ((code), 0, imm)


#define ia64_break_i(code, imm) ia64_break_i_pred ((code), 0, imm)


#define ia64_chk_s_i(code, r2,disp) ia64_chk_s_i_pred ((code), 0, r2,disp)

#define ia64_mov_to_br(code, b1, r2, disp, wh, ih) ia64_mov_to_br_pred ((code), 0, b1, r2, disp, wh, ih)
#define ia64_mov_ret_to_br(code, b1, r2, disp, wh, ih) ia64_mov_ret_to_br_pred ((code), 0, b1, r2, disp, wh, ih)


#define ia64_mov_from_br(code, r1, b2) ia64_mov_from_br_pred ((code), 0, r1, b2)


#define ia64_mov_to_pred(code, r2, mask) ia64_mov_to_pred_pred ((code), 0, r2, mask)


#define ia64_mov_to_pred_rot_imm(code, imm) ia64_mov_to_pred_rot_imm_pred ((code), 0, imm)


#define ia64_mov_from_ip(code, r1) ia64_mov_from_ip_pred ((code), 0, r1)
#define ia64_mov_from_pred(code, r1) ia64_mov_from_pred_pred ((code), 0, r1)


#define ia64_mov_to_ar_i(code, ar3, r2) ia64_mov_to_ar_i_pred ((code), 0, ar3, r2)


#define ia64_mov_to_ar_imm_i(code, ar3, imm) ia64_mov_to_ar_imm_i_pred ((code), 0, ar3, imm)


#define ia64_mov_from_ar_i(code, r1, ar3) ia64_mov_from_ar_i_pred ((code), 0, r1, ar3)


#define ia64_zxt1(code, r1, r3) ia64_zxt1_pred ((code), 0, r1, r3)
#define ia64_zxt2(code, r1, r3) ia64_zxt2_pred ((code), 0, r1, r3)
#define ia64_zxt4(code, r1, r3) ia64_zxt4_pred ((code), 0, r1, r3)
#define ia64_sxt1(code, r1, r3) ia64_sxt1_pred ((code), 0, r1, r3)
#define ia64_sxt2(code, r1, r3) ia64_sxt2_pred ((code), 0, r1, r3)
#define ia64_sxt4(code, r1, r3) ia64_sxt4_pred ((code), 0, r1, r3)
#define ia64_czx1_l(code, r1, r3) ia64_czx1_l_pred ((code), 0, r1, r3)
#define ia64_czx2_l(code, r1, r3) ia64_czx2_l_pred ((code), 0, r1, r3)
#define ia64_czx1_r(code, r1, r3) ia64_czx1_r_pred ((code), 0, r1, r3)
#define ia64_czx2_r(code, r1, r3) ia64_czx2_r_pred ((code), 0, r1, r3)

#define ia64_ld1_hint(code, r1, r3, hint) ia64_ld1_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_hint(code, r1, r3, hint) ia64_ld2_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_hint(code, r1, r3, hint) ia64_ld4_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_hint(code, r1, r3, hint) ia64_ld8_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_s_hint(code, r1, r3, hint) ia64_ld1_s_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_s_hint(code, r1, r3, hint) ia64_ld2_s_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_s_hint(code, r1, r3, hint) ia64_ld4_s_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_s_hint(code, r1, r3, hint) ia64_ld8_s_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_a_hint(code, r1, r3, hint) ia64_ld1_a_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_a_hint(code, r1, r3, hint) ia64_ld2_a_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_a_hint(code, r1, r3, hint) ia64_ld4_a_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_a_hint(code, r1, r3, hint) ia64_ld8_a_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_sa_hint(code, r1, r3, hint) ia64_ld1_sa_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_sa_hint(code, r1, r3, hint) ia64_ld2_sa_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_sa_hint(code, r1, r3, hint) ia64_ld4_sa_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_sa_hint(code, r1, r3, hint) ia64_ld8_sa_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_bias_hint(code, r1, r3, hint) ia64_ld1_bias_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_bias_hint(code, r1, r3, hint) ia64_ld2_bias_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_bias_hint(code, r1, r3, hint) ia64_ld4_bias_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_bias_hint(code, r1, r3, hint) ia64_ld8_bias_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_acq_hint(code, r1, r3, hint) ia64_ld1_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_acq_hint(code, r1, r3, hint) ia64_ld2_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_acq_hint(code, r1, r3, hint) ia64_ld4_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_acq_hint(code, r1, r3, hint) ia64_ld8_acq_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld8_fill_hint(code, r1, r3, hint) ia64_ld8_fill_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_c_clr_hint(code, r1, r3, hint) ia64_ld1_c_clr_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_c_clr_hint(code, r1, r3, hint) ia64_ld2_c_clr_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_c_clr_hint(code, r1, r3, hint) ia64_ld4_c_clr_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_c_clr_hint(code, r1, r3, hint) ia64_ld8_c_clr_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_c_nc_hint(code, r1, r3, hint) ia64_ld1_c_nc_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_c_nc_hint(code, r1, r3, hint) ia64_ld2_c_nc_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_c_nc_hint(code, r1, r3, hint) ia64_ld4_c_nc_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_c_nc_hint(code, r1, r3, hint) ia64_ld8_c_nc_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld1_c_clr_acq_hint(code, r1, r3, hint) ia64_ld1_c_clr_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld2_c_clr_acq_hint(code, r1, r3, hint) ia64_ld2_c_clr_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld4_c_clr_acq_hint(code, r1, r3, hint) ia64_ld4_c_clr_acq_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld8_c_clr_acq_hint(code, r1, r3, hint) ia64_ld8_c_clr_acq_hint_pred ((code), 0, r1, r3, hint)

#define ia64_ld16_hint(code, r1, r3, hint) ia64_ld16_hint_pred ((code), 0, r1, r3, hint)
#define ia64_ld16_acq_hint(code, r1, r3, hint) ia64_ld16_acq_hint_pred ((code), 0, r1, r3, hint)


#define ia64_ld1_inc_hint(code, r1, r2, r3, hint) ia64_ld1_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_inc_hint(code, r1, r2, r3, hint) ia64_ld2_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_inc_hint(code, r1, r2, r3, hint) ia64_ld4_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_inc_hint(code, r1, r2, r3, hint) ia64_ld8_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_s_inc_hint(code, r1, r2, r3, hint) ia64_ld1_s_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_s_inc_hint(code, r1, r2, r3, hint) ia64_ld2_s_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_s_inc_hint(code, r1, r2, r3, hint) ia64_ld4_s_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_s_inc_hint(code, r1, r2, r3, hint) ia64_ld8_s_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_a_inc_hint(code, r1, r2, r3, hint) ia64_ld1_a_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_a_inc_hint(code, r1, r2, r3, hint) ia64_ld2_a_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_a_inc_hint(code, r1, r2, r3, hint) ia64_ld4_a_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_a_inc_hint(code, r1, r2, r3, hint) ia64_ld8_a_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_sa_inc_hint(code, r1, r2, r3, hint) ia64_ld1_sa_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_sa_inc_hint(code, r1, r2, r3, hint) ia64_ld2_sa_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_sa_inc_hint(code, r1, r2, r3, hint) ia64_ld4_sa_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_sa_inc_hint(code, r1, r2, r3, hint) ia64_ld8_sa_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_bias_inc_hint(code, r1, r2, r3, hint) ia64_ld1_bias_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_bias_inc_hint(code, r1, r2, r3, hint) ia64_ld2_bias_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_bias_inc_hint(code, r1, r2, r3, hint) ia64_ld4_bias_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_bias_inc_hint(code, r1, r2, r3, hint) ia64_ld8_bias_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld1_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld2_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld4_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld8_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld8_fill_inc_hint(code, r1, r2, r3, hint) ia64_ld8_fill_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_c_clr_inc_hint(code, r1, r2, r3, hint) ia64_ld1_c_clr_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_c_clr_inc_hint(code, r1, r2, r3, hint) ia64_ld2_c_clr_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_c_clr_inc_hint(code, r1, r2, r3, hint) ia64_ld4_c_clr_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_c_clr_inc_hint(code, r1, r2, r3, hint) ia64_ld8_c_clr_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_c_nc_inc_hint(code, r1, r2, r3, hint) ia64_ld1_c_nc_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_c_nc_inc_hint(code, r1, r2, r3, hint) ia64_ld2_c_nc_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_c_nc_inc_hint(code, r1, r2, r3, hint) ia64_ld4_c_nc_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_c_nc_inc_hint(code, r1, r2, r3, hint) ia64_ld8_c_nc_inc_hint_pred ((code), 0, r1, r2, r3, hint)

#define ia64_ld1_c_clr_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld1_c_clr_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld2_c_clr_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld2_c_clr_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld4_c_clr_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld4_c_clr_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)
#define ia64_ld8_c_clr_acq_inc_hint(code, r1, r2, r3, hint) ia64_ld8_c_clr_acq_inc_hint_pred ((code), 0, r1, r2, r3, hint)


#define ia64_ld1_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_s_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_s_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_s_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_s_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_s_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_s_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_s_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_s_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_a_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_a_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_a_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_a_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_a_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_a_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_a_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_a_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_sa_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_sa_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_sa_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_sa_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_sa_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_sa_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_sa_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_sa_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_bias_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_bias_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_bias_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_bias_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_bias_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_bias_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_bias_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_bias_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld8_fill_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_fill_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_c_clr_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_c_clr_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_c_clr_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_c_clr_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_c_clr_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_c_clr_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_c_clr_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_c_clr_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_c_nc_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_c_nc_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_c_nc_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_c_nc_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_c_nc_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_c_nc_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_c_nc_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_c_nc_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)

#define ia64_ld1_c_clr_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld1_c_clr_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld2_c_clr_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld2_c_clr_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld4_c_clr_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld4_c_clr_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)
#define ia64_ld8_c_clr_acq_inc_imm_hint(code, r1, r3, imm, hint) ia64_ld8_c_clr_acq_inc_imm_hint_pred ((code), 0, r1, r3, imm, hint)


#define ia64_st1_hint(code, r3, r2, hint) ia64_st1_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st2_hint(code, r3, r2, hint) ia64_st2_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st4_hint(code, r3, r2, hint) ia64_st4_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st8_hint(code, r3, r2, hint) ia64_st8_hint_pred ((code), 0, r3, r2, hint)

#define ia64_st1_rel_hint(code, r3, r2, hint) ia64_st1_rel_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st2_rel_hint(code, r3, r2, hint) ia64_st2_rel_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st4_rel_hint(code, r3, r2, hint) ia64_st4_rel_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st8_rel_hint(code, r3, r2, hint) ia64_st8_rel_hint_pred ((code), 0, r3, r2, hint)

#define ia64_st8_spill_hint(code, r3, r2, hint) ia64_st8_spill_hint_pred ((code), 0, r3, r2, hint)

#define ia64_st16_hint(code, r3, r2, hint) ia64_st16_hint_pred ((code), 0, r3, r2, hint)
#define ia64_st16_rel_hint(code, r3, r2, hint) ia64_st16_rel_hint_pred ((code), 0, r3, r2, hint)


#define ia64_st1_inc_imm_hint(code, r3, r2, imm, hint) ia64_st1_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st2_inc_imm_hint(code, r3, r2, imm, hint) ia64_st2_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st4_inc_imm_hint(code, r3, r2, imm, hint) ia64_st4_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st8_inc_imm_hint(code, r3, r2, imm, hint) ia64_st8_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)

#define ia64_st1_rel_inc_imm_hint(code, r3, r2, imm, hint) ia64_st1_rel_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st2_rel_inc_imm_hint(code, r3, r2, imm, hint) ia64_st2_rel_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st4_rel_inc_imm_hint(code, r3, r2, imm, hint) ia64_st4_rel_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)
#define ia64_st8_rel_inc_imm_hint(code, r3, r2, imm, hint) ia64_st8_rel_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)

#define ia64_st8_spill_inc_imm_hint(code, r3, r2, imm, hint) ia64_st8_spill_inc_imm_hint_pred ((code), 0, r3, r2, imm, hint)


#define ia64_ldfs_hint(code, f1, r3, hint) ia64_ldfs_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_hint(code, f1, r3, hint) ia64_ldfd_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_hint(code, f1, r3, hint) ia64_ldf8_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_hint(code, f1, r3, hint) ia64_ldfe_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldfs_s_hint(code, f1, r3, hint) ia64_ldfs_s_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_s_hint(code, f1, r3, hint) ia64_ldfd_s_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_s_hint(code, f1, r3, hint) ia64_ldf8_s_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_s_hint(code, f1, r3, hint) ia64_ldfe_s_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldfs_a_hint(code, f1, r3, hint) ia64_ldfs_a_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_a_hint(code, f1, r3, hint) ia64_ldfd_a_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_a_hint(code, f1, r3, hint) ia64_ldf8_a_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_a_hint(code, f1, r3, hint) ia64_ldfe_a_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldfs_sa_hint(code, f1, r3, hint) ia64_ldfs_sa_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_sa_hint(code, f1, r3, hint) ia64_ldfd_sa_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_sa_hint(code, f1, r3, hint) ia64_ldf8_sa_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_sa_hint(code, f1, r3, hint) ia64_ldfe_sa_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldfs_c_clr_hint(code, f1, r3, hint) ia64_ldfs_c_clr_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_c_clr_hint(code, f1, r3, hint) ia64_ldfd_c_clr_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_c_clr_hint(code, f1, r3, hint) ia64_ldf8_c_clr_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_c_clr_hint(code, f1, r3, hint) ia64_ldfe_c_clr_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldfs_c_nc_hint(code, f1, r3, hint) ia64_ldfs_c_nc_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfd_c_nc_hint(code, f1, r3, hint) ia64_ldfd_c_nc_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldf8_c_nc_hint(code, f1, r3, hint) ia64_ldf8_c_nc_hint_pred ((code), 0, f1, r3, hint)
#define ia64_ldfe_c_nc_hint(code, f1, r3, hint) ia64_ldfe_c_nc_hint_pred ((code), 0, f1, r3, hint)

#define ia64_ldf_fill_hint(code, f1, r3, hint) ia64_ldf_fill_hint_pred ((code), 0, f1, r3, hint)


#define ia64_ldfs_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldfs_s_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_s_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_s_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_s_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_s_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_s_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_s_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_s_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldfs_a_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_a_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_a_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_a_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_a_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_a_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_a_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_a_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldfs_sa_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_sa_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_sa_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_sa_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_sa_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_sa_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_sa_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_sa_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldfs_c_clr_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_c_clr_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_c_clr_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_c_clr_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_c_clr_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_c_clr_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_c_clr_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_c_clr_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldfs_c_nc_inc_hint(code, f1, r3, r2, hint) ia64_ldfs_c_nc_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfd_c_nc_inc_hint(code, f1, r3, r2, hint) ia64_ldfd_c_nc_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldf8_c_nc_inc_hint(code, f1, r3, r2, hint) ia64_ldf8_c_nc_inc_hint_pred ((code), 0, f1, r3, r2, hint)
#define ia64_ldfe_c_nc_inc_hint(code, f1, r3, r2, hint) ia64_ldfe_c_nc_inc_hint_pred ((code), 0, f1, r3, r2, hint)

#define ia64_ldf_fill_inc_hint(code, f1, r3, r2, hint) ia64_ldf_fill_inc_hint_pred ((code), 0, f1, r3, r2, hint)


#define ia64_ldfs_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldfs_s_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_s_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_s_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_s_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_s_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_s_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_s_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_s_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldfs_a_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_a_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_a_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_a_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_a_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_a_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_a_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_a_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldfs_sa_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_sa_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_sa_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_sa_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_sa_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_sa_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_sa_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_sa_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldfs_c_clr_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_c_clr_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_c_clr_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_c_clr_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_c_clr_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_c_clr_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_c_clr_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_c_clr_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldfs_c_nc_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfs_c_nc_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfd_c_nc_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfd_c_nc_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldf8_c_nc_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf8_c_nc_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)
#define ia64_ldfe_c_nc_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldfe_c_nc_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)

#define ia64_ldf_fill_inc_imm_hint(code, f1, r3, imm, hint) ia64_ldf_fill_inc_imm_hint_pred ((code), 0, f1, r3, imm, hint)


#define ia64_stfs_hint(code, r3, f2, hint) ia64_stfs_hint_pred ((code), 0, r3, f2, hint)
#define ia64_stfd_hint(code, r3, f2, hint) ia64_stfd_hint_pred ((code), 0, r3, f2, hint)
#define ia64_stf8_hint(code, r3, f2, hint) ia64_stf8_hint_pred ((code), 0, r3, f2, hint)
#define ia64_stfe_hint(code, r3, f2, hint) ia64_stfe_hint_pred ((code), 0, r3, f2, hint)
#define ia64_stf_spill_hint(code, r3, f2, hint) ia64_stf_spill_hint_pred ((code), 0, r3, f2, hint)


#define ia64_stfs_inc_imm_hint(code, r3, f2, imm, hint) ia64_stfs_inc_imm_hint_pred ((code), 0, r3, f2, imm, hint)
#define ia64_stfd_inc_imm_hint(code, r3, f2, imm, hint) ia64_stfd_inc_imm_hint_pred ((code), 0, r3, f2, imm, hint)
#define ia64_stf8_inc_imm_hint(code, r3, f2, imm, hint) ia64_stf8_inc_imm_hint_pred ((code), 0, r3, f2, imm, hint)
#define ia64_stfe_inc_imm_hint(code, r3, f2, imm, hint) ia64_stfe_inc_imm_hint_pred ((code), 0, r3, f2, imm, hint)
#define ia64_stf_spill_inc_imm_hint(code, r3, f2, imm, hint) ia64_stf_spill_inc_imm_hint_pred ((code), 0, r3, f2, imm, hint)


#define ia64_ldfps_hint(code, f1, f2, r3, hint) ia64_ldfps_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_hint(code, f1, f2, r3, hint) ia64_ldfpd_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_hint(code, f1, f2, r3, hint) ia64_ldfp8_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_s_hint(code, f1, f2, r3, hint) ia64_ldfps_s_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_s_hint(code, f1, f2, r3, hint) ia64_ldfpd_s_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_s_hint(code, f1, f2, r3, hint) ia64_ldfp8_s_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_a_hint(code, f1, f2, r3, hint) ia64_ldfps_a_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_a_hint(code, f1, f2, r3, hint) ia64_ldfpd_a_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_a_hint(code, f1, f2, r3, hint) ia64_ldfp8_a_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_sa_hint(code, f1, f2, r3, hint) ia64_ldfps_sa_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_sa_hint(code, f1, f2, r3, hint) ia64_ldfpd_sa_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_sa_hint(code, f1, f2, r3, hint) ia64_ldfp8_sa_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_c_clr_hint(code, f1, f2, r3, hint) ia64_ldfps_c_clr_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_c_clr_hint(code, f1, f2, r3, hint) ia64_ldfpd_c_clr_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_c_clr_hint(code, f1, f2, r3, hint) ia64_ldfp8_c_clr_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_c_nc_hint(code, f1, f2, r3, hint) ia64_ldfps_c_nc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_c_nc_hint(code, f1, f2, r3, hint) ia64_ldfpd_c_nc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_c_nc_hint(code, f1, f2, r3, hint) ia64_ldfp8_c_nc_hint_pred ((code), 0, f1, f2, r3, hint)


#define ia64_ldfps_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_s_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_s_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_s_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_s_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_s_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_s_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_a_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_a_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_a_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_a_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_a_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_a_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_sa_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_sa_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_sa_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_sa_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_sa_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_sa_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_c_clr_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_c_clr_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_c_clr_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_c_clr_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_c_clr_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_c_clr_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_ldfps_c_nc_inc_hint(code, f1, f2, r3, hint) ia64_ldfps_c_nc_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfpd_c_nc_inc_hint(code, f1, f2, r3, hint) ia64_ldfpd_c_nc_inc_hint_pred ((code), 0, f1, f2, r3, hint)
#define ia64_ldfp8_c_nc_inc_hint(code, f1, f2, r3, hint) ia64_ldfp8_c_nc_inc_hint_pred ((code), 0, f1, f2, r3, hint)

#define ia64_lfetch_hint(code, r3, hint) ia64_lfetch_hint_pred ((code), 0, r3, hint)
#define ia64_lfetch_excl_hint(code, r3, hint) ia64_lfetch_excl_hint_pred ((code), 0, r3, hint)
#define ia64_lfetch_fault_hint(code, r3, hint) ia64_lfetch_fault_hint_pred ((code), 0, r3, hint)
#define ia64_lfetch_fault_excl_hint(code, r3, hint) ia64_lfetch_fault_excl_hint_pred ((code), 0, r3, hint)


#define ia64_lfetch_inc_hint(code, r3, r2, hint) ia64_lfetch_inc_hint_pred ((code), 0, r3, r2, hint)
#define ia64_lfetch_excl_inc_hint(code, r3, r2, hint) ia64_lfetch_excl_inc_hint_pred ((code), 0, r3, r2, hint)
#define ia64_lfetch_fault_inc_hint(code, r3, r2, hint) ia64_lfetch_fault_inc_hint_pred ((code), 0, r3, r2, hint)
#define ia64_lfetch_fault_excl_inc_hint(code, r3, r2, hint) ia64_lfetch_fault_excl_inc_hint_pred ((code), 0, r3, r2, hint)


#define ia64_lfetch_inc_imm_hint(code, r3, imm, hint) ia64_lfetch_inc_imm_hint_pred ((code), 0, r3, imm, hint)
#define ia64_lfetch_excl_inc_imm_hint(code, r3, imm, hint) ia64_lfetch_excl_inc_imm_hint_pred ((code), 0, r3, imm, hint)
#define ia64_lfetch_fault_inc_imm_hint(code, r3, imm, hint) ia64_lfetch_fault_inc_imm_hint_pred ((code), 0, r3, imm, hint)
#define ia64_lfetch_fault_excl_inc_imm_hint(code, r3, imm, hint) ia64_lfetch_fault_excl_inc_imm_hint_pred ((code), 0, r3, imm, hint)


#define ia64_cmpxchg1_acq_hint(code, r1, r3, r2, hint) ia64_cmpxchg1_acq_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg2_acq_hint(code, r1, r3, r2, hint) ia64_cmpxchg2_acq_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg4_acq_hint(code, r1, r3, r2, hint) ia64_cmpxchg4_acq_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg8_acq_hint(code, r1, r3, r2, hint) ia64_cmpxchg8_acq_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg1_rel_hint(code, r1, r3, r2, hint) ia64_cmpxchg1_rel_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg2_rel_hint(code, r1, r3, r2, hint) ia64_cmpxchg2_rel_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg4_rel_hint(code, r1, r3, r2, hint) ia64_cmpxchg4_rel_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg8_rel_hint(code, r1, r3, r2, hint) ia64_cmpxchg8_rel_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg16_acq_hint(code, r1, r3, r2, hint) ia64_cmpxchg16_acq_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_cmpxchg16_rel_hint(code, r1, r3, r2, hint) ia64_cmpxchg16_rel_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_xchg1_hint(code, r1, r3, r2, hint) ia64_xchg1_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_xchg2_hint(code, r1, r3, r2, hint) ia64_xchg2_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_xchg4_hint(code, r1, r3, r2, hint) ia64_xchg4_hint_pred ((code), 0, r1, r3, r2, hint)
#define ia64_xchg8_hint(code, r1, r3, r2, hint) ia64_xchg8_hint_pred ((code), 0, r1, r3, r2, hint)

#define ia64_fetchadd4_acq_hint(code, r1, r3, inc, hint) ia64_fetchadd4_acq_hint_pred ((code), 0, r1, r3, inc, hint)
#define ia64_fetchadd8_acq_hint(code, r1, r3, inc, hint) ia64_fetchadd8_acq_hint_pred ((code), 0, r1, r3, inc, hint)
#define ia64_fetchadd4_rel_hint(code, r1, r3, inc, hint) ia64_fetchadd4_rel_hint_pred ((code), 0, r1, r3, inc, hint)
#define ia64_fetchadd8_rel_hint(code, r1, r3, inc, hint) ia64_fetchadd8_rel_hint_pred ((code), 0, r1, r3, inc, hint)


#define ia64_setf_sig(code, f1, r2) ia64_setf_sig_pred ((code), 0, f1, r2)
#define ia64_setf_exp(code, f1, r2) ia64_setf_exp_pred ((code), 0, f1, r2)
#define ia64_setf_s(code, f1, r2) ia64_setf_s_pred ((code), 0, f1, r2)
#define ia64_setf_d(code, f1, r2) ia64_setf_d_pred ((code), 0, f1, r2)


#define ia64_getf_sig(code, r1, f2) ia64_getf_sig_pred ((code), 0, r1, f2)
#define ia64_getf_exp(code, r1, f2) ia64_getf_exp_pred ((code), 0, r1, f2)
#define ia64_getf_s(code, r1, f2) ia64_getf_s_pred ((code), 0, r1, f2)
#define ia64_getf_d(code, r1, f2) ia64_getf_d_pred ((code), 0, r1, f2)


#define ia64_chk_s_m(code, r2,disp) ia64_chk_s_m_pred ((code), 0, r2,disp)


#define ia64_chk_s_float_m(code, f2,disp) ia64_chk_s_float_m_pred ((code), 0, f2,disp)


#define ia64_chk_a_nc(code, r1,disp) ia64_chk_a_nc_pred ((code), 0, r1,disp)
#define ia64_chk_a_clr(code, r1,disp) ia64_chk_a_clr_pred ((code), 0, r1,disp)


#define ia64_chk_a_nc_float(code, f1,disp) ia64_chk_a_nc_float_pred ((code), 0, f1,disp)
#define ia64_chk_a_clr_float(code, f1,disp) ia64_chk_a_clr_float_pred ((code), 0, f1,disp)


#define ia64_invala(code) ia64_invala_pred ((code), 0)
#define ia64_fwb(code) ia64_fwb_pred ((code), 0)
#define ia64_mf(code) ia64_mf_pred ((code), 0)
#define ia64_mf_a(code) ia64_mf_a_pred ((code), 0)
#define ia64_srlz_d(code) ia64_srlz_d_pred ((code), 0)
#define ia64_stlz_i(code) ia64_stlz_i_pred ((code), 0)
#define ia64_sync_i(code) ia64_sync_i_pred ((code), 0)


#define ia64_flushrs(code) ia64_flushrs_pred ((code), 0)
#define ia64_loadrs(code) ia64_loadrs_pred ((code), 0)

#define ia64_invala_e(code, r1) ia64_invala_e_pred ((code), 0, r1)


#define ia64_invala_e_float(code, f1) ia64_invala_e_float_pred ((code), 0, f1)


#define ia64_fc(code, r3) ia64_fc_pred ((code), 0, r3)
#define ia64_fc_i(code, r3) ia64_fc_i_pred ((code), 0, r3)


#define ia64_mov_to_ar_m(code, ar3, r2) ia64_mov_to_ar_m_pred ((code), 0, ar3, r2)


#define ia64_mov_to_ar_imm_m(code, ar3, imm) ia64_mov_to_ar_imm_m_pred ((code), 0, ar3, imm)


#define ia64_mov_from_ar_m(code, r1, ar3) ia64_mov_from_ar_m_pred ((code), 0, r1, ar3)

#define ia64_mov_to_cr(code, cr3, r2) ia64_mov_to_cr_pred ((code), 0, cr3, r2)


#define ia64_mov_from_cr(code, r1, cr3) ia64_mov_from_cr_pred ((code), 0, r1, cr3)


#define ia64_alloc(code, r1, i, l, o, r) ia64_alloc_pred ((code), 0, r1, i, l, o, r)


#define ia64_mov_to_psr_l(code, r2) ia64_mov_to_psr_l_pred ((code), 0, r2)
#define ia64_mov_to_psr_um(code, r2) ia64_mov_to_psr_um_pred ((code), 0, r2)


#define ia64_mov_from_psr(code, r1) ia64_mov_from_psr_pred ((code), 0, r1)
#define ia64_mov_from_psr_um(code, r1) ia64_mov_from_psr_um_pred ((code), 0, r1)


#define ia64_break_m(code, imm) ia64_break_m_pred ((code), 0, imm)

/* The System/Memory Management instruction encodings (M38-M47) */


#define ia64_nop_m(code, imm) ia64_nop_m_pred ((code), 0, imm)
#define ia64_hint_m(code, imm) ia64_hint_m_pred ((code), 0, imm)

#define ia64_br_cond_hint(code, disp, bwh, ph, dh) ia64_br_cond_hint_pred ((code), 0, disp, bwh, ph, dh)
#define ia64_br_wexit_hint(code, disp, bwh, ph, dh) ia64_br_wexit_hint_pred ((code), 0, disp, bwh, ph, dh)
#define ia64_br_wtop_hint(code, disp, bwh, ph, dh) ia64_br_wtop_hint_pred ((code), 0, disp, bwh, ph, dh)


#define ia64_br_cloop_hint(code, disp, bwh, ph, dh) ia64_br_cloop_hint_pred ((code), 0, disp, bwh, ph, dh)
#define ia64_br_cexit_hint(code, disp, bwh, ph, dh) ia64_br_cexit_hint_pred ((code), 0, disp, bwh, ph, dh)
#define ia64_br_ctop_hint(code, disp, bwh, ph, dh) ia64_br_ctop_hint_pred ((code), 0, disp, bwh, ph, dh)


#define ia64_br_call_hint(code, b1, disp, bwh, ph, dh) ia64_br_call_hint_pred ((code), 0, b1, disp, bwh, ph, dh)


#define ia64_br_cond_reg_hint(code, b1, bwh, ph, dh) ia64_br_cond_reg_hint_pred ((code), 0, b1, bwh, ph, dh)
#define ia64_br_ia_reg_hint(code, b1, bwh, ph, dh) ia64_br_ia_reg_hint_pred ((code), 0, b1, bwh, ph, dh)
#define ia64_br_ret_reg_hint(code, b1, bwh, ph, dh) ia64_br_ret_reg_hint_pred ((code), 0, b1, bwh, ph, dh)


#define ia64_br_call_reg_hint(code, b1, b2, bwh, ph, dh) ia64_br_call_reg_hint_pred ((code), 0, b1, b2, bwh, ph, dh)

#define ia64_cover(code) ia64_cover_pred ((code), 0)
#define ia64_clrrrb(code) ia64_clrrrb_pred ((code), 0)
#define ia64_clrrrb_pr(code) ia64_clrrrb_pr_pred ((code), 0)
#define ia64_rfi(code) ia64_rfi_pred ((code), 0)
#define ia64_bsw_0(code) ia64_bsw_0_pred ((code), 0)
#define ia64_bsw_1(code) ia64_bsw_1_pred ((code), 0)
#define ia64_epc(code) ia64_epc_pred ((code), 0)


#define ia64_break_b(code, imm) ia64_break_b_pred ((code), 0, imm)
#define ia64_nop_b(code, imm) ia64_nop_b_pred ((code), 0, imm)
#define ia64_hint_b(code, imm) ia64_hint_b_pred ((code), 0, imm)


#define ia64_break_x(code, imm) ia64_break_x_pred ((code), 0, imm)


#define ia64_movl(code, r1, imm) ia64_movl_pred ((code), 0, (r1), (imm))


#define ia64_brl_cond_hint(code, disp, bwh, ph, dh) ia64_brl_cond_hint_pred ((code), 0, disp, bwh, ph, dh)


#define ia64_brl_call_hint(code, b1, disp, bwh, ph, dh) ia64_brl_call_hint_pred ((code), 0, b1, disp, bwh, ph, dh)


#define ia64_nop_x(code, imm) ia64_nop_x_pred ((code), 0, imm)
#define ia64_hint_x(code, imm) ia64_hint_x_pred ((code), 0, imm)

/*
 * Pseudo-ops
 */

#define ia64_mov_pred(code, qp, r1, r3) ia64_adds_imm_pred ((code), (qp), (r1), 0, (r3))
#define ia64_mov(code, r1, r3) ia64_mov_pred ((code), 0, (r1), (r3))

#endif
