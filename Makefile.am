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

AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

bin_PROGRAMS = frc-team-3341-aimer
frc_team_3341_aimer_LDFLAGS = `pkg-config --cflags --libs opencv`
frc_team_3341_aimer_CXXFLAGS = -Wall -std=gnu++0x
frc_team_3341_aimer_SOURCES = src/Constants.cpp src/Constants.hpp src/CRIOLink.cpp src/CRIOLink.hpp src/HSVImage.cpp src/HSVImage.hpp src/main.cpp src/Rectangle.cpp src/Rectangle.hpp src/RectangleProcessor.cpp src/RectangleProcessor.hpp src/RectangleDetector.cpp src/RectangleDetector.hpp src/VideoDevice.cpp src/VideoDevice.hpp

dist_noinst_SCRIPTS = autogen.sh
dist_noinst_DATA = images/
