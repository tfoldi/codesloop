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

/**
   @file bfd.cc
   @brief buffered file descriptor (fd)
 */

#include "codesloop/comm/bfd.hh"
#include "codesloop/common/libev/evwrap.h"
#include "codesloop/common/logger.hh"

namespace csl
{
  namespace comm
  {
    bfd::bfd() : fd_(bfd::not_initialized_), start_(0), len_(0), use_exc_(false) {}

    bfd::~bfd()
    {
      if( fd_ > 0 ) { CloseSocket( fd_ ); }
      fd_ = closed_;
    }

    bool bfd::read_buf(read_res & res, uint32_t sz)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF( L"read_buf(res,sz:%d) [len_:%d start_:%d fd_:%d]",sz,len_,start_,fd_ );

      if( len_ > 0 )
      {
        // fill res members
        res.bytes_      = (sz < len_ ? sz : len_);
        res.data_       = buf_+start_;
        res.failed_     = false;
        res.timed_out_  = false;

        // fill own members
        len_   -= static_cast<uint16_t>(res.bytes_);
        start_ += static_cast<uint16_t>(res.bytes_);

        CSL_DEBUGF( L"read_buf(res,sz:%d) => TRUE res{ bytes_:%lld data_:%p failed_:%s timed_out_:%s }",
                    sz,
                    res.bytes_,
                    res.data_,
                    (res.failed_==true?"TRUE":"FALSE"),
                    (res.timed_out_==true?"TRUE":"FALSE") );

        RETURN_FUNCTION( true );
      }
      else
      {
        CSL_DEBUGF( L"read_buf(res,sz:%d) => FALSE",sz );
        RETURN_FUNCTION( false );
      }
    }

    read_res bfd::read(uint32_t sz, uint32_t timeout_ms)
    {
      ENTER_FUNCTION();
      read_res ret;
      read(sz, timeout_ms, ret);
      RETURN_FUNCTION( ret );
    }

    read_res bfd::recv(uint32_t sz, uint32_t timeout_ms)
    {
      ENTER_FUNCTION();
      read_res ret;
      recv(sz, timeout_ms, ret);
      RETURN_FUNCTION( ret );
    }

    read_res bfd::recvfrom(uint32_t sz, SAI & from, uint32_t timeout_ms)
    {
      ENTER_FUNCTION();
      read_res ret;
      recvfrom(sz, from, timeout_ms, ret);
      RETURN_FUNCTION( ret );
    }

    read_res & bfd::read(uint32_t sz, uint32_t timeout_ms, read_res & ret)
    {
      ENTER_FUNCTION();
      ret.reset();

      CSL_DEBUGF( L"read(sz:%d, timeout_ms:%d)",
                  sz,
                  timeout_ms );

      if( sz == 0 )
      {
        CSL_DEBUGF( L"invalid parameter: sz:%d",sz );
        goto return_failed;
      }

      if( read_buf(ret,sz) ) goto return_ok;
      if( fd_ <= 0 )         goto return_failed;

      if( can_read(timeout_ms) )
      {
        if( fd_ < 0 ) goto return_failed;

        uint32_t len  = n_free();
        uint32_t pos  = sizeof(buf_)-len;
        int32_t  err  = ::read( fd_, buf_+pos, len );

        if( err < 0 )
        {
          /* fd error */
          CSL_DEBUGF( L"read(fd:%d, buf:(%p+%d), sz:%d) [pos:%d] => FAILED %d",fd_,buf_,pos,len,err );
          CloseSocket( fd_ );
          fd_ = fd_error_;
          goto return_failed;
        }
        else if( err  == 0 )
        {
          /* socket closed */
          CSL_DEBUGF( L"read(fd:%d, buf:(%p+%d), sz:%d) [pos:%d] => EOF %d",fd_,buf_,pos,len,err );
          CloseSocket( fd_ );
          fd_ = closed_;
          goto return_failed;
        }
        else
        {
          CSL_DEBUGF( L"read %d bytes",err );
          len_ += static_cast<uint16_t>(err);
          goto return_ok;
        }
      }
      else
      {
        ret.timed_out_=true;
        CSL_DEBUGF( L"timed out: timeout:%d",timeout_ms );
        goto return_ok;
      }

    return_failed:
      ret.failed_ = true;

    return_ok:
      CSL_DEBUGF( L"read(sz:%d, timeout_ms:%d) => res{ bytes_:%lld data_:%p failed_:%s timed_out_:%s }",
                  sz,
                  timeout_ms
                  res.bytes_,
                  res.data_,
                  (res.failed_==true?"TRUE":"FALSE"),
                  (res.timed_out_==true?"TRUE":"FALSE") );

      RETURN_FUNCTION( ret );
    }

    read_res & bfd::recv(uint32_t sz, uint32_t timeout_ms, read_res & ret)
    {
      ENTER_FUNCTION();
      ret.reset();

      CSL_DEBUGF( L"recv(sz:%d, timeout_ms:%d)", sz, timeout_ms );
      if( sz == 0 )
      {
        CSL_DEBUGF( L"invalid parameter: sz:%d",sz );
        goto return_failed;
      }

      if( read_buf(ret,sz) ) goto return_ok;
      if( fd_ <= 0 )         goto return_failed;

      if( can_read(timeout_ms) )
      {
        if( fd_ < 0 ) goto return_failed;

        uint32_t len  = n_free();
        uint32_t pos  = sizeof(buf_)-len;
        int32_t  err  = ::recv( fd_, buf_+pos, len, 0 );

        if( err < 0 )
        {
          /* fd error */
          CSL_DEBUGF( L"recv(fd:%d, buf:(%p+%d), sz:%d, 0) [pos:%d] => FAILED %d",fd_,buf_,pos,len,err );
          CloseSocket( fd_ );
          fd_ = fd_error_;
          goto return_failed;
        }
        else if( err  == 0 )
        {
          /* socket closed */
          CSL_DEBUGF( L"recv(fd:%d, buf:(%p+%d), sz:%d, 0) [pos:%d] => SOCKET CLOSED %d",fd_,buf_,pos,len,err );
          ShutdownCloseSocket( fd_ );
          fd_ = closed_;
          goto return_failed;
        }
        else
        {
          CSL_DEBUGF( L"received %d bytes",err );
          len_ += static_cast<uint16_t>(err);
          goto return_ok;
        }
      }
      else
      {
        ret.timed_out_=true;
        CSL_DEBUGF( L"timed out: timeout:%d",timeout_ms );
        goto return_ok;
      }

      return_failed:
      ret.failed_ = true;

      return_ok:
      CSL_DEBUGF( L"recv(sz:%d, timeout_ms:%d) => res{ bytes_:%lld data_:%p failed_:%s timed_out_:%s }",
                         sz,
                         timeout_ms
                         res.bytes_,
                         res.data_,
                         (res.failed_==true?"TRUE":"FALSE"),
                         (res.timed_out_==true?"TRUE":"FALSE") );

       RETURN_FUNCTION( ret );
    }

    read_res & bfd::recvfrom(uint32_t sz, SAI & from, uint32_t timeout_ms, read_res & ret)
    {
      ENTER_FUNCTION();
      ret.reset();

      CSL_DEBUGF( L"recvfrom(sz:%d, &from, timeout_ms:%d)", sz, timeout_ms );
      if( sz == 0 )
      {
        CSL_DEBUGF( L"invalid parameter: sz:%d",sz );
        goto return_failed;
      }

      if( read_buf(ret,sz) ) goto return_ok;
      if( fd_ <= 0 )         goto return_failed;

      if( can_read(timeout_ms) )
      {
        if( fd_ < 0 ) goto return_failed;

        uint32_t  len  = n_free();
        uint32_t  pos  = sizeof(buf_)-len;
        socklen_t slen = sizeof(SAI);
        int32_t   err  = ::recvfrom( fd_, buf_+pos, len, 0,
                                     reinterpret_cast<struct sockaddr *>(&from),
                                     &slen );

        if( err < 0 )
        {
          /* fd error */
          CSL_DEBUGF( L"recvfrom(fd:%d, buf:(%p+%d), sz:%d, 0) [pos:%d] => FAILED %d",fd_,buf_,pos,len,err );
          CloseSocket( fd_ );
          fd_ = fd_error_;
          goto return_failed;
        }
        else if( err  == 0 )
        {
          /* socket closed */
          CSL_DEBUGF( L"recvfrom(fd:%d, buf:(%p+%d), sz:%d, 0) [pos:%d] => SOCKET CLOSED %d",fd_,buf_,pos,len,err );
          ShutdownCloseSocket( fd_ );
          fd_ = closed_;
          goto return_failed;
        }
        else
        {
          CSL_DEBUGF( L"received %d bytes from:%s:%d",err,inet_ntoa(from.sin_addr),ntohs(from.sin_port));
          len_ += static_cast<uint16_t>(err);
          goto return_ok;
        }
      }
      else
      {
        ret.timed_out_=true;
        CSL_DEBUGF( L"timed out: timeout:%d",timeout_ms );
        goto return_ok;
      }

      return_failed:
      ret.failed_ = true;

      return_ok:
      CSL_DEBUGF( L"recvfrom(sz:%d, timeout_ms:%d) => res{ bytes_:%lld data_:%p failed_:%s timed_out_:%s }",
                  sz,
                  timeout_ms
                  res.bytes_,
                  res.data_,
                  (res.failed_==true?"TRUE":"FALSE"),
                  (res.timed_out_==true?"TRUE":"FALSE") );

      RETURN_FUNCTION( ret );
    }

    bool bfd::write(uint8_t * data, uint32_t sz)
    {
      ENTER_FUNCTION();
      int32_t err = 0;
      bool    ret = false;
      CSL_DEBUGF( L"write(data:%p, sz:%d)",data,sz );

      if( !data || !sz ) { CSL_DEBUGF( L"invalid params");    goto bail; }
      if( fd_ <= 0 )     { CSL_DEBUGF( L"invalid fd:%d",fd_); goto bail; }

      err = ::write( fd_, data, sz );

      if( err < 0 )
      {
        CSL_DEBUGF( L"write(fd:%d, ptr:%p, sz:%d) ERROR (returned %d)", fd_, data, sz, err );
        CloseSocket( fd_ );
        fd_ = fd_error_;
      }
      else if( err == 0 )
      {
        CSL_DEBUGF( L"write(fd:%d, ptr:%p, sz:%d) ERROR (returned 0)", fd_, data, sz );
      }
      else
      {
        ret = true;
      }

    bail:
      CSL_DEBUGF( L"write(data:%p, sz:%d) => %s",data,sz,(ret==true?"TRUE":"FALSE") );
      RETURN_FUNCTION( ret );
    }

    bool bfd::send(uint8_t * data, uint32_t sz)
    {
      ENTER_FUNCTION();
      int32_t err = 0;
      bool    ret = false;
      CSL_DEBUGF( L"send(data:%p, sz:%d)",data,sz );

      if( !data || !sz ) { CSL_DEBUGF( L"invalid params"); goto bail; }
      if( fd_ <= 0 )     { CSL_DEBUGF( L"invalid fd:%d",fd_); goto bail; }

      err = ::send( fd_, data, sz, 0 );

      if( err < 0 )
      {
        CSL_DEBUGF( L"send(fd:%d, ptr:%p, sz:%d, 0) ERROR (returned %d)", fd_, data, sz, err );
        ShutdownCloseSocket( fd_ );
        fd_ = fd_error_;
      }
      else if( err == 0 )
      {
        CSL_DEBUGF( L"send(fd:%d, ptr:%p, sz:%d, 0) SOCKET CLOSED (returned 0)", fd_, data, sz );
        ShutdownCloseSocket( fd_ );
        fd_ = closed_;
      }
      else
      {
        ret = true;
      }

    bail:
      CSL_DEBUGF( L"send(data:%p, sz:%d) => %s",data,sz,(ret==true?"TRUE":"FALSE") );
      RETURN_FUNCTION( ret );
    }

    bool bfd::sendto(uint8_t * data, uint32_t sz,const SAI & to)
    {
      ENTER_FUNCTION();
      int32_t   err  = 0;
      bool      ret  = false;
      socklen_t slen = sizeof(SAI);
      CSL_DEBUGF( L"sendto(data:%p, sz:%d, to:%s:%d)",data,sz,inet_ntoa(to.sin_addr),ntohs(to.sin_port) );

      if( !data || !sz ) { CSL_DEBUGF( L"invalid params"); goto bail; }
      if( fd_ <= 0 )     { CSL_DEBUGF( L"invalid fd:%d",fd_); goto bail; }


      err = ::sendto( fd_, data, sz, 0,
                      reinterpret_cast<const struct sockaddr *>(&to),
                      slen );

      if( err < 0 )
      {
        CSL_DEBUGF( L"sendto(fd:%d, ptr:%p, sz:%d, 0, %s:%d) ERROR (returned %d)",
                    fd_, data, sz, err, inet_nta(to.sin_addr),ntohs(to.sin_port) );
        ShutdownCloseSocket( fd_ );
        fd_ = fd_error_;
      }
      else if( err == 0 )
      {
        CSL_DEBUGF( L"send(fd:%d, ptr:%p, sz:%d, 0, %s:%d) SOCKET CLOSED (returned 0)",
                    fd_, data, sz, inet_ntoa(to.sin_addr),ntohs(to.sin_port) );
        ShutdownCloseSocket( fd_ );
        fd_ = closed_;
      }
      else
      {
        ret = true;
      }

    bail:
      CSL_DEBUGF( L"sendto(data:%p, sz:%d, to:%s:%d) => %s",
                  data,sz,inet_ntoa(to.sin_addr),ntohs(to.sin_port),
                  (ret==true?"TRUE":"FALSE") );
      RETURN_FUNCTION( ret );
    }

    int bfd::state() const
    {
      if( fd_ > 0 ) return ok_;
      else          return fd_;
    }

    bool bfd::can_read(uint32_t timeout_ms)
    {
      ENTER_FUNCTION();
      CSL_DEBUGF( L"can_read(timeout_ms:%d)  fd_:%d",timeout_ms,fd_ );

      if( fd_ <= 0 )
      {
        CSL_DEBUGF( L"invalid fd: %d",fd_ );
        RETURN_FUNCTION( false );
      }

      fd_set         fds;
      unsigned long  timeout_sec    = timeout_ms/1000;
      unsigned long  timeout_usec   = (timeout_ms%1000)*1000;
      struct timeval tv             = { timeout_sec, timeout_usec };

      FD_ZERO( &fds );
      FD_SET( fd_, &fds );

      int err = ::select( fd_+1,&fds,NULL,NULL,&tv );

      if( err < 0 ) /* error */
      {
        CSL_DEBUGF( L"select(...) => ERROR %d (closing socket)",err );
        CloseSocket( fd_ );
        fd_ = fd_error_;
        RETURN_FUNCTION( false );
      }
      else if( err == 0 ) /* timeout */
      {
        CSL_DEBUGF( L"timed out" );
        RETURN_FUNCTION( false );
      }
      else /* ok, can read */
      {
        RETURN_FUNCTION( true );
      }
    }

    /* not implemented
    bool bfd::can_write(uint32_t timeout_ms)
    {
      if( fd_ <= 0 ) return false;
      return false;
    }*/

    uint32_t bfd::size() const
    {
      return len_;
    }

    uint32_t bfd::n_free() const
    {
      return (sizeof(buf_)-(len_+start_));
    }
  }
}

/* EOF */
