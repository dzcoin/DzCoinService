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

#include <cassert>

namespace beast {
namespace sqdb {
namespace detail {

namespace
{

template <typename t>
inline t& as(void* data)
{
    return *static_cast<t*>(data);
}

template <typename t>
static void integer_into(void* data, sqlite3_int64 v)
{
    *(static_cast<t*>(data)) = t(v);
}

}

standard_into_type::~standard_into_type()
{
}

void standard_into_type::bind(statement_imp& st, int& icol)
{
    m_st = &st;
    m_icol = icol++;
}

void standard_into_type::do_into()
{
    int coltype = sqlite3_column_type(m_st->m_stmt, m_icol);

    if (coltype == sqlite_null)
    {
        // null encountered with no indicator
        assert (m_ind != nullptr);

        *m_ind = i_null;
    }
    else
    {
        if (m_ind)
            *m_ind = i_ok;

        switch (coltype)
        {
        case sqlite_integer:
        {
            sqlite3_int64 v = sqlite3_column_int64(m_st->m_stmt, m_icol);

            switch (m_type)
            {
            case x_bool:        as <bool> (m_data) = v != 0; break;
            case x_char:        integer_into <char> (m_data, v); break;
            case x_uchar:       integer_into <unsigned char> (m_data, v); break;
            case x_short:       integer_into <short> (m_data, v); break;
            case x_ushort:      integer_into <unsigned short> (m_data, v); break;
            case x_int:         integer_into <int> (m_data, v); break;
            case x_uint:        integer_into <unsigned int> (m_data, v); break;
            case x_long:        integer_into <long> (m_data, v); break;
            case x_ulong:       integer_into <unsigned long> (m_data, v); break;
            case x_longlong:    integer_into <long long> (m_data, v); break;
            case x_ulonglong:   integer_into <unsigned long long> (m_data, v); break;

            case x_beaststring:
                as <string> (m_data) = string(v);
                break;

            default:
                throw std::invalid_argument ("unknown case");
            }
        }
        break;

        case sqlite_float:
        {
            double const v (sqlite3_column_double (
                m_st->m_stmt, m_icol));

            switch (m_type)
            {
            case x_float:       as <float> (m_data) = static_cast <float> (v); break;
            case x_double:      as <double> (m_data) = v; break;
            case x_longdouble:  as <long double> (m_data) = v; break;
                break;

            case x_beaststring:
                as <string> (m_data) = string(v);
                break;

            default:
                throw std::invalid_argument ("unknown case");
            };
        }
        break;

        case sqlite_text:
        {
            switch (m_type)
            {
            case x_stdstring:
            {
                // excludes terminator
                int bytes = sqlite3_column_bytes(m_st->m_stmt, m_icol);
                unsigned char const* v = sqlite3_column_text(m_st->m_stmt, m_icol);
                std::string& result = as<std::string>(m_data);
                result.assign(reinterpret_cast<char const*>(v), bytes);
            }
            break;

            case x_stdwstring:
                throw std::invalid_argument ("invalid case");
                break;

            case x_beaststring:
            {
                // excludes terminator
                int bytes = sqlite3_column_bytes(m_st->m_stmt, m_icol);

                const charpointer_utf8::chartype* c = reinterpret_cast
                                                      <const charpointer_utf8::chartype*>
                                                      (sqlite3_column_text(m_st->m_stmt, m_icol));

                string& s = as <string> (m_data);
                s = string(charpointer_utf8(c), charpointer_utf8(c + bytes));
            }
            break;

            default:
            {
                sqlite3_int64 v = sqlite3_column_int64(m_st->m_stmt, m_icol);

                switch (m_type)
                {
                case x_bool:        as <bool> (m_data) = v != 0; break;
                case x_char:        integer_into <char> (m_data, v); break;
                case x_uchar:       integer_into <unsigned char> (m_data, v); break;
                case x_short:       integer_into <short> (m_data, v); break;
                case x_ushort:      integer_into <unsigned short> (m_data, v); break;
                case x_int:         integer_into <int> (m_data, v); break;
                case x_uint:        integer_into <unsigned int> (m_data, v); break;
                case x_long:        integer_into <long> (m_data, v); break;
                case x_ulong:       integer_into <unsigned long> (m_data, v); break;
                case x_longlong:    integer_into <long long> (m_data, v); break;
                case x_ulonglong:   integer_into <unsigned long long> (m_data, v); break;

                default:
                    throw std::invalid_argument ("unknown case");
                }

            }
            break;
            };
        }
        break;

        case sqlite_blob:
            throw std::invalid_argument ("invalid case");

        default:
            throw std::invalid_argument ("unknown case");
        };
    }

    convert_from_base();
}

} // detail
} // sqdb
} // beast


