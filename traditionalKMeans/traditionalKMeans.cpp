#include <cmath>
#include <string>
#include <time.h>
#include "pch.h"
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <fstream>
#define DATA_CNT  100000 
#define CLUST_CNT  4 
#define CLOCKS_PER_SEC ((clock_t)1000)

using namespace std;

float euclid_dist(float[], float[]);
int assign_to_clusters();
int find_new_clusters();
float* point_in_circle(float[], float);
void random_clusters();

float points[DATA_CNT][2];
int assigned_cluster[DATA_CNT];
float cluster[CLUST_CNT][2];

void readFile() {
    ifstream inFile("input.txt");
    std::string firstLine;
    std::getline(inFile, firstLine);
    cout << "first Line" << firstLine << endl;
    string xcoordStr;
    int xcoord;
    int ycoord;
    int i = 0;
    while (inFile.good()) {
        std::getline(inFile, xcoordStr, ',');
        //cout << "i is " << i << " xcoordStr is " << xcoordStr << endl;
        try {
            xcoord = std::stoi(xcoordStr);
        }
        catch (std::invalid_argument arg) {
            i--;
            break;
        }
        inFile >> ycoord;
        //cout << "xcoord " << xcoord << "," << ycoord << endl;
        points[i][0] = xcoord;
        points[i][1] = ycoord;
        i++;
    }
    cout << i << " elements read." << endl;
    inFile.close();
}

void outputDataToFile() {
    ofstream outFile("traditionalKMeansSerial.txt");
    std::stringstream jsonCsvStream;
    std::stringstream csvStream;
    jsonCsvStream << '[';
    for (int cluster_index = 0; cluster_index < CLUST_CNT; cluster_index++){
        csvStream << '"';
        for (int i = 0; i < DATA_CNT; i++){
            if (assigned_cluster[i] == cluster_index) {
                csvStream << points[i][0] << "," << points[i][1] << ";";
            }
        }
        csvStream << "\"";
        if (cluster_index < CLUST_CNT - 1) {
            csvStream << ',';
        }
        jsonCsvStream << csvStream.rdbuf();
    }
    jsonCsvStream << ']';
    outFile << jsonCsvStream.rdbuf();
    outFile.close();
}

int main()
{
    int clust_change;
    float* new_point;

    float outer_edge = 100.0;

    clock_t t;
    t = clock();

    cout << endl;

    srand(time(NULL));

    // random points
    //
    /*for (int j = 0; j < DATA_CNT; j++)
    {

        int clust_num = int(rand()*(float)CLUST_CNT / RAND_MAX);

        new_point = point_in_circle(pseudo_centroid[clust_num], pseudo_radius);
        points[j][0] = new_point[0];
        points[j][1] = new_point[1];
    }*/

    // random initial clusters
    //
    readFile();
    random_clusters();

    int pass_no = 1, int_pass_no = 1;
    int pass_limit = 10;

    clust_change = 1;
    while (clust_change == 1)
    {
        cout << "Pass number: " << pass_no << ")   ";

        for (int j = 0; j < CLUST_CNT; j++)
        {
            cout << j << ":(" << cluster[j][0] << "," << cluster[j][1] << ")   ";
        }
        cout << endl;

        if (assign_to_clusters() == 0 || int_pass_no > pass_limit)
        {
            random_clusters();
            int_pass_no = 1;
        }
        else
            clust_change = find_new_clusters();
        pass_no++;
        int_pass_no++;
    }

    t = clock() - t;
    cout << "Total runtime: " << (float)t / CLOCKS_PER_SEC << " seconds." << endl;

    outputDataToFile();
    return 0;
}

int assign_to_clusters()
{
    float min_distance, new_distance;
    int min_cluster;
    int clust_points[CLUST_CNT];

    for (int i = 0; i < CLUST_CNT; i++)
        clust_points[i] = 0;

    for (int i = 0; i < DATA_CNT; i++)
    {
        min_distance = euclid_dist(points[i], cluster[0]);
        min_cluster = 0;
        for (int j = 1; j < CLUST_CNT; j++)
        {
            new_distance = euclid_dist(points[i], cluster[j]);
            if (new_distance < min_distance)
            {
                min_cluster = j;
                min_distance = new_distance;
            }
        }
        assigned_cluster[i] = min_cluster;
        //      clust_points[min_cluster]+=1;
        clust_points[min_cluster] = 1;
    }

    int valid_clustering = 1;
    for (int i = 0; i < CLUST_CNT; i++)
    {
        if (clust_points[i] == 0)
            valid_clustering = 0;
    }

    return valid_clustering;
}

float euclid_dist(float point1[], float point2[])
{
    float calc_distance;

    calc_distance = sqrt(pow(point2[0] - point1[0], 2) + pow(point2[1] - point1[1], 2));
    return calc_distance;
}

void random_clusters()
{
    for (int i = 0; i < CLUST_CNT; i++)
    {
        cluster[i][0] = rand()*100.0 / RAND_MAX;
        cluster[i][1] = rand()*100.0 / RAND_MAX;
    }
    return;
}

int find_new_clusters()
{
    float clust_tot[CLUST_CNT][2];
    int clust_points[CLUST_CNT];
    int clust_changed = 0;
    float new_clust_x, new_clust_y;
    float clust_x, clust_y;
    int clust_found;

    for (int i = 0; i < CLUST_CNT; i++)
    {
        clust_tot[i][0] = 0;
        clust_tot[i][1] = 0;
        clust_points[i] = 0;
    }

    for (int j = 0; j < CLUST_CNT; j++)
    {
        clust_x = 0;
        clust_y = 0;
        clust_found = 0;
        for (int i = 0; i < DATA_CNT; i++)
        {
            if (assigned_cluster[i] == j) {
                clust_x += points[i][0];
                clust_y += points[i][1];
                clust_found += 1;
            }
            clust_tot[j][0] = clust_x;
            clust_tot[j][1] = clust_y;
            clust_points[j] = clust_found;
        }
    }
    for (int i = 0; i < CLUST_CNT; i++)
    {
        new_clust_x = clust_tot[i][0] / clust_points[i];
        new_clust_y = clust_tot[i][1] / clust_points[i];
        if (new_clust_x != cluster[i][0] || new_clust_y != cluster[i][1])
            clust_changed = 1;

        cluster[i][0] = clust_tot[i][0] / clust_points[i];
        cluster[i][1] = clust_tot[i][1] / clust_points[i];
    }
    return clust_changed;
}
