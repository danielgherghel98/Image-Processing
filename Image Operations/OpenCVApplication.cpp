// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <queue>
#include <random>


void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

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

void testNegativeImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]
		
		Mat src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height,width,CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src.at<uchar>(i,j);
				uchar neg = 255 - val;
				dst.at<uchar>(i,j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int) src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}



void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1,dst2;
		//without interpolation
		resizeImg(src,dst1,320,false);
		//with interpolation
		resizeImg(src,dst2,320,true);
		imshow("input image",src);
		imshow("resized image (without interpolation)",dst1);
		imshow("resized image (with interpolation)",dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src,dst,gauss;
		src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int) k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss,dst,pL,pH,3);
		imshow("input image",src);
		imshow("canny",dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}
		
	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n"); 
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];
	
	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;
		
		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115){ //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess) 
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
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
	for (int i = 0; i<hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}

void openImgIncrementGray() {
	char fName[MAX_PATH];
	Mat src, dst;
	float inc = 70;
	while (openFileDlg(fName))
	{
		src = imread(fName, 0);
		dst = Mat(src.rows, src.cols, CV_8UC1);
		for (int i = 0; i < src.rows; i++){
			for (int j = 0; j < src.cols; j++) {
				int val = src.at<uchar>(i, j) + inc;
				if (val > 255)
				{
					dst.at<uchar>(i, j) = 255;
				}
				else if (val < 0)
				{
					dst.at<uchar>(i, j) = 0;
				}
				else {
					dst.at<uchar>(i, j) = val;
				}
			}
		}
		imshow("SRC", src);
		imshow("DST", dst);

		// Wait until user press some key
		waitKey(0);
	}

}

void openImgMultiplyGray() {
	char fName[MAX_PATH];
	Mat src, dst;
	float inc = 1.5;
	while (openFileDlg(fName))
	{
		src = imread(fName, CV_8UC1);
		dst = Mat(src.rows, src.cols, CV_8UC1);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dst.at<uchar>(i, j) = src.at<uchar>(i, j) * inc;
			}
		}
		imshow("SRC", src);
		imshow("DST", dst);

		// Wait until user press some key
		waitKey(0);
	}

}

bool insideImage(Mat src, int x, int y) {
			
		bool inside = false;
		if ((x >= 0 && x < src.rows) && (y >= 0 && y < src.cols))
		{
			inside = true;
		}
		return inside;

}
void pointInside(Mat img) {

	int x, y;
	printf("Introduceti coordonatele punctului!\n");
	scanf("%d%d", &x, &y);
	if (insideImage(img, x, y))
	{
		printf("Punctul este in interior.\n");
	}
	else
	{
		printf("Punctul nu este in interior.\n");
	}
}


int* lab3_pct1(Mat_<uchar> src) {
	char fName[MAX_PATH];
	int* hist;
	hist = (int*)calloc(256, sizeof(int));
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			hist[src(i, j)]++;
		}
	}
	/*printf("Histograma:\n");
	for (int i = 0; i < 256; i++) {
		printf("%d ", hist[i]);
	}*/
	waitKey(0);
	return hist;
}


void lab3_pct4(Mat_<uchar> src, int m) {
	int* hist;
	hist = (int*)calloc(m, sizeof(int));
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			hist[src(i, j)/m]++;
		}
	}
	printf("Histograma cu numar redus de acumulatoare:\n");
	for (int i = 0; i < m; i++) {
		printf("%d ", hist[i]);
	}
	waitKey(0);
}


float* lab3_pct2(Mat_<uchar> src) {
	
	int* hist;
	float* fdp=(float*)calloc(256,sizeof(float));
	
		int size = src.rows * src.cols;
		hist = lab3_pct1(src);
		//printf("FDP is:\n");
		for (int i = 0; i < 256; i++) {
			fdp[i] = (float)hist[i] / size;
		}
		return fdp;
}


void lab3_pct3() {
	char fName[MAX_PATH];
	Mat_<uchar> src;
	int* hist;
	while (openFileDlg(fName))
	{
		src = imread(fName, CV_8UC1);
		hist = lab3_pct1(src);
		showHistogram("MyHistogram", hist, src.cols, src.rows);
		
		waitKey(0);
	}
}

Mat_<uchar> lab3_pct5(Mat_<uchar> src)
{
	Mat_<uchar> dst (src.rows, src.cols);
	float* fdp = (float*)calloc(256, sizeof(float));
	int WH = 5;
	float TH = 0.0003;
	float v=0.0f;
	float maxime[256];
	maxime[0] = 0;
	bool max = true;
	fdp = lab3_pct2(src);
	int contormax = 1;

	//determinare maxime
	for (int k = WH; k <= 255 - WH; k++)
	{
		v = 0.0f;
		max = true;
		for (int i = k - WH; i <= (k + WH); i++)
		{
			v =v + fdp[i];
			if (fdp[i] > fdp[k])
			{
				max = false;
			}
		}
		v = v / (2.0f * WH + 1.0f);
		if ((fdp[k] >(v + TH)) && (max == true))
		{
			maxime[contormax] = k;
			contormax++;
		}
	}
	maxime[contormax] = 255;
	contormax++;
	printf("\nLista Maxime:\n");
	for (int i = 0; i < contormax; i++) {
		printf("%f ", maxime[i]);
	}

	//float nearestMax = 255;
	//determinare praguri
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			float pixelColor = src(i, j);
			float nearestMax = 255;
			float color;
			int k = 0;
			while (k < contormax)
			{
				if (abs(pixelColor - maxime[k]) <= nearestMax)
				{
					nearestMax = abs(pixelColor - maxime[k]);
					color = maxime[k];
				}
				k++;
			}
			dst(i, j) = color;
		}

	}
	
	return dst;
	waitKey(0);
}

void mouseEvent(int evt, int x, int y, int flags, void* param)
{
	Mat_<Vec3b>* img = (Mat_<Vec3b>*)param;
	Vec3b color;
	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		//calculare arie
		color = (Vec3b)(*img)(y, x);
		uchar R = color[2];
		uchar G = color[1];
		uchar B = color[0];
		long aria = 0;
		long row = 0;
		long col = 0;

		long cmin = MAX_PATH;
		long cmax = 0;
		long rmin = MAX_PATH;
		long rmax = 0;
		//proiectie figura
		int* h =(int*)calloc((int)(*img).rows,sizeof(int));
		int* v = (int*)calloc((int)(*img).cols, sizeof(int));
		for (int i = 0; i < (int)(*img).rows; i++) {
			for (int j = 0; j < (int)(*img).cols; j++) {
				if ((color[0] == (int)(*img)(i, j)[0]) && (color[1] == (int)(*img)(i, j)[1]) && (color[2] == (int)(*img)(i, j)[2]))
				{
					aria++;
					row += i;
					col += j;
					if (i > rmax)
					{
						rmax = i;
						
					}
					if (i < rmin)
					{
						rmin = i;
					}
					if (j > cmax)
					{
						cmax = j;

					}
					if (j < cmin)
					{
						cmin = j;
					}
					h[i]++;
				}
			}
		}

		for (int i = 0; i < (int)(*img).cols; i++) {
			for (int j = 0; j < (int)(*img).rows; j++) {
				if ((color[0] == (int)(*img)(j, i)[0]) && (color[1] == (int)(*img)(j, i)[1]) && (color[2] == (int)(*img)(j, i)[2]))
				{
					v[i]++;
				}
			}
		}
		printf("Aria figurii este: %ld pixeli.\n", aria);

		//calculare
		int r = row / aria;
		int c = col / aria;
		printf("Centrul de masa este: (%d,%d).\n", r, c);

		//axa de alungire
		long numarator = 0;
		long numitor = 0;
		long p1 = 0, p2 = 0;
		Mat_<Vec3b> copie((*img).rows, (*img).cols, CV_8UC3);
		int perimetru = 0;
		for (int i = 0; i < (int)(*img).rows; i++) {
			for (int j = 0; j < (int)(*img).cols; j++) {
				if ((color[0] == (int)(*img)(i, j)[0]) && (color[1] == (int)(*img)(i, j)[1]) && (color[2] == (int)(*img)(i, j)[2]))
				{
					numarator += (i - r) * (j - c);
					p1 += (j - c) * (j - c);
					p2 += (i - r) * (i - r);
					if ((R != (*img)(i, j + 1)[2] || G != (*img)(i, j + 1)[1] || B != (*img)(i, j + 1)[0] || //dreapta
						R != (*img)(i, j - 1)[2] || G != (*img)(i, j - 1)[1] || B != (*img)(i, j - 1)[0] || //stanga
						R != (*img)(i - 1, j)[2] || G != (*img)(i - 1, j)[1] || B != (*img)(i - 1, j)[0] || //sus
						R != (*img)(i + 1, j)[2] || G != (*img)(i + 1, j)[1] || B != (*img)(i + 1, j)[0] || //jos
						R != (*img)(i - 1, j - 1)[2] || G != (*img)(i - 1, j - 1)[1] || B != (*img)(i - 1, j - 1)[0] || //stanga sus
						R != (*img)(i - 1, j + 1)[2] || G != (*img)(i - 1, j + 1)[1] || B != (*img)(i - 1, j + 1)[0] || //dreapta sus
						R != (*img)(i + 1, j - 1)[2] || G != (*img)(i + 1, j - 1)[1] || B != (*img)(i + 1, j - 1)[0] || //stanga jos
						R != (*img)(i + 1, j + 1)[2] || G != (*img)(i + 1, j + 1)[1] || B != (*img)(i + 1, j + 1)[0])) //dreapta jos
					{
						perimetru++;
						copie(i, j) = color;
					}
				}
				
			}
		}
		numarator *= 2;
		numitor = p1 - p2;
		float phi = atan2(numarator, numitor)/2;
		printf("Phi este egal cu: %f\n", phi);

		//perimetru
		perimetru = perimetru * PI / 4;
		printf("Perimetrul este: %d.\n", perimetru);
		

		//factor de subtiere
		float t = 4 * PI * ((float)aria / (perimetru * perimetru));
		printf("Factorul de subtiere este: %f.\n", t);

		//elongatia
		float el = ((float)(cmax - cmin + 1)) / (rmax - rmin + 1);
		printf("Elongatia este: %f.\n", el);
		

		//afisare axa de alungire
		int r1 = (int)round(r + 100 * sin(phi));
		int c1 = (int)round(c + 100 * cos(phi));
		
		Point pt2 = Point(c1, r1);
		int r2 = (int)round(r + (-100) * sin(phi));
		int c2 = (int)round(c + (-100) * cos(phi));
		Point pt1 = Point(c2, r2);
		line(copie, pt1, pt2, CV_RGB(255, 0, 255));


		//afisare centru de masa
		copie(r, c)[0] = 0;
		copie(r, c)[1] = 0;
		copie(r, c)[2] = 255;

		//proiectie figura
		//Mat_<Vec3b> proiectie((*img).rows, (*img).cols, CV_8UC3);
		for (int i = rmin; i < rmax; i++) {
			for (int j = 0; j < h[i]; j++) {
				copie(i, j) = color;
			}
		}
		for (int i = cmin; i < cmax; i++) {
			for (int j = 0; j < v[i]; j++) {
				copie(j, i) = color;
			}
		}
		imshow("Copie", copie);
		//imshow("Proiectie", proiectie);
	}
}

void lab4_1() {

	char fName[MAX_PATH];
	Mat_<Vec3b> img;
	while (openFileDlg(fName))
	{
		img = imread(fName, CV_LOAD_IMAGE_COLOR);

		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", mouseEvent, &img);

		//show the image
		imshow("My Window", img);

	}

	waitKey(0);

}

void lab4_2() {

	char fName[MAX_PATH];
	Mat_<Vec3b> img;
	int TH_arie;
	float phi_LOW, phi_HIGH;
	printf("Introduceti aria maxima!\n");
	scanf("%d", &TH_arie);
	printf("Introduceti phi minim!\n");
	scanf("%f", &phi_LOW);
	printf("Introduceti phi maxim!\n");
	scanf("%f", &phi_HIGH);
	while (openFileDlg(fName))
	{
		img = imread(fName, CV_LOAD_IMAGE_COLOR);

		Vec3b colors[200];
		colors[0] = (255,255,255);
		int contor = 1;
		bool dif = true;
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				uchar R = img(i,j)[2];
				uchar G = img(i, j)[1];
				uchar B = img(i, j)[0];
				dif = true;
				for (int k = 0; k < contor; k++) {
					//if (R == colors[k][0] && G == colors[k][1] && B == colors[k][2])
					if (img(i,j) == colors[k])
					{
						dif = false;
					}
				}
				if (dif == true)
				{
					colors[contor++] = img(i, j);
					//index++;
				}
			}
		}
		Mat_<Vec3b> img1(img.rows, img.cols, CV_8UC3);
		Mat_<Vec3b> img2(img.rows, img.cols, CV_8UC3);

		
		
		for (int k = 1; k < contor; k++) {
			long aria = 0;
			int row = 0, col = 0;
			for (int i = 0; i < img.rows; i++) {
				for (int j = 0; j < img.cols; j++) {
					if (colors[k] == img(i, j))
					{
						aria++;
						row += i;
						col += j;
					}
				}
			}
			int r = row / aria;
			int c = col / aria;
			long numarator = 0;
			long numitor = 0;
			long p1 = 0, p2 = 0;
			for (int i = 0; i < img.rows; i++) {
				for (int j = 0; j < img.cols; j++) {
					if (colors[k] == img(i, j))
					{
						numarator += (i - r) * (j - c);
						p1 += (j - c) * (j - c);
						p2 += (i - r) * (i - r);
					}
				}
			}
			numarator *= 2;
			numitor = p1 - p2;
			float phi = atan2(numarator, numitor) / 2;

			if (aria < TH_arie)
			{
				for (int i = 0; i < img.rows; i++) {
					for (int j = 0; j < img.cols; j++) {
						if (colors[k] == img(i, j))
						{
							img1(i, j) = img(i, j);
						}
					}
				}
			}

			if (phi>phi_LOW && phi < phi_HIGH)
			{
				for (int i = 0; i < img.rows; i++) {
					for (int j = 0; j < img.cols; j++) {
						if (colors[k] == img(i, j))
						{
							img2(i, j) = img(i, j);
						}
					}
				}
			}
		}

		imshow("My Window", img);
		imshow("Restrictionare Arie", img1);
		imshow("Restrictionare Phi", img2);

	}

	waitKey(0);

}

void lab5_2(int** labels, Mat_<uchar> img, int label,String s)
{
	Mat_<Vec3b> colorat(img.rows, img.cols, CV_8UC3);
	Vec3b* colors = (Vec3b*)calloc(label+1 , sizeof(Vec3b));
	colors[0] = Vec3b(255, 255, 255);
	std::default_random_engine gen;
	std::uniform_int_distribution<int> d(0, 255);
	for (int i = 1; i <= label; i++)
	{
		uchar a = d(gen);
		uchar b = d(gen);
		uchar c = d(gen);
		colors[i] = Vec3b(a, b, c);

	}
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				colorat(i, j) = colors[labels[i][j]];
			}
		}
	
	imshow("Image", img);
	imshow(s, colorat);

}


void lab5_1(boolean flag) {
	
	char fName[MAX_PATH];
	int label = 0;
	Mat_<uchar> img;
	while (openFileDlg(fName))
	{
		
		int size;
		if (flag)
		{
			size = 8;
		}
		else
		{
			size = 4;
		}
		int di[8] = { -1,0,1,0,-1,-1,1,1 };
		int dj[8] = { 0,-1,0,1,-1,1,-1,1 };
		img = imread(fName, CV_LOAD_IMAGE_GRAYSCALE);
		int** labels = (int**)malloc(img.rows*sizeof(int*));
		for (int i = 0; i < img.rows; i++) {
			labels[i] = (int*)calloc(img.cols, sizeof(int));
			memset(labels[i], 0, img.cols * sizeof(int));
		}
		
		
				
		std::queue<Point> Q;
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				if (img(i, j) == 0 && labels[i][j] == 0)
				{
					label++;
					labels[i][j] = label;
					Q.push({ i,j });
					while (!Q.empty())
					{
						Point q = Q.front();
						Q.pop();
						for (int k = 0; k < size; k++)
						{
							Point neighbor = Point(q.x+di[k], q.y+dj[k]);
							if (insideImage(img, q.x + di[k], q.y + dj[k]))
							{
								if (img(neighbor.x,neighbor.y) == 0 && labels[neighbor.x][neighbor.y] == 0)
								{
									labels[neighbor.x][neighbor.y] = label;
									Q.push(neighbor);
								}
							}
						}
					}
				}
			}
		}
		//return labels;
		//printf("bla bla bla");
		lab5_2(labels, img, label,"Colored image");
	}
	waitKey(0);
}

void lab5_3(boolean flag)
{
	char fName[MAX_PATH];
	int label = 0;
	Mat_<uchar> img;
	while (openFileDlg(fName))
	{
		int di[8] = { -1,0,1,0,-1,-1,1,1 };
		int dj[8] = { 0,-1,0,1,-1,1,-1,1 };
		int neighbors_size = 0;
		img = imread(fName, CV_LOAD_IMAGE_GRAYSCALE);
		if (!(flag))
		{
			neighbors_size = 4;
		}
		else
		{
			neighbors_size = 8;
		}
		label = 0;
		int** labels;
		labels = (int**)malloc(img.rows * sizeof(int*));
		for (int i = 0; i < img.rows; i++)
		{
			labels[i] = (int*)calloc(img.cols, sizeof(int));
		}
		std::vector<std::vector<int>> edges;
		std::vector<int> vec;
		edges.push_back(vec);
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				if (img(i,j) == 0 && labels[i][j] == 0)
				{
					std::vector<int> L;
					int x = label + 1;
					for (int k = 0; k < neighbors_size; k++)
					{
						if (insideImage(img, i + di[k], j + dj[k]))
						{
							if (labels[i + di[k]][j + dj[k]] > 0)
							{
								L.push_back(labels[i + di[k]][j + dj[k]]);
								if (labels[i + di[k]][j + dj[k]] < x)
								{
									x = labels[i + di[k]][j + dj[k]];
								}
							}
						}
					}
					if (L.size() == 0)
					{
						std::vector<int> vec;
						edges.push_back(vec);
						label++;
						labels[i][j] = label;
					}
					else
					{
						labels[i][j] = x;
						for (int k = 0; k < L.size(); k++)
						{
							if (L.at(k) != x)
							{
								edges[x].push_back(L[k]);
								edges[L[k]].push_back(x);
							}
						}
					}
				}
			}
		}
		lab5_2(labels, img, label,"Colored image 1");
		std::queue<int> Q;
		int newLabel = 0;
		int* newLabels = (int*)calloc(label + 1, sizeof(int));
		for (int i = 1; i < label; i++)
		{
			if (newLabels[i] == 0)
			{
				newLabel++;
				newLabels[i] = newLabel;
				Q.push(i);
				while (!Q.empty())
				{
					int x = Q.front();
					Q.pop();
					for (int j = 0; j < edges[x].size(); j++)
					{
						int y = edges[x][j];
						if (newLabels[y] == 0)
						{
							newLabels[y] = newLabel;
							Q.push(y);
						}
					}
				}
			}
		}

		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				labels[i][j] = newLabels[labels[i][j]];
			}

		}
		lab5_2(labels, img, label,"Colored image 2");
	}
	
}


void lab6_1(Mat_<uchar> src)
{
	Mat_<uchar> contur(src.rows,src.cols,1);
	Point start,start1;
	Point prev,curent;
	int dir = 7;
	int di[8] = { 0,-1,-1,-1,0,1,1,1 };
	int dj[8] = { 1,1,0,-1,-1,-1,0,1 };
	int ok = 0;
	prev = Point(0, 0);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			if (src(i, j) != 255 && ok==0)
			{
				start = Point(i, j);
				ok = 1;
				printf("Curent: %d  %d\n", start.x, start.y);
				break;
			}

		}

	}
	curent = start;
	printf("Start: %d  %d\n", start.x, start.y);
	if (dir % 2 == 0)
	{
		dir = (dir + 7) % 8;
	}
	else
	{
		dir = (dir + 6) % 8;
	}
	while(prev==Point(0,0))
	{
		if (src(curent.x+di[dir], curent.y+dj[dir]) == src(start.x,start.y))
		{
			prev = curent;
			curent = Point(curent.x + di[dir], curent.y + dj[dir]);
			start1 = curent;

		}
		dir = (dir+1) % 8;
	}

	curent = Point(0, 0);
	prev = Point(0, 0);
	while (curent != start1 && prev != start)
	{
		if (curent == Point(0,0))
		{
			curent = start1;
			prev = start;
		}
		if (dir % 2 == 0)
		{
			dir = (dir + 7) % 8;
		}
		else
		{
			dir = (dir + 6) % 8;
		}
		while (src(curent.x + di[dir], curent.y + dj[dir]) != src(start.x, start.y))
		{
			dir = (dir+1) % 8;
		}
		prev = curent;
		curent = Point(curent.x + di[dir], curent.y + dj[dir]);
		contur(curent.x, curent.y) = 255;
	}
	imshow("SRC", src);
	imshow("Contur", contur);
}

void lab6_2(Mat_<uchar> src)
{
	Mat_<uchar> contur(src.rows, src.cols, 1);
	Point start, start1;
	Point prev, curent;
	int* directii = (int*)malloc(sizeof(int));
	int size = 1;
	int dir = 7;
	int di[8] = { 0,-1,-1,-1,0,1,1,1 };
	int dj[8] = { 1,1,0,-1,-1,-1,0,1 };
	int ok = 0;
	prev = Point(0, 0);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			if (src(i, j) != 255 && ok == 0)
			{
				start = Point(i, j);
				ok = 1;
				//printf("Curent: %d  %d\n", start.x, start.y);
				break;
			}

		}

	}
	curent = start;
	printf("Start: %d  %d\n", start.x, start.y);
	if (dir % 2 == 0)
	{
		dir = (dir + 7) % 8;
	}
	else
	{
		dir = (dir + 6) % 8;
	}
	
	while (prev == Point(0, 0))
	{
		if (src(curent.x + di[dir], curent.y + dj[dir]) == src(start.x, start.y))
		{
			prev = curent;
			curent = Point(curent.x + di[dir], curent.y + dj[dir]);
			start1 = curent;
			directii[size - 1] = dir;
			size++;
			directii = (int*)realloc(directii, size * sizeof(int));

		}
		dir = (dir + 1) % 8;
	}

	curent = Point(0, 0);
	prev = Point(0, 0);
	while (curent != start1 && prev != start)
	{
		if (curent == Point(0, 0))
		{
			curent = start1;
			prev = start;
		}
		if (dir % 2 == 0)
		{
			dir = (dir + 7) % 8;
		}
		else
		{
			dir = (dir + 6) % 8;
		}
		while (src(curent.x + di[dir], curent.y + dj[dir]) != src(start.x, start.y))
		{
			dir = (dir + 1) % 8;
		}
		directii[size - 1] = dir;
		size++;
		directii = (int*)realloc(directii, size * sizeof(int));
		prev = curent;
		curent = Point(curent.x + di[dir], curent.y + dj[dir]);
		contur(curent.x, curent.y) = 255;
	}
	printf("Size: %d\n",size-3);
	printf("Vector de directii: ");
	for (int i = 0; i < size-3; i++)
	{
		printf("%d ", directii[i]);
	}
	int* derivata = (int*)malloc((size-3)*sizeof(int));
	printf("\nDerivata: ");
	for (int i = 0; i < size - 4; i++)
	{
		derivata[i] = (directii[i + 1] - directii[i] + 8) % 8;
		printf("%d ", derivata[i]);
	}
	derivata[size - 4] = (directii[0] - directii[size - 4] + 8) % 8;
	printf("%d ", derivata[size-4]);
}

void lab6_3(Mat_<uchar> src)
{
	int di[8] = { 0,-1,-1,-1,0,1,1,1 };
	int dj[8] = { 1,1,0,-1,-1,-1,0,1 };
	char fName[MAX_PATH];
	while (openFileDlg(fName))
	{
		FILE* f = fopen(fName, "r");
		int x, y;
		fscanf(f,"%d %d", &y, &x);
		Point start = Point(x, y);
		int size;
		fscanf(f, "%d",&size);
		src(x, y) = 0;
		for (int i = 0; i < size; i++)
		{
			int dir;
			fscanf(f, "%d", &dir);
			x += di[dir];
			y += dj[dir];
			src(x, y) = 0;
		}
		imshow("Img", src);
		waitKey(0);
	}
	
}

Mat_<uchar> dilatarea(Mat_<uchar> src, int* di, int* dj)
{
	Mat_<uchar> dilatare(src.rows, src.cols, -1);
	//dilatare
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src(i, j) != 255)
			{

				dilatare(i, j) = src(i, j);
				for (int k = 0; k < 4; k++)
				{
					if (insideImage(src, i + di[k], j + dj[k]))
					{
						dilatare(i + di[k], j + dj[k]) = src(i, j);
					}
				}
			}
		}
	}
	return dilatare;
}

Mat_<uchar> eroziunea(Mat_<uchar> src, int* di, int* dj)
{
	Mat_<uchar> eroziune(src.rows, src.cols, -1);
	//eroziunea
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src(i, j) != 255)
			{
				int ok = 1;
				for (int k = 0; k < 4; k++)
				{
					if (insideImage(src, i + di[k], j + dj[k]))
					{
						if (src(i + di[k], j + dj[k]) != src(i, j))
						{
							ok = 0;
						}
					}
					else {
						ok = 0;
					}
				}
				if (ok == 1)
				{
					eroziune(i, j) = src(i, j);
				}
			}
		}
	}
	return eroziune;
}



void lab7_1(Mat_<uchar> src)
{
	int di[4] = { -1,0,1,0 };
	int dj[4] = { 0,-1,0,1 };
	Mat_<uchar> dilatare(src.rows, src.cols, -1);
	Mat_<uchar> eroziune(src.rows, src.cols, -1);
	Mat_<uchar> deschidere(src.rows, src.cols, -1);
	Mat_<uchar> inchidere(src.rows, src.cols, -1);

	
	//dilatarea
	dilatare = dilatarea(src, di, dj);
	//eroziunea
	eroziune = eroziunea(src, di, dj);


	//deschiderea
	deschidere = dilatarea(eroziune, di, dj);

	//inchiderea
	inchidere = eroziunea(dilatare, di, dj);

	imshow("Sursa", src);
	imshow("Dilatare", dilatare);
	imshow("Eroziune", eroziune);
	imshow("Deschiderea", deschidere);
	imshow("Inchiderea", inchidere);
}


void lab7_2(Mat_<uchar> src, int n)
{
	int di[4] = { -1,0,1,0 };
	int dj[4] = { 0,-1,0,1 };
	Mat_<uchar> dilatareAux(src.rows, src.cols, -1);
	Mat_<uchar> eroziuneAux(src.rows, src.cols, -1);

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			dilatareAux(i, j) = src(i, j);
			eroziuneAux(i, j) = src(i, j);
		}
	}

	Mat_<uchar> dilatare(src.rows, src.cols, CV_16UC1);
	Mat_<uchar> eroziune(src.rows, src.cols, CV_16UC1);
	Mat_<uchar> deschidere(src.rows, src.cols, CV_16UC1);
	Mat_<uchar> inchidere(src.rows, src.cols, CV_16UC1);

	while (n != 0)
	{
		//initializare imagini cu alb
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dilatare(i, j) = 255;
				eroziune(i, j) = 255;
				deschidere(i, j) = 255;
				inchidere(i, j) = 255;
			}
		}

		//dilatare
		dilatare = dilatarea(dilatareAux, di, dj);
		


		//eroziunea
		eroziune = eroziunea(eroziuneAux, di, dj);
		
		//reinitializare matrici
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dilatareAux(i,j) = dilatare(i,j);
				eroziuneAux(i,j) = eroziune(i,j);
			}
		}


		//deschiderea
		deschidere = dilatarea(eroziune, di, dj);

		//inchiderea
		inchidere = eroziunea(dilatare, di, dj);
		
		n--;
	}

	imshow("Sursa", src);
	//imshow("Dilatare", dilatare);
	imshow("Eroziune", eroziune);
	//imshow("Deschiderea", deschidere);
	//imshow("Inchiderea", inchidere);
}


void lab7_3(Mat_<uchar> src)
{
	int di[4] = { -1,0,1,0 };
	int dj[4] = { 0,-1,0,1 };
	Mat_<uchar> eroziune(src.rows, src.cols, -1);
	Mat_<uchar> contur(src.rows, src.cols, -1);

	//eroziunea
	eroziune = eroziunea(src, di, dj);

	//diferenta(obtinere contur)
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			if (src(i, j) != eroziune(i,j))
			{
				contur(i, j) = src(i, j);
			}
		}
	}
	imshow("Sursa", src);
	imshow("Contur", contur);
}


void lab7_4(int evt, int x, int y, int flags, void* param)
{
	int di[4] = { -1,0,1,0 };
	int dj[4] = {0,-1,0,1};
	Mat_<uchar> src = *(Mat_<uchar>*)param;
	Mat_<uchar> complementara(src.rows, src.cols, -1);
	Mat_<uchar> xk1(src.rows, src.cols, -1);
	Mat_<uchar> xk0(src.rows, src.cols, -1);
	Mat_<uchar> final(src.rows, src.cols, -1);
	if (evt == CV_EVENT_LBUTTONDOWN)
	{
		xk0(y, x) = 0;


		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (src(i, j) == 255)
				{
					complementara(i, j) = 0;
				}
			}
		}
		int ok = 1;
		while (ok)
		{
			//initializare xk1
			for (int i = 0; i < xk1.rows; i++) {
				for (int j = 0; j < xk1.cols; j++) {
					xk1(i, j) = 255;
				}
			}

			Mat_<uchar> intermediara(src.rows, src.cols, -1);
			//dilatare
			intermediara = dilatarea(xk0, di, dj);
			

			//intersectie dilatare cu complementara
			for (int i = 0; i < xk0.rows; i++) {
				for (int j = 0; j < xk0.cols; j++) {
					if (intermediara(i, j) == complementara(i, j))
					{
						xk1(i, j) = intermediara(i, j);
					}
				}
			}
			ok = 0;
			//verificare egalitate xk1 cu xk0
			for (int i = 0; i < xk0.rows; i++) {
				for (int j = 0; j < xk0.cols; j++) {
					if (xk0(i, j) != xk1(i, j))
					{
						ok = 1;
					}
				}
			}

			//xk0<-xk1
			for (int i = 0; i < xk0.rows; i++) {
				for (int j = 0; j < xk0.cols; j++) {
					xk0(i, j) = xk1(i, j);
				}
			}
		}

		//reuniune xk1 cu src
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if (xk1(i, j) == 0 || src(i, j) == 0)
				{
					final(i, j) = 0;
				}
			}
		}
		//imshow("Sursa", src);
		imshow("Imagine plina", final);
	}
}


float medieN(int* hist, int start, int stop);
void lab8_1(Mat_<uchar> src)
{
	//calculare medie
	int* med;
	float media=0.0f;
	med = lab3_pct1(src);
	media = medieN(med, 0, 255);
	printf("Media nivelurilor de intensitate: %f",media);

	//deviatia standard
	float* fdp;
	float m = 0.0f;
	float deviatia;
	fdp = lab3_pct2(src);
	for (int j = 0; j < 256; j++) {
		m += (j-media)*(j-media) * fdp[j];
	}
	deviatia = sqrt(m);
	printf("\nDeviatia standard a nivelurilor de intensitate: %f", deviatia);

	//histograma
	int* hist = lab3_pct1(src);
	showHistogram("MyHistogram", hist, 256, src.rows);

	//histograma cumulativa
	int* h = (int*)calloc(256, sizeof(int));
	h[0] = hist[0];
	for (int i = 1; i < 255; i++) {
		h[i] = h[i - 1] + hist[i];
	}
	h[255] = src.rows * src.cols;
	showHistogram("Histograma cumulativa", h, 256, src.rows);

}


float medieN(int* hist, int start, int stop)
{
	int n = 0;
	int medie = 0;
	for (int i = start; i <= stop; i++)
	{
		n += hist[i];
		medie += i * hist[i];
	}
	float med = (float)medie / n;
	return med;
}
void lab8_2(Mat_<uchar> src)
{
	//calculare histograma
	int* hist = lab3_pct1(src);
	float eroare = 0.1;
	//intensitate minima/maxima
	int imin=MAX_PATH, imax = 0;
	for (int i = 0; i < 256; i++)
	{
		if (hist[i]!=0)
		{
			if (imin > i)
			{
				imin = i;
			}
			if (imax < i)
			{
				imax = i;
			}
		}
	}

	int t0 = 0;
	int t1 = (imin + imax) / 2;
	float m1, m2;
	while (t1 - t0 >= eroare)
	{
		m1=medieN(hist, imin, t1);
		m2=medieN(hist, t1 + 1, imax);

		t0 = t1;
		t1 = (int)(m1 + m2) / 2;
	}

	printf("Prag: %d", t1);
	Mat_<uchar> binara(src.rows, src.cols, -1);

	for (int i = 0; i<src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (src(i, j) <= t1)
			{
				binara(i, j) = 0;
			}
			else
			{
				binara(i, j) = 255;
			}
		}
	}

	imshow("SRC", src);
	imshow("Imagine binara", binara);
}

Mat_<uchar> negativulImg(Mat_<uchar> src)
{
	Mat_<uchar> negativ(src.rows, src.cols, -1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			negativ(i, j) = 255 - src(i, j);
		}
	}
	return negativ;
}

Mat_<uchar> corectieGamma(Mat_<uchar> src,float gamma)
{
	Mat_<uchar> gammaC(src.rows, src.cols, -1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			float r = src(i, j)*1.0f / 255;
			int p = 255*pow(r*1.0f,gamma);

			if (p > 255)
			{
				gammaC(i, j) = 255;
			}
			else if (p < 0)
			{
				gammaC(i, j) = 0;
			}
			else
			{
				gammaC(i, j) = p;
			}
		}
	}
	return gammaC;
}

Mat_<uchar> nivelLum(Mat_<uchar> src, int nivel)
{
	Mat_<uchar> luminozitate(src.rows, src.cols, -1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			int p = src(i,j)+nivel;

			if (p > 255)
			{
				luminozitate(i, j) = 255;
			}
			else if (p < 0)
			{
				luminozitate(i, j) = 0;
			}
			else
			{
				luminozitate(i, j) = p;
			}
		}
	}
	return luminozitate;
}
int* histograma(Mat_<uchar> src)
{
	int* hist;
	hist = (int*)calloc(256, sizeof(int));
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			hist[src(i, j)]++;
		}
	}
	return hist;
}

Mat_<uchar> contrastImg(Mat_<uchar> src, int gmin, int gmax, int Imin, int Imax)
{
	Mat_<uchar> contrast(src.rows, src.cols, -1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			contrast(i,j)= gmin + ((src(i, j) - Imin) * ((gmax * 1.0f - gmin) / (Imax * 1.0f - Imin) * 1.0f));
		}
	}
	return contrast;
}

void lab8_3(Mat_<uchar> src)
{
	int* hist = histograma(src);
	Mat_<uchar> negativ(src.rows, src.cols, -1);
	Mat_<uchar> gammaC(src.rows, src.cols, -1);
	Mat_<uchar> contrast(src.rows, src.cols, -1);
	Mat_<uchar> luminozitate(src.rows, src.cols, -1);

	float gamma;
	int nivel;
	int gmin;
	int gmax;
	

	//intensitate minima/maxima
	int imin = MAX_PATH, imax = 0;
	for (int i = 0; i < 256; i++)
	{
		if (hist[i] != 0)
		{
			if (imin > i)
			{
				imin = i;
			}
			if (imax < i)
			{
				imax = i;
			}
		}
	}

	int s;
	printf("Optiuni:\n");
	printf(" 1 - Negativul imaginii\n");
	printf(" 2 - Corectia gamma\n");
	printf(" 3 - Luminozitate\n");
	printf(" 4 - Contrast\n");
	printf("Introduceti optiunea dorita: ");
	scanf("%d", &s);
	switch (s) {
	case 1: {
		//negativul imaginii
		imshow("SRC", src);
		//histograma imagine sursa
		showHistogram("Sursa", hist, 256, src.rows);
		negativ = negativulImg(src);
		int* histNeg = histograma(negativ);
		showHistogram("Histograma Negativ", histNeg, 256, src.rows);
		imshow("Negativ", negativ);
		break;
	}
	case 2: {
		//corectia gamma
		printf("Introduceti coeficientul gamma: ");
		scanf("%f", &gamma);
		imshow("SRC", src);
		//histograma imagine sursa
		showHistogram("Sursa", hist, 256, src.rows);
		gammaC = corectieGamma(src, gamma);
		int* histGamma = histograma(gammaC);
		showHistogram("Histogram Gamma", histGamma, 256, src.rows);
		imshow("Gamma", gammaC);
		break;
	}
	case 3: {
		//modificare luminozitate
		printf("Introduceti nivelul de crestere al luminozitatii: ");
		scanf("%d", &nivel);
		imshow("SRC", src);
		//histograma imagine sursa
		showHistogram("Sursa", hist, 256, src.rows);
		luminozitate = nivelLum(src, nivel);
		int* histLum = histograma(luminozitate);
		showHistogram("Histograma Luminozitate", histLum, 256, src.rows);
		imshow("Luminozitate", luminozitate);
		break;
	}
	case 4: {
		//contrast
		printf("Introduceti gmin: ");
		scanf("%d", &gmin);
		printf("Introduceti gmax: ");
		scanf("%d", &gmax);
		imshow("SRC", src);
		//histograma imagine sursa
		showHistogram("Sursa", hist, 256, src.rows);
		contrast = contrastImg(src, gmin, gmax, imin, imax);
		int* histContrast = histograma(contrast);
		showHistogram("Histograma Contrast", histContrast, 256, src.rows);
		imshow("Contrast", contrast);
		break;
	}
	}
}

void lab8_4(Mat_<uchar> src)
{
	Mat_<uchar> dst = Mat(src.rows, src.cols, CV_8UC1);
	int* hist = (int*)calloc(256, sizeof(int));
	float* fdp = (float*)calloc(256, sizeof(float));
	float* fdpc = (float*)calloc(256, sizeof(float));


	hist = histograma(src);

	fdp = lab3_pct2(src);


	fdpc[0] = fdp[0];
	for (int i = 1; i < 256; i++)
	{
		fdpc[i] = fdpc[i - 1] + fdp[i];
	}


	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst(i, j) = (int)(255 * fdpc[src(i, j)]);
		}
	}


	int* histf = (int*)calloc(256,sizeof(int));
	histf=histograma(dst);


	showHistogram("Histograma initiala", hist, 256, 250);
	showHistogram("Histograma egalizata", histf, 256, 250);
	imshow("SRC", src);
	imshow("DST", dst);
}


float factorScalare(Mat_<float> filtru)
{
	int dimensiune = filtru.rows;
	float coef = 0.0f;
	for (int u = 0; u < dimensiune; u++)
	{
		for (int v = 0; v < dimensiune; v++)
		{
			coef += filtru(u,v);
		}
	}
	if (coef == 0.0f)
	{
		return 1.0f;
	}
	return coef;
}


void afisare(Mat_<int> dest,int dimensiune)
{
	int k = (dimensiune - 1) / 2;
	int ok = 0;
	for (int i = k; i < dest.rows-k; i++)
	{
		for (int j = k; j < dest.cols-k; j++)
		{
			if (dest(i, j) < 0)
			{
				ok = 1;
				break;
			}
		}
	}
	//printf("%d", ok);
	Mat_<uchar> dest1 = Mat_<uchar>(dest.rows, dest.cols);
	for (int i = 0; i < dest.rows; i++)
	{
		for (int j = 0; j < dest.cols; j++)
		{
			if (ok==0)
			{
				if (dest(i, j) < 0)
				{
					dest1(i, j) = 0;
				}
				else if (dest(i, j) > 255)
				{
					dest1(i, j) = 255;
				}
				else
				{
					dest1(i, j) = (uchar)dest(i, j);
				}
			}
			else
			{
				int aux= dest(i, j) + 128;
				if (aux < 0)
				{
					dest1(i, j) = 0;
				}
				else if (aux > 255)
				{
					dest1(i, j) = 255;
				}
				else
				{
					dest1(i, j) = aux;
				}
			}
		}
	}
	imshow("Destinatie", dest1);

}

Mat_<int> convolutie(Mat_<uchar> src, Mat_<float> filtru)
{
	float factor = factorScalare(filtru);
	int dimensiune = filtru.rows;
	int k = (dimensiune - 1) / 2;

	Mat_<int> dest=Mat_<int>(src.rows, src.cols,-1);
	
	printf("Factor=%f\n", factor);
	for (int i = k; i < dest.rows-k; i++)
	{
		for (int j = k; j < dest.cols-k; j++)
		{
			int s = 0;
			for (int u = 0; u < dimensiune; u++)
			{
				for (int v = 0; v < dimensiune; v++)
				{
					s += filtru(u,v) * src(i + u - k,j + v - k);
				}
			}
			dest(i, j) = s/factor;
		}
	}

	return dest;
}


void lab9_1high(Mat_<uchar> src, int **filtru, int dimensiune)
{

	int k = (dimensiune - 1) / 2;

	Mat_<uchar> dest=Mat(src.rows - 2 * k, src.cols - 2 * k, CV_8UC1);
	//coeficient scalare trece-jos
	int coefpoz = 0;
	int coefneg = 0;
	for (int u = 0; u < dimensiune; u++)
	{
		for (int v = 0; v < dimensiune; v++)
		{
			if (filtru[u][v] >= 0)
			{
				coefpoz += filtru[u][v];
			}
			else
			{
				coefneg += abs(filtru[u][v]);
			}
		}
	}
	//printf("\n%d  %d   %d", coefpoz, coefneg,dimensiune);
	int maxim = 0;
	if (coefpoz >= coefneg)
	{
		maxim = coefpoz;
	}
	else
	{
		maxim = abs(coefneg);
	}
	float S = (1.0f / (2 * maxim));
	for (int i = 0; i < dest.rows; i++)
	{
		for (int j = 0; j < dest.cols; j++)
		{
			int s = 0;
			for (int u = 0; u < dimensiune; u++)
			{
				for (int v = 0; v < dimensiune; v++)
				{
					s += filtru[u][v] * src(i + u - k, j + v - k);
				}
			}
			
			dest(i, j) =(uchar)( s * S + 127);
			
		}
	}


	imshow("SRC", src);
	imshow("Trece-sus", dest);

}

void centering_transform(Mat img) {
	// imaginea trebuie să aibă elemente de tip float
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}

void logaritmMagnitudine(Mat_<float> mag)
{
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			mag(i, j) = log(mag(i, j) + 1);
		}
	}
	float min = 256, max = 0;
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			if (mag(i, j) > max)
			{
				max = mag(i, j);
			}
			if (mag(i, j) < min)
			{
				min = mag(i, j);
			}
		}
	}

	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			mag(i, j) = (mag(i, j) - min) / (max - min);
		}
	}
	imshow("LogMag", mag);
}

Mat_<float> trece_jos(Mat_<float> mag)
{
	Mat_<float> dest = Mat(mag.rows, mag.cols, CV_8UC1);
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			if ((mag.rows / 2 - i) * (mag.rows / 2 - i) + (mag.cols / 2 - j) * (mag.cols / 2 - j) <= 400)
			{
				dest(i, j) = mag(i, j);
			}
			else
			{
				dest(i, j) = 0;
			}
		}
	}
	return dest;
}


Mat_<float> trece_sus(Mat_<float> mag)
{
	Mat_<float> dest = Mat(mag.rows, mag.cols, CV_8UC1);
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			if ((mag.rows / 2.0f - i) * (mag.rows / 2.0f - i) + (mag.cols / 2.0f - j) * (mag.cols / 2.0f - j) > 400)
			{
				dest(i, j) = mag(i, j);
			}
			else
			{
				dest(i, j) = 0;
			}
		}
	}
	return dest;
}


Mat_<float> gaussian_trece_sus(Mat_<float> mag)
{
	Mat_<float> dest = Mat(mag.rows, mag.cols, CV_8UC1);
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			dest(i, j) = mag(i, j) *(1.0f - exp(-((mag.rows / 2.0f - i) * (mag.rows / 2.0f - i) + (mag.cols / 2.0f - j) * (mag.cols / 2.0f - j)) / 400.0f));
		}
	}
	return dest;
}

Mat_<float> gaussian_trece_jos(Mat_<float> mag)
{
	Mat_<float> dest = Mat(mag.rows, mag.cols, CV_8UC1);
	for (int i = 0; i < mag.rows; i++) {
		for (int j = 0; j < mag.cols; j++) {
			dest(i, j) = mag(i, j) * exp(-(((mag.rows / 2.0f - i) * (mag.rows / 2.0f - i) + (mag.cols / 2.0f - j) * (mag.cols / 2.0f - j)) / 400.0f));
		}
	}
	return dest;
}

Mat_<uchar> mergeChanels(Mat channels[], Mat fourier)
{
	//aplicarea transformatei Fourier inversă și punerea rezultatului în dstf
	Mat_<uchar> dst;
	Mat_<float> dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	//transformarea de centrare inversă
	centering_transform(dstf);
	//normalizarea rezultatului în imaginea destinație
	normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	//Notă: normalizarea distorsionează rezultatul oferind o afișare îmbunătățită în intervalul
	//[0,255]. Dacă se dorește afișarea rezultatului cu exactitate (vezi Activitatea 3) se va
	//folosi în loc de normalizare conversia:
	//dstf.convertTo(dst, CV_8UC1);
	return dst;
}

Mat generic_frequency_domain_filter(Mat src) {
	//imaginea trebuie să aibă elemente de tip float
	Mat srcf;
	src.convertTo(srcf, CV_32FC1);
	//transformarea de centrare
	centering_transform(srcf);
	//aplicarea transformatei Fourier, se obține o imagine cu valori numere complexe
	Mat fourier;
	dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
	//divizare în două canale: partea reală și partea imaginară
	Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	split(fourier, channels); // channels[0] = Re(DFT(I)), channels[1] = Im(DFT(I))
	//calcularea magnitudinii și fazei în imaginile mag, respectiv phi, cu elemente de tip float
	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);
	phase(channels[0], channels[1], phi);

	Mat dst;
	Mat channels1[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	Mat channels2[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	Mat channels3[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	Mat channels4[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	//alegerea functiei care sa fie aplicata
	printf("Functii aplicabile:\n");
	printf("1. Fourier.\n");
	printf("2. Logaritmul magnitudinii.\n");
	printf("3. Filtru secvential trece-jos.\n");
	printf("4. Filtru secvential trece-sus.\n");
	printf("5. Filtru Gaussian trece-jos.\n");
	printf("6. Filtru Gaussian trece-sus.\n");
	printf("Introduceti numarul functie care sa fie apelata: ");
	int nr;
	scanf("%d", &nr);
	switch (nr) {
	case 1:
		dst = mergeChanels(channels, fourier);
		imshow("SRC", src);
		imshow("DST", dst);
		break;
	case 2:
		//aici afișați imaginile cu fazele și magnitudinile
		imshow("SRC", src);
		logaritmMagnitudine(mag);
		break;
	case 3:
		split(fourier, channels1);
		channels1[0] = trece_jos(channels1[0]);
		channels1[1] = trece_jos(channels1[1]);
		imshow("SRC", src);
		imshow("F_jos", mergeChanels(channels1, fourier));
		break;
	case 4:
		split(fourier, channels2);
		channels2[0] = trece_sus(channels2[0]);
		channels2[1] = trece_sus(channels2[1]);
		imshow("SRC", src);
		imshow("F_sus", mergeChanels(channels2, fourier));
		break;
	case 5:
		split(fourier, channels3);
		channels3[0] = gaussian_trece_jos(channels3[0]);
		channels3[1] = gaussian_trece_jos(channels3[1]);
		imshow("SRC", src);
		imshow("G_jos", mergeChanels(channels3, fourier));
		break;
	case 6:
		split(fourier, channels4);
		channels4[0] = gaussian_trece_sus(channels4[0]);
		channels4[1] = gaussian_trece_sus(channels4[1]);
		imshow("SRC", src);
		imshow("G_sus", mergeChanels(channels4, fourier));
		break;
	}
	
	return dst;
}

Mat_<int> creareFiltru(int v[],int dim)
{
	Mat_<int> filtru(dim, dim);
	int k1 = 0;
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			filtru(i, j) = v[k1];
			//printf("%d  ", v[k1]);
			k1++;
		}
	}
	return filtru;
}

Mat_<int> alegereFiltru()
{
	printf("Filtre disponibile:\n");
	printf("1. Filtru medie aritmetica.\n");
	printf("2. Filtru Gaussian.\n");
	printf("3. Filtru Laplace 1.\n");
	printf("4. Filtru Laplace 2\n");
	printf("5. Filtru high-pass1\n");
	printf("6. Filtru hish-pass2\n");
	printf("7. Filtru oarecare.\n");
	printf("Introduceti filtrul dorit: ");

	int o;
	scanf("%d", &o);
	Mat_<int> filtru(3, 3);
	Mat_<int> oarecare;
	//medie aritmetica
	int v1[] = { 1,1,1,1,1,1,1,1,1 };
	//Gaussian
	int v3[] = { 1,2,1,2,4,2,1,2,1 };
	//Laplace
	int v[] = { 0,-1,0,-1,4,-1,0,-1,0 };
	//Laplace2
	int v2[] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	//high-pass1
	int v4[] = { 0,-1,0,-1,5,-1,0,-1,0 };
	//high-pass1
	int v5[] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	//oarecare
	int v6[] = { -1,-1,-1,-1,-1,-1,-1,-1,-1 };
	int k6 = 0;

	switch (o) {
	case 1:
		filtru = creareFiltru(v1,3);
		break;
	case 2:
		
		filtru = creareFiltru(v3,3);
		break;

	case 3:
		
		filtru = creareFiltru(v,3);
		break;

	case 4:
		
		filtru = creareFiltru(v2,3);
		break;
	case 5:
		
		filtru = creareFiltru(v4,3);
		break;

	case 6:
		
		filtru = creareFiltru(v5,3);
		break;
	case 7:
		
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				oarecare(i, j) = v6[k6];
				k6++;
			}
		}
		return oarecare;
		break;
	}
	return filtru;
}

void filtruMedian(Mat_<uchar> src, int dimFiltru)
{
	int k = (dimFiltru - 1) / 2;
	Mat_<uchar> dest = Mat_<int>(src.rows, src.cols, -1);
	int dim = dimFiltru * dimFiltru;
	for (int i = k; i < dest.rows - k; i++)
	{
		for (int j = k; j < dest.cols - k; j++)
		{
			int* v1 = (int*)calloc(dim, sizeof(int));
			int poz = 0;
			for (int u = 0; u < dimFiltru; u++)
			{
				for (int v = 0; v < dimFiltru; v++)
				{
					v1[poz] = src(i + u - k, j + v - k);
					poz++;
				}
			}

			std::sort(v1, v1 + dim);
			dest(i, j) = v1[dim / 2 ];
		}
	}
	imshow("Src", src);
	imshow("Dest", dest);
}

void lab10_1(Mat_<uchar> src)
{
	printf("Introduceti dimensiunea filtrului(3, 5 sau 7): ");
	int dimensiune;
	scanf("%d", &dimensiune);
	double t = (double)getTickCount(); // Găsește timpul curent [ms]
	filtruMedian(src, dimensiune);
	// Găsește timpul current din nou și calculează timpul scurs [ms]
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Afișarea la consolă a timpului de procesare [ms]
	printf("Time = %.3f [ms]\n", t * 1000);
}

Mat_<float> filtruGaussian(Mat_<uchar> src, int w)
{
	float sigma = w / 6.0f;
	int x0 = w / 2;
	int y0 = w / 2;
	Mat_<float> G(w,w);
	for (int u = 0; u < w; u++)
	{
		for (int v = 0; v < w; v++)
		{
			float ex = 1.0f / (2.0f * PI * (float)sigma * (float)sigma);
			float ex1= exp(-(((u - x0) * (u - x0) + (v - y0) * (v - y0)) / (2.0f * (float)sigma * (float)sigma)));
			G[u][v] = ex * ex1;
			//printf("%f  ", G[u][v]);
		}
	}
	return G;
}


void convolutie1(Mat_<uchar> src,float* Gx,float factor,int dimensiune)
{
	int k = (dimensiune - 1) / 2;

	Mat_<int> dest = Mat_<int>(src.rows, src.cols, -1);

	printf("Factor=%f\n", factor);
	for (int i = k; i < dest.rows - k; i++)
	{
		for (int j = k; j < dest.cols - k; j++)
		{
			int s = 0;
			for (int u = 0; u < dimensiune; u++)
			{
				s += Gx[u] * src(i + dimensiune/2 - k, j + u - k);
				s += Gx[u] * src(i + u - k, j + dimensiune/2 - k);
			}
			dest(i, j) = s / factor;
		}
	}

	imshow("SRC", src);
	afisare(dest, dimensiune);
	//imshow("Filtrata", dest);
}


void filtruGaussianVectorial(Mat_<uchar> src, int w)
{
	float sigma = w / 6.0f;
	int x0 = w / 2;
	int y0 = w / 2;
	//Mat_<float> G(w, w);
	float s = 0.0f;
	float* Gx = (float*)malloc(w * sizeof(float));
	float* Gy = (float*)malloc(w * sizeof(float));
	for (int u = 0; u < w; u++)
	{
		float ex = 1.0f / sqrt(2.0f * PI * (float)sigma * (float)sigma);
		float ex1 = exp(-((u - x0) * (u - x0) / (2.0f * (float)sigma * (float)sigma)));
		Gx[u] = ex * ex1;
		s += Gx[u];
	}
	convolutie1(src, Gx, 2 * s, w);
	
}

void lab10_2(Mat_<uchar> src)
{
	printf("Introduceti dimensiunea filtrului Gaussian(3, 5 sau 7): ");
	int dimensiune;
	scanf("%d", &dimensiune);
	double t = (double)getTickCount(); // Găsește timpul curent [ms]
	Mat_<float> filtru=filtruGaussian(src, dimensiune);
	imshow("SRC", src);
	afisare(convolutie(src, filtru), filtru.rows);
	// Găsește timpul current din nou și calculează timpul scurs [ms]
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Afișarea la consolă a timpului de procesare [ms]
	printf("Time = %.3f [ms]\n", t * 1000);
}


void lab10_3(Mat_<uchar> src)
{
	printf("Introduceti dimensiunea filtrului Gaussian(3, 5 sau 7): ");
	int dimensiune;
	scanf("%d", &dimensiune);
	double t = (double)getTickCount(); // Găsește timpul curent [ms]
	filtruGaussianVectorial(src, dimensiune);
	// Găsește timpul current din nou și calculează timpul scurs [ms]
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Afișarea la consolă a timpului de procesare [ms]
	printf("Time = %.3f [ms]\n", t * 1000);
}

void afisareModulCorectie(Mat_<uchar> modul)
{
	Mat_<uchar> modulCorectie(modul.rows, modul.cols, -1);
	
	for (int i = 0; i < modul.rows; i++)
	{
		for (int j = 0; j < modul.cols; j++)
		{
			modulCorectie(i,j) = modul(i,j) / (4 * sqrt(2));
			if (modulCorectie(i,j) > 255)
			{
				modulCorectie(i, j) = 255;
			}
			else if (modulCorectie(i, j) < 0)
			{
				modulCorectie(i, j) = 0;
			}
		}

	}
	imshow("Modul", modulCorectie);

}

Mat_<uchar> directieGradient(Mat_<uchar> src1, Mat_<uchar> src2, int corectie)
{
	
	Mat_<uchar> directie(src1.rows, src1.cols, -1);
	for (int i = 0; i < src1.rows; i++)
	{
		for (int j = 0; j < src1.cols; j++)
		{

				//directie(i, j) = at + corectie;
				int dir;
				float teta = atan2((float)src2(i,j), (float)src1(i,j));
				if ((teta > 3 * PI / 8 && teta < 5 * PI / 8) || (teta > -5 * PI / 8 && teta < -3 * PI / 8)) dir = 0;
				if ((teta > PI / 8 && teta < 3 * PI / 8) || (teta > -7 * PI / 8 && teta < -5 * PI / 8)) dir = 1;
				if ((teta > -PI / 8 && teta < PI / 8) || teta > 7 * PI / 8 && teta < -7 * PI / 8) dir = 2;
				if ((teta > 5 * PI / 8 && teta < 7 * PI / 8) || (teta > -3 * PI / 8 && teta < -PI / 8)) dir = 3;
				/*if (dir != 0)
				{
					directie(i, j) = dir;
				}
				else
				{
					directie(i, j) = dir+128;
				}*/
				directie(i, j) = dir;
		}
	}

	//imshow("Directie", directie);
	return directie;
}

Mat_<uchar> modulGradient(Mat_<uchar> src1, Mat_<uchar> src2, int corectie)
{
	Mat_<uchar> modulBinarizat(src1.rows, src1.cols, -1);
	Mat_<uchar> modul(src1.rows, src1.cols, -1);
	
	for (int i = 0; i < src1.rows; i++)
	{
		for (int j = 0; j < src1.cols; j++)
		{
			float val = sqrt(pow(src1(i, j), 2) + pow(src2(i, j), 2));
			modul(i, j) = val;
			if (val > 255)
			{
				modulBinarizat(i, j) = 255;
			}
			else
			{
				if (val > 50)
				{
					modulBinarizat(i, j) = 255;
				}
				else
				{
					modulBinarizat(i, j) = 0;
				}
			}
		}
	}
	
	//imshow("Modul binarizat", modul);
	return modul;
}


typedef struct matrici {
	Mat_<uchar> dest1;
	Mat_<uchar> dest2;
}Matrici;

Matrici lab11_1_2_3(Mat_<uchar> src,int o)
{
	int Prewitt_x[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int Prewitt_y[9] = { 1, 1, 1, 0, 0, 0, -1, -1, -1 };
	int Sobel_x[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int Sobel_y[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int Roberts_x[4] = { 1, 0, 0, -1 };
	int Roberts_y[4] = { 0, -1, 1, 0 };

	Mat_<uchar> dest1 = Mat_<int>(src.rows, src.cols, -1);
	Mat_<uchar> dest2 = Mat_<int>(src.rows, src.cols, -1);

	Matrici m;

	switch (o)
	{
	case 1:
		dest1 = convolutie(src, creareFiltru(Prewitt_x, 3));
		dest2 = convolutie(src, creareFiltru(Prewitt_y, 3));
		imshow("Src", src);
		imshow("Prewitt_x", dest1);
		imshow("Prewitt_y", dest2);
		directieGradient(dest1, dest2,0);
		modulGradient(dest1, dest2, 0);
		break;
	case 2:
		dest1 = convolutie(src, creareFiltru(Sobel_x, 3));
		dest2 = convolutie(src, creareFiltru(Sobel_y, 3));
		imshow("Src", src);
		imshow("Sobel_x", dest1);
		imshow("Sobel_y", dest2);
		directieGradient(dest1, dest2, 0);
		modulGradient(dest1, dest2, 0);
		break;
	case 3:
		dest1 = convolutie(src, creareFiltru(Roberts_x, 2));
		dest2 = convolutie(src, creareFiltru(Roberts_y, 2));
		imshow("Src", src);
		imshow("Roberts_x", dest1);
		imshow("Roberts_y", dest2);
		directieGradient(dest1, dest2, 135);
		modulGradient(dest1, dest2, 135);
		break;
	}
	return m;
}

Mat_<uchar> suprimareMaxime(Mat directie, Mat modul) {

	int dx[] = { -1,-1,0,1,1,1,0,-1 };
	int dy[] = { 0,1,1,1,0,-1,-1,-1 };
	int height = modul.rows;
	int width = modul.cols;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uchar pixel = directie.at<uchar>(i, j);
			int n1X = i + dy[pixel];
			int n1Y = j + dx[pixel];
			int n2X = i + dy[(pixel + 4) % 8];
			int n2Y = j + dx[(pixel + 4) % 8];
			bool isGreater = true;
			if (insideImage(directie,n1X, n1Y)) {
				if (modul.at<uchar>(i, j) <= modul.at<uchar>(n1X, n1Y))
					isGreater = false;
			}
			if (insideImage(directie,n2X, n2Y)) {
				if (modul.at<uchar>(i, j) <= modul.at<uchar>(n2X, n2Y))
					isGreater = false;
			}
			if (!isGreater) {
				modul.at<uchar>(i, j) = 0.0;
			}
		}

	}
	//imshow("Suprimare", modul);
	return modul;
}

Mat_<uchar> lab11_4(Mat_<uchar> src)
{
	//suprimare maxime
	Mat_<float> filtru=filtruGaussian(src, 3);
	Mat_<uchar> img = convolutie(src, filtru);

	int Sobel_x[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int Sobel_y[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

	Mat_<uchar> dest1 = Mat_<int>(src.rows, src.cols, -1);
	Mat_<uchar> dest2 = Mat_<int>(src.rows, src.cols, -1);

	dest1 = convolutie(img, creareFiltru(Sobel_x, 3));
	dest2 = convolutie(img, creareFiltru(Sobel_y, 3));

	Mat_<uchar> modul(src.rows, src.cols, -1);
	Mat_<uchar> directie(src.rows, src.cols, -1);

	directie=directieGradient(dest1, dest2, 0);
	modul=modulGradient(dest1, dest2, 0);

	modul /= (4 * sqrt(2));
	
	//imshow("X", dest1);
	//imshow("Y", dest2);
	//imshow("Src", src);
	Mat_<uchar> suprimare=suprimareMaxime(directie, modul);
	return suprimare;
}

int binarizareAdaptiva(Mat_<uchar> modul)
{
	int nul = 0;

	for (int i = 0; i < modul.rows; i++) {
		for (int j = 0; j < modul.cols; j++) {
			if (modul(i, j) == 0)
			{
				nul++;
			}
		}
	}
	float p = 0.1f;
	int dimensiune = (modul.rows-2) * (modul.cols-2);
	int NrPuncteMuchie = p * (dimensiune - nul);

	int hist[256] = { 0 };
	for (int i = 1; i < modul.rows-1; i++) {
		for (int j = 1; j < modul.cols-1; j++) {
			hist[modul(i, j)]++;
		}
	}

	int NrNonMuchie = (1 - p) * (dimensiune - hist[0]);

	//printf("   %d\n", hist[0]);

	//printf("nonmuchii  %d\n", NrNonMuchie);

	int sum = 0;
	int k=1;
	for (k ; k < 256; k++) {
		sum =sum + hist[k];
		if (sum > NrNonMuchie)
		{
			return k;
		}
		
	}

	return k;

}

void binarizareModul(Mat_<uchar> modul, int prag)
{
	Mat_<uchar> dest = Mat_<int>(modul.rows, modul.cols, -1);

	for (int i = 1; i < modul.rows - 1; i++) {
		for (int j = 1; j < modul.cols - 1; j++) {
			if (modul(i, j) <= prag)
			{
				dest(i, j) = 0;
			}
			else
			{
				dest(i, j) = 255;
			}
		}
	}
	imshow("Modul binarizat", dest);
}

void prelungireMuchii(Mat_<uchar> modul, int prag)
{
	int pragInalt = prag;
	float k = 0.4f;
	int pragCoborat = k * pragInalt;

	Mat_<uchar> final = Mat_<int>(modul.rows, modul.cols, -1);
	Mat_<uchar> inalt = Mat_<int>(modul.rows, modul.cols, -1);
	Mat_<uchar> scazut = Mat_<int>(modul.rows, modul.cols, -1);

	for (int i = 1; i < modul.rows - 1; i++) {
		for (int j = 1; j < modul.cols - 1; j++) {
			if (modul(i, j) >= pragInalt)
			{
				inalt(i, j) = 255;
				final(i, j) = 255;
			}
			else if (modul(i, j) >= pragCoborat && modul(i, j) < pragInalt)
			{
				scazut(i, j) = 128;
				final(i, j) = 128;
			}
		}
	}

	imshow("Muchii tari", inalt);
	imshow("Muchii slabe", scazut);

	std::queue<Point> Q;
	int di[8] = { -1,0,1,0,-1,-1,1,1 };
	int dj[8] = { 0,-1,0,1,-1,1,-1,1 };

	for (int i = 0; i < final.rows; i++)
	{
		for (int j = 0; j < final.cols; j++)
		{
			if (final(i, j) == 255)
			{
				Q.push(Point(i, j));
				while (!(Q.empty()))
				{
					Point q = Q.front();
					Q.pop();
					for (int k = 0; k < 8; k++)
					{
						if (insideImage(final, q.x + di[k], q.y + dj[k]))
						{
							if (final((q.x + di[k]), q.y + dj[k]) == 128)
							{
								final((q.x + di[k]), q.y + dj[k]) = 255;
								Q.push(Point(q.x + di[k], q.y + dj[k]));
							}
						}
					}
				}
			}

		}
	}
	for (int i = 0; i < final.rows; i++)
	{
		for (int j = 0; j < final.cols; j++)
		{
			if (final(i, j) == 128)
			{
				final(i, j) = 0;
			}
		}
	}

	imshow("Canny Final", final);
}
void lab12_1Canny(Mat_<uchar> src)
{
	Mat_<uchar> modul = lab11_4(src);
	imshow("Modul", modul);

	int pragAdaptiv=binarizareAdaptiva(modul);
	printf("        %d", pragAdaptiv);

	binarizareModul(modul, pragAdaptiv);

	prelungireMuchii(modul, pragAdaptiv);

}

void openSeparateColors() {
	char fName[MAX_PATH];
	Mat src, red, green, blue;
	while (openFileDlg(fName))
	{
		src = imread(fName, -1);
		red = Mat(src.rows, src.cols, CV_8UC3);
		green = Mat(src.rows, src.cols, CV_8UC3);
		blue = Mat(src.rows, src.cols, CV_8UC3);
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				blue.at<Vec3b>(i, j)[0] = src.at<Vec3b>(i, j)[0];
				blue.at<Vec3b>(i, j)[1] = 0;
				blue.at<Vec3b>(i, j)[2] = 0;

				green.at<Vec3b>(i, j)[1] = src.at<Vec3b>(i, j)[1];
				green.at<Vec3b>(i, j)[2] = 0;
				green.at<Vec3b>(i, j)[0] = 0;

				red.at<Vec3b>(i, j)[2] = src.at<Vec3b>(i, j)[2];
				red.at<Vec3b>(i, j)[1] = 0;
				red.at<Vec3b>(i, j)[0] = 0;
			}
		}
		imshow("SRC", src);
		imshow("RED", red);
		imshow("GREEN", green);
		imshow("BLUE", blue);

		// Wait until user press some key
		waitKey(0);
	}

}

void allGrayFromColor() {
	char fName[MAX_PATH];
	Mat src, gray;
	while (openFileDlg(fName))
	{
		src = imread(fName, -1);
		gray = Mat(src.rows, src.cols, CV_8UC1);
		
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				gray.at<uchar>(i, j) = (src.at<Vec3b>(i, j)[0]+ src.at<Vec3b>(i, j)[1]+ src.at<Vec3b>(i, j)[2])/3;		
			}
		}
		imshow("SRC", src);
		imshow("GRAY", gray);
		

		// Wait until user press some key
		waitKey(0);
	}

}

float maxim(float a, float b, float c)
{
	if (a > b)
	{
		if (a > c)
		{
			return a;
		}
	}
	else if (b > c)
	{
		return b;
	}
	return c;
}

float minim(float a, float b, float c)
{
	if (a < b)
	{
		if (a < c)
		{
			return a;
		}
	}
	else if (b < c)
	{
		return b;
	}
	return c;
}




void RGBtoHSV() {
	char fname[MAX_PATH];
	openFileDlg(fname);
	Mat_<Vec3b> img = imread(fname, CV_LOAD_IMAGE_COLOR);
	Mat_<uchar> iH(img.rows, img.cols), iS(img.rows, img.cols), iV(img.rows, img.cols);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			Vec3b pixel = img(i, j);
			float r = (float)pixel[2] / 255.0f;
			float g = (float)pixel[1] / 255.0f;
			float b = (float)pixel[0] / 255.0f;
			float M = max(r, max(g, b));
			float m = min(r, min(g, b));
			float C = M - m;
			//Value
			float V = M;

			//Saturation
			float S = 0.0f;
			if (V != 0)
				S = C / V;
			else
				S = 0;

			//Hue
			float H = 0.0f;
			if (C != 0)
			{
				if (M == r)
					H = 60.0f * (g - b) / C;
				if (M == g)
					H = 120.0f + 60.0f * (b - r) / C;
				if (M == b)
					H = 240.0f + 60.0f * (r - g) / C;
			}
			else
				H = 0.0f;
			if (H < 0)
				H = H + 360.0f;

			iH(i, j) = (uchar)(H * 255.0f / 360.0f);
			iS(i, j) = (uchar)(S * 255.0f);
			iV(i, j) = (uchar)(V * 255.0f);

		}
	}

	imshow("image", img);
	imshow("H", iH);
	imshow("S", iS);
	imshow("V", iV);
	waitKey(0);
}

void WhiteBlack() {
	char fName[MAX_PATH];
	Mat src, gray;
	while (openFileDlg(fName))
	{
		src = imread(fName, -1);
		gray = Mat(src.rows, src.cols, CV_8UC1);
		float threshold = 123;
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				if ((src.at<Vec3b>(i, j)[0] + src.at<Vec3b>(i, j)[1] + src.at<Vec3b>(i, j)[2]) / 3 < threshold)
				{
					gray.at<uchar>(i, j) = 0;
				}
				else
				{
					gray.at<uchar>(i, j) = 255;
				}
			}
		}
		imshow("SRC", src);
		imshow("GRAY", gray);


		// Wait until user press some key
		waitKey(0);
	}

}

void createImg() {
	const int rows = 256;
	const int cols = 256;
	Mat img = Mat(rows, cols, CV_8UC3);
	int halfRows = rows / 2;
	int halfCols = cols / 2;

	for (int i = 0; i < halfRows; i++) {
		for (int j = 0; j < halfCols; j++) {
			img.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i, j)[1] = img.at<Vec3b>(i, j)[0] = 255;
		}
	}

	for (int i = halfRows; i < rows; i++) {
		for (int j = 0; j < halfCols; j++) {
			img.at<Vec3b>(i, j)[2] = 255; img.at<Vec3b>(i, j)[1] = img.at<Vec3b>(i, j)[0] = 0;
		}
	}

	for (int i = 0; i < halfRows; i++) {
		for (int j = halfCols; j < cols; j++) {
			img.at<Vec3b>(i, j)[2] = img.at<Vec3b>(i, j)[1] = 255;  img.at<Vec3b>(i, j)[0] = 0;
		}
	}

	for (int i = halfRows; i < rows; i++) {
		for (int j = halfCols; j < cols; j++) {
			img.at<Vec3b>(i, j)[2] = 0;  img.at<Vec3b>(i, j)[1] = 255;  img.at<Vec3b>(i, j)[0] = 0;
		}
	}

	imshow("IMG", img);
	waitKey(0);
}

void createMatFromFloats() {
	float vals[] = { 1,2,3,4,5,6,5,8,4};
	Mat m = Mat(3, 3, CV_32FC1, vals);
	std::cout << m.inv() << std::endl;
	while (true) {
		
		waitKey(0);
	}
}

void minmax(int* v, int size)
{
	int i = 0;
	int j = size - 1;
	while (i != j) {
		printf("%d %d ", v[j], v[i]);
		i++;
		j--;
	}
	if (j == i)
	{
		printf("%d\n", v[i]);
	}
}

int main()
{

	
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - Increment Gray\n");
		printf(" 11 - Multiply Gray\n");
		printf(" 12 - Create 4 color image\n");
		printf(" 13 - Create matrix from float\n");
		printf(" 14 - Create Separate Colors\n");
		printf(" 15 - Create All Gray From Color\n");
		printf(" 16 - Create white-black From Color\n");
		printf(" 17 - RGB TO HSV\n");
		printf(" 18 - Point inside or outside\n");
		printf(" 19 - Histograma\n");
		printf(" 20 - FDP\n");
		printf(" 21 - Show histogram\n");
		printf(" 22 - Histograma cu numar redus de acumulatoare\n");
		printf(" 23 - Determinarea pragurilor multiple\n");
		printf(" 24 - Afisare proprietati figura\n");
		printf(" 25 - Afisare figuri cu anumite proprietati\n");
		printf(" 26 - Eticheta imagine\n");
		printf(" 27 - Eticheta imagine prin 2 treceri\n");
		printf(" 28 - Contur obiect\n");
		printf(" 29 - Directii contur\n");
		printf(" 30 - Afisare contur dupa directii\n");
		printf(" 31 - Dilatare, Eroziune, Deschidere, Inchidere\n");
		printf(" 32 - Apelari repetate ale operatiilor morfologice\n");
		printf(" 33 - Extragerea conturului\n");
		printf(" 34 - Umplerea regiunilor\n");
		printf(" 35 - Media, Deviatia, Histpgrama si Histograma Cumulativa\n");
		printf(" 36 - Binarizare automata\n");
		printf(" 37 - Negativ, Latire/ingustare, Corectia gamma, Modificare luminozitate\n");
		printf(" 38 - Egalizare histograma\n");
		printf(" 39 - Filtrare imagine\n");
		printf(" 40 - Fourier/Logaritm magnitudine/Filtre ideale\n");
		printf(" 41 - Calcularea si afisarea timpului de procesare\n");
		printf(" 42 - Calcularea si afisarea timpului de procesare pentru filtrare cu filtru Gaussian variabil\n");
		printf(" 43 - Calcularea si afisarea timpului de procesare pentru filtrare cu filtru Gaussian variabil vectorial\n");
		printf(" 44 - Calcularea componente orizontale si verticale\n");
		printf(" 45 - Suprimare maxime\n");
		printf(" 46 - Binarizare adaptiva/Prelungirea muchiilor prin histereza\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
		case 1:
			testOpenImage();
			break;
		case 2:
			testOpenImagesFld();
			break;
		case 3:
			testParcurgereSimplaDiblookStyle(); //diblook style
			break;
		case 4:
			//testColor2Gray();
			testBGR2HSV();
			break;
		case 5:
			testResize();
			break;
		case 6:
			testCanny();
			break;
		case 7:
			testVideoSequence();
			break;
		case 8:
			testSnap();
			break;
		case 9:
			testMouseClick();
			break;
		case 10:
			openImgIncrementGray();
			break;
		case 11:
			openImgMultiplyGray();
			break;
		case 12:
			createImg();
			break;
		case 13:
			createMatFromFloats();
			break;
		case 14:
			openSeparateColors();
			break;
		case 15:
			allGrayFromColor();
			break;
		case 16:
			WhiteBlack();
			break;
		case 17:
			RGBtoHSV();
			break;
		case 18:
			char fName[MAX_PATH];
			while (openFileDlg(fName))
			{
				Mat src = imread(fName, CV_8UC1);
				pointInside(src);
				waitKey(0);
			}
			break;
		case 19:
			//int *hist = histograma();
			while (openFileDlg(fName))
			{
				lab3_pct1(imread(fName, CV_LOAD_IMAGE_GRAYSCALE));
				waitKey(0);
			}
			break;
		case 20:
			while (openFileDlg(fName))
			{
				float* fdp = lab3_pct2(imread(fName, CV_LOAD_IMAGE_GRAYSCALE));
				printf("FDP is:\n");
				for (int i = 0; i < 256; i++) {
					printf("%f ", fdp[i]);
				}
				waitKey(0);
			}
			break;
		case 21:
			lab3_pct3();
			break;
		case 22:
			while (openFileDlg(fName))
			{
				int m = 23;
				//scanf("%d", &m);
				Mat_<uchar> src = imread(fName, CV_LOAD_IMAGE_GRAYSCALE);
				lab3_pct4(src, m);
				imshow("SRC", src);
				waitKey(0);
			}
			break;
		case 23:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_LOAD_IMAGE_GRAYSCALE);
				Mat_<uchar> dst = lab3_pct5(src);
				imshow("SRC", src);
				imshow("DST", dst);
				waitKey(0);
			}
			break;
		case 24:
			lab4_1();
			break;
		case 25:
			lab4_2();
			break;
		case 26:
			lab5_1(1);
			break;
		case 27:
			lab5_3(1);
			break;
		case 28:
			//char fName[MAX_PATH];
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab6_1(src);
				waitKey(0);
			}
			break;
		case 29:
			//char fName[MAX_PATH];
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab6_2(src);
				waitKey(0);
			}
			break;
		case 30:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab6_3(src);
				waitKey(0);
			}
			break;
		case 31:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab7_1(src);
				waitKey(0);
			}
			break;
		case 32:
			int n;
			printf("Introduceti numarul de rulari: ");
			scanf("%d", &n);
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab7_2(src, n);
				waitKey(0);
			}
			break;
		case 33:
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab7_3(src);
				waitKey(0);
			}
			break;
		case 34:
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				//Create a window
				namedWindow("My Window", 1);

				//set the callback function for any mouse event
				setMouseCallback("My Window", lab7_4, &src);

				//show the image
				imshow("My Window", src);
				//lab7_4(src);
				waitKey(0);
			}
			break;
		case 35:
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab8_1(src);
				waitKey(0);
			}
			break;
		case 36:
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				int T;
				/*printf("Introduceti T: ");
				scanf("%d", &T);
				int eroare;
				printf("Introduceti eroare: ");
				scanf("%d", &eroare);*/
				lab8_2(src);
				waitKey(0);
			}
			break;
		case 37:
			while (openFileDlg(fName))
			{


				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab8_3(src);
				waitKey(0);
			}
			break;
		case 38:
			while (openFileDlg(fName))
			{

				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab8_4(src);
				waitKey(0);
			}
			break;
		case 39:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				Mat_<int> filtru = alegereFiltru();
				imshow("SRC", src);
				afisare(convolutie(src, filtru), filtru.rows);
				waitKey(0);
			}
			break;
		case 40:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				generic_frequency_domain_filter(src);


				waitKey(0);
			}
			break;
		case 41:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab10_1(src);


				waitKey(0);
			}
			break;
		case 42:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab10_2(src);


				waitKey(0);
			}
			break;
		case 43:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab10_3(src);
				waitKey(0);
			}
			break;
		case 44:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				printf("Filtre disponibile:\n");
				printf("1.Prewitt.\n");
				printf("2.Sobel.\n");
				printf("3.Roberts.\n");
				printf("Introduceti filtrul dorit: ");
				int o;
				scanf("%d", &o);
				lab11_1_2_3(src,o);
				waitKey(0);
			}
			break;
		case 45:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab11_4(src);
				waitKey(0);
			}
			break;
		case 46:
			while (openFileDlg(fName))
			{
				Mat_<uchar> src = imread(fName, CV_8UC1);
				lab12_1Canny(src);
				waitKey(0);
			}
			break;
		}
	}
	while (op!=0);
	return 0;
}



