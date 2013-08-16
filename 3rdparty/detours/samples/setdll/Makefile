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
    $(BIND)\setdll.exe \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\setdll.bsc \
!ENDIF
	option

##############################################################################

clean:
    -del *~ 2>nul
    -del $(BIND)\setdll.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

##############################################################################

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

$(OBJD)\setdll.obj : setdll.cpp

$(BIND)\setdll.exe : $(OBJD)\setdll.obj $(DEPS)
    cl $(CFLAGS) /Fe$@ /Fd$(@R).pdb $(OBJD)\setdll.obj \
        /link $(LINKFLAGS) $(LIBS) /subsystem:console /incremental:no

$(OBJD)\setdll.bsc : $(OBJD)\setdll.obj
    bscmake /v /n /o $@ $(OBJD)\setdll.sbr

############################################### Install non-bit-size binaries.

!IF "$(DETOURS_OPTION_PROCESSOR)" != ""

$(OPTD)\slept$(DETOURS_OPTION_BITS).dll:
$(OPTD)\slept$(DETOURS_OPTION_BITS).pdb:

$(BIND)\slept$(DETOURS_OPTION_BITS).dll : $(OPTD)\slept$(DETOURS_OPTION_BITS).dll
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).
$(BIND)\slept$(DETOURS_OPTION_BITS).pdb : $(OPTD)\slept$(DETOURS_OPTION_BITS).pdb
	@if exist $? copy /y $? $(BIND) >nul && echo $@ copied from $(DETOURS_OPTION_PROCESSOR).

option: \
	$(BIND)\slept$(DETOURS_OPTION_BITS).dll \
	$(BIND)\slept$(DETOURS_OPTION_BITS).pdb \

!ELSE

option:

!ENDIF

##############################################################################

test: all
    @echo -------- Reseting test binaries to initial state. -----------------------
    $(BIND)\setdll.exe -d:$(BIND)\slept$(DETOURS_BITS).dll $(BIND)\sleepold.exe
    @echo -------- Should load slept$(DETOURS_BITS).dll statically -------------------------------
    $(BIND)\sleepold.exe
    @echo -------- Reseting test binaries to initial state. -----------------------
    $(BIND)\setdll.exe -r $(BIND)\sleepold.exe
    @echo -------- Should not load slept$(DETOURS_BITS).dll --------------------------------------
    $(BIND)\sleepold.exe

################################################################# End of File.
