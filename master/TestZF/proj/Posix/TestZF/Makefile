#============================================================
# Usage:
#   make                same as make debug
#   make debug          make a debug version
#   make release        make a release version
#   make run            make a debug version and run
#   make clean          clean every output files
#   make gprof          make and run debug version with gprof
#
#                                           http://zsaber.com
#                                        ZSaberLv0 2016-04-20
#============================================================


#============================================================
# general project structure
#============================================================
# sources dirs
SRCDIRS         =
SRCDIRS         += ../../../../../ZF/src
SRCDIRS         += ../../../../../ZF_impl/src
SRCDIRS         += ../../../../../ZF_impl_ZFUIWebKit/src
SRCDIRS         += ../../../../../demo/ZFFramework_test/src
# header search dirs, no recursive support
HEADERDIRS      =
HEADERDIRS      += ../../../../../ZF/src
HEADERDIRS      += ../../../../../ZF_impl/src
HEADERDIRS      += ../../../../../ZF_impl_ZFUIWebKit/src
# output directory
OUTDIR          := ../../../../../_tmp/Posix
OBJDIR          := $(OUTDIR)/obj
# the executable to build to
TARGET          := $(OUTDIR)/test

#============================================================
# flags for different builds
#============================================================
BASE_FLAGS      := -Wall -rdynamic -pthread -Wno-psabi -I../../../ZF/src -I../../../ZF_impl/src
BASE_LFLAGS     := -pthread -ldl -lrt
DEBUG_FLAGS     := -g
DEBUG_LFLAGS    :=
RELEASE_FLAGS   := -O2
RELEASE_LFLAGS  :=

#============================================================
# other
#============================================================
# cross-compiler
CROSS_COMPILE   :=



#============================================================
# no need to change these
#============================================================
SRCEXTS     := c C cc cpp CPP c++ cxx cp
HEADEREXTS  := h H hh hpp HPP h++ hxx hp
CC          := gcc
CXX         := g++
RM          := rm -rf
MKDIR       := mkdir -p
LS          := ls
#CLEAR       := clear
CLEAR       := echo

# files and directories
ALL_SRC_DIRS:=$(strip $(subst \,/,$(abspath $(shell find $(SRCDIRS) -type d))))
ALL_OBJ_DIRS:=$(strip $(foreach srcdir,$(ALL_SRC_DIRS),$(OBJDIR)/_$(abspath $(srcdir))))
ALL_SRC_FILES:=$(strip $(foreach srcdir,$(ALL_SRC_DIRS), $(foreach srcext,$(SRCEXTS),$(abspath $(wildcard $(srcdir)/*.$(srcext))))))
ALL_OBJ_FILES:=$(strip $(foreach srcfile,$(ALL_SRC_FILES),$(abspath $(OBJDIR)/_$(addsuffix .o,$(basename $(srcfile))))))
ALL_DEPEND_FILES:=$(strip $(foreach srcfile,$(ALL_SRC_FILES),$(abspath $(OBJDIR)/_$(addsuffix .d,$(basename $(srcfile))))))
MAIN_DEPEND_FILE:=$(OUTDIR)/.depend
ALL_HEADER_FILES:=$(strip $(foreach srcdir,$(ALL_SRC_DIRS), $(foreach headerext,$(HEADEREXTS),$(abspath $(wildcard $(srcdir)/*.$(headerext))))))
ALL_CODE_FILES:=$(ALL_SRC_FILES) $(ALL_HEADER_FILES)
ALL_SRC_EXCEPT_C:=$(filter-out %.c %.C,$(ALL_SRC_FILES))

# compiler
HEADER_FLAGS:=$(foreach headerdir,$(HEADERDIRS),-I$(headerdir))

ifeq ($(ALL_SRC_EXCEPT_C),)     # C program
	CXX_FIXED:=$(CROSS_COMPILE)$(CC)
else                            # C++ program
	CXX_FIXED:=$(CROSS_COMPILE)$(CXX)
endif

.PHONY: all debug release run clean prebuild postbuild

all: debug

# Make a debug version
debug: CXXFLAGS:=$(BASE_FLAGS) $(DEBUG_FLAGS) $(HEADER_FLAGS)
debug: CXXLFLAGS:=$(BASE_LFLAGS) $(DEBUG_LFLAGS)
debug: $(MAIN_DEPEND_FILE) $(ALL_DEPEND_FILES) prebuild $(TARGET) postbuild

# Make a release version
release: CXXFLAGS:=$(BASE_FLAGS) $(RELEASE_FLAGS) $(HEADER_FLAGS)
release: CXXLFLAGS:=$(BASE_LFLAGS) $(RELEASE_LFLAGS)
release: clean $(MAIN_DEPEND_FILE) $(ALL_DEPEND_FILES) prebuild $(TARGET) postbuild

# Make and run
run: debug
	@$(CLEAR)
	./$(TARGET)

# Make everything go away (be clean)
clean:
	@$(RM) $(OBJDIR)
	@$(RM) $(OUTDIR)
	@$(CLEAR)
	@$(LS)

# actual code dependencies section
$(TARGET): $(ALL_OBJ_FILES)
	@echo building $(TARGET)
	@$(CXX_FIXED) $(CXXFLAGS) $+ -o $@ $(CXXLFLAGS)

# depend file dependecies
$(MAIN_DEPEND_FILE):$(ALL_CODE_FILES)
	@sh ../../../../../thirdparty_setup.sh
	@$(RM) $(MAIN_DEPEND_FILE)
	@echo updating main depend file $(MAIN_DEPEND_FILE)
	@echo $(foreach srcfile,$(ALL_SRC_FILES),"\n"$(abspath $(OBJDIR))/_$(addsuffix .d,$(basename $(srcfile))):$(srcfile)) >> $(MAIN_DEPEND_FILE)

ifneq ($(MAKECMDGOALS),clean)
-include $(MAIN_DEPEND_FILE)
-include $(ALL_DEPEND_FILES)
endif

%.d:
	@$(RM) $@
	@$(MKDIR) $(dir $@)
	@echo updating $(notdir $@)
	@$(CXX_FIXED) $(HEADER_FLAGS) -MM $(filter $(foreach srcext,$(SRCEXTS),%.$(srcext)),$(shell find $(basename $(subst $(abspath $(OBJDIR))/_,,$(abspath $@))).* -type f)) -MT $(abspath $(addsuffix .o, $(basename $@))) -MF $(abspath $@)

# object dependencies
%.o:
	@echo building $(notdir $@)
	@$(CXX_FIXED) $(CXXFLAGS) -c $(filter $(foreach srcext,$(SRCEXTS),%.$(srcext)),$(shell find $(basename $(subst $(abspath $(OBJDIR))/_,,$(abspath $@))).* -type f)) -o $(abspath $@)

prebuild:
	@$(CLEAR)
	@$(MKDIR) $(OBJDIR)
	@for objdir in $(ALL_OBJ_DIRS); do $(MKDIR) $${objdir}; done

postbuild:

# gprof
GPROF_RESULT:= $(OUTDIR)/gprof_result.txt
gprof: CXXFLAGS:=$(BASE_FLAGS) $(DEBUG_FLAGS) $(HEADER_FLAGS) -pg
gprof: CXXLFLAGS:=$(BASE_LFLAGS) $(DEBUG_LFLAGS) -pg
gprof: $(MAIN_DEPEND_FILE) $(ALL_DEPEND_FILES) prebuild $(TARGET) postbuild
	@$(RM) gmon.out
	@$(RM) $(GPROF_RESULT)
	@./$(TARGET)
	@gprof -b -A -p -q $(TARGET) gmon.out > $(GPROF_RESULT)
	@$(RM) gmon.out
	@gedit $(GPROF_RESULT)

# gperftools
GPERFTOOLS_TMP:= $(OUTDIR)/gperftools_tmp.prof
GPERFTOOLS_RESULT:= $(OUTDIR)/gperftools_result.txt
gperftools: CXXFLAGS:=$(BASE_FLAGS) $(DEBUG_FLAGS) $(HEADER_FLAGS)
gperftools: CXXLFLAGS:=$(BASE_LFLAGS) $(DEBUG_LFLAGS) -lprofiler
gperftools: $(MAIN_DEPEND_FILE) $(ALL_DEPEND_FILES) prebuild $(TARGET) postbuild
	@$(RM) $(GPERFTOOLS_TMP)
	@$(RM) $(GPERFTOOLS_RESULT)
	@./$(TARGET)
	@pprof --text $(abspath $(TARGET)) $(abspath $(GPERFTOOLS_TMP)) > $(abspath $(GPERFTOOLS_RESULT))
	@$(RM) $(GPERFTOOLS_TMP)
	@gedit $(GPERFTOOLS_RESULT)

