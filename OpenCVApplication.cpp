// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <iostream>
#include <cmath>
#include <queue>
#include <random>
#include <fstream>
#include "OpenCVApplication.h"
#include <fstream>

using namespace std;

ofstream outstream("data.jpg", ios::out | ios::binary);

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}


/* Histogram display function - display a q using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i < hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double) hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(0x8E, 0x44, 0xAD)); // histogram bins colored in purple
	}

	imshow(name, imgHist);
}

void loadPrintImage() {
	Mat src;
	src = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("source", src);
	waitKey(0);
}

Mat RGBToYCbCr(Mat img) {
	Mat imgYCbCr = Mat(img.size(), CV_8UC3);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b bgr = img.at<Vec3b>(i, j);
			uchar R = bgr[2];
			uchar G = bgr[1];
			uchar B = bgr[0];

			uchar Y = 0 + (0.299f * R) + (0.587f * G) + (0.114f * B);
			uchar Cb = 128 - (0.168736f * R) - (0.331264f * G) + (0.5f * B);
			uchar Cr = 128 + (0.5f * R) - (0.418688f * G) - (0.081312f * B);

			imgYCbCr.at<Vec3b>(i, j) = Vec3b(Y, Cr, Cb);
		}
	}
	/*
	// Check if correct
	Mat imgRe = Mat(img.size(), CV_8UC3);
	cvtColor(imgYCbCr, imgRe, cv::COLOR_YCrCb2BGR);

	imshow("Re", imgRe);
	*/

	return imgYCbCr;
}

char* zigZagMatrix(char mat[8][8])
{
	int row = 0, col = 0;
	char* arr = new char[64]();
	int index = 0;

	// Boolean variable that will true if we 
	// need to increment 'row' value otherwise 
	// false- if increment 'col' value 
	bool row_inc = 0;

	// Print matrix of lower half zig-zag pattern 
	for (int len = 1; len <= 8; ++len) {
		for (int i = 0; i < len; ++i) {
			arr[index++] = mat[row][col];

			if (i + 1 == len)
				break;
			// If row_increment value is true 
			// increment row and decrement col 
			// else decrement row and increment 
			// col 
			if (row_inc)
				++row, --col;
			else
				--row, ++col;
		}

		if (len == 8)
			break;

		// Update row or col vlaue according 
		// to the last increment 
		if (row_inc)
			++row, row_inc = false;
		else
			++col, row_inc = true;
	}

	// Update the indexes of row and col variable 
	if (row == 0) {
		if (col == 7)
			++row;
		else
			++col;
		row_inc = 1;
	}
	else {
		if (row == 7)
			++col;
		else
			++row;
		row_inc = 0;
	}

	// Print the next half zig-zag pattern 
	for (int len, diag = 7; diag > 0; --diag) {

		if (diag > 8)
			len = 8;
		else
			len = diag;

		for (int i = 0; i < len; ++i) {
			arr[index++] = mat[row][col];

			if (i + 1 == len)
				break;

			// Update row or col vlaue according 
			// to the last increment 
			if (row_inc)
				++row, --col;
			else
				++col, --row;
		}

		// Update the indexes of row and col variable 
		if (row == 0 || col == 7) {
			if (col == 7)
				++row;
			else
				++col;

			row_inc = true;
		}

		else if (col == 0 || row == 7) {
			if (row == 7)
				++col;
			else
				++row;

			row_inc = false;
		}
	}

	return arr;
}

void runLengthEncoding(char* strChar)
{
	char buffer[5];

	for (int i = 0; i < 64; i++) {
		cout << (int)strChar[i] << " ";
	}
	cout << endl << endl;

	for (int i = 0; i < 64; i++) {
		int count = 0;
		while (strChar[i] == 0 && i < 64) {
			count++;
			i++;
		}
		// Print character and its zero count 
		if (i < 64) {
			buffer[0] = strChar[i];
			outstream.write(buffer, 1);
			memset(buffer, count, sizeof(count));
			outstream.write(buffer, sizeof(count));
		}
	}
	buffer[0] = '0';
	outstream.write(buffer, 1);
}

float alpha(int u) {
	return u == 0 ? 1.0f / sqrt(2) : 1;
}

void quantitize(Mat imgYCrCb) {

	Mat imgCentered;
	Mat dctMatrix(8, 8, CV_32FC3);

	float quantitizationMatrixData[8][8] = {
	{16, 11, 10, 16, 24, 40, 51, 61},
	{12, 12, 14, 19, 26, 58, 60, 55},
	{14, 13, 16, 24, 40, 57, 69, 56},
	{14, 17, 22, 29, 51, 87, 80, 62},
	{18, 22, 37, 56, 68, 109, 103, 77},
	{24, 35, 55, 64, 81, 104, 113, 92},
	{49, 64, 78, 87, 103, 121, 120, 101},
	{72, 92, 95, 98, 112, 100, 103, 99}
	};

	//Mat g(8, 8, CV_8SC1, &gData);
	Mat B(8, 8, CV_8SC1);
	Mat Bplus128(8, 8, CV_8UC1);
	Mat quantMatrix = Mat(8, 8, CV_32FC1, quantitizationMatrixData);
	vector<Mat> planes;
	split(imgYCrCb, planes);

	for (int i = 0; i < imgYCrCb.rows - 8; i += 8) {
		for (int j = 0; j < imgYCrCb.cols - 8; j += 8) {
			for (int plane = 0; plane < imgYCrCb.channels() - 2; plane++) { //////
				Mat block = planes[plane](Rect(j, i, 8, 8));
				block.convertTo(block, CV_32FC1);

				for (int u = 0; u < 8; u++) {
					for (int v = 0; v < 8; v++) {
						block.at<float>(u, v) -= 128.0f;
					}
				}

				for (int u = 0; u < 8; u++) {
					for (int v = 0; v < 8; v++) {

						float sum = 0.0f;
						for (int x = 0; x < 8; x++) {
							for (int y = 0; y < 8; y++) {
								sum += (float) block.at<float>(x, y) * cos((2 * x + 1) * u * PI / 16) * cos((2 * y + 1) * v * PI / 16);
							}
						}
						float dctPixel = 0.25f * alpha(u) * alpha(v) * sum;
						B.at<char>(u, v) = roundf(dctPixel / quantMatrix.at<float>(u, v));
					}
				}

				char Bdata[8][8];
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 8; j++) {
						Bdata[i][j] = B.at<char>(i, j);
						cout << (int)Bdata[i][j] << " ";
					}
					cout << endl;
				}

				cout << "MAT" << endl;
				cout << B << endl << endl;
				char* zigzag = zigZagMatrix(Bdata);

				runLengthEncoding(zigzag);
				return;

			/*	for (int u = 0; u < 8; u++) {
					for (int v = 0; v < 8; v++) {
						Bplus128.at<uchar>(u, v) = B.at<char>(u, v) + 128;
					}
				}*/
			//	 Bplus128.copyTo(planes[plane](Rect(j, i, 8, 8)));
			}
		
		}
	}


	Mat finalImage;
	merge(planes, finalImage);
	//cvtColor(imgYCrCb, imgYCrCb, cv::COLOR_YCrCb2BGR);
	//cvtColor(finalImage, finalImage, cv::COLOR_YCrCb2BGR);
	imshow("asdf", finalImage);
	imshow("Original", imgYCrCb);
	waitKey();
}

int main()
{
	cout << "Conversion Start" << endl << endl;
	char* path = "Images/Lena_24bits.bmp";
	Mat img = imread(path, CV_LOAD_IMAGE_COLOR);

	Mat imgYCbCr = RGBToYCbCr(img);
	quantitize(imgYCbCr);

	cout << endl << endl << "Conversion End" << endl;

	return 0;
}