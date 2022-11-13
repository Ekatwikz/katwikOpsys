# base filename goes here:
FILENAME:=lab4Pract

# 1 for ez compile,
# 0 to see a few more warnings,
# -1 if you you're perfectionist
FEELINGLAZY:=0

# 0 for single process programs
# 1 for multi-process programs
ERR_MULTIPROCESS=0

# describe which arguments your program expects
# eg: USAGE_STRING="arg1 arg2 arg3"
USAGE_STRING="inputFile outputFile chunks\nwhere chunks >= 1"

# 1 if randomizers should use RAND_R
# 0 if not
USE_RAND_R=1

# 1 to clean whenever you make, 0 to not
CLEANONMAKE:=1

# 1 to clear whenever you make, 0 to not
CLEARONMAKE:=1

# you can list your message queues here and "make clean" will try to remove them if they exist
# I've found this useful when I mess up and have an open/full message queue or smthn
# remember: they can stay alive even after all processes using them exit
# also the directory isn't the same on all systems, but on the school ones I think it's /dev/mqueue
# you can change that tho
#
# probably only useful on lab6 tho
IPCMSGQS:=
IPCMSGQPATH=:/dev/mqueues

MUNDANE_MESSAGES=0
KATLIBS:=errorHelpers easyCheck myList stringCrap
SYSLIBS:=pthread rt
DEBUGFLAGS:=-g3 -O0
EXTRAFLAGS:=track-macro-expansion=0 no-omit-frame-pointer sanitize=address,undefined
AUTOCLEANKATOBJS:=0
USECOLOR:=1

ifeq ($(CLEANONMAKE), 0)
	SHOULDCLEAN =
else
	SHOULDCLEAN = clean
endif

ifeq ($(CLEARONMAKE), 0)
	SHOULDCLEAR =
else
	SHOULDCLEAR = clear
endif

ifeq ($(USECOLOR), 1)
	BLACK		:= $(shell tput -Txterm setaf 0)
	RED		:= $(shell tput -Txterm setaf 1)
	GREEN		:= $(shell tput -Txterm setaf 2)
	YELLOW		:= $(shell tput -Txterm setaf 3)
	LIGHTPURPLE	:= $(shell tput -Txterm setaf 4)
	PURPLE		:= $(shell tput -Txterm setaf 5)
	BLUE		:= $(shell tput -Txterm setaf 6)
	WHITE		:= $(shell tput -Txterm setaf 7)
	RESET		:= $(shell tput -Txterm sgr0)
else
	BLACK		:= ""
	RED		:= ""
	GREEN		:= ""
	YELLOW		:= ""
	LIGHTPURPLE	:= ""
	PURPLE		:= ""
	BLUE		:= ""
	WHITE		:= ""
	RESET		:= ""
endif

WARNINGS:=all extra
ifeq ($(FEELINGLAZY), 0)
else ifeq ($(FEELINGLAZY), -1)
	WARNINGS+=pedantic error
else # ifeq ($(FEELINGLAZY), 1) # default
	WARNINGS+=no-unused-parameter no-unused-variable no-unused-function no-cpp
endif

CC=gcc
CFLAGS=$(PREPROCESS) $(ASSEMBLE) $(WARNINGS:%=-W%) $(EXTRAFLAGS:%=-f%) $(DEBUGFLAGS)
LDFLAGS=$(SYSLIBS:%=-l%)

INCLUDE=..
PREDEFINES=-D ERR_MULTIPROCESS='$(ERR_MULTIPROCESS)' -D USAGE_STRING='$(USAGE_STRING)' -D USE_RAND_R='$(USE_RAND_R)' -D MUNDANE_MESSAGES='$(MUNDANE_MESSAGES)'
KATOBJPATH=../obj
KATSRCPATH=../src
KATOBJS=$(KATLIBS:%=$(KATOBJPATH)/%.o)
COMPILEMSG="$(BLUE)===\t$(GREEN)$<$(BLUE)\t->\t$(GREEN)$@\t$(BLUE)===$(RESET)\n"

.PHONY: all clean clear

all: $(SHOULDCLEAN) $(SHOULDCLEAR) $(FILENAME)

$(KATOBJPATH)/%.o: $(KATSRCPATH)/%.c
	@mkdir -pv $(KATOBJPATH)
	@printf $(COMPILEMSG)
	@$(CC) $(INCLUDE:%=-I %) $(CFLAGS) $(PREDEFINES) -c $< -o $@ $(LDFLAGS)

$(FILENAME): $(FILENAME).c $(KATOBJS)
	@printf $(COMPILEMSG)
	@$(CC) $(INCLUDE:%=-I %) $(CFLAGS) $(PREDEFINES) $(KATOBJS) $< -o $@ $(LDFLAGS)

%: %.c $(KATOBJS)
	@printf $(COMPILEMSG)
	@$(CC) $(INCLUDE:%=-I %) $(CFLAGS) $(PREDEFINES) $(KATOBJS) $< -o $@ $(LDFLAGS)

clean: OUTPUTS=$(FILENAME)
clean:
ifneq ($(IPCMSGQS),)
	@printf "$(BLUE)Trying to remove $(GREEN)$(IPCMSGQS)$(RESET) (message queues) from $(GREEN)$(IPCMSGQPATH)$(RESET)\n"
	-@cd $(IPCMSGQPATH); rm -fv $(IPCMSGQS)
endif
ifeq ($(AUTOCLEANKATOBJS), 1)
	@printf "$(BLUE)Trying to remove $(GREEN)*.o$(RESET) from $(GREEN)$(KATOBJPATH)$(RESET)\n"
	-@cd $(KATOBJPATH); rm -fv *.o
endif
	@printf "$(BLUE)Trying to remove $(GREEN)$(OUTPUTS)$(GREEN)$(RESET)\n"
	-@rm -fv $(OUTPUTS)

clear:
	@clear