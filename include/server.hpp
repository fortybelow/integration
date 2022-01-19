#pragma once

#include <cstdlib>
#include <functional>
#include <iostream>
#include <tuple>
#include <cctype>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace nstd
{
    void hexdump(void* pointer, int length, const std::string& prefix);
}

using boost::asio::ip::tcp;

constexpr const char* red = "\033[38;2;212;81;78m";
constexpr const char* green = "\033[38;2;32;211;48m";
constexpr const char* light = "\033[38;2;81;162;248m";
constexpr const char* blue = "\033[38;2;52;75;238m";
constexpr const char* yellow = "\033[38;2;218;187;32m";
constexpr const char* reset = "\033[0m";


namespace
{
    template <class Tuple>
    struct colored_arg_t
    {
        const char* col;
        Tuple args;

        colored_arg_t(const char* first, Tuple&& second)
            : col(first), args(std::move(second)) { }

        friend std::ostream& operator<<(std::ostream& os, const colored_arg_t& argument) {
            auto impl = [&os](const auto&... args) {
                (os << ... << args);
            };

            os << argument.col;
            std::apply(impl, argument.args);
            return os << reset;
        }
    };

    template <class... Args>
    auto colored(const char* color, const Args&... args) {
        return colored_arg_t{ color, std::make_tuple(args...) };
    }
}

namespace net
{
    class session : public std::enable_shared_from_this<session>
    {
    public:
        session(boost::asio::ssl::stream<tcp::socket> socket)
            : socket_(std::move(socket)) { }

        ~session()
        {
            std::clog << "    " << colored(yellow, "0x", (intptr_t)this) << " : closing\n";
        }

        void start() { do_handshake(); }

    private:
        void do_handshake()
        {
            auto self(shared_from_this());

            std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : handshake\n";

            socket_.async_handshake(boost::asio::ssl::stream_base::server,
                [this, self](const boost::system::error_code &error)
                {
                    if (!error)
                    {
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : handshake " << colored(green, "succeeded") << "\n";
                        do_read();
                    }
                    else
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : handshake " << colored(red, "failed") << "\n";
                }
            );
        }

        void do_read()
        {
            auto self(shared_from_this());

            socket_.async_read_some(boost::asio::buffer(data_),
                [this, self](const boost::system::error_code &ec, std::size_t length)
                {
                    if (!ec)
                    {
                        std::ostringstream oss;
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : read " << colored(green, "succeeded") << "\n";
                        oss << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " :        ";
                        nstd::hexdump(data_, length, oss.str());

                        do_write(length);
                    }
                    else
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : read " << colored(red, "failed") << "\n";
                }
            );
        }

        void do_write(std::size_t length)
        {
            auto self(shared_from_this());
            boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](const boost::system::error_code &ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : write " << colored(green, "succeeded") << "\n";
                        do_read();
                    }
                    else
                        std::clog << "    " << colored(yellow, "0x", (intptr_t)self.get()) << " : write " << colored(red, "failed") << "\n";
                }
            );
        }

        boost::asio::ssl::stream<tcp::socket> socket_;
        char data_[1024];
    };


    class server
    {
    public:
        server(boost::asio::io_context &io_context, unsigned short port, std::string certfile, std::string privkey)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
            context_(boost::asio::ssl::context::sslv23)
        {
            auto options = boost::asio::ssl::context::default_workarounds
                        | boost::asio::ssl::context::no_sslv2;
                        //| boost::asio::ssl::context::single_dh_use;

            context_.set_options(options);
            context_.use_certificate_file(certfile, boost::asio::ssl::context::file_format::pem);
            //context_.use_certificate_chain_file("server.pem");
            context_.use_private_key_file(privkey, boost::asio::ssl::context::pem);
            //context_.use_tmp_dh_file("dh2048.pem");

            do_accept();
        }

    private:
        void do_accept()
        {
            std::clog <<  colored(light, "server") << ": " << colored(blue, "0x", (intptr_t)this) << " : listening\n";

            acceptor_.async_accept([this](const boost::system::error_code &error, tcp::socket socket) {
                if (!error)
                {
                    auto shared = std::make_shared<session>(boost::asio::ssl::stream<tcp::socket>(std::move(socket), context_));

                    std::clog << colored(light, "server") << ": " << colored(blue, "0x", (intptr_t)this)
                            << " : connection " << colored(green, "accepted") << " : "
                            << colored(yellow, "0x", (intptr_t)shared.get()) << "\n";

                    std::move(shared)->start();
                }
                else
                    std::clog <<  colored(light, "server") << ": " << colored(blue, "0x", (intptr_t)this) << " : connection " << colored(red, "rejected") << "\n";

                do_accept();
            });
        }

        tcp::acceptor acceptor_;
        boost::asio::ssl::context context_;
    };


    int run(int argc, char *argv[]);
}

