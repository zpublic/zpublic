##############################################################################
##
##  Makefile for Detours Test Programs.
##
##  Microsoft Research Detours Package, Version 3.0.
##
##  Copyright (c) Microsoft Corporation.  All rights reserved.
##

!include ..\common.mak

CFLAGS = $(CFLAGS) \
        /Fdcping$(DETOURS_BITS).pdb \
        /DWIN32_LEAN_AND_MEAN /DCONST_VTABLE \
        /D_WIN32_WINNT=0x0400 /DCOBJMACROS \
        -DWIN32 -DNT -DDEBUG -DUNICODE -D_UNICODE

C__FLAGS=-DENTRY_PREFIX=iping_ -DREGISTER_PROXY_DLL 
CPPFLAGS=

##############################################################################

.SUFFIXES: .c .cpp .h .idl .obj .res .rc
{$(OBJD)}.c{$(OBJD)}.obj:
    @$(CC) /nologo $(CFLAGS:/W4=/W3) $(C__FLAGS) /I$(OBJD) /Fo$@ /c $(OBJD)\$(*B).c
.cpp{$(OBJD)}.obj:
    @$(CC) /nologo $(CFLAGS) $(CPPFLAGS) /I$(OBJD) /Fo$@ /c $(*B).cpp
.rc{$(OBJD)}.res:
    @rc /nolog /Fo$@ .\$(*B).rc

##############################################################################
##
C__FLAGS=-DENTRY_PREFIX=iping_ -DREGISTER_PROXY_DLL 
CPPFLAGS=


LINKFLAGS=/nologo /nod /debug /machine:IX86 /subsystem:windows /incremental:no
MIDLFLAGS=/nologo /Oif /no_format_opt /no_robust /win32

OBJS = \
    $(OBJD)\cping.obj   \
    \
    $(OBJD)\iping_i.obj \
    $(OBJD)\iping_p.obj \
    $(OBJD)\iping_d.obj \

LIBS=\
    $(LIBS) \
    uuid.lib        \
    ole32.lib       \
    rpcrt4.lib      \
    msvcrt.lib      \
    advapi32.lib    \
    wsock32.lib     \

##############################################################################

all: dirs \
    $(BIND)\cping.exe   \
!IF $(DETOURS_SOURCE_BROWSING)==1
    $(OBJD)\cping.bsc
!ENDIF

##############################################################################

clean:
    -del iping.h *.c *.obj *.sbr *~ 2>nul
    -del $(BIND)\cping.* 2>nul
    -rmdir /q /s $(OBJD) 2>nul

realclean: clean
    -rmdir /q /s $(OBJDS) 2>nul

##############################################################################

dirs:
    @if not exist $(BIND) mkdir $(BIND) && echo.   Created $(BIND)
    @if not exist $(OBJD) mkdir $(OBJD) && echo.   Created $(OBJD)

$(OBJD)\cping.bsc : $(OBJS)
    bscmake /v /n /o $@ $(OBJS:.obj=.sbr)

$(BIND)\cping.exe : $(OBJS) $(DEPS)
    cl $(CFLAGS) /Fe$@ $(OBJS) /link $(LINKFLAGS) \
        /subsystem:console $(LIBS)

$(OBJD)\cping.obj: cping.cpp $(OBJD)\iping.h

##############################################################################
##
$(OBJD)\iping.h $(OBJD)\iping_d.c $(OBJD)\iping_i.c $(OBJD)\iping_p.c : iping.idl
    midl $(MIDLFLAGS) /out $(OBJD) /prefix all iping_ /dlldata iping_d.c iping.idl

$(OBJD)\iping_i.obj: $(OBJD)\iping_i.c
$(OBJD)\iping_p.obj: $(OBJD)\iping_p.c $(OBJD)\iping.h
$(OBJD)\iping_d.obj: $(OBJD)\iping_d.c

##############################################################################

test: $(BIND)\cping.exe
    start $(BIND)\cping.exe /s
    $(BIND)\cping.exe /p localhost

################################################################# End of File.
