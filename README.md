# Colorimetric-Descriptor-HSV-Space
Simultaneous localization and mapping systems (also known as SLAM techniques for Simultaneous Localization And Mapping) as well as augmented reality applications often rely, at low level, on the recognition of local invariants in the successive images of a video stream.
Most of the algorithms developed are based solely on luminance (color images are converted to gray levels for subsequent processing).
We have examined the results that we could obtain with another approach, that of chroma for which we rely on tints (colors).


# Programming language / Library
Implementation of the latter in traditional C ++ programming language through the OpenCV library, reference in image processing.

# How to use

**Step 1 :** Choose a keypoint detection "Threshold" the bigger it is the least but precise keypoint are detected.

**Step 2 :** Choose a video to input by precising the absolute path of it (a video stream could be use by modifiying a line in the code but not tested).

**Step3 :** Choose a name for the output video (within the main directorie of the programme are the directorie of the output video).

**Ps :** All file input files must be in file format of **.mp4** same for the output files they will all be in mp4 file format (this can be changed by modifieying one line of code). 

**Ps2 :** This was a project i did to learn a little more about OpenCV i know it's not perfect you can modifie it as many time you want if you have any advice feel free to message me thanks.
