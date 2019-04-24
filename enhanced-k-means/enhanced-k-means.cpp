// enhanced-k-means.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;
int globalColumnChosen;
class Point {
public:
    double xcoord;
    double ycoord;
    int assigned_cluster;
    Point(int xcoordIn, int ycoordIn, int assigned_clusterIn) : xcoord(xcoordIn), ycoord(ycoordIn), assigned_cluster(assigned_clusterIn) {};
    bool operator > (const Point& point) const
    {
        if (globalColumnChosen == 0) {
            return xcoord > point.xcoord;

        }
        else {
            return ycoord > point.ycoord;
        }
    }

    bool operator < (const Point& point) const
    {
        if (globalColumnChosen == 0) {
            return xcoord < point.xcoord;
        }
        else {
            return ycoord < point.ycoord;
        }
    }
};

class Cluster {
public:
    double xMeanCoord;
    double xSum;
    double yMeanCoord;
    double ySum;
    int numOfPoints;
    Cluster(double xcoordIn, double ycoordIn) : xMeanCoord(xcoordIn), yMeanCoord(ycoordIn), xSum(0), ySum(0), numOfPoints(0) {};
};

void readFile(vector<Point>& points, int& pointCount, int& columnChosen) {
    ifstream inFile("input.txt");
    std::string firstLine;

    // the first line contains column headers, skip this line.
    std::getline(inFile, firstLine);
    cout << "first Line" << firstLine << endl;
    string xcoordStr;

    int xcoord;
    int ycoord;
    int i = 0;

    Point tempCoordinates(0, 0, -1);

    /*Since it is so tempting to optimize to find the maximum range of each column, I do it here
    */
    int maxX = 0;
    int minX = 1000000;
    int maxY = 0;
    int minY = 1000000;

    while (inFile.good()) {
        std::getline(inFile, xcoordStr, ',');
        //cout << "i is " << i << " xcoordStr is " << xcoordStr << endl;
        try {
            xcoord = std::stoi(xcoordStr);
        }
        catch (std::invalid_argument arg) {
            i;
            break;
        }
        inFile >> ycoord;
        //cout << "xcoord " << xcoord << "," << ycoord << endl;

        /*Since it is so tempting to optimize to find the maximum range of each column, I do it here
            This is not related to reading data from input file.
        */
        if (xcoord > maxX) {
            maxX = xcoord;
        }
        if (xcoord < minX) {
            minX = xcoord;
        }
        if (ycoord > maxY) {
            maxY = ycoord;
        }
        if (ycoord < minY) {
            minY = ycoord;
        }
        /* now store the point just read into the vector*/
        tempCoordinates.xcoord = xcoord;
        tempCoordinates.ycoord = ycoord;
        points.push_back(Point(tempCoordinates));
        i++;
    }

    int rangeX = maxX - minX;
    int rangeY = maxY - minY;
    if (rangeX > rangeY) {
        columnChosen = 0;
    }
    else {
        columnChosen = 1;
    }

    cout << i << " elements read." << endl;
    pointCount = i;
    inFile.close();
}

void dividePointsIntoKequalParts(int& k, vector<Point>& pointsVector, vector<Cluster>& clusterCentroidCoordinates) {
    int pointVectorLength = pointsVector.size();
    int sizeOfEachPart = pointVectorLength / k;
    int partIndex = 0;
    double temporaryXmeanCoord;
    double temporaryYmeanCoord;
    int temporaryXsum = 0;
    int temporaryYsum = 0;
    int count = 0;
    //cout << "size of each part " << sizeOfEachPart << endl;
    int lastPointIndexForThisPart = (partIndex + 1) * sizeOfEachPart - 1;
    for (int i = 0; i < pointVectorLength; i++) {
        temporaryXsum += pointsVector[i].xcoord;
        temporaryYsum += pointsVector[i].ycoord;
        if (i == lastPointIndexForThisPart - 1) {
            temporaryXmeanCoord = temporaryXsum / ((double)sizeOfEachPart);
            temporaryYmeanCoord = temporaryYsum / ((double)sizeOfEachPart);

            clusterCentroidCoordinates.push_back( Cluster(temporaryXmeanCoord, temporaryYmeanCoord));
            partIndex++;
            //cout << "partIndex " << partIndex << endl;
            lastPointIndexForThisPart = (partIndex + 1) * sizeOfEachPart - 1;
            temporaryXsum = 0;
            temporaryYsum = 0;
        }
    }
}

float euclid_dist(Point& point1, Cluster& cluster)
{
    float calc_distance;

    calc_distance = sqrt(pow(cluster.xMeanCoord - point1.xcoord, 2) + pow(cluster.yMeanCoord - point1.ycoord, 2));
    return calc_distance;
}

bool assign_to_clusters(vector<Point>& pointsVector, vector<Cluster>& clustersVector)
{
    float min_distance, new_distance;
    int min_cluster;

    int lastCoordinateX, lastCoordinateY;
    int lastOldClusterIndex, lastNewClusterIndex;
    int numberOfChanges = 0;
    bool pointChangeToNewCluster = false;
    for (int i = 0; i < pointsVector.size(); i++)
    {
        Point& point = pointsVector[i];

        min_distance = euclid_dist(pointsVector[i], clustersVector[0]);
        min_cluster = -1;
        if (point.assigned_cluster < 0) {
            clustersVector[0].numOfPoints += 1;
            clustersVector[0].xSum += point.xcoord;
            clustersVector[0].ySum += point.ycoord;
            pointsVector[i].assigned_cluster = 0;
        }
        for (int j = 1; j < clustersVector.size(); j++)
        {
            new_distance = euclid_dist(point, clustersVector[j]);
            if (new_distance < min_distance)
            {
                min_cluster = j;
                min_distance = new_distance;
            }
        }
        if (min_cluster >= 0 && min_cluster != pointsVector[i].assigned_cluster) {
            
            if (pointsVector[i].assigned_cluster >= 0) {
                clustersVector[pointsVector[i].assigned_cluster].numOfPoints -= 1;
                clustersVector[pointsVector[i].assigned_cluster].xSum -= point.xcoord;
                clustersVector[pointsVector[i].assigned_cluster].ySum -= point.ycoord;
            }
            clustersVector[min_cluster].numOfPoints += 1;
            clustersVector[min_cluster].xSum += pointsVector[i].xcoord;
            clustersVector[min_cluster].ySum += pointsVector[i].ycoord;

            lastOldClusterIndex = pointsVector[i].assigned_cluster;
            lastNewClusterIndex = min_cluster;
            pointsVector[i].assigned_cluster = min_cluster;
            lastCoordinateX = pointsVector[i].xcoord;
            lastCoordinateY = pointsVector[i].ycoord;
            min_distance = new_distance;
            pointChangeToNewCluster = true;
            numberOfChanges++;
        }
        //pointsVector[i].assigned_cluster = min_cluster;
    }
    if (pointChangeToNewCluster == true){
        //cout << " point " << lastCoordinateX << "," << lastCoordinateY << " changed cluster from " << lastOldClusterIndex << " to " << lastNewClusterIndex << endl;
        //cout << "number of changes " << numberOfChanges << endl;
    }
    return pointChangeToNewCluster;
}
void sortPointsBasedOnColumnChosen(int& columnChosen, int pointCount, vector<Point>& points) {
    double tempX = 0;
    double tempY = 0;
    /*for (int i = 0; i < pointCount - 1; i++) {
        for (int j = i + 1; j < pointCount; j++) {
            // quick and dirty
            if (columnChosen == 0) {
                if (points[i].xcoord > points[j].xcoord) {
                    tempX = points[i].xcoord;
                    tempY = points[i].ycoord;
                    points[i].xcoord = points[j].xcoord;
                    points[i].ycoord = points[j].ycoord;
                    points[j].xcoord = tempX;
                    points[j].ycoord = tempY;
                }
            }
            else {
                if (points[i].ycoord > points[j].ycoord) {
                    tempX = points[i].xcoord;
                    tempY = points[i].ycoord;
                    points[i].xcoord = points[j].xcoord;
                    points[i].ycoord = points[j].ycoord;
                    points[j].xcoord = tempX;
                    points[j].ycoord = tempY;
                }
            }
        }
        cout << " i is " << i << endl;
    }*/
    std::sort(points.begin(), points.end());
    cout << " sort done " << endl;
}


void executeKmeans(vector<Point>& pointsVector, vector<Cluster>&clustersVector) {

    int clust_change;
    float* new_point;
    clock_t t;
    t = clock();

    cout << endl;

    srand(time(NULL));

    int pass_no = 1;
    int pass_limit = 20;

    clust_change = 1;
    while (true)
    {
        cout << "Pass number: " << pass_no << ")   ";

        for (int j = 0; j < clustersVector.size(); j++)
        {
            cout << j << ":(" << clustersVector[j].xMeanCoord << "," << clustersVector[j].yMeanCoord << ")   ";
        }
        cout << endl;

        if (pass_no > pass_limit) { break; }
        if (assign_to_clusters(pointsVector, clustersVector) == false) {
            {
                break;
            }
        }
        else {
            for (int i = 0; i < clustersVector.size(); i++) {
                if (clustersVector[i].numOfPoints > 0) {
                    clustersVector[i].xMeanCoord = clustersVector[i].xSum / clustersVector[i].numOfPoints;
                    clustersVector[i].yMeanCoord = clustersVector[i].ySum / clustersVector[i].numOfPoints;
                }
            }
        }
        pass_no++;

    }

    t = clock() - t;
    cout << "Total runtime: " << (float)t / CLOCKS_PER_SEC << " seconds." << endl;
}
int main()
{
    vector<Point> pointsVector;
    vector<Cluster> clusterCentroidCoordinates;
    int pointCount;

    // Please read the algorithm Enhanced K-means in the paper to understand the use of this variable
    int columnChosen;
    
    int clusterCount = 4;

    readFile(pointsVector, pointCount, columnChosen);
    globalColumnChosen = columnChosen;
    sortPointsBasedOnColumnChosen(columnChosen, pointCount, pointsVector);
    dividePointsIntoKequalParts(clusterCount,pointsVector,clusterCentroidCoordinates);
    executeKmeans(pointsVector, clusterCentroidCoordinates);
}

