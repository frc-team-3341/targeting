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
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <thread>
#include <memory>
#include <getopt.h>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/lexical_cast.hpp>

#include "Constants.hpp"
#include "VideoDevice.hpp"
#include "Rectangle.hpp"
#include "RectangleDetector.hpp"
#include "RectangleProcessor.hpp"
#include "GUIManager.hpp"
#include "CmdLineInterface.hpp"
#include "Application.hpp"

Application::Application(int argc, char *argv[])
{
	constList = new Constants();
	
	cmdLineInterface = new CmdLineInterface(argc, argv);
	config = cmdLineInterface->getConfig();
	delete cmdLineInterface;
}

Application::~Application()
{
	if (!config.getIsHeadless())
		delete guiManager;
	if (config.getIsNetworking())
		delete networkController;
	if (config.getIsDevice())
		delete videoDevice;
	delete constList;
}

int Application::exec()
{
	initVideoDevice();
	initNetworking();
	initGUI();
	targetingInit();
	while (true)
		targetingContinuous();

	return EXIT_SUCCESS;
}

void Application::initVideoDevice()
{	
	if (config.getIsDevice()) {
		videoDevice = new VideoDevice(constList);
		videoDevice->startCapture(config.getDeviceID());
	}
}

void Application::initNetworking()
{
	if (config.getIsNetworking()) {
		networkController = new NetworkController(constList);
		networkController->startServer();
	}
}

void Application::initGUI()
{
	if (! config.getIsHeadless()) {
		guiManager = new GUIManager(constList);
		guiManager->init();
	}
}

void Application::targetingInit()
{
	/*if (! config.getIsHeadless()) {
		cv::Mat image = loadImage();
		std::cout << "Image resolution: " << image.cols << "x" << image.rows << std::endl;
		}*/
}

void Application::targetingContinuous()
{
	if (config.getIsNetworking())
		networkController->waitForPing();

	cv::Mat image = loadImage();
	if (! config.getIsHeadless())
		guiManager->setImage(image);

	RectangleDetector rectDetector(constList);
	Rectangle foundRectangle = rectDetector.processImage(image);
	RectangleProcessor rectProcessor(constList);
	if (rectDetector.rectangleWasFound()) {
		rectProcessor.processRectangle(foundRectangle);

		int distance = rectProcessor.getDistance();
		int horizontalDistance = rectProcessor.getHorizontalDistance();
		float azimuth = rectProcessor.getAzimuth() * 180.0 / constList->mathPi;
		float velocity = rectProcessor.getVelocity();
		float elevation = rectProcessor.getElevation() * 180.0 / constList->mathPi;
		int height = rectProcessor.getHeight();
		float tilt = rectProcessor.getTilt() * 180.0 / constList->mathPi;
		float rpm = rectProcessor.getRPM();
		
		std::cout << "Distance: " << distance << "mm" << std::endl;
		std::cout << "Horizontal Distance: " << horizontalDistance << " mm" << std::endl;
		std::cout << "Height: " << height << " mm" << std::endl;
		std::cout << "Velocity: " << velocity << " m/s" << std::endl;
		std::cout << "Azimuth: " << azimuth << " degrees" << std::endl;
		std::cout << "Tilt: " << tilt << " degrees" << std::endl;
		std::cout << "Elevation: " << elevation << " degrees" << std::endl;
		std::cout << "RPM: " << rpm << " RPM" << std::endl;

		if (config.getIsNetworking())
			networkController->sendMessage(boost::lexical_cast<std::string>(rectProcessor.getRPM()) + std::string(";") + boost::lexical_cast<std::string>(rectProcessor.getAzimuth()) + std::string(";") + boost::lexical_cast<std::string>(rectProcessor.getTilt()));
	} else {
		std::cout << "No rectangle" << std::endl;
		
		if (config.getIsNetworking())
			networkController->sendMessage("No rectangle");
	}

	if (! config.getIsHeadless()) {
		std::string message;
		if (rectDetector.rectangleWasFound())
			message = boost::lexical_cast<std::string>(rectProcessor.getDistance()) + " mm @ " + boost::lexical_cast<std::string>(rectProcessor.getAzimuth()) + " degrees";
		else
			message = "No rectangle";
		guiManager->setImageText(message);
		guiManager->show(rectDetector.getAllRectangles(), rectDetector.getFinalRectangles());
		int keycode = cv::waitKey(10);
		if (keycode == 57) {
			std::stringstream filename;
			filename << time(NULL) << ".jpg";
			cv::imwrite(filename.str().c_str(), image);
		} else if (keycode == 27)
			exit(EXIT_SUCCESS);
		if (config.getIsFile()) {
			cv::waitKey();
			exit(EXIT_SUCCESS);
		}
	} else {
		char waitForKey;
		std::cin >> waitForKey;
		exit(EXIT_SUCCESS);
	}
}

cv::Mat Application::loadImage()
{
	cv::Mat returnVal;

	if (config.getIsFile())
		cv::imread(config.getFileName().c_str()).copyTo(returnVal);
	else if (config.getIsDevice())
		returnVal = videoDevice->getImage();
	else
		exit(1);

	constList->imgCols = returnVal.cols;
	constList->imgRows = returnVal.rows;

	return returnVal;
}
