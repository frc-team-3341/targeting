#! /bin/bash -f

cd ../experimental
rm -f experiment
for imagename in `ls`
do
    cp $imagename experiment
    cd ../test
    ./squares
    echo $imagename
    cd ../experimental
done
