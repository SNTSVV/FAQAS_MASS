# -------------------------------------------------------------------------------------------------
# \file Makefile
# \par ESA Contract Number
#      4000117691/16/NL/FE
# \brief Makefile to build libm.a
# \version c07acbf539f8880d8c1255e7f85138b89f33387b
# \date 2018-03-22 17:44:24 +0100
# \author GTD GmbH, Fabian Schriever
# \par Last Commit
# Fabian Schriever, 2018-03-22 17:44:24 +0100
#
# \par Detailed Description
# This makefile builds the libmlfs.a.
#
# -------------------------------------------------------------------------------------------------
# \par Changelog
# Date       | Name         | Change
# -----------|--------------|-------------------------------------------------------
# 2016-11-30 | F. Schriever | First creation
# 2017-02-07 | F. Schriever | Remove deprecated symbol from ESA copyright notice.
# 2017-05-15 | F. Schriever | Add rem2pi and rem2pif.
# 2017-05-15 | F. Schriever | Add deg2rad and deg2radf.
# 2017-05-23 | F. Schriever | Add conditional compilation for hw sqrt, and coverage.
# 2017-06-21 | F. Schriever | Add __signbitf and __signbitd (same file).
#
# \copyright Copyright European Space Agency, 2017
# \par License
# Permission to use, copy, modify, and distribute this
# software is freely granted, provided that this notice 
# is preserved.

SHELL := /bin/bash

ifndef ARCH
    ARCH = $(shell $(CROSS_COMPILE)gcc -v 2>&1 | grep Target | cut -d' ' -f 2)
endif

ifeq ($(V),1)
	Q =
else
	Q = @
endif

CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
MKDIR = mkdir -p
CP = cp
MV = mv
RM = rm -rf

INCLUDE = -Iinclude -Ilibm/common -Ilibm/math -Ilibm/mlfs

SRC=libm/common/s_copysign.c \
    libm/common/s_fmax.c \
    libm/common/s_fmin.c \
    libm/common/s_fpclassify.c \
    libm/common/s_modf.c \
    libm/common/s_nan.c \
    libm/common/s_round.c \
    libm/common/s_scalbn.c \
    libm/common/s_signbit.c \
    libm/common/s_trunc.c \
    libm/common/sf_copysign.c \
    libm/common/sf_fmax.c \
    libm/common/sf_fmin.c \
    libm/common/sf_fpclassify.c \
    libm/common/sf_modf.c \
    libm/common/sf_nan.c \
    libm/common/sf_round.c \
    libm/common/sf_scalbn.c \
    libm/common/sf_trunc.c \
    libm/mlfs/s_rem2pi.c \
    libm/mlfs/sf_rem2pi.c \
    libm/mlfs/s_deg2rad.c \
    libm/mlfs/sf_deg2rad.c \
    libm/math/e_acos.c \
    libm/math/e_asin.c \
    libm/math/e_atan2.c \
    libm/math/e_exp.c \
    libm/math/e_fmod.c \
    libm/math/e_hypot.c \
    libm/math/e_log.c \
    libm/math/e_log10.c \
    libm/math/e_pow.c \
    libm/math/e_rem_pio2.c \
    libm/math/ef_acos.c \
    libm/math/ef_asin.c \
    libm/math/ef_atan2.c \
    libm/math/ef_exp.c \
    libm/math/ef_fmod.c \
    libm/math/ef_hypot.c \
    libm/math/ef_log.c \
    libm/math/ef_log10.c \
    libm/math/ef_pow.c \
    libm/math/ef_rem_pio2.c \
    libm/math/k_cos.c \
    libm/math/k_rem_pio2.c \
    libm/math/k_sin.c \
    libm/math/k_tan.c \
    libm/math/kf_cos.c \
    libm/math/kf_rem_pio2.c \
    libm/math/kf_sin.c \
    libm/math/kf_tan.c \
    libm/math/s_atan.c \
    libm/math/s_ceil.c \
    libm/math/s_cos.c \
    libm/math/s_fabs.c \
    libm/math/s_floor.c \
    libm/math/s_sin.c \
    libm/math/s_tan.c \
    libm/math/sf_atan.c \
    libm/math/sf_ceil.c \
    libm/math/sf_cos.c \
    libm/math/sf_fabs.c \
    libm/math/sf_floor.c \
    libm/math/sf_sin.c \
    libm/math/sf_tan.c \
    libm/math/w_acos.c \
    libm/math/w_asin.c \
    libm/math/w_atan2.c \
    libm/math/w_exp.c \
    libm/math/w_fmod.c \
    libm/math/w_hypot.c \
    libm/math/w_log.c \
    libm/math/w_log10.c \
    libm/math/w_pow.c \
    libm/math/w_sqrt.c \
    libm/math/wf_acos.c \
    libm/math/wf_asin.c \
    libm/math/wf_atan2.c \
    libm/math/wf_exp.c \
    libm/math/wf_fmod.c \
    libm/math/wf_hypot.c \
    libm/math/wf_log.c \
    libm/math/wf_log10.c \
    libm/math/wf_pow.c \
    libm/math/wf_sqrt.c


ifndef SQRT
    SQRT=sw
endif

ifeq ('$(SQRT)', 'hw')
    SRC+=libm/machine/$(ARCH)/e_sqrt.c \
         libm/machine/$(ARCH)/ef_sqrt.c
else
    SRC+=libm/math/e_sqrt.c \
         libm/math/ef_sqrt.c
endif

SRC_ROOT = .
SRC_DIRS = $(sort $(dir $(SRC)))

BUILD_ROOT = build-$(ARCH)

OBJ_ROOT = $(BUILD_ROOT)/obj
OBJ_DIRS = $(addprefix $(OBJ_ROOT)/,$(SRC_DIRS))

BIN_DIR = $(BUILD_ROOT)/bin

OUT = $(BIN_DIR)/libmlfs.a

CFLAGS = -c -Wall -std=gnu99 -pedantic -Wextra -frounding-math -fsignaling-nans -g -O2 -fno-builtin $(EXTRA_CFLAGS)

ifndef COVERAGE
    COVERAGE=false
endif

ifeq ('$(COVERAGE)', 'true')
    CFLAGS +=-O0 -fprofile-arcs -ftest-coverage
endif

OBJ = $(addprefix $(OBJ_ROOT)/, $(SRC:.c=.o))

.SUFFIXES:
.SUFFIXES: .o .c

# Include dependency info for *existing* .o files
-include $(OBJ:.o=.d)

.PHONY: all clean cleanall
all: $(OUT)

$(OUT): $(OBJ) | $(BIN_DIR)
	@echo "[AR] $@"
	$(Q)$(AR) -scr $@ $?

$(OBJ_ROOT)/%.o: $(SRC_ROOT)/%.c | $(OBJ_DIRS)
	@echo "[CC] $< > $@"
	$(Q)$(CC) $(CFLAGS) $(CFLAGS_TARGET) $(INCLUDE) -MD -MP $< -o $@

$(OBJ_DIRS) $(BIN_DIR):
	@echo "[MKDIR] $@"
	$(Q)$(MKDIR) $@

clean:
	@echo "[RM] $(BUILD_ROOT)"
	$(Q)$(RM) $(BUILD_ROOT)

cleanall:
	@echo "[RM] build-*"
	$(Q)$(RM) build-*
