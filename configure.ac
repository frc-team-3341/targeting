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

AC_PREREQ([2.65])
AC_INIT([FRC Team 3341 Aimer], [2012.3], [software@wvrobotics.com], [frc-team-3341-aimer], [http://wvrobotics.com/])
AM_INIT_AUTOMAKE([dist-bzip2])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])

# Set Environment Variables
#CFLAGS="-g -pipe"
CFLAGS="-march=native -O2 -pipe"
CXXFLAGS="${CFLAGS}"

# Check For Compiler
AC_PROG_CXX

# Check for Installer
AC_PROG_INSTALL

# Check for Headers
AC_CHECK_HEADERS([sys/socket.h netinet/in.h netdb.h getopt.h opencv2/core/core.hpp opencv2/imgproc/imgproc.hpp opencv2/highgui/highgui.hpp])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
