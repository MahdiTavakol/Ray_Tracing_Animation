#!/bin/bash

export NP=40
export exe=./RayTracing_15
export width=1920
export samples_per_pixel=100
export max_depth=50
export vfov=20
export aspect_width=16.0
export aspect_height=9.0
export fps=60
export seconds=10

echo "Creating individual frames"
mpirun -np $NP $exe --w $width --s $samples_per_pixel --d $max_depth --v $vfov --a $aspect_width $aspect_height --f $fps --t $seconds
echo "Merging the frames into a single movie"
ffmpeg -framerate $fps - i *.ppm - c:v libx264 - b:v 1M - pix_fmt yuv420p RT15.mp4
echo "Removing temporary files"
rm -f *.ppm
echo "All done!"