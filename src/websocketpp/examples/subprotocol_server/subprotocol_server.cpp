﻿#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::ref;


bool validate(server & s, connection_hdl hdl) {
    server::connection_ptr con = s.get_con_from_hdl(hdl);

    std::cout << "cache-control: " << con->get_request_header("cache-control") << std::endl;

    const std::vector<std::string> & subp_requests = con->get_requested_subprotocols();
    std::vector<std::string>::const_iterator it;

    for (it = subp_requests.begin(); it != subp_requests.end(); ++it) {
        std::cout << "requested: " << *it << std::endl;
    }

    if (subp_requests.size() > 0) {
        con->select_subprotocol(subp_requests[0]);
    }

    return true;
}

int main() {
    try {
        server s;

        s.set_validate_handler(bind(&validate,ref(s),::_1));

        s.init_asio();
        s.listen(9005);
        s.start_accept();

        s.run();
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}


