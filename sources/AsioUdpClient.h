/*
 * AsioUdpClient.h
 *
 *  Created on: May 1, 2016
 *      Author: ivan
 */

#ifndef SOURCES_ASIOUDPCLIENT_H_
#define SOURCES_ASIOUDPCLIENT_H_

#include "IClient.h"

#include <iostream>
#include <istream>
#include <ostream>

#include <vector>

#include <boost/thread.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>

namespace DevBed
{

class AsioUdpClient : public IClient
{
public:
	virtual Result::Code initialize(int argc, char* argv[]);
	virtual Result::Code run();
	virtual Result::Code terminate();

	AsioUdpClient(boost::asio::io_service& io_service)
		: _socket( io_service )
		, _resolver( io_service )
	{}

private:
	//boost::asio::io_service _io_service;

	boost::asio::ip::udp::socket _socket;
	boost::asio::streambuf _request;
	boost::asio::streambuf _response;

	boost::asio::ip::udp::resolver _resolver;
};

}


#endif /* SOURCES_ASIOUDPCLIENT_H_ */
