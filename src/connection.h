//
// connection.h
// Project Spitfire
//
// Copyright (c) 2013 Daizee (rensiadz at gmail dot com)
//
// This file is part of Spitfire.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef HTTP_CONNECTION_HPP
#define HTTP_CONNECTION_HPP

#define MAXPACKETSIZE 32768

#include <asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "reply.h"
#include "request.h"
#include "request_handler.h"

class Server;
class Client;

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>,
    private boost::noncopyable
{
public:
  /// Construct a connection with the given io_service.
  explicit connection(asio::io_service& io_service,
      Server& gserver, request_handler& handler);

  /// Get the socket associated with the connection.
  asio::ip::tcp::socket& socket();

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();

  void write(const char * data, const int32_t size);

private:
  /// Handle completion of a read operation.
  void handle_read_header(const asio::error_code& e,
      std::size_t bytes_transferred);
  void handle_read(const asio::error_code& e,
	  std::size_t bytes_transferred);
  /// Handle completion of a write operation.
  void handle_write(const asio::error_code& e);

  /// Socket for the connection.
  asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  Server& server;

  /// The handler used to process the incoming request.
  request_handler& request_handler_;

  /// Buffer for incoming data.
  boost::array<char, MAXPACKETSIZE> buffer_;

  /// The incoming request.
  request request_;

  /// The reply to be sent back to the client.
  reply reply_;

  int32_t size;

public:
  uint64_t uid;

  Client * client_;

  string address;
};

typedef boost::shared_ptr<connection> connection_ptr;

#endif // HTTP_CONNECTION_HPP
