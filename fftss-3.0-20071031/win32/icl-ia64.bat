@ECHO OFF

md Release
del /Q Release\*.*
icl /O3 /Qopenmp /Qc99 /c /FoRelease\ ..\libfftss\fftss*.c ..\libfftss\r?_ia64_asm.s ..\libfftss\*ia64*.c ..\libfftss\r4_?.c ..\libfftss\r4_u?.c ..\libfftss\r4_fma*.c ..\libfftss\r8_?.c ..\libfftss\r8_u?.c ..\libfftss\r8_fma*.c  /I..\include
lib /OUT:Release\fftss.lib Release\*.obj
icl /O3 /Qopenmp /Qc99 /Fe1d_ia64.exe /FoRelease\ ..\test\test.c Release\fftss.lib /I..\include 
icl /O3 /Qopenmp /Qc99 /Fe2d_ia64.exe /FoRelease\ ..\test\test2d.c Release\fftss.lib /I..\include
icl /O3 /Qopenmp /Qc99 /Fe3d_ia64.exe /FoRelease\ ..\test\test3d.c Release\fftss.lib /I..\include
