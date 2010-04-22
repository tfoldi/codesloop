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

#if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
#endif

#include "codesloop/common/stream_nop_target.hh"
#include "codesloop/common/stream_buffer.hh"
#include "codesloop/common/stream.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>

using namespace csl::common;

namespace test_stream {

  template <typename T> class counting_target
  {
    public:
      uint32_t start_count_;
      uint32_t end_count_;
      uint32_t flush_count_;
      uint32_t data_count_;
      size_t   data_amount_;

      const stream_flags & start(stream_base<T> &)
      {
        ++start_count_;
        return flags_;
      }

      const stream_flags & end(stream_base<T> &)
      {
        ++end_count_;
        return flags_;
      }

      const stream_flags & flush(stream_base<T> &)
      {
        ++flush_count_;
        return flags_;
      }

      const stream_flags & data(stream_base<T> & s, size_t sz)
      {
        ++data_count_;
        data_amount_ += sz;
        return flags_;
      }
      
      counting_target() : start_count_(0),
                          end_count_(0),
                          flush_count_(0),
                          data_count_(0),
                          data_amount_(0) {}

      stream_flags flags_;
  };

  void test_baseline_nop_target()
  {
    stream_nop_target<char> t;
  }

  void test_baseline_buffer()
  {
    stream_buffer<char> b;
  }

  void test_baseline_stream()
  {
    stream_buffer<double> b;
    stream<double> s(b);
  }
  
  void test_flags()
  {
    stream_buffer<uint32_t,1,2> b;
    typedef stream<uint32_t,stream_buffer,stream_nop_target,1,2> ui32stream_t;
    
    ui32stream_t s(b);
    // initial flags are ok_
    assert( s.flags() == stream_flags::ok_ );
        
    // get from empty buffer
    ui32stream_t::part_t p;
    ui32stream_t::part_t & sp(s.get(10,p));
    
    assert( p.flags() & stream_flags::empty_buffer_ );
    assert( sp.flags() & stream_flags::empty_buffer_ );
  }
  
  void test_reserve_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    ui32stream_t::part_t sp;
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( s.confirmed_items() == 1 );
  }
  
  void test_confirm_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    ui32stream_t::part_t sp;
    s.reserve(2,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.reserve(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( t.data_count_ == 2 );
    assert( t.data_amount_ == 2 );
    assert( s.confirmed_items() == 2 );
  }

  void test_get_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    ui32stream_t::part_t sp;
    s.reserve(2,sp);
    assert( s.confirmed_items() == 0 );
    assert( sp.flags() == stream_flags::ok_ );
    s.confirm(1,sp);
    assert( s.confirmed_items() == 1 );
    assert( sp.flags() == stream_flags::ok_ );
    s.get(1,sp);
    assert( sp.flags() == stream_flags::ok_ );
    assert( s.confirmed_items() == 1 );
  }
  
  void test_start_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    assert( s.start() == stream_flags::ok_ );
    assert( t.start_count_ == 1 );
    assert( s.confirmed_items() == 0 );
  }

  void test_end_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    assert( s.end() == stream_flags::ok_ );
    assert( t.end_count_ == 1 );
    assert( s.confirmed_items() == 0 );
  }
  
  void test_flush_ok()
  {
    stream_buffer<uint32_t,1,2> b;
    counting_target<uint32_t> t;
    typedef stream<uint32_t,stream_buffer,counting_target,1,2> ui32stream_t;
    ui32stream_t s(t,b);
    assert( s.flush() == stream_flags::ok_ );
    assert( t.flush_count_ == 1 );
    assert( s.confirmed_items() == 0 );
  }

  void test_reserve_failed() {}
  void test_confirm_failed() {}
  void test_get_failed() {}
  
  void test_start_failed()
  {
    stream_buffer<double,1,2> b;
    counting_target<double> t;
    t.flags_ << stream_flags::security_error_;
    typedef stream<double,stream_buffer,counting_target,1,2> dblstream_t;
    dblstream_t s(t,b);
    assert( s.start() == stream_flags::security_error_ );
  }
  
  void test_end_failed() {}
  void test_flush_failed() {}
}

using namespace test_stream;

int main()
{
#ifdef DEBUG

  test_reserve_ok();
  test_confirm_ok();
  test_get_ok();
  test_start_ok();
  test_end_ok();
  test_flush_ok();

  test_reserve_failed();
  test_confirm_failed();
  test_get_failed();
  test_start_failed();
  test_end_failed();
  test_flush_failed();
      
  test_flags();
  test_baseline_stream();
  
#else

  csl_common_print_results( "baseline nop target ", csl_common_test_timer_v0(test_baseline_nop_target),"" );
  csl_common_print_results( "baseline buffer     ", csl_common_test_timer_v0(test_baseline_buffer),"" );
  csl_common_print_results( "baseline stream     ", csl_common_test_timer_v0(test_baseline_stream),"" );
  csl_common_print_results( "flags               ", csl_common_test_timer_v0(test_flags),"" );
  
  csl_common_print_results( "reserve ok          ", csl_common_test_timer_v0(test_reserve_ok),"" );
  csl_common_print_results( "confirm ok          ", csl_common_test_timer_v0(test_confirm_ok),"" );
  csl_common_print_results( "get ok              ", csl_common_test_timer_v0(test_get_ok),"" );
  csl_common_print_results( "start ok            ", csl_common_test_timer_v0(test_start_ok),"" );
  csl_common_print_results( "end ok              ", csl_common_test_timer_v0(test_end_ok),"" );
  csl_common_print_results( "flush ok            ", csl_common_test_timer_v0(test_flush_ok),"" );

  csl_common_print_results( "reserve failed      ", csl_common_test_timer_v0(test_reserve_failed),"" );
  csl_common_print_results( "confirm failed      ", csl_common_test_timer_v0(test_confirm_failed),"" );
  csl_common_print_results( "get failed          ", csl_common_test_timer_v0(test_get_failed),"" );
  csl_common_print_results( "start failed        ", csl_common_test_timer_v0(test_start_failed),"" );
  csl_common_print_results( "end failed          ", csl_common_test_timer_v0(test_end_failed),"" );
  csl_common_print_results( "flush failed        ", csl_common_test_timer_v0(test_flush_failed),"" );
  
#endif
  return 0;
}

/* EOF */

