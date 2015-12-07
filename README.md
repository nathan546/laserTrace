## Synopsis

This program takes an input image and outputs a resultant boundary extracted image (using canny boundary extraction). It also outputs an ILDA formatted file for use in laser projection systems.

## Prerequisites

ImageMagick must be installed

## Compiling

gcc -o laserTrace laserTrace.c -lm

## Running

./laserTrace</BR>
File: in.png</BR>
Sigma: 1.4</BR>
Low Threshold: .3</BR>
High Threshold: .75</BR>
ILDA Compression: .8</BR>
