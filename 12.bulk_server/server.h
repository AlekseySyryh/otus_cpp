#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "async.h"

boost::asio::io_service service;
boost::asio::ip::tcp::acceptor acceptor(service);
const size_t buffer_size = 1024;

void on_read(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
             std::shared_ptr<std::array<char, buffer_size>> buff,
             async::handle_t handle,
             const boost::system::error_code &e,
             size_t len) {
    if (!e) {
        async::receive(handle, buff->data(), len);
        sock->async_read_some(boost::asio::buffer(buff->data(), buffer_size),
                              boost::bind(on_read, sock, buff, handle, _1, _2));
    } else {
        async::disconnect(handle);
        sock->close();
    }
}


void on_accept(std::shared_ptr<boost::asio::ip::tcp::socket> sock, size_t bulk, const boost::system::error_code &e) {
    auto socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(service);
    acceptor.async_accept(*socketPtr, boost::bind(on_accept, socketPtr, bulk, _1));
    if (!e) {
        auto handle = async::connect(bulk);
        auto buff = std::make_shared<std::array<char, buffer_size>>();
        sock->async_read_some(boost::asio::buffer(buff->data(), buffer_size),
                              boost::bind(on_read, sock, buff, handle, _1, _2));
    }
}

void server(unsigned short port, size_t bulk) {
    auto ip = boost::asio::ip::address_v4::any();
    boost::asio::ip::tcp::endpoint ep(ip, port);
    acceptor = boost::asio::ip::tcp::acceptor(service, ep);
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(service);
    acceptor.async_accept(*socket, boost::bind(on_accept, socket, bulk, _1));
    service.run();
}

