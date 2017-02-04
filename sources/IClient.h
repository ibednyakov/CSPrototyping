/*
 * IClient.h
 *
 *  Created on: Apr 4, 2016
 *      Author: ivan
 */

#ifndef SOURCES_ICLIENT_H_
#define SOURCES_ICLIENT_H_

#include <exception>
#include <string>

namespace DevBed
{

	namespace Result
	{
		enum Code {
			Success = 0,
			GenericFailure = 1,
		};
	}

	namespace State
	{
		enum Code {
			NotStarted = 0,
			Starting,
			Processing,
			Suspended,
			Terminating,
		};
	}


	class ClientException : public std::exception
	{
		std::string _description;
	public:
		explicit ClientException( const char* desc ) : _description( desc ) {}
		virtual ~ClientException() throw () {}
		virtual const char* what() const throw () {
			return _description.c_str();
		}
	};

	///
	/// Declares Template-pattern interface for Client-asio approach
	///
	class IClient {
	public:
		virtual ~IClient() {}

		///
		/// \brief Perform initializing of a Client
		///
		virtual Result::Code initialize(int argc, char* argv[]) =0;

		///
		/// \brief Runs Client execution thread
		///
		virtual Result::Code run() =0;

		///
		/// \brief Performs terminating actions for outside calls
		///
		virtual Result::Code terminate() =0;
	};
}

#endif /* SOURCES_ICLIENT_H_ */
