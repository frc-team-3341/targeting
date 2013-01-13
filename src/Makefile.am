#    This file is part of FRC Team 3341 Aimer.
#
#    FRC Team 3341 Aimer is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    FRC Team 3341 Aimer is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with FRC Team 3341 Aimer.  If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS = frc-team-3341-targeting
frc_team_3341_targeting_SOURCES = \
AppConfig.cpp \
Application.cpp \
CmdLineInterface.cpp \
Constants.cpp \
GUIManager.cpp \
HSVImage.cpp \
NetworkController.cpp \
Rectangle.cpp \
RectangleDetector.cpp \
RectangleProcessor.cpp \
VideoDevice.cpp \
main.cpp \
AppConfig.hpp \
Application.hpp \
CmdLineInterface.hpp \
Constants.hpp \
GUIManager.hpp \
HSVImage.hpp \
NetworkController.hpp \
Rectangle.hpp \
RectangleDetector.hpp \
RectangleProcessor.hpp \
VideoDevice.hpp
frc_team_3341_targeting_CXXFLAGS = $(AM_CXXFLAGS) $(BOOST_CPPFLAGS) -std=gnu++0x -Wall -Werror
frc_team_3341_targeting_CPPFLAGS = $(AM_CPPFLAGS) $(OPENCV_CFLAGS)
frc_team_3341_targeting_LDFLAGS = $(LDFLAGS) $(BOOST_LDFLAGS) $(BOOST_ASIO_LDFLAGS) -pthread -lboost_system -lboost_thread
frc_team_3341_targeting_LDADD = $(LDADD) $(OPENCV_LIBS)
frc_team_3341_targeting_LIBS = $(BOOST_ASIO_LIBS)
