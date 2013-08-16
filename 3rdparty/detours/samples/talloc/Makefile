##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package, Version 3.0.
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!include ..\common.mak

all: dirs \
    $(BIND)\tdll1x$(DETOURS_BITS).dll \
    $(BIND)\tdll2x$(DETOURS_BITS).dll \
    $(BIND)\tdll3x$(DETOURS_BITS).dll \
    $(BIND)\tdll4x$(DETOURS_BITS).dll \
    $(BIND)\tdll5x$(DETOURS_BITS).dll \
    $(BIND)\tdll6x$(DETOURS_BITS).dll \
    $(BIND)\tdll7x$(DETOURS_BITS).dll \
    $(BIND)\tdll8x$(DETOURS_BITS).dll \
    $(BIND)\tdll9x$(DETOURS_BITS).dll \
    $(BIND)\talloc.exe \
    \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\tdll1x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll2x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll3x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll4x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll5x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll6x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll7x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll8x$(DETOURS_BITS).bsc \
    $(OBJD)\tdll9x$(DETOURS_BITS).bsc \
    $(OBJD)\talloc.bsc \
!ENDIF
	option

clean:
    -del *~ 2>nul
    -del $(BIND)\tdll1x*.* 2>nul
    -del $(BIND)\tdll2x*.* 2>nul
    -del $(BIND)\tdll3x*.* 2>nul
    -del $(BIND)\tdll4x*.* 2>nul
    -del $(BIND)\tdll5x*.* 2>nul
    -del $(BIND)\tdll6x*.* 2>nul
    -del $(BIND)\tdll7x*.* 2>nul
    -del $(BIND)\tdll8x*.* 2>nul
    -del $(BIND)\tdll9x*.* 2>nul
    -del $(BIND)\talloc.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

##############################################################################

$(OBJD)\talloc.obj : talloc.cpp

$(BIND)\talloc.exe : $(OBJD)\talloc.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb $(OBJD)\talloc.obj \
        /link $(LINKFLAGS) $(LIBS) \
        $(BIND)\tdll1x$(DETOURS_BITS).lib \
		$(BIND)\tdll2x$(DETOURS_BITS).lib \
		$(BIND)\tdll3x$(DETOURS_BITS).lib \
		$(BIND)\tdll4x$(DETOURS_BITS).lib \
		$(BIND)\tdll5x$(DETOURS_BITS).lib \
		$(BIND)\tdll6x$(DETOURS_BITS).lib \
		$(BIND)\tdll7x$(DETOURS_BITS).lib \
		$(BIND)\tdll8x$(DETOURS_BITS).lib \
		$(BIND)\tdll9x$(DETOURS_BITS).lib \
        /subsystem:console /entry:WinMainCRTStartup /incremental:no

$(OBJD)\talloc.bsc : $(OBJD)\talloc.obj
    bscmake /v /n /o $@ $(OBJD)\talloc.sbr

$(OBJD)\tdll1x.obj : tdll1x.cpp

$(BIND)\tdll1x$(DETOURS_BITS).dll : $(OBJD)\tdll1x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll1x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:windows \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x280000000

$(OBJD)\tdll1x$(DETOURS_BITS).bsc : $(OBJD)\tdll1x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll1x.sbr

$(OBJD)\tdll2x.obj : tdll2x.cpp

$(BIND)\tdll2x$(DETOURS_BITS).dll : $(OBJD)\tdll2x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll2x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x380000000

$(OBJD)\tdll2x$(DETOURS_BITS).bsc : $(OBJD)\tdll2x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll2x.sbr

$(OBJD)\tdll3x.obj : tdll3x.cpp

$(BIND)\tdll3x$(DETOURS_BITS).dll : $(OBJD)\tdll3x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll3x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x480000000

$(OBJD)\tdll3x$(DETOURS_BITS).bsc : $(OBJD)\tdll3x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll3x.sbr

$(OBJD)\tdll4x.obj : tdll4x.cpp

$(BIND)\tdll4x$(DETOURS_BITS).dll : $(OBJD)\tdll4x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll4x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x580000000

$(OBJD)\tdll4x$(DETOURS_BITS).bsc : $(OBJD)\tdll4x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll4x.sbr


$(OBJD)\tdll5x.obj : tdll5x.cpp

$(BIND)\tdll5x$(DETOURS_BITS).dll : $(OBJD)\tdll5x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll5x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x680000000

$(OBJD)\tdll5x$(DETOURS_BITS).bsc : $(OBJD)\tdll5x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll5x.sbr


$(OBJD)\tdll6x.obj : tdll6x.cpp

$(BIND)\tdll6x$(DETOURS_BITS).dll : $(OBJD)\tdll6x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll6x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x680040000

$(OBJD)\tdll6x$(DETOURS_BITS).bsc : $(OBJD)\tdll6x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll6x.sbr


$(OBJD)\tdll7x.obj : tdll7x.cpp

$(BIND)\tdll7x$(DETOURS_BITS).dll : $(OBJD)\tdll7x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll7x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x680080000

$(OBJD)\tdll7x$(DETOURS_BITS).bsc : $(OBJD)\tdll7x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll7x.sbr


$(OBJD)\tdll8x.obj : tdll8x.cpp

$(BIND)\tdll8x$(DETOURS_BITS).dll : $(OBJD)\tdll8x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll8x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x6800c0000

$(OBJD)\tdll8x$(DETOURS_BITS).bsc : $(OBJD)\tdll8x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll8x.sbr


$(OBJD)\tdll9x.obj : tdll9x.cpp

$(BIND)\tdll9x$(DETOURS_BITS).dll : $(OBJD)\tdll9x.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb \
        $(OBJD)\tdll9x.obj /LDd \
        /link $(LINKFLAGS) $(LIBS) \
        /subsystem:console \
        /entry:$(DLLENTRY) \
        /incremental:no \
		/fixed \
        /base:0x680100000

$(OBJD)\tdll9x$(DETOURS_BITS).bsc : $(OBJD)\tdll9x.obj
    bscmake /v /n /o $@ $(OBJD)\tdll9x.sbr

############################################### Install non-bit-size binaries.

!IF "$(DETOURS_OPTION_PROCESSOR)" != ""

$(OPTD)\tdll1x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll1x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll2x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll2x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll3x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll3x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll4x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll4x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll5x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll5x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll6x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll6x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll7x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll7x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll8x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll8x$(DETOURS_OPTION_BITS).pdb:
$(OPTD)\tdll9x$(DETOURS_OPTION_BITS).dll:
$(OPTD)\tdll9x$(DETOURS_OPTION_BITS).pdb:

$(BIND)\tdll1x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll1x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll1x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll1x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll2x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll2x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll2x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll2x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll3x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll3x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll3x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll3x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll4x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll4x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll4x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll4x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll5x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll5x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll5x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll5x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll6x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll6x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll6x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll6x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll7x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll7x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll7x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll7x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll8x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll8x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll8x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll8x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll9x$(DETOURS_OPTION_BITS).dll : $(OPTD)\tdll9x$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\tdll9x$(DETOURS_OPTION_BITS).pdb : $(OPTD)\tdll9x$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).

option: \
	$(BIND)\tdll1x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll1x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll2x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll2x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll3x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll3x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll4x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll4x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll5x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll5x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll6x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll6x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll7x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll7x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll8x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll8x$(DETOURS_OPTION_BITS).pdb \
	$(BIND)\tdll9x$(DETOURS_OPTION_BITS).dll \
	$(BIND)\tdll9x$(DETOURS_OPTION_BITS).pdb \

!ELSE

option:

!ENDIF

##############################################################################

test: all
    $(BIND)\talloc.exe

################################################################# End of File.
