# AdaptiveIntegralThresholding
Adaptive Thresholding Using the Integral Image

Based on https://github.com/rmtheis/bradley-adaptive-thresholding
Converted to opencv 3

It gives better result then opencv cv::adaptiveThreshold

Build:

  cmake .

  make

Usage:

  ./aithreshold in.png out.png [T=0.15] [part=8] [blur=0]

Example:

  ./aithreshold images/image.png images/output.png

image.png

![Image](https://github.com/zvezdochiot/aithreshold/blob/master/images/image.png)

output.png

![Image](https://github.com/zvezdochiot/aithreshold/blob/master/images/output.png)
