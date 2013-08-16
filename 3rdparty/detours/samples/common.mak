##############################################################################
##
##  Common makefile for Detours test programs.
##
##  Microsoft Research Detours Package, Version 3.0.
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!IF "$(ROOT)" == ""
ROOT = ..\..
!ENDIF
!include "$(ROOT)\system.mak"

!IF "$(DETOURS_SOURCE_BROWSING)" == ""
DETOURS_SOURCE_BROWSING=0
!ENDIF

##############################################################################

!IFNDEF CLIB
CLIB=/MT
!ENDIF

AFLAGS=/nologo /Zi /c /Fl
CFLAGS=/nologo /Zi $(CLIB) /Gm- /W4 /WX /Od /DDETOURS_BITS=$(DETOURS_BITS)
!IF $(DETOURS_SOURCE_BROWSING)==1
CFLAGS=$(CFLAGS) /FR
!ELSE
CFLAGS=$(CFLAGS) /I$(INCD)
!ENDIF

LIBFLAGS=/nologo
LINKFLAGS=/release /incremental:no

!IF "$(DETOURS_TARGET_PROCESSOR)" == "X86"

ASM=ml
AFLAGS=$(AFLAGS) /DDETOURS_X86=1
CFLAGS=$(CFLAGS) /Gs /DDETOURS_X86=1 /DDETOURS_32BIT=1 /D_X86_
LIBFLAGS=$(LIBFLAGS) /machine:x86
DLLENTRY=_DllMainCRTStartup@12
LINKFLAGS=$(LINKFLAGS) /machine:x86

!ELSEIF "$(DETOURS_TARGET_PROCESSOR)" == "X64"

ASM=ml64
AFLAGS=$(AFLAGS) /DDETOURS_X64=1
CFLAGS=$(CFLAGS) /D_WIN64 /DDETOURS_X64=1 /DDETOURS_64BIT=1 /D_AMD64_
LIBFLAGS=$(LIBFLAGS) /machine:amd64
DLLENTRY=_DllMainCRTStartup
LINKFLAGS=$(LINKFLAGS) /machine:amd64

!ELSEIF "$(DETOURS_TARGET_PROCESSOR)" == "IA64"

ASM=ias
AFLAGS=-F COFF32_PLUS
CFLAGS=$(CFLAGS) /D_WIN64 /DDETOURS_IA64=1 /DDETOURS_64BIT=1 /D_IA64_ /wd4163
LIBFLAGS=$(LIBFLAGS) /machine:ia64
DLLENTRY=_DllMainCRTStartup
LINKFLAGS=$(LINKFLAGS) /machine:ia64

!ELSEIF "$(DETOURS_TARGET_PROCESSOR)" == "ARM"

ASM=armasm
AFLAGS=-coff_thumb2_only
CFLAGS=$(CFLAGS) /Gs /DDETOURS_ARM=1 /DDETOURS_32BIT=1 /D_ARM_ /D_ARM_WORKAROUND_
LIBFLAGS=$(LIBFLAGS) /machine:arm
DLLENTRY=_DllMainCRTStartup
LINKFLAGS=$(LINKFLAGS) /machine:arm /nodefaultlib:oldnames.lib

!ENDIF

!IF "$(DETOURS_OPTION_BITS)" != ""
CFLAGS=$(CFLAGS) /DDETOURS_OPTION_BITS=$(DETOURS_OPTION_BITS)
!ENDIF

LIBS = $(LIBD)\syelog.lib $(LIBD)\detours.lib \
        kernel32.lib gdi32.lib user32.lib shell32.lib
DEPS = $(LIBD)\syelog.lib $(LIBD)\detours.lib

##############################################################################
##

.SUFFIXES: .cpp .h .obj .rc .res

.cpp{$(OBJD)}.obj:
    $(CC) /nologo $(CFLAGS) /Fd$(OBJD)\vc.pdb /Fo$@ /c $(*B).cpp

.rc{$(OBJD)}.res:
    rc /nologo /DDETOURS_BITS=$(DETOURS_BITS) /fo$(@) /i$(INCD) $(*B).rc

##
################################################################# End of File.
