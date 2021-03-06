# Copyright (c) 2008,2009,2010, CodeSloop Team
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

SET(GCC_COMMON_WARNINGS " -Wall -Wwrite-strings -Wcast-qual -Wpointer-arith -Wconversion -Wcomment -Wcast-align -Wshadow -Wredundant-decls ")

SET(GCC_C_FLAGS_RELEASE "")
SET(GCC_C_FLAGS_DEBUG "")
SET(GCC_CXX_FLAGS_RELEASE "")
SET(GCC_CXX_FLAGS_DEBUG "")

IF(WIN32)
  IF(MSYS)
    SET(GCC_C_FLAGS_RELEASE   " -O3 ${GCC_COMMON_WARNINGS} -DRELEASE_BUILD ")
    SET(GCC_C_FLAGS_DEBUG     " -g3 ${GCC_COMMON_WARNINGS} -DDEBUG_BUILD ")
    SET(GCC_CXX_FLAGS_RELEASE " -O3 ${GCC_COMMON_WARNINGS} -DRELEASE_BUILD -Wold-style-cast ")
    SET(GCC_CXX_FLAGS_DEBUG   " -g3 ${GCC_COMMON_WARNINGS} -DDEBUG_BUILD -Wold-style-cast ")
  ENDIF(MSYS)
ELSE(WIN32)
  SET(GCC_C_FLAGS_RELEASE   " -O3 ${GCC_COMMON_WARNINGS} -DRELEASE_BUILD ")
  SET(GCC_C_FLAGS_DEBUG     " -g3 ${GCC_COMMON_WARNINGS} -DDEBUG_BUILD ")
  SET(GCC_CXX_FLAGS_RELEASE " -O3 ${GCC_COMMON_WARNINGS} -DRELEASE_BUILD -Wold-style-cast ")
  SET(GCC_CXX_FLAGS_DEBUG   " -g3 ${GCC_COMMON_WARNINGS} -DDEBUG_BUILD -Wold-style-cast ")
ENDIF(WIN32)

# -- EOF --

