/*
 * sender.cpp
 *
 *  Created on: 23 февр. 2016 г.
 *      Author: pavelzelenov
 */

#ifndef SRC_SENDER_HPP_
#define SRC_SENDER_HPP_

#include <chrono>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"

class sender
{
public:
	sender(boost::asio::io_service& io_service,
		const boost::asio::ip::address& UE_address)
		: endpoint_(UE_address, 30001),
		  socket_(io_service, endpoint_.protocol()),
		  timer_(io_service)
	{
		socket_.set_option(boost::asio::socket_base::broadcast(true));
	}

	void send(std::string s)
	{
		socket_.async_send_to(
				boost::asio::buffer(s), endpoint_,
//				[this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/){}
				[&](
				        const boost::system::error_code& error,
				        std::size_t bytes_transferred)
				      {
				        ;
				      }
		);
	}

private:
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::deadline_timer timer_;
	std::string message_;
};
#endif /* SRC_SENDER_HPP_ */
