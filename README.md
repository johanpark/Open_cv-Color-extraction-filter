# Open_cv-Color-extraction-filter

It works only when a webcam is installed. If you want to use an image

Lines 59 to 63 are the code to find the webcam, and you can change it to an image
Like 
Mat img = imread("Target_image.jpg"); 

----------------------------------------------

If you click the color you want to extract from the image, the color is left and the rest of the area is grayed out.

Similar areas are extracted together, so if there are similar colors in the surrounding background, they will be extracted together.
