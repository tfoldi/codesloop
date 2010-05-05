/*
Copyright (c) 2008,2009,2010, CodeSLoop Team

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _csl_comm_tcp_stream_hh_included_
#define _csl_comm_tcp_stream_hh_included_
#include "codesloop/comm/network_stream.hh"
#ifdef __cplusplus

namespace csl
{
  namespace comm
  {
    namespace tcp
    {
      template <typename T,
                template <typename,size_t,size_t> class Buffer = csl::common::stream_buffer,
                template <typename> class Target = csl::common::stream_nop_target,
                size_t Preallocated=1024,
                size_t MaxSize=(256*1024)>
      class stream : public csl::comm::network_stream_base<T>,
                     public csl::common::stream<T,Buffer,Target,Preallocated,MaxSize>
      {
      public:
      
        stream(target_t & t, buffer_t & b);
        stream(buffer_t & b);

      private:
        CSL_OBJ(csl::common::tcp,stream);
      };
    }
  }
}

#endif /*__cplusplus*/
#include "codesloop/comm/tcp_stream_impl.hh"
#endif /*_csl_comm_tcp_stream_hh_included_*/

