# Copyright (c) 2008,2009,2010, CodeSLoop Team
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

# -- common --

IF(NOT WIN32)
  SUBDIRS( libev )
ENDIF(NOT WIN32)

INCLUDE_DIRECTORIES( ../.. ${ZLIB_INCLUDE_DIR} )

ADD_LIBRARY( csl_common STATIC
             cexc.cc       cexc.hh
             exc.cc        exc.hh
             zfile.cc      zfile.hh
             pbuf.cc       pbuf.hh
             xdrbuf.cc     xdrbuf.hh
             test_timer.c  test_timer.h
             common.h      pvlist.hh
             circbuf.hh    queue.hh
             mpool.hh      tbuf.hh
             logger.cc     logger.hh
             arch.cc       arch.hh
             serializable.hh
             obj.cc        obj.hh
             hlprs.cc      hlprs.hh
             hash.cc       hash.hh
             hash_helpers.hh
             auto_close.hh
             read_res.cc   read_res.hh
             var.hh
             int64.cc      int64.hh
             dbl.cc        dbl.hh
             str.cc        str.hh
             ustr.cc       ustr.hh
             binry.cc      binry.hh
             # -- replacements for rdbuf, read_res, tbuf
             stream_buffer.hh stream_buffer_impl.hh
	     stream_nop_target.hh
             stream.hh stream_impl.hh
	     stream_part.hh stream_part_impl.hh
	     stream_flags.cc stream_flags.hh
             preallocated_array.hh preallocated_array_impl.hh )

# ADD_DEFINITIONS( -DENABLE_LOGGER )
SET_SOURCE_FILES_PROPERTIES( logger.cc PROPERTIES 
                             COMPILE_FLAGS -DENABLE_LOGGER )
# SET_SOURCE_FILES_PROPERTIES( stream_flags.cc PROPERTIES 
#                              COMPILE_FLAGS "-DDEBUG -DDEBUG_ENABLE_INDENT" )

ADD_DEPENDENCIES( csl_common libev/config.h )

FILE(GLOB includes "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
INSTALL( FILES ${includes} DESTINATION include/codesloop/common ) 
INSTALL(TARGETS csl_common
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# -- EOF --
