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

#include "Constants.hpp"
#include "MultiRectangleProcessor.hpp"

MultiRectangleProcessor::MultiRectangleProcessor(Constants* inputConstList)
{
	constList = inputConstList;
  foundFinal = false;
}

MultiRectangleProcessor::~MultiRectangleProcessor()
{
	if (finalRectangle)
		delete finalRectangle;
}

void MultiRectangleProcessor::processRectangles(std::vector<Rectangle> inputRectangles)
{
  std::cout << "Processing remaining rectangles..." << std::endl;
  foundFinal = false;
  rectList = inputRectangles;
  horizontalRectangleList.clear();
  findHorizontalRectangles();

  std::cout << "Identifying whether horizontal Rectangle exists" << std::endl;
  if(horizontalRectangleList.size() == 0){
    std::cout << "No Horizontal Rectangle found" << std::endl;
  }
  if(horizontalRectangleList.size() > 0){
    std::cout << "Horizontal Rectangle found" << std::endl; 
  }


  /*std::cout << "Picking out correct vertical Rectangle..." << std::endl;
  if(verticalRectangleList.size() == 0){
        std::cout << "No Vertical Rectangles Found" << std::endl;
        std::cout << "Horizontal Rectangle List Size: " << horizontalRectangleList.size() << std::endl;
        if(horizontalRectangleList.size() > (0))
            horizontalRectangleList.at(0).to_string();
   }

  else if (verticalRectangleList.size() == 1) {
            std::cout << std::endl << std::endl;
            std::cout << "Vertical Rectangle: " << std::endl;
            verticalRectangleList.at(0).to_string();
            if(verticalRectangleList.at(0).pairedHorizontalRectangles.size() > 0){
                std::cout << "Horizontal Rectangle: " << std::endl;
                horizontalRectangleList.at(verticalRectangleList.at(0).pairedHorizontalRectangles.at(0)).to_string();
            }
            else{
                std::cout << "No Matching Horizontal Rectangle Found" << std::endl;
            }
            finalRectangle = &verticalRectangleList.at(0);
            foundFinal = true;
        }

  else if (verticalRectangleList.size() == 2){
        std::cout << "Two vertical rectangles found assuming seeing right and left sides of field. Returning side with matching horizontal if any found." << std::endl; 
        std::cout << std::endl << std::endl;
            std::cout << "Vertical Rectangle: " << std::endl;
            //verticalRectangleList.at(0).to_string();
            if(verticalRectangleList.at(0).pairedHorizontalRectangles.size() > 0){
                std::cout << "Horizontal Rectangle: " << std::endl;
                horizontalRectangleList.at(verticalRectangleList.at(0).pairedHorizontalRectangles.at(0)).to_string();
            }
            else if(verticalRectangleList.at(1).pairedHorizontalRectangles.size() > 0){
                verticalRectangleList.at(1).to_string();

                if(verticalRectangleList.at(1).pairedHorizontalRectangles.size() > 1){
                    std::cout << "Horizontal Rectangle: " << std::endl;
                    horizontalRectangleList.at(verticalRectangleList.at(0).pairedHorizontalRectangles.at(0)).to_string();
                }
                else{
                    std::cout << "No Horizontal Rectangles Found" << std::endl;
                }
            }
            finalRectangle = &verticalRectangleList.at(0);
            foundFinal = true;
  }
  else {
      std::cout << "Multiple(3+) Vertical Rectangles Found" << std::endl;
  }
  */
  /*
     else if (rectProcessors.size() > 1) {
     int processIndex = 0;
     for (int i = 0; i < (int)rectProcessors.size(); i++) {
     if (rectProcessors.at(i).at(0).getElevation() > rectProcessors.at(processIndex).at(0).getElevation())
     processIndex = i;
     }
     finalProcessor = new RectangleProcessor(rectProcessors.at(processIndex).at(0));
     finalTarget = 0;
     }
     */
}

void MultiRectangleProcessor::findVerticalRectangles(){
    for (unsigned i = 0; i < rectList.size(); ++i) {
        if (rectList.at(i).lengthSquaredLeft > rectList.at(i).lengthSquaredTop){
            verticalRectangleList.push_back(rectList.at(i));
            rectList.at(i).setRectType(constList -> targetVertical);
        }
    }
}

void MultiRectangleProcessor::findHorizontalRectangles(){
    for (unsigned i = 0; i < rectList.size(); ++i) {
        if (rectList.at(i).lengthSquaredTop > rectList.at(i).lengthSquaredLeft){
            horizontalRectangleList.push_back(rectList.at(i));
            if(rectList.at(i).area < 900 && rectList.at(i).area > 600)
                rectList.at(i).setRectType(constList -> targetHorizontal);
        }
    }
}

void MultiRectangleProcessor::findVHPairs(){

    std::vector<Rectangle> outputRectangles;

    for (unsigned i = 0; i < verticalRectangleList.size(); ++i) {
        for (unsigned j = 0; j < horizontalRectangleList.size(); ++j) {
            if (rectanglePairMatches(verticalRectangleList.at(i), horizontalRectangleList.at(j))){
                verticalRectangleList.at(i).pairedHorizontalRectangles.push_back(j);
                outputRectangles.push_back(verticalRectangleList.at(i));
                outputRectangles.push_back(horizontalRectangleList.at(j));
            }
        }
    }
    /*
       for (int i = 0; i < (int)verticalRectangleList.size(); ++i) {

//verticalRectangleList.at(i).to_string();

if ( verticalRectangleList.at(i).pairedHorizontalRectangles.size() > 0){
//horizontalRectangleList.at(verticalRectangleList.at(i).pairedHorizontalRectangles.at(0)).to_string();
}
else{
std::cout << "No Matching Horizontal Rectangle found" << std::endl;
}

}
*/
}

void MultiRectangleProcessor::classifyVHPairs(){
    for( unsigned i=0; i < verticalRectangleList.size(); ++i){
       if(verticalRectangleList.at(i).pairedHorizontalRectangles.size() > 0){
            verticalRectangleList.at(i).rectangleSide = classifyVHPair(verticalRectangleList.at(i), horizontalRectangleList.at(verticalRectangleList.at(i).pairedHorizontalRectangles.at(0)));
            horizontalRectangleList.at(verticalRectangleList.at(i).pairedHorizontalRectangles.at(0)).rectangleSide = classifyVHPair(verticalRectangleList.at(i), horizontalRectangleList.at(verticalRectangleList.at(i).pairedHorizontalRectangles.at(0)));
        }
    }

}

int MultiRectangleProcessor::classifyVHPair(Rectangle vertical, Rectangle horizontal){
    if(vertical.topLeft.x < horizontal.topLeft.x){
        return 1;
        //Right goal
    }

    else if(vertical.topLeft.x > horizontal.topLeft.x){
        return 2;
        //Left goal
    }
    
    else{
        return 0;
    }
}

bool MultiRectangleProcessor::rectanglePairMatches(Rectangle vertical, Rectangle horizontal)
{
    double toleranceDistance = 900;  
    if(horizontal.topLeft.y > vertical.topLeft.y){
        return false;
    }

    if (horizontal.topLeft.x > vertical.topLeft.x - toleranceDistance &&
            horizontal.topLeft.x < vertical.topLeft.x){
        return true;
    }

    if (horizontal.topRight.x < vertical.topRight.x + toleranceDistance &&
            horizontal.topRight.x > vertical.topRight.x){ 
        return true;
    }
    return false;

}

Rectangle* MultiRectangleProcessor::getFinalRectangle()
{
    return finalRectangle;
}

void MultiRectangleProcessor::printFinalRectangleInformation()
{
    std::cout << "Final Rectangles Information" << std::endl;
    std::cout << "========================" << std::endl;

    if(foundFinal)
        getFinalRectangle()->to_string();
    else
        std::cout << "No Rectangle Found" << std::endl;
}


