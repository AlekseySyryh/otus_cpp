#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

boost::asio::io_service service;
boost::asio::ip::tcp::acceptor acceptor(service);

void on_read(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
             std::shared_ptr<boost::asio::streambuf> buff,
             const boost::system::error_code &e);

void on_write(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
              std::shared_ptr<boost::asio::streambuf> buff,
              const boost::system::error_code &e) {
    if (!e) {
        boost::asio::async_read_until(*sock, *buff, '\n', boost::bind(on_read, sock, buff, _1));
    }
}

void on_read(std::shared_ptr<boost::asio::ip::tcp::socket> sock,
             std::shared_ptr<boost::asio::streambuf> buff,
             const boost::system::error_code &e) {
    if (!e) {
        std::string data;
        std::istream is(buff.get());
        std::getline(is, data);
        data += '\n';
        std::cout << data;
        sock->async_write_some(boost::asio::buffer(data), boost::bind(on_write, sock, buff, _1));
    }
}

void on_accept(std::shared_ptr<boost::asio::ip::tcp::socket> sock, const boost::system::error_code &e) {
    auto socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(service);
    acceptor.async_accept(*socketPtr, boost::bind(on_accept, socketPtr, _1));
    if (!e) {
        auto buffPtr = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(*sock, *buffPtr, '\n', boost::bind(on_read, sock, buffPtr, _1));
    }
}

void server(unsigned short port) {
    auto ip = boost::asio::ip::address_v4::any();
    boost::asio::ip::tcp::endpoint ep(ip, port);
    acceptor = boost::asio::ip::tcp::acceptor(service, ep);
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(service);
    acceptor.async_accept(*socket, boost::bind(on_accept, socket, _1));
    service.run();
}