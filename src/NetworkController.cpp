/*
 *    Copyright 2012, 2013, 2014 Tushar Pankaj
 *
 *    This file is part of FRC Team 3341 Targeting.
 *
 *    FRC Team 3341 Targeting is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Targeting is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Targeting.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#include <boost/asio.hpp>

#include "Constants.hpp"
#include "NetworkController.hpp"

NetworkController::NetworkController(Constants *constList)
{
	portNumber = constList->portNumber;
}

NetworkController::~NetworkController()
{
	delete socket;
	delete acceptor;
	delete io_service;
}

void NetworkController::startServer()
{
	try {
		io_service = new boost::asio::io_service();
		acceptor = new boost::asio::ip::tcp::acceptor(*io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), portNumber));
		socket = new boost::asio::ip::tcp::socket(*io_service);
		acceptor->accept(*socket);
		boost::asio::socket_base::keep_alive option(true);
		socket->set_option(option);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

void NetworkController::waitForPing()
{
	std::vector<char> buffer(256);
	socket->read_some(boost::asio::buffer(buffer), error);
	if (error == boost::asio::error::eof)
		exit(EXIT_SUCCESS);
	else if (error)
		throw boost::system::system_error(error);
}

void NetworkController::sendMessage(std::string message)
{
	boost::asio::write(*socket, boost::asio::buffer(message + std::string("\n")), ignored_error);
}
