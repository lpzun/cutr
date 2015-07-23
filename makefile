# Generic C Makefile

# We assume by default that every .c file depends on *all* .h files in
# the current directory.  The rationale is that it is close to impossible
# to keep track of the true dependencies (think of: x.c incl. a.h
# incl. b.h!). Having too liberal dependencies can cause weird seg faults.
# Having too strict dependencies only causes too much re-compilation.


###########################################################################
# 1. Public Region. The following settings are useful for compiling a     #
# single executable file out of many source files (*.c). If the directory #
# contains many independent files to be compiled to independent           #
# executables, use SOURCES=$(BASE).c, HEADERS=$(BASE).h.                  #
# See an example in EXAMPLES/makefile-local-vars.                         #
###########################################################################
# Override these variables (or add new ones) locally
APP	     = ursula # the name of application
Z3DIR        = /usr/local/Z3#          
ILIBS        = -L $(Z3DIR)/lib -lz3#                                   -lm # config your z3 lib     here
IINCLUDE     = -I $(Z3DIR)/include/#                                       # config your z3 include here

#ISTD	      = -std=c++0x                                                 # for old cpp standard
ISTD	     = -std=c++11

BINDIR       = bin
OBJDIR       = obj
SRCDIR       = src
SRCDIRS      = $(shell find $(SRCDIR) -name '*.$(CSUFF)' -exec dirname {} \; | uniq)

CSUFF        = cc
#CSUFF        = c

DEFAULT      = $(BASE)
EDITFILES    = test.$(TSUFF) $(wildcard *.$(HSUFF)) $(wildcard *.$(CSUFF)) $(BASE).$(CSUFF)
FLAGS        = -Wall -g $(ISTD)#                          -O3, -D__SAFE_COMPUTATION__, etc
SOURCES      = $(shell find $(SRCDIR) -name '*.$(CSUFF)') #$(wildcard *.$(CSUFF))#            list of local files that will be compiled and linked into executable

# For compiling:
IDIRS        =#                                   -I$(C)
HEADERS      = $(wildcard *.$(HSUFF))#            may set to a single .h file if only one specific file is compiled

# For linking:
#BASE         = $(firstword $(BASES))#             executable (final compilation). If directory contains several .c files, redefine this
BASE         = $(BINDIR)/$(APP)
ROBJVARS     =
LDIRS        =#
LIBS         =$(ILIBS)#                                   -lm

EXPORT       = CCOMP=$(CCOMP) FLAGS="$(FLAGS)"

DISTCLEAN    =#                                   any additional commands to be executed by the distclean command (like cleaning sub directories)

#####################################
# 2. Private Region (do not change) #
#####################################

ifeq ($(CSUFF),cc)
  HSUFF        = hh
  TSUFF        = CC
  CCOMP        = g++
else
  HSUFF        = h
  TSUFF        = C
  CCOMP        = gcc
endif

SHELL    = /bin/bash
BASES    = $(wildcard *.$(CSUFF))
BASES   := $(BASES:.$(CSUFF)=)
LOBJECTS = $(patsubst %.$(CSUFF),$(OBJDIR)/%.o, $(SOURCES:.$(CSUFF)=.o)) # 
RDIRS    = $(foreach VAR,$(ROBJVARS),$(dir $(firstword $($(VAR)))))
ROBJECTS = $(foreach VAR,$(ROBJVARS),$($(VAR)))
OBJECTS  = $(LOBJECTS) $(ROBJECTS)
CFLAGS   = $(FLAGS) $(IDIRS) $(IINCLUDE)#
LFLAGS   = $(FLAGS) $(LDIRS)
RERROR   = { echo "error in recursive make robjects";  exit 1; }
DERROR   = { echo "error in recursive make distclean"; exit 1; }

default: $(DEFAULT)

edit:
	editor $(EDITFILES) &

new: clean default

distnew: distclean default


# Add new targets locally. This is included after 'default' above, so that the default remains the default.
-include makefile-local-targets

unexport MAKEFLAGS # do not export variables by default (only those mentioned in EXPORT)


##################################
# Targets Region (do not change) #
##################################

$(DEFAULT): $(OBJECTS) #robjects
	@mkdir -p `dirname $@`
	$(CCOMP) $(LFLAGS) $(OBJECTS) $(LIBS) -o $@

$(OBJECTS): %.o: %.$(CSUFF) $(HEADERS)
	$(CCOMP) $(CFLAGS) $< -c -o $@

robjects:
	$(foreach VAR,$(ROBJVARS),$(MAKE) -C $(dir $(firstword $($(VAR)))) $(EXPORT) $(notdir $($(VAR))) || $(RERROR);)

############################
# Cleaning (do not change) #
############################

clean: 	CLEANOBJS
	#rm -f $(BASES)
	rm -f *.o test a.out

distclean: clean CLEANOBJS
	rm -rf $(BINDIR)
	rm -f *~
	$(foreach DIR,$(RDIRS),$(MAKE) -C $(DIR) $(EXPORT) distclean || $(DERROR);)
	$(DISTCLEAN)

CLEANOBJS:
	@$(call clean-obj)

# description: for cleaning all objects
define clean-obj
	find . -name '*.o' -type f -delete
endef
