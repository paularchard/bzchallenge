// Copyright (C) 2018 Paul Archard
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License, version 3,
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <url_reader.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>

static const int HTTP_VERSION = 11; // HTTP/1.1
static const std::string VERSION_STRING = "UrlReader/1.0";

url_reader::url_reader(const std::string &url)
: full_url(url), path("/"), port("443")
{
    // extract the url components
    auto proto_offset = url.find("://");
    if (proto_offset != std::string::npos)
    {
        if (url.substr(0, proto_offset) != "https")
            throw std::invalid_argument("invalid argument");
    }

    auto host_offset = (proto_offset == std::string::npos) ? 0 : proto_offset + 3;
    auto port_offset = url.find(':', host_offset);
    auto path_offset = url.find('/', host_offset);

    this->hostname = url.substr(host_offset, (std::min(port_offset, path_offset) - host_offset));
    if (this->hostname.empty())
            throw std::invalid_argument("invalid argument");
 
    if (port_offset != std::string::npos)
        this->port = url.substr(port_offset + 1, path_offset - (port_offset + 1));

    if (path_offset != std::string::npos)
        this->path = url.substr(path_offset);
}

bool 
url_reader::read(void)
{
    auto result = true;
    using tcp = boost::asio::ip::tcp;
    namespace http = boost::beast::http;
    namespace ssl = boost::asio::ssl;

    // initialize the ASIO and SSL contexts
    boost::asio::io_context ioc;
    ssl::context ctx{ssl::context::sslv23_client};
    ssl::stream<tcp::socket> ssl_stream{ioc, ctx};

    try
    {
        // Set SNI Hostname
        if (!SSL_set_tlsext_host_name(ssl_stream.native_handle(), hostname.c_str()))
        {
            std::cerr << "Error setting TLS hostname" << std::endl;
            return false;
        }

        // resolve the hostname and connect
        tcp::resolver resolver{ioc};
        auto const results = resolver.resolve(this->hostname, this->port);
        boost::asio::connect(ssl_stream.next_layer(), results.begin(), results.end());
        ssl_stream.handshake(ssl::stream_base::client);

        // set up and send request
        http::request<http::string_body> req{http::verb::get, this->path, HTTP_VERSION};
        req.set(http::field::host, this->hostname);
        req.set(http::field::user_agent, VERSION_STRING);
        http::write(ssl_stream, req);

        // read the response
        boost::beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(ssl_stream, buffer, res);
        if (res.result_int() == 200)
        {
            // store the response in our internal data
            this->data = std::move(res.body());
        }
        else
        {
            std::cout << "HTTPS request returned " << res.result_int() << std::endl;
            result = false;
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "An error occurred retrieving url \"" << this->full_url 
            << "\": " << e.what() << std::endl;

        result = false;
    }

    // close the stream (safe even if never connected)
    boost::system::error_code ec;
    ssl_stream.shutdown(ec);

    return result;
}
