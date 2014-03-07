/*
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

#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <boost/lexical_cast.hpp>

#include "AppConfig.hpp"
#include "CmdLineInterface.hpp"

CmdLineInterface::CmdLineInterface(int argc, char *argv[])
{
	int isHeadless = 0;
	int isNetworking = 0;
	int isCaptureMode = 0;

	while (true) {
		static struct option long_options[] = {
			{"headless", no_argument, &isHeadless, 1},
			{"no-networking", no_argument, &isNetworking, 0},
			{"capture-mode", no_argument, &isCaptureMode, 1},
			{"device", required_argument, 0, 'd'},
			{"file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

		int option_index = 0;
		int c = getopt_long(argc, argv, "d:f:", long_options, &option_index);

		if (c == -1) // End of Options
			break;

		switch (c) {
		case 0:
			break;
		case 'd':
			config.setDeviceID(boost::lexical_cast<int>(optarg));
			config.setIsDevice(1);
			break;
		case 'f':
			config.setFileName(boost::lexical_cast<std::string>(optarg));
			config.setIsFile(1);
			break;
		case '?':
			exit(127);
			break;
		default:
			abort();
		}
	}

	config.setIsHeadless(isHeadless);
	config.setIsNetworking(isNetworking);
	config.setIsCaptureMode(isCaptureMode);

	if (! config.getIsFile() && ! config.getIsDevice()) {
		std::cerr << argv[0] << ": missing required options" << std::endl;
		exit(127);
	}
}

AppConfig CmdLineInterface::getConfig()
{
	return config;
}
