
/* copy mask x color -> dst  (color assumed pre-mul) */

/*-----*/
#ifdef OP_FN /* do it in mmx */
OPFN(_op_copy_mas_c_dp_mmx) {
   DATA32 *d, *e, c;
   DATA8  *m;
   d = p->dst.p; e = d + p->l;
   m = p->src.m;
   c = p->src.c;

   pxor_r2r(mm0, mm0);
   MOV_P2R(c, mm2, mm0)
   while (d < e)
     {
	DATA32 a = *m;

	switch(a)
	  {
	    case 0:
		*d = 0;
		break;
	    case 255:
		*d = c;
		break;
	    default:
		a++;
		MOV_A2R(a, mm1)
		MUL4_256_R2R(mm2, mm1)
		MOV_R2P(mm1, *d, mm0)
		break;
	  }
	m++;  d++;
     }
   did_mmx = 1;
}
#endif

#ifdef OP_FN /* do it in mmx */
OPFN(_op_copy_mas_cn_dp_mmx) {
   DATA32 *d, *e;
   DATA8  *m;
   d = p->dst.p; e = d + p->l;
   m = p->src.m;

   while (d < e)
     {
	DATA32 a = *m;

	switch(a)
	  {
	    case 0:
		*d = 0;
		break;
	    case 255:
		*d = 0xffffffff;
		break;
	    default:
		*d = ARGB_JOIN(a, a, a, a);
		break;
	  }
	m++;  d++;
     }
   did_mmx = 1;
}
#endif


#ifdef OP_FN /* do it in mmx */
OPFN(_op_copy_mas_caa_dp_mmx) {
   DATA32 *d, *e, c, ca;
   DATA8  *m;
   d = p->dst.p; e = d + p->l;
   m = p->src.m;
   c = p->src.c;

   pxor_r2r(mm0, mm0);
   ca = 1 + (c & 0xff);
   MOV_A2R(ca, mm2)
   while (d < e)
     {
	DATA32 a = *m;

	switch(a)
	  {
	    case 0:
		*d = 0;
		break;
	    case 255:
		*d = c;
		break;
	    default:
		MOV_A2R(a, mm1)
		MUL4_256_R2R(mm2, mm1)
		MOV_R2P(mm1, *d, mm0)
		break;
	  }
	m++;  d++;
     }
   did_mmx = 1;
}
#endif

#ifdef OP_FN
#define _op_copy_mas_can_dp_mmx _op_copy_mas_c_dp_mmx

#define _op_copy_mas_c_dpan_mmx _op_copy_mas_c_dp_mmx
#define _op_copy_mas_cn_dpan_mmx _op_copy_mas_cn_dp_mmx
#define _op_copy_mas_can_dpan_mmx _op_copy_mas_can_dp_mmx
#define _op_copy_mas_caa_dpan_mmx _op_copy_mas_caa_dp_mmx

#define _op_copy_mas_c_dpas_mmx _op_copy_mas_c_dp_mmx
#define _op_copy_mas_cn_dpas_mmx _op_copy_mas_cn_dp_mmx
#define _op_copy_mas_can_dpas_mmx _op_copy_mas_can_dp_mmx
#define _op_copy_mas_caa_dpas_mmx _op_copy_mas_caa_dp_mmx
#endif

#ifdef OP_REG
OPREG(_op_copy_mas_cn_dp_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_N, DP, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_c_dp_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC, DP, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_can_dp_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AN, DP, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_caa_dp_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AA, DP, CPU_MMX); /* what parameters it does */

OPREG(_op_copy_mas_cn_dpan_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_N, DP_AN, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_c_dpan_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC, DP_AN, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_can_dpan_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AN, DP_AN, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_caa_dpan_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AA, DP_AN, CPU_MMX); /* what parameters it does */

OPREG(_op_copy_mas_cn_dpas_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_N, DP_AS, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_c_dpas_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC, DP_AS, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_can_dpas_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AN, DP_AS, CPU_MMX); /* what parameters it does */
OPREG(_op_copy_mas_caa_dpas_mmx, PIXEL_OP_COPY, /* register the function */
      SP_N, SM_AS, SC_AA, DP_AS, CPU_MMX); /* what parameters it does */
#endif
/*-----*/

