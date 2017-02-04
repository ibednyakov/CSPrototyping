/*
 * Client1.h
 *
 *  Created on: Apr 3, 2016
 *      Author: ivan
 */

#ifndef SOURCES_CLIENT1_H_
#define SOURCES_CLIENT1_H_

#include "IClient.h"

#include <iostream>
#include <istream>
#include <ostream>

#include <vector>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>

namespace DevBed
{

	enum {
		ClientBufferSize = 1024,
	};

	class Client1 : public IClient {
	public:
		virtual Result::Code initialize(int argc, char* argv[]);
		virtual Result::Code run();
		virtual Result::Code terminate();

		Client1( boost::asio::io_service& io_service )
			: _io_service( io_service )
			, _socket(io_service)
			, _resolver(io_service)
			, _deadline(io_service)
			, _heartbeat_timer(_io_service)
			, _state(State::NotStarted)
		{}

	protected:
	private:
		std::string _host_name;
		std::string _port;

		boost::asio::io_service& _io_service;
		boost::asio::ip::tcp::socket _socket;
		boost::asio::streambuf _request;
		boost::asio::streambuf _response;

		boost::asio::ip::tcp::resolver _resolver;

		boost::asio::deadline_timer _deadline;
		boost::asio::deadline_timer _heartbeat_timer;

		uint32_t _state;
		mutable boost::mutex _mutex;

		boost::condition_variable_any _cond;

		std::vector<unsigned char> _data;

		void handle_connect( const boost::system::error_code& e );
		void handle_read( const boost::system::error_code& e, std::size_t bytes_transferred );
		void handle_write( const boost::system::error_code& e, std::size_t bytes_transferred );
		void check_deadline( const boost::system::error_code& error );

	};

} // DevBed

#endif /* SOURCES_CLIENT1_H_ */
