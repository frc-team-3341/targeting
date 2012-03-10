/*
 *    This file is part of FRC Team 3341 Aimer.
 *
 *    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VideoDevice_hpp
#define VideoDevice_hpp

class VideoDevice {
public:
  // Methods
  VideoDevice(Constants /* Constants List */);
  void startCapture(int /* Device ID */, int /* Is HD */);
  cv::Mat getImage();
  ~VideoDevice();

private:
  // Methods
  void initCamera(int /* Device ID */, int /* Is HD */);
  void captureFromCamera();

  // Data
  Constants constList;
  std::thread *captureThread;
  cv::VideoCapture camera;
  cv::Mat image;
  int isFinished;
  int isReady;
};

#endif /* VideoDevice_hpp */
