/*
 * AsioUdpClient.cpp
 *
 *  Created on: May 1, 2016
 *      Author: ivan
 */


#include "AsioUdpClient.h"

namespace DevBed
{

/*virtual*/ Result::Code AsioUdpClient::initialize(int argc, char* argv[])
{
	return Result::Success;
}

/*virtual*/ Result::Code AsioUdpClient::run()
{
	return Result::Success;
}

/*virtual*/ Result::Code AsioUdpClient::terminate()
{
	return Result::Success;
}


} // namespace DevBed
