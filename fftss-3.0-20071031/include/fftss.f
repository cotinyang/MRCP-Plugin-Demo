C
C Copyright (C) 2002-2007 Akira Nukada. All rights reserved.
C Copyright (C) 2002-2007 The SSI Project. All rights reserved. 
C
C Redistribution and use in source and binary forms, with or without
C modification, are permitted provided that the following conditions are met:
C     * Redistributions of source code must retain the above copyright
C       notice, this list of conditions and the following disclaimer.
C     * Redistributions in binary form must reproduce the above copyright
C       notice, this list of conditions and the following disclaimer in the
C       documentation and/or other materials provided with the distribution.
C     * Neither the name of the project nor the names of its contributors 
C       may be used to endorse or promote products derived from this software 
C       without specific prior written permission.
C
C THIS SOFTWARE IS PROVIDED BY THE SSI PROJECT ``AS IS'' AND ANY EXPRESS 
C OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
C WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
C DISCLAIMED. IN NO EVENT SHALL THE SSI PROJECT BE LIABLE FOR ANY DIRECT,
C INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
C (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
C SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
C HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
C  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
C ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
C THE POSSIBILITY OF SUCH DAMAGE.
C
      PARAMETER (FFTSS_FORWARD=-1)
      PARAMETER (FFTSS_BACKWARD=1)

      PARAMETER (FFTSS_MEASURE=0)
      PARAMETER (FFTSS_DESTROY_INPUT=1)
      PARAMETER (FFTSS_UNALIGNED=2)
      PARAMETER (FFTSS_CONSERVE_MEMORY=4)
      PARAMETER (FFTSS_EXHAUSTIVE=8)
      PARAMETER (FFTSS_PRESERVE_INPUT=16)
      PARAMETER (FFTSSPATIENT_=32)
      PARAMETER (FFTSS_ESTIMATE=64)

      PARAMETER (FFTSS_NO_SIMD=131072)

      PARAMETER (FFTSS_VERBOSE=1048576)
      PARAMETER (FFTSS_INOUT=2097152)
