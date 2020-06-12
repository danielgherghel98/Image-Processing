// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <time.h>

using namespace std;
//TODO x,y sa fie vector
struct Point1 {
	vector<double> point;     // coordinates(x, y), R, G, B, H, S, V
	int cluster;     // a way to represent that a point belongs to a specific cluster
};

//TODO si weigh trebuie tot un vector sa fie
typedef struct weight
{
	vector<float> weights; //
}WEIGHT;

typedef double(*DistanceFunction)(const Point1& p1, const Point1& p2, const WEIGHT& weight);

//deprecated
//============pick k random pixels, the initial centroids ======================
vector<Point1> pick_k_random_points(vector<Point1>& points, const int& k) {

	vector<Point1> centroids;
	srand(time(0));
	for (int i = 0; i < k; i++) {
		centroids.push_back(points.at(rand() % (points.size() - 1)));
	}

	return centroids;
}

void computeCentroids(vector<Point1>& points, const int& k, vector<Point1>& centroids) {

	int** sum = new int* [k];

	int* nPoints = new int[k];
	int size;
	for (int i = 0; i < k; ++i) {
		size = centroids.at(i).point.size();
		sum[i] = new int[size];
	}

	for (int j = 0; j < k; ++j) {
		nPoints[j] = 0;
		size = centroids.at(j).point.size();
		for (int i = 0; i < size; ++i) {
			sum[j][i] = 0;
		}
	}


	// Iterate over points to append data to centroids
	for (int i = 0; i < points.size(); i++) {
		int clusterId = points.at(i).cluster;
		nPoints[clusterId] += 1;
		size = points.at(i).point.size();
		for (int j = 0; j < size; ++j) {
			//printf("%d, %d\n", clusterId, j) ;
			sum[clusterId][j] += points.at(i).point.at(j);
		}
	}

	size = centroids.at(0).point.size();
	// Compute the new centroids
	for (int j = 0; j < size; j++) {
		for (int i = 0; i < k; i++) {
			if (0 != nPoints[i])
			{
				//auto x = sum[centroids.at(i).cluster][j];
				//auto y = nPoints[centroids.at(i).cluster];
				auto x = sum[i][j];
				auto y = nPoints[i];
				centroids.at(i).point.at(j) = x / y;
			}
				
			else
			{
				//auto x = sum[centroids.at(i).cluster][j];
				auto x = sum[i][j];
				centroids.at(i).point.at(j) = x;
			}
				
		}
	}
}


bool sameCentroids(const vector<Point1>& centroids, const vector<Point1>& prviouscentroids, const double& error, const WEIGHT& weighs) {

	int count = 0;

	for (int i = 0; i < centroids.size(); i++)
	{
		double dist = 0.0;
		for (int j = 0; j < centroids.at(i).point.size(); j++) {
			dist += weighs.weights.at(j) * (centroids.at(i).point.at(j) - prviouscentroids.at(i).point.at(j)) * (centroids.at(i).point.at(j) - prviouscentroids.at(i).point.at(j));
		}
		if (sqrt(dist) <= error)
		{
			count++;
		}
	}
	return count == centroids.size();
}

double euclidianDistance(const Point1& p1, const Point1& p2, const WEIGHT& weight) {
	//ponderi la tarsaturile din puncte => distante ponderate
	//vectorul de ponderi constant, trimis ca parametru la euclidianDistance
	double result = 0.0;
	for (int i = 0; i < p1.point.size(); ++i) {
		result += weight.weights.at(i) * ((p1.point.at(i) - p2.point.at(i)) * (p1.point.at(i) - p2.point.at(i)));
	}
	return sqrt(result);
}

/*
	similaritate cosinus, valori intre -1 si 1, -1 insemand ca nu exista similaritate intre cei 2 vectori, 1 insemnand ca sunt exact la fel.

	returnam valoarea negativa a rezultatului din cosinus, pentru ca in functia kmeans, calculam "distanta" cea mai mica, cand sunt la fel
	vectorii, vom returna -1, insemand ca distanta intre cei doi e mica.
*/
double cosineSimilarity(const Point1& p1, const Point1& p2, const WEIGHT& weight)
{
	double result = 0.0;

	double sum_numarator = 0.0;
	double sum_numitor1 = 0.0, sum_numitor2 = 0.0;

	for (int i = 0; i < p1.point.size(); i++)
	{
		sum_numarator += p1.point.at(i) * p2.point.at(i) * weight.weights.at(i);

		sum_numitor1 += p1.point.at(i) * p1.point.at(i) * weight.weights.at(i);

		sum_numitor2 += p2.point.at(i) * p2.point.at(i) * weight.weights.at(i);
	}

	sum_numitor1 = sqrt(sum_numitor1);
	sum_numitor2 = sqrt(sum_numitor2);

	if (0 == sum_numitor1 * sum_numitor2)
	{
		if (sum_numarator > 0)
			result = 1;
		else
			result = -1;
	}
	else
	{
		result = sum_numarator / (sum_numitor1 * sum_numitor2);
	}

	return -result;
}

double L1Norm(const Point1& p1, const Point1& p2, const WEIGHT& weight)
{
	double sum = 0.0;
	for (int i = 0; i < p1.point.size(); i++)
	{
		sum += abs(weight.weights.at(i) * (p1.point.at(i) - p2.point.at(i)));
	}
	return sum;
}

//alegem noi centroizii sa fie unul langa altul ca sa se observe ca se deplaseaza 
//k = 2
vector<Point1> kMeansClustering(vector<Point1>& points1, const int& k, const int& nrRepetitions, const WEIGHT& weights, const double& error, const DistanceFunction distanceFunction) {//the larger nrRepetitions, the better the solution. k-nr of clusters

	vector<Point1> centroids;
	vector<Point1> prviouscentroids;
	vector<Point1>& points = points1;
	int reps = 0;

	for (int i = 0; i < k; ++i) {
		int index = rand() % (points.size());
		centroids.push_back(Point1{ points.at(index).point, i });
	}


	do {

		prviouscentroids = centroids;
		for (int i = 0; i < points.size(); i++)
		{

			double shortest = INFINITY;

			for (int j = 0; j < centroids.size(); j++)
			{
				double dist;
				
				dist = distanceFunction(points.at(i), centroids.at(j), weights); 
				

				if (dist < shortest) {		//if the distance between a point and curent cluster
											//is smaller than distance between this point and previous 
											//cluster, update the point to be part of the 
											//current cluster and the new distance also.
					shortest = dist;
					points.at(i).cluster = centroids.at(j).cluster;

				}
			}
		}
		computeCentroids(points, k, centroids);


		printf("Finished repetition #%d\n", reps);
		reps++;

	} while (reps < nrRepetitions && !sameCentroids(centroids, prviouscentroids, error, weights));
	//after the first iteretion the points will not be equal distributed to each cluster
	//there has to be a second part where the new centroids are computed, done by 
	//calling computeCentroids method
	std::cout << std::endl;
	return centroids;
}


Mat_<Vec3b> generateImage(vector<Point1> points) {
	Mat_<Vec3b> img(100, 100);
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			for (int k = 0; k < points.size(); i++) {

			}

			img(i, j) = 255;
		}
	}

	return img;
}


vector<Point1> extractFeatures(Mat_<Vec3b> src, int mode)
{
	Mat_<uchar> h(src.rows, src.cols);
	Mat_<uchar> s(src.rows, src.cols);
	Mat_<uchar> v(src.rows, src.cols);
	vector<Point1> features;
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {

			float r = (float)src(i, j)[2] / 255;
			float g = (float)src(i, j)[1] / 255;
			float b = (float)src(i, j)[0] / 255;

			float M = max(max(r, g), b);
			float m = min(min(r, g), b);

			float C = M - m;

			float V, S, H;
			V = M;

			if (V) {
				S = C / V;
			}
			else {
				S = 0;
			}

			if (C) {
				if (r == M) {
					H = 60 * (g - b) / C;
				}
				if (g == M) {
					H = 120 + 60 * (b - r) / C;
				}
				if (b == M) {
					H = 240 + 60 * (r - g) / C;
				}
			}
			else {
				H = 0;
			}
			if (H < 0) {
				H = H + 360;
			}

			float H_norm = H * 255 / 360;
			float S_norm = S * 255;
			float V_norm = V * 255;

			h(i, j) = H_norm;
			s(i, j) = S_norm;
			v(i, j) = V_norm;
		}
	}
	if (mode == 1)
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {

				//if(src(i, j)[2]!=0, (double)src(i, j)[1]!=0, (double)src(i, j)[0]!=0)
				{
					vector<double> p{ (double)i ,  (double)j, (double)src(i,j)[2], (double)src(i,j)[1], (double)src(i,j)[0], (double)h(i,j), (double)s(i,j), (double)v(i,j) };
					features.push_back(Point1{ p, 0 });
				}
			}
		}
	else
	{
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {

				if(src(i, j)[2]!=0, (double)src(i, j)[1]!=0, (double)src(i, j)[0]!=0)
				{
					vector<double> p{ (double)i ,  (double)j, (double)src(i,j)[2], (double)src(i,j)[1], (double)src(i,j)[0], (double)h(i,j), (double)s(i,j), (double)v(i,j) };
					features.push_back(Point1{ p, 0 });
				}
			}
		}
	}
	//imshow("src", src);
	return features;

}

//deprecated
vector<Vec3b> getClusterColorPalette() {
	vector<Vec3b> v{ { 0, 0, 0 }, { 0, 0, 255 }, { 0, 255, 255 }, { 0, 255, 0 }, { 255, 255, 0 }, { 255, 0, 255 }, { 128, 0, 128 }, { 255, 0, 0 } , { 0, 128, 0 },
	{ 128, 128, 128 } };
	return v;
}

void generateKMeansResult(vector<Point1> features, vector<Point1> centroids, Mat_<Vec3b> src, std::string dispalyName, int mode) {
	Mat_<Vec3b> dst(src.rows, src.cols, -1);

	for (int i = 0; i < dst.rows; ++i) {
		for (int j = 0; j < dst.cols; ++j) {
			dst(i, j) = { 0, 0, 0 };
		}
	}

	vector<Vec3b> colors;
	vector<Vec3b> v{ { 125, 0, 255 } ,{ 0, 0, 255 }, { 0, 255, 255 }, { 0, 255, 0 }, { 255, 255, 0 }, { 255, 0, 255 }, { 128, 0, 128 }, { 255, 0, 0 } , { 0, 128, 0 },
	{ 128, 128, 128 } };



	for (int i = 0; i < centroids.size(); ++i) {
		colors.push_back(Vec3b(centroids.at(i).point.at(4), centroids.at(i).point.at(3), centroids.at(i).point.at(2)));
	}

	if (mode == 1)
		for (const auto& p : features)
		{
			dst(p.point.at(0), p.point.at(1)) =colors.at(p.cluster);
		}
	else
		for (const auto& p : features)
		{
			dst(p.point.at(0), p.point.at(1)) = v.at(p.cluster);
		}

	imshow(dispalyName, dst);
}

int main()
{

	//const WEIGHT weights{ {0.0f, 0.0f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f, 0.8f} };
	const WEIGHT weights{ {0.6f, 0.6f,1.0f, 1.0f, 1.0f, 1.0f, 1.0f,1.0f } };
	int numberOfRepetitions = 18;
	int K[5] = { 8, 20, 50 ,100, 200 };
	double error = 0.001f;

	char fName[MAX_PATH];
	while (openFileDlg(fName))
	{
		system("cls");
		destroyAllWindows();

		Mat_<Vec3b> src = imread(fName, CV_LOAD_IMAGE_COLOR);
		

		std::cout << "Select Mode - 0 for random colors, 1 for real colors" << std::endl;
		int mode = 1;
		std::cin >> mode;

		vector<Point1> points1 = extractFeatures(src, mode);
		vector<Point1> points2 = points1;
		vector<Point1> points3 = points1;




		int k_input = 0;
		cin.clear();
		std::cout << "Select value K (0 for running k=[8, 20, 50 ,100, 200]):";
		std::cin >> k_input;
		
		if (k_input == 0)
		{
			for (auto k : K)
			{
				std::cout << "Euclidean Distance, " << to_string(k) <<std::endl;
				vector<Point1> centroids = kMeansClustering(points1, k, numberOfRepetitions, weights, error, &euclidianDistance);
				generateKMeansResult(points1, centroids, src, "eu" + to_string(k), mode );

				std::cout << "Cosine Distance, " << to_string(k) << std::endl;
				centroids.clear();
				centroids = kMeansClustering(points2, k, numberOfRepetitions, weights, error, &cosineSimilarity);
				generateKMeansResult(points2, centroids, src, "cos" + to_string(k), mode);

				std::cout << "L1(Manhattan) Distance, " << to_string(k) << std::endl;
				centroids.clear();
				centroids = kMeansClustering(points3, k, numberOfRepetitions, weights, error, &L1Norm);
				generateKMeansResult(points3, centroids, src, "man" + to_string(k), mode);
			}
		}
		else
		{
			std::cout << "Euclidean Distance, " << to_string(k_input) << std::endl;
			vector<Point1> centroids = kMeansClustering(points1, k_input, numberOfRepetitions, weights, error, &euclidianDistance);
			generateKMeansResult(points1, centroids, src, "eu" + to_string(k_input), mode);

			std::cout << "Cosine Distance, " << to_string(k_input) << std::endl;
			centroids.clear();
			centroids = kMeansClustering(points2, k_input, numberOfRepetitions, weights, error, &cosineSimilarity);
			generateKMeansResult(points2, centroids, src, "cos" + to_string(k_input), mode);

			std::cout << "L1(Manhattan) Distance, " << to_string(k_input) << std::endl;
			centroids.clear();
			centroids = kMeansClustering(points3, k_input, numberOfRepetitions, weights, error, &L1Norm);
			generateKMeansResult(points3, centroids, src, "man" + to_string(k_input) , mode);
		}

		namedWindow("src");
		moveWindow("src", 20, 20);
		imshow("src", src);
		cin.clear();
		waitKey(0);
	}

	return 0;
}
