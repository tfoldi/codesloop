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

// #if 0
#ifndef DEBUG
#define DEBUG
#define DEBUG_ENABLE_INDENT
//#define DEBUG_VERBOSE
#endif /* DEBUG */
// #endif

#include "codesloop/common/pbuf.hh"
#include "codesloop/common/preallocated_array.hh"
#include "codesloop/common/str.hh"
#include "codesloop/common/test_timer.h"
#include "codesloop/common/common.h"
#include <assert.h>
#include <string>

using csl::common::pbuf;
using csl::common::preallocated_array;
using csl::common::str;

namespace test_preallocated_array {

  void preallocated_array_baseline()
  {
    preallocated_array<char,128> b;
  }
  
  void pbuf_baseline()
  {
    pbuf b;
  }

  void string_baseline()
  {
    std::string b;
  }

  void str_baseline()
  {
    str b;
  }

  void preallocated_array_hello()
  {
    preallocated_array<char,128> b;
    b.set( "Hello", 6 );
  }

  void pbuf_hello()
  {
    pbuf b;
    b.append( reinterpret_cast<const unsigned char *>("Hello"),6);
  }

  void string_hello()
  {
    std::string b;
    b = "Hello";
  }

  void str_hello()
  {
    str b;
    b = L"Hello";
  }

  void test_selfequal()
  {
    preallocated_array<char, 10> t;
    t = t;
  }

  void baseline_1()
  {
    preallocated_array<char, 1> t;
  }
      
  void baseline_10()
  {
    preallocated_array<char, 10> t;
  }
  
  void alloc_10_static()
  {
    preallocated_array<char, 10> t;
    for(size_t i=0;i<10;++i)
    {
      assert(t.allocate(i) != 0);
    }
  }
  
  void alloc_10_static_of_20()
  {
    preallocated_array<char, 10> t;
    for(size_t i=0;i<20;++i)
    {
      assert(t.allocate(i) != 0);
    }
  }
  
  void reset_1()
  {
    preallocated_array<char, 10> t;
    t.reset();
  }

  void reset_10()
  {
    preallocated_array<char, 10> t;
    for(size_t i=0;i<10;++i)
    {
      t.reset();
    }
  }

  void set_get_1()
  {
    preallocated_array<char, 10> t;
    char tx = '0';
    t.set(&tx,1);
    tx = 'x';
    t.get(&tx);
    assert( tx == '0' );
  }

  void set_get_1_str()
  {
    std::string s;
    s += '0';
    assert( s[0] == '0' );
  }
  
  void append_t()
  {
    preallocated_array<char, 10> t;
    char tx = '0';
    t.append(tx);
  }
  
  void append_ptr()
  {
    preallocated_array<char, 10> t;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,sizeof(tx));
  }
  
  void append_pa()
  {
    preallocated_array<char, 10> t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,sizeof(tx));
    t2.append(t);
  }
  
  void append_t_str()
  {
    std::string t;
    char tx = '0';
    t.append(&tx,(&tx)+1);
  }
  
  void append_ptr_str()
  {
    std::string t;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,tx+sizeof(tx));
  }
  
  void append_pa_str()
  {
    std::string t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,tx+sizeof(tx));
    t2.append(t);
  }
    
  void assign_10()
  {
    preallocated_array<char, 10> t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,sizeof(tx));
    for(int i=0;i<10;++i) t2 = t;
  }

  void assign_10_str()
  {
    std::string t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,tx+sizeof(tx));
    for(int i=0;i<10;++i) t2 = t;
  }

  void equal_10()
  {
    preallocated_array<char, 10> t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,sizeof(tx));
    bool eq = false;
    for(int i=0;i<10;++i) { eq = (t2 == t); }
  }

  void equal_10_str()
  {
    std::string t,t2;
    char tx[] = { 'h','e','l','l','o' };
    t.append(tx,tx+sizeof(tx));
    bool eq = false;
    for(int i=0;i<10;++i) { eq = (t2 == t); }
  }
  
  class Direct
  {
  public:
    void append(char c) { pa10_.append(c); }
    size_t size()       { return pa10_.size(); }
  private:
    preallocated_array<char, 10> pa10_;
  };
  
  void test_direct()
  {
    Direct d;
    d.append('a');
    d.append('b');
    assert( d.size() == 2 );
  }

  void test_direct0()
  {
    Direct d;
    d.append('a');
  }
      
  class Base
  {
  public:
    virtual void append(char c) = 0;
    virtual size_t size() = 0;
    virtual ~Base() {}
  };
  
  class Virtual : public Base
  {
  public:
    void append(char c) { pa10_.append(c); }
    size_t size()       { return pa10_.size(); }
  private:
    preallocated_array<char, 10> pa10_;
  };
  
  void test_virtual()
  {
    Virtual v;
    Base* bp = &v;
    bp->append('a');
    bp->append('b');
    assert( bp->size() == 2 );    
  }

  void test_virtual0()
  {
    Virtual v;
    Base* bp = &v;
    bp->append('a');
  }

} // end of test_preallocated_array

using namespace test_preallocated_array;

int main()
{
  test_selfequal();

  csl_common_print_results( "direct0            ", csl_common_test_timer_v0(test_direct0),"" );
  csl_common_print_results( "virtual0           ", csl_common_test_timer_v0(test_virtual0),"" );
  csl_common_print_results( "direct             ", csl_common_test_timer_v0(test_direct),"" );
  csl_common_print_results( "virtual            ", csl_common_test_timer_v0(test_virtual),"" );
  csl_common_print_results( "baseline_1         ", csl_common_test_timer_v0(baseline_1),"" );
  csl_common_print_results( "baseline_10        ", csl_common_test_timer_v0(baseline_10),"" );  
  csl_common_print_results( "alloc_10_static    ", csl_common_test_timer_v0(alloc_10_static),"" );
  csl_common_print_results( "10/20 alloc        ", csl_common_test_timer_v0(alloc_10_static_of_20),"" );
  csl_common_print_results( "reset_1            ", csl_common_test_timer_v0(reset_1),"" );
  csl_common_print_results( "reset_10           ", csl_common_test_timer_v0(reset_10),"" );
  csl_common_print_results( "set_get_1          ", csl_common_test_timer_v0(set_get_1),"" );
  csl_common_print_results( "set_get_1_str      ", csl_common_test_timer_v0(set_get_1_str),"" );
  csl_common_print_results( "append_t           ", csl_common_test_timer_v0(append_t),"" );
  csl_common_print_results( "append_ptr         ", csl_common_test_timer_v0(append_ptr),"" );
  csl_common_print_results( "append_pa          ", csl_common_test_timer_v0(append_pa),"" );
  csl_common_print_results( "append_t_str       ", csl_common_test_timer_v0(append_t_str),"" );
  csl_common_print_results( "append_ptr_str     ", csl_common_test_timer_v0(append_ptr_str),"" );
  csl_common_print_results( "append_pa_str      ", csl_common_test_timer_v0(append_pa_str),"" );
  csl_common_print_results( "assign_10          ", csl_common_test_timer_v0(assign_10),"" );
  csl_common_print_results( "assign_10_str      ", csl_common_test_timer_v0(assign_10_str),"" );
  csl_common_print_results( "equal_10           ", csl_common_test_timer_v0(equal_10),"" );
  csl_common_print_results( "equal_10_str       ", csl_common_test_timer_v0(equal_10_str),"" );

  csl_common_print_results( "PA_baseline        ", csl_common_test_timer_v0(preallocated_array_baseline),"" );
  csl_common_print_results( "pbuf_baseline      ", csl_common_test_timer_v0(pbuf_baseline),"" );
  csl_common_print_results( "str_baseline       ", csl_common_test_timer_v0(str_baseline),"" );
  csl_common_print_results( "string_baseline    ", csl_common_test_timer_v0(string_baseline),"" );

  csl_common_print_results( "PA_hello           ", csl_common_test_timer_v0(preallocated_array_hello),"" );
  csl_common_print_results( "pbuf_hello         ", csl_common_test_timer_v0(pbuf_hello),"" );
  csl_common_print_results( "str_hello          ", csl_common_test_timer_v0(str_hello),"" );
  csl_common_print_results( "string_hello       ", csl_common_test_timer_v0(string_hello),"" );
  
  return 0;
}

/* EOF */
