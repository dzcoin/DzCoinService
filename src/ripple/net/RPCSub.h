﻿//------------------------------------------------------------------------------
/*
    this file is part of rippled: https://github.com/ripple/rippled
    copyright (c) 2012, 2013 ripple labs inc.

    permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    the  software is provided "as is" and the author disclaims all warranties
    with  regard  to  this  software  including  all  implied  warranties  of
    merchantability  and  fitness. in no event shall the author be liable for
    any  special ,  direct, indirect, or consequential damages or any damages
    whatsoever  resulting  from  loss  of use, data or profits, whether in an
    action  of  contract, negligence or other tortious action, arising out of
    or in connection with the use or performance of this software.
*/
//==============================================================================

#ifndef ripple_net_rpc_rpcsub_h_included
#define ripple_net_rpc_rpcsub_h_included

#include <ripple/core/jobqueue.h>
#include <ripple/net/infosub.h>
#include <beast/threads/stoppable.h>
#include <boost/asio/io_service.hpp>

namespace ripple {

/** subscription object for json rpc. */
class rpcsub : public infosub
{
public:
    typedef std::shared_ptr <rpcsub> pointer;
    typedef pointer const& ref;

    // vfalco why is the io_service needed?
    static pointer new (infosub::source& source,
        boost::asio::io_service& io_service, jobqueue& jobqueue,
            std::string const& strurl, std::string const& strusername,
            std::string const& strpassword);

    virtual void setusername (std::string const& strusername) = 0;
    virtual void setpassword (std::string const& strpassword) = 0;

protected:
    explicit rpcsub (infosub::source& source);
};

} // ripple

#endif


