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

#ifndef NetworkController_hpp
#define NetworkController_hpp

#include <boost/asio.hpp>

#include "Constants.hpp"

class NetworkController
{
public:
	NetworkController(Constants *constList);
	~NetworkController();
	void startServer();
	void waitForPing();
	void sendMessage(std::string message);

private:
	int portNumber;
	boost::asio::io_service *io_service;
	boost::asio::ip::tcp::acceptor *acceptor;
	boost::asio::ip::tcp::socket *socket;
	boost::system::error_code ignored_error;
	boost::system::error_code error;
};

#endif /* NetworkController_hpp  */
