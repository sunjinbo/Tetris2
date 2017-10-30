# ============================================================================
#  Name        : Tetris2Icons_aif.mk
#  Created     : 03/27/09
#  Modified    : 05/03/09
#  Author      : Sun Jinbo
#
#  Copyright (c): Tieto, All rights reserved
# ============================================================================
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif

TARGETDIR=$(ZDIR)\resource\apps
ICONTARGETFILENAME=$(TARGETDIR)\Tetris2_icon.mif

ICONDIR=..\aif

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : $(ICONTARGETFILENAME)

$(ICONTARGETFILENAME) : $(ICONDIR)\Tetris2_icon.svg
	mifconv $(ICONTARGETFILENAME) \
		/c32 $(ICONDIR)\Tetris2_icon.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing

