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

# SET(CMAKE_VERBOSE_MAKEFILE ON)

# -- rpc --

# ADD_DEFINITIONS( -DDEBUG )

INCLUDE_DIRECTORIES( ../.. ) 
LINK_DIRECTORIES( ../common )
LINK_LIBRARIES( csl_common )

ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/csrparser.cc
                  PRE_BUILD
                  COMMAND ragel -G2 ARGS -o csrparser.cc csrparser.rl
                  MAIN_DEPENDENCY csrparser.rl
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_EXECUTABLE( csrgen  csrparser.cc    csrparser.hh
                iface.cc        iface.hh 
                stub_base.cc    stub_base.hh
                stub_header.cc  stub_header.hh
                stub_server.cc  stub_server.hh
                stub_client.cc  stub_client.hh
                exc.cc          exc.hh
                csrgen_main.cc 
                csrgen.hh )

ADD_LIBRARY( csl_rpc STATIC
             handle.cc        handle.hh
             cli_trans_tcp.cc cli_trans_tcp.hh
             srv_trans_tcp.cc srv_trans_tcp.hh
             srv_trans.cc     srv_trans.hh
             cli_trans.cc     cli_trans.hh )

FILE(GLOB includes "${CMAKE_CURRENT_SOURCE_DIR}/*.h*")
INSTALL( FILES ${includes} DESTINATION include/codesloop/rpc )

INSTALL(TARGETS csrgen
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

INSTALL(TARGETS csl_rpc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# -- EOF 
