/*
 * sender.cpp
 *
 *  Created on: 23 февр. 2016 г.
 *      Author: pavelzelenov
 */



#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

#include "tracker.hpp"

const short UE_port = 30001;

#define PI 3.14159265

class sender
{
public:
	sender(boost::asio::io_service& io_service,
		const boost::asio::ip::address& UE_address)
		: endpoint_(UE_address, UE_port),
		  socket_(io_service, endpoint_.protocol()),
		  timer_(io_service),
		  mc(0),
		  round(0)
	{
		socket_.set_option(boost::asio::socket_base::broadcast(true));
	}
	void add(tracker& t)
	{
		trackers[t.get_color()] = t;
	}

	void send()
	{
		std::ostringstream os;
		os << "{\n  \"general\" : {\n    \"timestamp\" : "
				<< std::chrono::high_resolution_clock::now().time_since_epoch().count() ;

		for(auto tr = trackers.begin(); tr != trackers.end(); tr++)
		{
			auto t = &tr->second;
			os  << "\n  },"
				<< "\n  \"" << t->get_color() <<"\" : { "
				<< "\n    \"a\": " << t->X() << ","
				<< "\n    \"x\": " << t->X() << ","
				<< "\n    \"y\": " << t->Y()
				<< "\n  ";
		}

		os << "}\n}$\n";
		message_ = os.str();

		if ( mc++ >= 1000 ) {
			mc = 0;
			round++;
			std::cerr << "Round: " << round << "\n";
		}

		socket_.async_send_to(
				boost::asio::buffer(message_), endpoint_,
				[this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/){}
			);
	}

private:
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::deadline_timer timer_;
	int mc;
	int round;
	std::string message_;
	std::map<std::string, tracker> trackers;
};
