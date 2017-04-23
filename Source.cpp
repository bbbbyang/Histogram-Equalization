//-------------------------------------------------------------------
//				Histogram Equalization
//				BINGYANG LIU
//				2015 - 07 - 30
//				OpenCV 3.0.0
//-------------------------------------------------------------------

//---------------- Head  File ---------------------------------------
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

//---------------- Name space ---------------------------------------
using namespace std;
using namespace cv;

const int histSize = 255;

int main(){
	// Load the image as grayscale
	Mat Img = imread("source.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("The Original Pic");
	imshow("The Original Pic", Img);

	Mat ImgHist;													// Histogram before equalization
	Mat Image(Img.rows, Img.cols, CV_LOAD_IMAGE_GRAYSCALE);			// Image after equalization
	Mat ImageHist;													// Histogram after equalization	

	// Calculate the histogram
	const int Channels[1] = {0};
	const int HistSize[1] = {256};
	float HRanges[2] = {0, 255};
	const float* Ranges[1] = {HRanges};
/*-------------------- calcHist() Function ---------------------------
	const int SCALE = 256;
	double Hist[SCALE];
	memset(Hist, 0, sizeof(double) * SCALE);

	// Calculate the histogram of the image
	for(int i = 0; i < Img.rows; i++){
		for(int j = 0; j < Img.cols; j++){
			Hist[(int)Img.at<uchar>(i, j)]++;
		}
	}
-------------------------------------------------------------------*/
	calcHist(&Img, 1, Channels, Mat(), ImgHist, 1, HistSize, Ranges);

//--------------------- Equalization Process ---------------------------
	float CDF[histSize + 1];
	memset(CDF, 0, (histSize + 1) * sizeof(int));
	for(int i  = 0; i <= histSize; i++){
		CDF[i] = ImgHist.at<float>(i);
		if(i != 0)
			CDF[i] += CDF[i - 1];
	}

	// Scale the CDF table
	for(int i = 0; i <= histSize; i++){
		CDF[i] = CDF[i] * histSize / (Img.rows * Img.cols);
	}

	// Get the result image
	for(int i = 0; i < Img.rows; i++){
		for(int j = 0; j < Img.cols; j++){
			Image.at<uchar>(i, j) = CDF[(int)Img.at<uchar>(i, j)];
		}
	}
	namedWindow("Pic After Equalization");
	imshow("Pic After Equalization", Image);
	imwrite("Pic After Equalization.jpg", Image);
//--------------------------------------------------------------------
	
//---------- Show the histogram before equalization -----------------
	// Normalize the histogram
	normalize(ImgHist, ImgHist, 0, Img.rows, NORM_MINMAX, -1, Mat());
	
	// Create the canvas for histogram
	int hist_w = 400;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);
	Mat histImg(hist_w, hist_h, CV_8UC3, Scalar( 0,0,0));

	// Show the original image histogram
	for(int i = 0; i < HistSize[0]; i++){
//		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(Hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(Hist.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
		rectangle(histImg, Point(bin_w * i, hist_h - 1), Point(bin_w * (i + 1), hist_h - cvRound(ImgHist.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
	}
	namedWindow("The Original Hist", CV_WINDOW_AUTOSIZE );
	imshow("The Original Hist", histImg);
	imwrite("The Original Hist.jpg", histImg);
//--------------------------------------------------------------------

//------------ Show the histogram after equalization -----------------
	// Calculate the histogram
	calcHist(&Image, 1, Channels, Mat(), ImageHist, 1, HistSize, Ranges);
	// Normalize the histogram
	normalize(ImageHist, ImageHist, 0, Img.rows, NORM_MINMAX, -1, Mat());
	
	Mat histImgAfterEqual(hist_w, hist_h, CV_8UC3, Scalar( 0,0,0));

	// Show the original image histogram
	for(int i = 0; i < HistSize[0]; i++){
//		line(histImage, Point(bin_w*(i-1), hist_h - cvRound(Hist.at<float>(i - 1))), Point(bin_w*(i), hist_h - cvRound(Hist.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
		rectangle(histImgAfterEqual, Point(bin_w * i, hist_h - 1), Point(bin_w * (i + 1), hist_h - cvRound(ImageHist.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
	}
	namedWindow("The Hist After Equalization", CV_WINDOW_AUTOSIZE );
	imshow("The Hist After Equalization", histImgAfterEqual);
	imwrite("The Hist After Equalization.jpg", histImgAfterEqual);
//--------------------------------------------------------------------

	// Keep the window alive
	waitKey();
	return 1;
}