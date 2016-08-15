﻿//------------------------------------------------------------------------------
/*
    this file is part of beast: https://github.com/vinniefalco/beast
    copyright 2013, vinnie falco <vinnie.falco@gmail.com>

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

    portions based on soci - the c++ database access library: 

    soci: http://soci.sourceforge.net/

    this file incorporates work covered by the following copyright
    and permission notice:

    copyright (c) 2004 maciej sobczak, stephen hutton, mateusz loskot,
    pawel aleksander fedorynski, david courtney, rafal bobrowski,
    julian taylor, henning basold, ilia barahovski, denis arnaud,
    daniel lidstr鰉, matthieu kermagoret, artyom beilis, cory bennett,
    chris weed, michael davidsaver, jakub stachowski, alex ott, rainer bauer,
    martin muenstermann, philip pemberton, eli green, frederic chateau,
    artyom tonkikh, roger orr, robert massaioli, sergey nikulov,
    shridhar daithankar, s鰎en meyer-eppler, mario valesco.

    boost software license - version 1.0, august 17th, 2003

    permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "software") to use, reproduce, display, distribute,
    execute, and transmit the software, and to prepare derivative works of the
    software, and to permit third-parties to whom the software is furnished to
    do so, all subject to the following:

    the copyright notices in the software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the software, in whole or in part, and
    all derivative works of the software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    the software is provided "as is", without warranty of any kind, express or
    implied, including but not limited to the warranties of merchantability,
    fitness for a particular purpose, title and non-infringement. in no event
    shall the copyright holders or anyone distributing the software be liable
    for any damages or other liability, whether in contract, tort or otherwise,
    arising from, out of or in connection with the software or the use or other
    dealings in the software.
*/
//==============================================================================

#ifndef beast_sqdb_statement_h_included
#define beast_sqdb_statement_h_included

#include <memory>

namespace beast {
namespace sqdb {

//
// lightweight copyable handle to a statement. this
// is just a wrapper around the actual statement implementation.
//
class statement
{
public:
    statement(session& s);
    statement(detail::prepare_temp_type const& prep);
    ~statement();

    statement(statement const& o);
    void operator= (statement const& o);

    void exchange(detail::into_type_ptr const& i);
    void exchange(detail::use_type_ptr const& u);

    void prepare(std::string const& query, bool brepeatable = true);

    error execute();
    bool execute_and_fetch(error& error);
    bool fetch(error& error);

    bool got_data() const;
    rowid last_insert_rowid();

private:
    friend class detail::ref_counted_statement;

    void clean_up();

private:
    std::shared_ptr <detail::statement_imp> m_imp;
};

} // sqdb
} // beast

#endif


