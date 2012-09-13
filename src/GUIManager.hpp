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

#ifndef GUIManager_hpp
#define GUIManager_hpp

class GUIManager
{
public:
    // Methods
    GUIManager(Constants* /* Constants List */); // Constructor
    void init(); // Initialize GUI
    void show(cv::Mat /* Image to Display */); // Show Image

private:
    // Data
    Constants constList;

    // Methods
};

#endif /* GUIManager_hpp */
