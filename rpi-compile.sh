#!/bin/bash

startdir="`pwd`"
make distcheck -j9
cd /tmp
rm -rf crosscomp
mkdir crosscomp
cd crosscomp
cp ${startdir}/frc-team-3341-targeting-1.0.tar.xz .
tar xJf frc-team-3341-targeting-1.0.tar.xz
cd frc-team-3341-targeting-1.0
./configure
cd src
for f in *.cpp
do
    armv6j-hardfloat-linux-gnueabi-g++ -I. -I/usr/include -std=gnu++0x -c $f -o "frc_team_3341_targeting-`basename $f .cpp`.o"
done
cd ../..
mv frc-team-3341-targeting-1.0 targeting-rpi
tar cJf targeting-rpi.tar.xz targeting-rpi
cd $startdir
mv /tmp/crosscomp/targeting-rpi.tar.xz .
rm frc-team-3341-targeting-1.0.tar.gz frc-team-3341-targeting-1.0.tar.xz
