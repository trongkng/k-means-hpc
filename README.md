# How to build the executables
Open visual studio and build the solution. All executables will be placed in the Debug folder of the solution folder.

# How to generate input into a text file

In the Debug folder, there is a file called generatorParams.txt. This file outlines the parameters to generate the points and their coordinates.

* Line 1: number of groups (let's call it k)
* Line 2: number of points
* Line 3->k+3: coordinates of each center of each group
* The rest: each line contains the radius for each group.

Each group of points will be a set of points generated in a circle using the same logic of the traditional K means. The difference here is the uniform distribution is actually being used instead of just random() in C.

Run generateInput.exe. It then generates input.txt.

# How to run kmeans
Run traditionalKmeans.exe. This uses the current code of our group with the logic of reading from file added to the project.

Run enhanced-k-means.exe. This uses the enhanced-k-means algorithm in the paper.

At the moment, they are all serial versions.

# How to run the visualization

Currently, this works for the input only.

First, build your solution. All the executables will be placed in the Debug folder of the solution folder.

Change your directory to the Debug folder of the Solution folder.

Run python -m http.server if you are using python3, or python -m SimpleHttpServer if you are using python2.

Access the url: localhost:8000/visualization.html. The computer should have access to the internet.

The visualization reads input.txt and then draws the points on the web browser.

