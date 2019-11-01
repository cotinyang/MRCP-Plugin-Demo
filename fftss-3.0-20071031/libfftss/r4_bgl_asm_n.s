#
# Copyright (C) 2002-2007 Akira Nukada. All rights reserved.
# Copyright (C) 2002-2007 The SSI Project. All rights reserved. 
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the project nor the names of its contributors 
#       may be used to endorse or promote products derived from this software 
#       without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE SSI PROJECT ``AS IS'' AND ANY EXPRESS 
# OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE SSI PROJECT BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
.set r0,0; .set SP,1; .set RTOC,2; .set r3,3; .set r4,4
.set r5,5; .set r6,6; .set r7,7; .set r8,8; .set r9,9
.set r10,10; .set r11,11; .set r12,12; .set r13,13; .set r14,14
.set r15,15; .set r16,16; .set r17,17; .set r18,18; .set r19,19
.set r20,20; .set r21,21; .set r22,22; .set r23,23; .set r24,24
.set r25,25; .set r26,26; .set r27,27; .set r28,28; .set r29,29
.set r30,30; .set r31,31
.set fp0,0; .set fp1,1; .set fp2,2; .set fp3,3; .set fp4,4
.set fp5,5; .set fp6,6; .set fp7,7; .set fp8,8; .set fp9,9
.set fp10,10; .set fp11,11; .set fp12,12; .set fp13,13; .set fp14,14
.set fp15,15; .set fp16,16; .set fp17,17; .set fp18,18; .set fp19,19
.set fp20,20; .set fp21,21; .set fp22,22; .set fp23,23; .set fp24,24
.set fp25,25; .set fp26,26; .set fp27,27; .set fp28,28; .set fp29,29
.set fp30,30; .set fp31,31
.set MQ,0; .set XER,1; .set FROM_RTCU,4; .set FROM_RTCL,5; .set FROM_DEC,6
.set LR,8; .set CTR,9; .set TID,17; .set DSISR,18; .set DAR,19; .set TO_RTCU,20
.set TO_RTCL,21; .set TO_DEC,22; .set SDR_0,24; .set SDR_1,25; .set SRR_0,26
.set SRR_1,27
.set BO_dCTR_NZERO_AND_NOT,0; .set BO_dCTR_NZERO_AND_NOT_1,1
.set BO_dCTR_ZERO_AND_NOT,2; .set BO_dCTR_ZERO_AND_NOT_1,3
.set BO_IF_NOT,4; .set BO_IF_NOT_1,5; .set BO_IF_NOT_2,6
.set BO_IF_NOT_3,7; .set BO_dCTR_NZERO_AND,8; .set BO_dCTR_NZERO_AND_1,9
.set BO_dCTR_ZERO_AND,10; .set BO_dCTR_ZERO_AND_1,11; .set BO_IF,12
.set BO_IF_1,13; .set BO_IF_2,14; .set BO_IF_3,15; .set BO_dCTR_NZERO,16
.set BO_dCTR_NZERO_1,17; .set BO_dCTR_ZERO,18; .set BO_dCTR_ZERO_1,19
.set BO_ALWAYS,20; .set BO_ALWAYS_1,21; .set BO_ALWAYS_2,22
.set BO_ALWAYS_3,23; .set BO_dCTR_NZERO_8,24; .set BO_dCTR_NZERO_9,25
.set BO_dCTR_ZERO_8,26; .set BO_dCTR_ZERO_9,27; .set BO_ALWAYS_8,28
.set BO_ALWAYS_9,29; .set BO_ALWAYS_10,30; .set BO_ALWAYS_11,31
.set CR0_LT,0; .set CR0_GT,1; .set CR0_EQ,2; .set CR0_SO,3
.set CR1_FX,4; .set CR1_FEX,5; .set CR1_VX,6; .set CR1_OX,7
.set CR2_LT,8; .set CR2_GT,9; .set CR2_EQ,10; .set CR2_SO,11
.set CR3_LT,12; .set CR3_GT,13; .set CR3_EQ,14; .set CR3_SO,15
.set CR4_LT,16; .set CR4_GT,17; .set CR4_EQ,18; .set CR4_SO,19
.set CR5_LT,20; .set CR5_GT,21; .set CR5_EQ,22; .set CR5_SO,23
.set CR6_LT,24; .set CR6_GT,25; .set CR6_EQ,26; .set CR6_SO,27
.set CR7_LT,28; .set CR7_GT,29; .set CR7_EQ,30; .set CR7_SO,31
.set TO_LT,16; .set TO_GT,8; .set TO_EQ,4; .set TO_LLT,2; .set TO_LGT,1

	.file	"r4_bgl_asm_n.s"
	.globl	fftss_r4_bgl_asm_n_f
	.type	fftss_r4_bgl_asm_n_f,@function
	.size	fftss_r4_bgl_asm_n_f,1700

	.section	".text"
	.align	3
.The_CodeLC:
fftss_r4_bgl_asm_n_f:
	stwu		SP,-400(SP)
	
	mftb		r8
	stw			r8,28(SP)

	stw			r16,32(SP)
	stw			r17,36(SP)
	stw			r18,40(SP)
	stw			r19,44(SP)
	stw			r20,48(SP)
	stw			r21,52(SP)
	stw			r22,56(SP)
	stw			r23,60(SP)
	stw			r24,64(SP)
	stw			r25,68(SP)
	stw			r26,72(SP)
	stw			r27,76(SP)
	stw			r28,80(SP)
	stw			r29,84(SP)

        addi            r8,SP,96
        li              r9,16
        stfpdux         fp14,r8,r9
        stfpdux         fp15,r8,r9
        stfpdux         fp16,r8,r9
        stfpdux         fp17,r8,r9
        stfpdux         fp18,r8,r9
        stfpdux         fp19,r8,r9
        stfpdux         fp20,r8,r9
        stfpdux         fp21,r8,r9
        stfpdux         fp22,r8,r9
        stfpdux         fp23,r8,r9
#       stfpdux         fp24,r8,r9
#       stfpdux         fp25,r8,r9
#       stfpdux         fp26,r8,r9
#       stfpdux         fp27,r8,r9
#       stfpdux         fp28,r8,r9
#       stfpdux         fp29,r8,r9
#       stfpdux         fp30,r8,r9
#       stfpdux         fp31,r8,r9
	
#	r16=vi0,r17=vi1,r18=vi2,r19=vi3
	slwi		r19,r6,4
	addi		r16,r3,0
	add		r17,r3,r19
	add		r18,r17,r19
	add		r19,r18,r19
#	slwi		r20,r6,5
#	slwi		r21,r6,4
#	add		r23,r21,r20
	li		r22,16
#	addi		r23,r23,16

#	r3=vi,r4=vo,r5=wn,r6=bsize,r7=blocks
#   
	mullw		r11,r6,r7
	srawi		r26,r11,2
	addi		r26,r26,-1
	mtctr		r26
	slwi		r11,r11,4
	
#	r24=count,r25=mask0,r26=mask1.
	li		r24,1
	srawi		r25,r6,2
	addi		r26,r25,-1

#	r8=vo0,r9=vo1,r10=vo2,r11=vo3
	addi		r8,r4,-16
	add		r9,r8,r11
	add		r10,r9,r11
	add		r11,r10,r11
	
#	r4=w1,r5=w2,r6=w3
	addi		r4,r5,0
	sub		r6,r11,r8
	add		r6,r6,r5

#	index updates.
#
#	and		r27,r24,r25
#	mulli		r29,r27,192
#	and		r24,r24,r26
#	slwi		r28,r27,6
#	add		r23,r22,r29
#	addi		r21,r28,0
#	add		r20,r28,r28
#	addi		r24,r24,1

	
	lfpdx		fp2,r0,r18
	lfxdx		fp20,r0,r5
	lfpdx		fp3,r0,r19
	lfxdx		fp21,r0,r6
	lfpdx		fp1,r0,r17
	lfxdx		fp19,r0,r4
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdx		fp0,r0,r16
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	lfpdx		fp17,r0,r5
	lfpdux		fp3,r19,r22
	lfpdx		fp18,r0,r6
	lfpdux		fp1,r17,r22
	lfpdx		fp16,r0,r4
	fxcxnsma	fp8,fp17,fp6,fp0
	fxcxnpma	fp10,fp17,fp6,fp0
	fxcsmadd	fp9,fp18,fp7,fp5
	fxcsnmsub	fp11,fp18,fp7,fp5
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	and			r27,r24,r25
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnpma	fp13,fp16,fp9,fp8
	mulli		r29,r27,192
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	and			r24,r24,r26
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	slwi		r28,r27,6
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnpma	fp13,fp16,fp9,fp8
	add		r23,r22,r29
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	addi		r21,r28,0
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2	
	add		r20,r28,r28
	addi		r24,r24,1	
		

#	D=fxcxnpma(C,B,A) -> fxcxnpma D,A,B,C
F0:
	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r23
	fxcxnpma	fp5,fp19,fp1,fp1
	lfxdx		fp20,r5,r20
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r23
	fxcxnpma	fp13,fp16,fp9,fp8
	lfxdx		fp21,r6,r21
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r23
	fxcsmadd	fp15,fp16,fp11,fp10
	lfxdx		fp19,r4,r21
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r23
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	lfpdux		fp17,r5,r20
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnpma	fp13,fp16,fp9,fp8
	lfpdux		fp18,r6,r21
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	lfpdux		fp16,r4,r21
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	and		r27,r24,r25
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnpma	fp13,fp16,fp9,fp8
	mulli		r29,r27,192
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	and		r24,r24,r26
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnpma	fp5,fp19,fp1,fp1
	slwi		r28,r27,6
	fxcxnsma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnpma	fp13,fp16,fp9,fp8
	add		r23,r22,r29
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	addi		r21,r28,0
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnpma	fp6,fp20,fp2,fp2	
	add		r20,r28,r28
	addi		r24,r24,1	

	bdnz+	F0

	lfpdux		fp0,r16,r22
	fxcxnpma	fp7,fp21,fp3,fp3
	fxcxnpma	fp5,fp19,fp1,fp1
	fxcxnsma	fp12,fp16,fp9,fp8
	fxcxnpma	fp13,fp16,fp9,fp8
	fxcsnmsub	fp14,fp16,fp11,fp10
	fxcsmadd	fp15,fp16,fp11,fp10
	fxcxnsma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnpma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	
	fxcxnsma	fp12,fp16,fp9,fp8
	fxcxnpma	fp13,fp16,fp9,fp8
	fxcsnmsub	fp14,fp16,fp11,fp10
	fxcsmadd	fp15,fp16,fp11,fp10
	stfpdux		fp12,r8,r22
	stfpdux		fp13,r10,r22
	stfpdux		fp14,r9,r22
	stfpdux		fp15,r11,r22

        addi            r8,SP,96
        li              r9,16
        lfpdux          fp14,r8,r9
        lfpdux          fp15,r8,r9
        lfpdux          fp16,r8,r9
        lfpdux          fp17,r8,r9
        lfpdux          fp18,r8,r9
        lfpdux          fp19,r8,r9
        lfpdux          fp20,r8,r9
        lfpdux          fp21,r8,r9
        lfpdux          fp22,r8,r9
        lfpdux          fp23,r8,r9
#       lfpdux          fp24,r8,r9
#       lfpdux          fp25,r8,r9
#       lfpdux          fp26,r8,r9
#       lfpdux          fp27,r8,r9
#       lfpdux          fp28,r8,r9
#       lfpdux          fp29,r8,r9
#       lfpdux          fp30,r8,r9
#       lfpdux          fp31,r8,r9
	

	lwz		r9,28(SP)
	lwz		r16,32(SP)
	lwz		r17,36(SP)
	lwz		r18,40(SP)
	lwz		r19,44(SP)
	lwz		r20,48(SP)
	lwz		r21,52(SP)
	lwz		r22,56(SP)
	lwz		r23,60(SP)
	lwz		r24,64(SP)
	lwz		r25,68(SP)
	lwz		r26,72(SP)
	lwz		r27,76(SP)
	lwz		r28,80(SP)
	lwz		r29,84(SP)
	
	
	addi	SP,SP,400
	bclr	BO_ALWAYS,CR0_LT

	.globl	fftss_r4_bgl_asm_n_b
	.type	fftss_r4_bgl_asm_n_b,@function
	.size	fftss_r4_bgl_asm_n_b,1700

	.section	".text"
	.align	3
fftss_r4_bgl_asm_n_b:
	stwu		SP,-400(SP)
	

	stw			r16,32(SP)
	stw			r17,36(SP)
	stw			r18,40(SP)
	stw			r19,44(SP)
	stw			r20,48(SP)
	stw			r21,52(SP)
	stw			r22,56(SP)
	stw			r23,60(SP)
	stw			r24,64(SP)
	stw			r25,68(SP)
	stw			r26,72(SP)
	stw			r27,76(SP)
	stw			r28,80(SP)
	stw			r29,84(SP)

        addi            r8,SP,96
        li              r9,16
        stfpdux         fp14,r8,r9
        stfpdux         fp15,r8,r9
        stfpdux         fp16,r8,r9
        stfpdux         fp17,r8,r9
        stfpdux         fp18,r8,r9
        stfpdux         fp19,r8,r9
        stfpdux         fp20,r8,r9
        stfpdux         fp21,r8,r9
        stfpdux         fp22,r8,r9
        stfpdux         fp23,r8,r9
#       stfpdux         fp24,r8,r9
#       stfpdux         fp25,r8,r9
#       stfpdux         fp26,r8,r9
#       stfpdux         fp27,r8,r9
#       stfpdux         fp28,r8,r9
#       stfpdux         fp29,r8,r9
#       stfpdux         fp30,r8,r9
#       stfpdux         fp31,r8,r9
	
#	r16=vi0,r17=vi1,r18=vi2,r19=vi3
	slwi		r19,r6,4
	addi		r16,r3,0
	add		r17,r3,r19
	add		r18,r17,r19
	add		r19,r18,r19
#	slwi		r20,r6,5
#	slwi		r21,r6,4
#	add		r23,r21,r20
	li		r22,16
#	addi		r23,r23,16

#	r3=vi,r4=vo,r5=wn,r6=bsize,r7=blocks
#   
	mullw		r11,r6,r7
	srawi		r26,r11,2
	addi		r26,r26,-1
	mtctr		r26
	slwi		r11,r11,4
	
#	r24=count,r25=mask0,r26=mask1.
	li		r24,1
	srawi		r25,r6,2
	addi		r26,r25,-1

#	r8=vo0,r9=vo1,r10=vo2,r11=vo3
	addi		r8,r4,-16
	add		r9,r8,r11
	add		r10,r9,r11
	add		r11,r10,r11
	
#	r4=w1,r5=w2,r6=w3
	addi		r4,r5,0
	sub		r6,r11,r8
	add		r6,r6,r5

#	index updates.
#
#	and		r27,r24,r25
#	mulli		r29,r27,192
#	and		r24,r24,r26
#	slwi		r28,r27,6
#	add		r23,r22,r29
#	addi		r21,r28,0
#	add		r20,r28,r28
#	addi		r24,r24,1

	
	lfpdx		fp2,r0,r18
	lfxdx		fp20,r0,r5
	lfpdx		fp3,r0,r19
	lfxdx		fp21,r0,r6
	lfpdx		fp1,r0,r17
	lfxdx		fp19,r0,r4
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdx		fp0,r0,r16
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	lfpdx		fp17,r0,r5
	lfpdux		fp3,r19,r22
	lfpdx		fp18,r0,r6
	lfpdux		fp1,r17,r22
	lfpdx		fp16,r0,r4
	fxcxnpma	fp8,fp17,fp6,fp0
	fxcxnsma	fp10,fp17,fp6,fp0
	fxcsmadd	fp9,fp18,fp7,fp5
	fxcsnmsub	fp11,fp18,fp7,fp5
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	and			r27,r24,r25
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnsma	fp13,fp16,fp9,fp8
	mulli		r29,r27,192
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	and			r24,r24,r26
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	slwi		r28,r27,6
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnsma	fp13,fp16,fp9,fp8
	add		r23,r22,r29
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	addi		r21,r28,0
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2	
	add		r20,r28,r28
	addi		r24,r24,1	
		

#	D=fxcxnpma(C,B,A) -> fxcxnpma D,A,B,C
B0:
	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r23
	fxcxnsma	fp5,fp19,fp1,fp1
	lfxdx		fp20,r5,r20
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r23
	fxcxnsma	fp13,fp16,fp9,fp8
	lfxdx		fp21,r6,r21
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r23
	fxcsmadd	fp15,fp16,fp11,fp10
	lfxdx		fp19,r4,r21
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r23
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	lfpdux		fp17,r5,r20
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnsma	fp13,fp16,fp9,fp8
	lfpdux		fp18,r6,r21
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	lfpdux		fp16,r4,r21
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	and		r27,r24,r25
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnsma	fp13,fp16,fp9,fp8
	mulli		r29,r27,192
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	and		r24,r24,r26
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2
	
	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	lfpdux		fp2,r18,r22
	fxcxnsma	fp5,fp19,fp1,fp1
	slwi		r28,r27,6
	fxcxnpma	fp12,fp16,fp9,fp8
	lfpdux		fp3,r19,r22
	fxcxnsma	fp13,fp16,fp9,fp8
	add		r23,r22,r29
	fxcsnmsub	fp14,fp16,fp11,fp10
	lfpdux		fp1,r17,r22
	fxcsmadd	fp15,fp16,fp11,fp10
	addi		r21,r28,0
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	fxcxnsma	fp6,fp20,fp2,fp2	
	add		r20,r28,r28
	addi		r24,r24,1	

	bdnz+	B0

	lfpdux		fp0,r16,r22
	fxcxnsma	fp7,fp21,fp3,fp3
	fxcxnsma	fp5,fp19,fp1,fp1
	fxcxnpma	fp12,fp16,fp9,fp8
	fxcxnsma	fp13,fp16,fp9,fp8
	fxcsnmsub	fp14,fp16,fp11,fp10
	fxcsmadd	fp15,fp16,fp11,fp10
	fxcxnpma	fp8,fp17,fp6,fp0
	stfpdux		fp12,r8,r22
	fxcxnsma	fp10,fp17,fp6,fp0
	stfpdux		fp13,r10,r22
	fxcsmadd	fp9,fp18,fp7,fp5
	stfpdux		fp14,r9,r22
	fxcsnmsub	fp11,fp18,fp7,fp5
	stfpdux		fp15,r11,r22
	
	fxcxnpma	fp12,fp16,fp9,fp8
	fxcxnsma	fp13,fp16,fp9,fp8
	fxcsnmsub	fp14,fp16,fp11,fp10
	fxcsmadd	fp15,fp16,fp11,fp10
	stfpdux		fp12,r8,r22
	stfpdux		fp13,r10,r22
	stfpdux		fp14,r9,r22
	stfpdux		fp15,r11,r22

        addi            r8,SP,96
        li              r9,16
        lfpdux          fp14,r8,r9
        lfpdux          fp15,r8,r9
        lfpdux          fp16,r8,r9
        lfpdux          fp17,r8,r9
        lfpdux          fp18,r8,r9
        lfpdux          fp19,r8,r9
        lfpdux          fp20,r8,r9
        lfpdux          fp21,r8,r9
        lfpdux          fp22,r8,r9
        lfpdux          fp23,r8,r9
#       lfpdux          fp24,r8,r9
#       lfpdux          fp25,r8,r9
#       lfpdux          fp26,r8,r9
#       lfpdux          fp27,r8,r9
#       lfpdux          fp28,r8,r9
#       lfpdux          fp29,r8,r9
#       lfpdux          fp30,r8,r9
#       lfpdux          fp31,r8,r9

	lwz		r9,28(SP)
	lwz		r16,32(SP)
	lwz		r17,36(SP)
	lwz		r18,40(SP)
	lwz		r19,44(SP)
	lwz		r20,48(SP)
	lwz		r21,52(SP)
	lwz		r22,56(SP)
	lwz		r23,60(SP)
	lwz		r24,64(SP)
	lwz		r25,68(SP)
	lwz		r26,72(SP)
	lwz		r27,76(SP)
	lwz		r28,80(SP)
	lwz		r29,84(SP)
	
	
	addi	SP,SP,400
	bclr	BO_ALWAYS,CR0_LT



	.section	".rodata","a"
	.align	3
	.type    .const_dr,@object
	.size    .const_dr,8
.const_dr:
	.long 0x66004942
	.long 0x3f800000

	.ident  "Fri Oct 14 20:47:17 2005 .IBM XL C/C++ Advanced Edition for Linux, Version 7.0.0.2 --- ."

	.section	".eh_frame","wa"
	.align	2
	.long 0x0000000c
	.long 0x00000000
	.long 0x0100047c
	.long 0x410c0100
	.long 0x00000018
	.long 0x00000014
	.long .The_CodeLC
	.long 0x00000070
	.long 0x410e2041
	.long 0x9f01570a
	.long 0x41df420b

