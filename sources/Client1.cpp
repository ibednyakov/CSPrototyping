/*
 * Client1.cpp
 *
 *  Created on: Apr 3, 2016
 *      Author: ivan
 */


#include "Client1.h"

#include <boost/bind.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


namespace DevBed {

Result::Code Client1::initialize(int argc, char* argv[]) {
	std::cout << "initializing..." << std::endl;
	if (argc < 3)
		throw ClientException("Not enough arguments!");
	_host_name = argv[1];
	_port = argv[2];

	_data.resize(ClientBufferSize);


	// Set a deadline for the connect operation.
	std::size_t timer_setup_result = _deadline.expires_from_now( boost::posix_time::seconds(10) );

	std::cout << "Initializing done! Server: " <<  _host_name << "  Port: " << _port << "  Timer setup: " << timer_setup_result << std::endl;
	return Result::Success;
}

Result::Code Client1::run() {
	std::cout << "running..." << std::endl;

	//boost::asio::ip::tcp::resolver::query query( host, service );
	//boost::asio::ip::tcp::resolver::iterator endpoint_iterator = _resolver.resolve(query);
	boost::asio::ip::tcp::endpoint endpoint /*= *endpoint_iterator*/;

	unsigned short int port = std::atoi( _port.c_str() );

	endpoint.address( boost::asio::ip::address::from_string(_host_name) );
	endpoint.port( port );

	std::cout << "Connecting to " << endpoint.address() << "  port: " << endpoint.port() << std::endl;


	boost::asio::ip::tcp::resolver::query query( /*boost::asio::ip::tcp::v6(), */_host_name, _port );
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
	        _resolver.resolve( query );
	endpoint = *endpoint_iterator;

	_state = State::Starting;

	_socket.async_connect( endpoint,
	        boost::bind(&Client1::handle_connect, this,
	            boost::asio::placeholders::error));

	_deadline.async_wait( boost::bind(&Client1::check_deadline, this,
	        boost::asio::placeholders::error) );

	std::cout << "Start waiting for deadline!" << std::endl;

//
//	while( _state != State::Terminating )
//	{
//		std::cout << "Waiting terminate! " << _socket.is_open() << std::endl;
//		/// NOTE: better - use ConditionalVariable!
//		//boost::unique_lock<boost::mutex> lock(_mutex);
//		//_cond.wait( _mutex );
//		sleep(20);
//	}

	std::cout << "Running completed!.." << std::endl;

	return Result::Success;
}

Result::Code Client1::terminate() {
	std::cout << "terminating..." << std::endl;
	_io_service.post([this]() { _socket.close(); });

	boost::unique_lock<boost::mutex> lock(_mutex);
	_state = State::Terminating;
	_cond.notify_all();
	_cond.wait( _mutex );

	return Result::Success;
}

void Client1::handle_connect( const boost::system::error_code& e )
{
	std::cout << "Handling connect!.. " << e.message() << std::endl;
	if ( e )
	{
		std::cerr << "Error: " << e.message() << std::endl;
		//boost::lock_guard<boost::mutex> lock( _mutex );
		terminate();
	}
	else
	{
		static const char* some_data = "Some data";

		_socket.async_read_some( boost::asio::buffer( _data.data(), _data.size() ),
				boost::bind( &Client1::handle_read, this,
		            boost::asio::placeholders::error, _2 ) );
		_socket.async_write_some( boost::asio::buffer( some_data, sizeof(some_data) ),
				boost::bind( &Client1::handle_write, this,
		            boost::asio::placeholders::error, _2 ) );
	}

}

void Client1::handle_write(const boost::system::error_code& e, std::size_t bytes_transferred)
{
	if ( e )
	{
		std::cerr << "Error writing!.. " << e.message() << std::endl;
		terminate();
	}
	else
	{
		std::cout << "Writing: " << e.message() << " count: " << bytes_transferred << std::endl;
	}
}

void Client1::handle_read( const boost::system::error_code& e, std::size_t bytes_transferred )
{
	if ( !e )
	{
		std::cout << "Reading " << bytes_transferred << " from socket" << std::endl;
		// TODO: process the _data of size bytes_transferred
		std::string received_string;
		received_string.assign( _data.begin(), _data.end() );
		std::cout << "Data: " << received_string << std::endl;
	}
	else
	{
		std::cerr << "Reading error occurred: " << e.message() << std::endl;
	}
}

void Client1::check_deadline( const boost::system::error_code& error )
{
	std::cout << "Checking deadline!.." << std::endl;

	terminate();

/*	if (stopped_)
		return;

	// Check whether the deadline has passed. We compare the deadline against
	// the current time since a new asynchronous operation may have moved the
	// deadline before this actor had a chance to run.
	if (deadline_.expires_at() <= deadline_timer::traits_type::now())
	{
		// The deadline has passed. The socket is closed so that any outstanding
		// asynchronous operations are cancelled.
		socket_.close();

		// There is no longer an active deadline. The expiry is set to positive
		// infinity so that the actor takes no action until a new deadline is set.
		deadline_.expires_at(boost::posix_time::pos_infin);
	}

	// Put the actor back to sleep.
	deadline_.async_wait(boost::bind(&client::check_deadline, this));*/
}

} // namespace


int main(int argc, char* argv[]) {
	// TODO Client-realization initializing flow
	// TODO Client's processing flow
	try {
		boost::asio::io_service io_service;
		DevBed::Client1 tcp_client(io_service);

		tcp_client.initialize(argc, argv);

		tcp_client.run();

		io_service.run();

		tcp_client.terminate();

	} catch(std::exception& e) {
		std::cerr << "exception! " << e.what() << std::endl;
		return -1;
	}
	std::cout << "done!" << std::endl;
	return 0;
}

