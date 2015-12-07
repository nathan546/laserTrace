## Synopsis

This program takes an input image and outputs a resultant boundary extracted image (using canny boundary extraction). It also outputs an ILDA formatted file for use in laser projection systems.

## Prerequisites

ImageMagick must be installed

## Compiling

gcc -o laserTrace laserTrace.c -lm

## Running

./laserTrace