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
CRIOLink.cpp \
CRIOLink.hpp \
Constants.cpp \
Constants.hpp \
GUIManager.cpp \
GUIManager.hpp \
HSVImage.cpp \
HSVImage.hpp \
Rectangle.cpp \
Rectangle.hpp \
RectangleDetector.cpp \
RectangleDetector.hpp \
RectangleProcessor.cpp \
RectangleProcessor.hpp \
VideoDevice.cpp \
VideoDevice.hpp \
main.cpp
frc_team_3341_targeting_CXXFLAGS = $(AM_CXXFLAGS) -std=gnu++0x `pkg-config --libs --cflags opencv`
frc_team_3341_targeting_CPPFLAGS = $(AM_CPPFLAGS)
frc_team_3341_targeting_LDFLAGS = $(LDFLAGS)
frc_team_3341_targeting_LDADD = $(LDADD)
