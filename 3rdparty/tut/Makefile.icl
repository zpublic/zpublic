CXX=icl
CXXOPTS=/Ox /GX /GR -Wall -Qwd383,981,193 -c $(INCLUDES) -I$(TUT) -DTUT_USE_SEH /Fo
LNK=link
LNKOPTS=/link $(LIBS) /OUT:
SFX=_icl.exe
OFX=_icl.obj
RM=del 


