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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

#include "RectangleProcessor.hpp"
#include "Constants.hpp"
#include "MultiRectangleProcessor.hpp"

MultiRectangleProcessor::MultiRectangleProcessor(Constants* inputConstList)
{
	constList = inputConstList;
	finalTarget = -1;
}

MultiRectangleProcessor::~MultiRectangleProcessor()
{
	if (finalProcessor)
		delete finalProcessor;
}

void MultiRectangleProcessor::processRectangles(std::vector<Rectangle> inputRectangles)
{
	rectangles = inputRectangles;

	for (int i = 0; i < (int)rectangles.size(); i++) {
		std::vector<RectangleProcessor> mRectProcessor(2, RectangleProcessor(constList));
		mRectProcessor.at(0).processRectangle(rectangles.at(i), constList->targetHighIndex);
		mRectProcessor.at(1).processRectangle(rectangles.at(i), constList->targetMiddleIndex);
		rectProcessors.push_back(mRectProcessor);
	}

	if (rectProcessors.size() == 1) {
		if (rectProcessors.at(0).at(0).getLogAspectRatios() < rectProcessors.at(0).at(1).getLogAspectRatios()) {
			finalProcessor = new RectangleProcessor(rectProcessors.at(0).at(0));
			finalTarget = 0;
		} else {
			finalProcessor = new RectangleProcessor(rectProcessors.at(0).at(1));
			finalTarget = 1;
		}
	}
	else if (rectProcessors.size() > 1) {
		int processIndex = 0;
		for (int i = 0; i < (int)rectProcessors.size(); i++) {
			if (rectProcessors.at(i).at(0).getElevation() > rectProcessors.at(processIndex).at(0).getElevation())
				processIndex = i;
		}
		finalProcessor = new RectangleProcessor(rectProcessors.at(processIndex).at(0));
		finalTarget = 0;
	}

	if (finalProcessor) {
		std::cout << "Final Target Information" << std::endl;
		std::cout << "========================" << std::endl;
		std::cout << "Target: ";
		if (finalTarget == 0)
			std::cout << "High target" << std::endl;
		else
			std::cout << "Middle target" << std::endl;
		std::cout << "Distance: " << finalProcessor->getProportionalDistance() << " mm" << std::endl;
		std::cout << "Horizontal distance: " << finalProcessor->getHorizontalDistance() << " mm" << std::endl;
		std::cout << "Azimuth: " << finalProcessor->getAzimuth() << " radians" << std::endl;
		std::cout << std::endl;
	}
}

std::vector<std::vector<RectangleProcessor> > MultiRectangleProcessor::getRectProcessors()
{
	return rectProcessors;
}

RectangleProcessor* MultiRectangleProcessor::getFinalProcessor()
{
	return finalProcessor;
}
