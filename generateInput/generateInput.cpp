
#include "pch.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
using namespace std;

void generatePointsAndOutput(int num_of_points, vector<vector<int>>& centroidCoordinates, vector<int> centroids_radius, ofstream& outFile) {
    random_device rd;
    int num_of_centroids = centroidCoordinates.size();
    int number_of_points_each_centroid = num_of_points / num_of_centroids;
    int number_of_points_last_centroid = number_of_points_each_centroid;
    int accumulated_num_of_points = 0;
    float randomRadius;
    float randomAngle;
    for (int j = 0; j < centroidCoordinates.size(); j++)
    {
        int centroidXvalue = centroidCoordinates.at(j).at(0);
        int centroidYvalue = centroidCoordinates.at(j).at(1);
        int radius = centroids_radius.at(j);
        //cout << "radius is " << radius;
        uniform_real_distribution<> randomAngleGenerator(0, 2 * 3.14);
        uniform_real_distribution<> randomRadiusGenerator(0, radius);

        int num_of_points_this_centroid = number_of_points_each_centroid - 1;
        if (j == centroidCoordinates.size() - 1)
            num_of_points_this_centroid = num_of_points - accumulated_num_of_points;
        accumulated_num_of_points += num_of_points_this_centroid;
        long xPoint;
        long yPoint;
        //cout << "num of points " << num_of_points << endl;
        //cout << "num of points this centroid : " << num_of_points_this_centroid << endl;
        for (int i = 0; i < num_of_points_this_centroid; i++) {
            randomRadius = randomRadiusGenerator(rd);
            randomAngle = randomAngleGenerator(rd);
            //cout << centroidXvalue << "|" << randomRadius << " " << randomAngle << " " << std::cos(randomAngle) <<  " " 
            //    << centroidXvalue + randomRadius * std::cos(randomAngle) << endl;
            xPoint = std::lround(centroidXvalue + randomRadius * std::cos(randomAngle));
            yPoint = std::lround(centroidYvalue + randomRadius * std::sin(randomAngle));
            //cout << xPoint << "," << yPoint << endl;
            outFile << xPoint << "," << yPoint << endl;
        }
    }
}
int main()
{
    ifstream inFile("generatorParams.txt");
    if (!inFile.is_open()) {
        cout << "input file is not open." << endl;
        exit(-1);
    }
    int num_of_centroids = 0;
    int num_of_points = 0;

    vector<int> radius_for_each_centroid(0);
    vector<vector<int>> coordinates_of_each_centroids(num_of_centroids);

    if (inFile >> num_of_centroids) {
        cout << "done reading centroids" << endl;
    }
    inFile >> num_of_points;

    int temporary_coordinate_centroid_value;
    vector<int> coordinates(0);
    for (int i = 0; i < num_of_centroids * 2; i++) {
        if (i % 2 == 0 && i != 0) {
            coordinates_of_each_centroids.push_back(vector<int>(coordinates));
            coordinates.clear();
        }
        inFile >> temporary_coordinate_centroid_value;
        coordinates.push_back(temporary_coordinate_centroid_value);
    }
    coordinates_of_each_centroids.push_back(vector<int>(coordinates));

    /*for (int i = 0; i < num_of_centroids; i++) {
        cout << coordinates_of_each_centroids.at(i).at(0) << " " << coordinates_of_each_centroids.at(i).at(1) << endl;
    }*/
    int temporary_radius;
    for (int i = 0; i < num_of_centroids; i++) {
        inFile >> temporary_radius;
        radius_for_each_centroid.push_back(temporary_radius);
    }

    int avg_num_of_points = num_of_points / num_of_centroids;

    ofstream outFile("input.txt");
    outFile << "x,y" << endl;
    generatePointsAndOutput(num_of_points, coordinates_of_each_centroids, radius_for_each_centroid, outFile);
    outFile.close();


    inFile.close();
}


