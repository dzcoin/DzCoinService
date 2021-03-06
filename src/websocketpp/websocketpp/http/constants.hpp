﻿/*
 * copyright (c) 2014, peter thorson. all rights reserved.
 *
 * redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * neither the name of the websocket++ project nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * this software is provided by the copyright holders and contributors "as is"
 * and any express or implied warranties, including, but not limited to, the
 * implied warranties of merchantability and fitness for a particular purpose
 * are disclaimed. in no event shall peter thorson be liable for any
 * direct, indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused and
 * on any theory of liability, whether in contract, strict liability, or tort
 * (including negligence or otherwise) arising in any way out of the use of this
 * software, even if advised of the possibility of such damage.
 *
 */

#ifndef http_constants_hpp
#define http_constants_hpp

#include <map>
#include <string>
#include <vector>

namespace websocketpp {
/// http handling support
namespace http {
    /// the type of an http attribute list
    /**
     * the attribute list is an unordered key/value map. encoded attribute
     * values are delimited by semicolons.
     */
    typedef std::map<std::string,std::string> attribute_list;

    /// the type of an http parameter list
    /**
     * the parameter list is an ordered pairing of a parameter and its
     * associated attribute list. encoded parameter values are delimited by
     * commas.
     */
    typedef std::vector< std::pair<std::string,attribute_list> > parameter_list;

    /// literal value of the http header delimiter
    static char const header_delimiter[] = "\r\n";

    /// literal value of the http header separator
    static char const header_separator[] = ":";

    /// literal value of an empty header
    static std::string const empty_header = "";

    /// maximum size in bytes before rejecting an http header as too big.
    size_t const max_header_size = 16000;

    /// number of bytes to use for temporary istream read buffers
    size_t const istream_buffer = 512;

    /// invalid http token characters
    /**
     * 0x00 - 0x32, 0x7f-0xff
     * ( ) < > @ , ; : \ " / [ ] ? = { }
     */
    static char const header_token[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..0f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 10..1f
        0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,0, // 20..2f
        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, // 30..3f
        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 40..4f
        1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1, // 50..5f
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 60..6f
        1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0, // 70..7f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 80..8f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 90..9f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // a0..af
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // b0..bf
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // c0..cf
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // d0..df
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // e0..ef
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // f0..ff
    };

    /// is the character a token
    inline bool is_token_char(unsigned char c) {
        return (header_token[c] == 1);
    }

    /// is the character a non-token
    inline bool is_not_token_char(unsigned char c) {
        return !header_token[c];
    }

    /// is the character whitespace
    /**
     * whitespace is space (32) or horizontal tab (9)
     */
    inline bool is_whitespace_char(unsigned char c) {
        return (c == 9 || c == 32);
    }

    /// is the character non-whitespace
    inline bool is_not_whitespace_char(unsigned char c) {
        return (c != 9 && c != 32);
    }

    /// http status codes
    namespace status_code {
        enum value {
            uninitialized = 0,

            continue_code = 100,
            switching_protocols = 101,

            ok = 200,
            created = 201,
            accepted = 202,
            non_authoritative_information = 203,
            no_content = 204,
            reset_content = 205,
            partial_content = 206,

            multiple_choices = 300,
            moved_permanently = 301,
            found = 302,
            see_other = 303,
            not_modified = 304,
            use_proxy = 305,
            temporary_redirect = 307,

            bad_request = 400,
            unauthorized = 401,
            payment_required = 402,
            forbidden = 403,
            not_found = 404,
            method_not_allowed = 405,
            not_acceptable = 406,
            proxy_authentication_required = 407,
            request_timeout = 408,
            conflict = 409,
            gone = 410,
            length_required = 411,
            precondition_failed = 412,
            request_entity_too_large = 413,
            request_uri_too_long = 414,
            unsupported_media_type = 415,
            request_range_not_satisfiable = 416,
            expectation_failed = 417,
            im_a_teapot = 418,
            upgrade_required = 426,
            precondition_required = 428,
            too_many_requests = 429,
            request_header_fields_too_large = 431,

            internal_server_error = 500,
            not_implemented = 501,
            bad_gateway = 502,
            service_unavailable = 503,
            gateway_timeout = 504,
            http_version_not_supported = 505,
            not_extended = 510,
            network_authentication_required = 511
        };

        // todo: should this be inline?
        inline std::string get_string(value c) {
            switch (c) {
                case uninitialized:
                    return "uninitialized";
                case continue_code:
                    return "continue";
                case switching_protocols:
                    return "switching protocols";
                case ok:
                    return "ok";
                case created:
                    return "created";
                case accepted:
                    return "accepted";
                case non_authoritative_information:
                    return "non authoritative information";
                case no_content:
                    return "no content";
                case reset_content:
                    return "reset content";
                case partial_content:
                    return "partial content";
                case multiple_choices:
                    return "multiple choices";
                case moved_permanently:
                    return "moved permanently";
                case found:
                    return "found";
                case see_other:
                    return "see other";
                case not_modified:
                    return "not modified";
                case use_proxy:
                    return "use proxy";
                case temporary_redirect:
                    return "temporary redirect";
                case bad_request:
                    return "bad request";
                case unauthorized:
                    return "unauthorized";
                case payment_required:
                    return "payment required";
                case forbidden:
                    return "forbidden";
                case not_found:
                    return "not found";
                case method_not_allowed:
                    return "method not allowed";
                case not_acceptable:
                    return "not acceptable";
                case proxy_authentication_required:
                    return "proxy authentication required";
                case request_timeout:
                    return "request timeout";
                case conflict:
                    return "conflict";
                case gone:
                    return "gone";
                case length_required:
                    return "length required";
                case precondition_failed:
                    return "precondition failed";
                case request_entity_too_large:
                    return "request entity too large";
                case request_uri_too_long:
                    return "request-uri too long";
                case unsupported_media_type:
                    return "unsupported media type";
                case request_range_not_satisfiable:
                    return "requested range not satisfiable";
                case expectation_failed:
                    return "expectation failed";
                case im_a_teapot:
                    return "i'm a teapot";
                case upgrade_required:
                    return "upgrade required";
                case precondition_required:
                    return "precondition required";
                case too_many_requests:
                    return "too many requests";
                case request_header_fields_too_large:
                    return "request header fields too large";
                case internal_server_error:
                    return "internal server error";
                case not_implemented:
                    return "not implemented";
                case bad_gateway:
                    return "bad gateway";
                case service_unavailable:
                    return "service unavailable";
                case gateway_timeout:
                    return "gateway timeout";
                case http_version_not_supported:
                    return "http version not supported";
                case not_extended:
                    return "not extended";
                case network_authentication_required:
                    return "network authentication required";
                default:
                    return "unknown";
            }
        }
    }

    class exception : public std::exception {
    public:
        exception(const std::string& log_msg,
                  status_code::value error_code,
                  const std::string& error_msg = "",
                  const std::string& body = "")
          : m_msg(log_msg)
          , m_error_msg(error_msg)
          , m_body(body)
          , m_error_code(error_code) {}

        ~exception() throw() {}

        virtual const char* what() const throw() {
            return m_msg.c_str();
        }

        std::string         m_msg;
        std::string         m_error_msg;
        std::string         m_body;
        status_code::value  m_error_code;
    };
}
}

#endif // http_constants_hpp


