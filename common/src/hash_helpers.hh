/*
Copyright (c) 2008,2009, CodeSLoop Team

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

#ifndef _csl_common_hash_helpers_hh_included_
#define _csl_common_hash_helpers_hh_included_

#include "inpvec.hh"
#include "obj.hh"
#include "logger.hh"
#include "common.h"

#ifdef __cplusplus

namespace csl
{
  namespace common
  {
    namespace hash_helpers
    {
      typedef uint64_t hash_key_t;

      template <typename K, typename V>
      class contained
      {
        public:
          typedef K    key_t;
          typedef V    value_t;

        private:
          key_t        key_;
          value_t      value_;
          hash_key_t   hash_key_;
          size_t       pos_;
          contained *  next_;

        public:
          contained( const contained & other ) :
            key_(other.key_), value_(other.value_), hash_key_(other.hash_key_), pos_(0), next_(0) {}

          contained( key_t k, value_t v, hash_key_t hk, size_t p  ) :
              key_(k), value_(v), hash_key_(hk), pos_(p), next_(0) {}

          contained() :
              hash_key_(0), pos_(0), next_(0) {}

          bool is_equal( const key_t k, const value_t v )
          {
            if( k == key_ && v == value_ ) { return true;  }
            else                           { return false; }
          }

          contained * next() const   { return next_;  }
          void next(contained * n)   { next_ = n;     }

          hash_key_t hash_key() const   { return hash_key_;  }
          void hash_key(hash_key_t hk)  { hash_key_ = hk;    }

          const key_t & key() const      { return key_; }
          const value_t & value() const  { return value_; }

          size_t pos() const { return pos_; }
      };

      template <typename K, typename V, size_t SZ=32, size_t BITS=5>
      class page
      {
        public:
          typedef K                                               key_t;
          typedef V                                               value_t;
          typedef contained<K,V>                                  contained_t;
          typedef inpvec<contained_t>                             contained_vec_t;
          typedef typename inpvec<contained_t>::iterator          contained_iter_t;
          typedef inpvec< page >                                  page_vec_t;
          typedef inpvec< size_t >                                pos_vec_t;

          static const size_t sz_         = SZ;
          static const size_t bits_       = BITS;
          static const hash_key_t mask_   = 0x1ff;

          static const int  ok_            = 1;
          static const int  bad_pos_       = 2;
          static const int  has_already_   = 3;

          int add(size_t pos, const key_t & k, const value_t & v, hash_key_t hk)
          {
            ENTER_FUNCTION();
            if( pos > sz_ ) { THR(exc::rs_invalid_param,bad_pos_); }

            contained_iter_t it = data_.iterator_at(pos);

            if( it.is_empty() ) { it.set( k,v,hk,pos ); }
            else
            {
              contained_t * p = (*it);

              while( p )
              {
                if( p->is_equal( k,v ) )
                {
                  /* already have this item */
                  RETURN_FUNCTION(has_already_);
                }

                if( p->next() == 0 )
                {
                  it = data_.iterator_at( sz_ );
                  if( it.is_empty() == false ) it = data_.last_free();
                  it.set( k,v,hk,data_.iterator_pos(it) );
                  p->next(*it);
                  break;
                }
                p = p->next();
              }
            }

            RETURN_FUNCTION(ok_);
          }

          void remove(size_t pos, const key_t & k)
          {
            /* TODO */
          }

          void split(size_t shift, page_vec_t & pagev, pos_vec_t & posv)
          {
            typedef typename page_vec_t::iterator it_t;

            contained_iter_t it = data_.begin();
            contained_iter_t en = data_.end();
            size_t pos = 0;

            for( ;(it!=en) && (pos<32);++it )
            {
              if( it.is_empty() == false && pos != 0 )
              {
                it_t pit = pagev.last_free();
                size_t pageid = pagev.iterator_pos(pit);

                /* create the new page */
                pit.construct();

                /* register the new page position */
                posv.set( pos, pageid );
                contained_t * p = (*it);
                size_t px = 0;

                while( p )
                {
                  hash_key_t hk = p->hash_key();
                  (*pit)->add( ((hk>>shift)&mask_),p->key(),p->value(),hk );
                  px = p->pos();
                  p = p->next();
                  /* remove the item from current container */
                  data_.iterator_at(px).free();
                }
              }
              ++pos;
            }
          }

          contained_t * get(size_t pos)
          {
            if( pos > sz_ ) return 0;
            else return *(data_.iterator_at(pos));
          }

          size_t n_items() { return data_.n_items(); }

          bool has_item(size_t pos)
          {
            ENTER_FUNCTION();
            if( pos > sz_ ) { THR(exc::rs_invalid_param,false); }
            else RETURN_FUNCTION(data_.free_at(pos) == false);
          }

        private:
          contained_vec_t  data_;

          CSL_OBJ(csl::common::hash_helpers,page);
          USE_EXC();
      };

      class index
      {
        public:
          typedef uint64_t page_index_t;
      };
    }
  }
}

#endif /* __cplusplus */
#endif /* _csl_common_hash_helpers_hh_included_ */
