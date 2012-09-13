#    This file is part of FRC Team 3341 Targeting.
#
#    FRC Team 3341 Targeting is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    FRC Team 3341 Targeting is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with FRC Team 3341 Targeting.  If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.65])
AC_INIT([FRC Team 3341 Targeting], [1.0], [software@wvrobotics.com], [frc-team-3341-targeting], [http://wvrobotics.com/])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([dist-xz])

AC_LANG([C++])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

PKG_CHECK_MODULES(OPENCV, opencv >= 2.2)
AC_SUBST(OPENCV_CFLAGS)
AC_SUBST(OPENCV_LIBS)

BOOST_REQUIRE([1.34.1])
BOOST_ASIO

AC_CHECK_HEADERS([sys/socket.h netinet/in.h netdb.h getopt.h])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
