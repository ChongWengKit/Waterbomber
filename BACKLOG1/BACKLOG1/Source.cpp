#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <math.h>
#include <mutex>

using namespace std;
std::mutex mtx;
const int INF = 1e9;

// Represents a point on the terrain
struct Point {
    string type; // "Airport", "Fire", or "Tree"
    double x;
    double y;
    bool operator==(const Point& other) const {
        return (type == other.type && x == other.x && y == other.y);
    }

};

// Computes the Euclidean distance between two points
double distance(Point p1, Point p2) {
    //return Euclidean distance
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}
double rectangleArea(Point corner1, Point corner2, Point corner3, Point corner4) {
    //calculate rectangle euclidean distance between width and length
    double d1 = distance(corner1, corner2);

    double d2 = distance(corner2, corner3);
    //width*length
    return d1 * d2;
}
bool isInsideRectangle(Point p, Point corner1, Point corner2, Point corner3, Point corner4) {
    //calculate rectangle euclidean distance between width and length

    double d1 = sqrt(pow(corner2.x - corner1.x, 2) + pow(corner2.y - corner1.y, 2) * 1.0);
    double d2 = sqrt(pow(corner3.x - corner2.x, 2) + pow(corner3.y - corner2.y, 2) * 1.0);
    //width*length

    double area = d1 * d2;
    //calculate 4 triangles by using two coordinate of rectangle and the point
    double area1 = 0.5 * abs(corner1.x * corner2.y + corner2.x * p.y + p.x * corner1.y - corner2.x * corner1.y - p.x * corner2.y - corner1.x * p.y);
    double area2 = 0.5 * abs(corner2.x * corner3.y + corner3.x * p.y + p.x * corner2.y - corner3.x * corner2.y - p.x * corner3.y - corner2.x * p.y);
    double area3 = 0.5 * abs(corner3.x * corner4.y + corner4.x * p.y + p.x * corner3.y - corner4.x * corner3.y - p.x * corner4.y - corner3.x * p.y);
    double area4 = 0.5 * abs(corner4.x * corner1.y + corner1.x * p.y + p.x * corner4.y - corner1.x * corner4.y - p.x * corner1.y - corner4.x * p.y);
    //difference between area of 4 triangles and area of actual rectangle
    if ((fabs(area - (area1 + area2 + area3 + area4))) < 0.0001) {
        return true;
    }
    else {
        return false;
    }

}
bool isNotCovered(vector<Point> fires, Point fire) {
        //search fires vector
        for (int i = 0; i < fires.size(); i++) {
            if (fires[i].type == fire.type) {
                return false;

            }

        }
    

    return true;
}
bool isNotTemp(vector<Point> temp, Point fire) {
    for (int i = 0; i < temp.size();i++) {
        //search temp vector
        if (temp[i].type == fire.type) {
            return false;
        }

    }

    return true;
}

int main() {

    clock_t startTime = clock();
    ifstream infile("terrain.txt");

    // Read in the points from the file
    vector<Point> points;
    vector<Point> all;
    string name;
    double x, y;
    //read file
    while (infile >> name >> x >> y) {
        points.push_back({ name, x, y });
    }

    // Find the airport and the fires
    Point airport;
    vector<Point> fires;
    int i = 0;
    Point p;

        for (int i = 0; i < points.size(); i++) {
            p = points[i];
            if (p.type == "Airport") {
                airport = p;
            }
            else if (p.type.find("Fire") != std::string::npos) {
                fires.push_back(p);
            }
            else {

            }
            all.push_back(p);
        }

    


    





    // Brute force approach to find the shortest path from the airport to the nearest fire
    double minDist = INF;
    vector<Point> path;
    vector<int> perm(fires.size());

    for (int i = 0; i < fires.size(); i++) {
        perm[i] = i;

    }
    bool covered;
    double dist = 0;
    double distance1 = 0;
    double angle;
    double corner1_x;
    double corner1_y;
    double corner2_x;
    double corner2_y;
    double corner3_x;
    double corner3_y;
    double corner4_x;
    double corner4_y;
    int previous;
    const double PI = M_PI;
    const double width = 100 / 2.0;
    Point corner1, corner2, corner3, corner4;
    //Assign 2d array
    double** distance_fires = new double* [fires.size() + 1];
    for (int i = 0; i < fires.size() + 1 ; i++) {
        distance_fires[i] = new double[fires.size() +1 ];
    }
    for (int i = 0; i < fires.size() +1 ; i++) {
        for (int j = 0; j < fires.size()+1 ; j++) {
            //Assign every distance as 0
            distance_fires[i][j] = 0;
        }
    }
    //For every start point
    for (int i = 0; i < fires.size()+1; i++) {
        //To every end point
        for (int j = 0; j < fires.size()+1; j++) {
            //find out distance between every point to every point
            distance_fires[i][j] = distance(all[i], all[j]);
        }


    }

   

    // Assign 3D array

    double*** covered_fires = new double** [fires.size() + 1];
    for (int i = 0; i < fires.size() + 1; i++) {
        covered_fires[i] = new double*[fires.size() + 1];
        for (int j = 0; j < fires.size()+1; j++) {
            covered_fires[i][j] = new double[fires.size() +1];
        }
    }   

     for (int i = 0; i < fires.size() + 1; i++) {
        //To every end point

        for (int j = 0; j < fires.size() + 1; j++) {
            //Which point is it covered

            for (int z = 0; z < fires.size() + 1; z++) {

                if (i != j && i != z && j != z) {
                    //find the rectangles of start point of end point
                    angle = atan2(all[j].y - all[i].y, all[j].x - all[i].x);
                    corner1.x = all[i].x + width * cos(angle - PI / 2.0);
                    corner1.y = all[i].y + width * sin(angle - PI / 2.0);
                    corner2.x = all[i].x + width * cos(angle + PI / 2.0);
                    corner2.y = all[i].y + width * sin(angle + PI / 2.0);
                    corner3.x = all[j].x + width * cos(angle + PI / 2.0);
                    corner3.y = all[j].y + width * sin(angle + PI / 2.0);
                    corner4.x = all[j].x + width * cos(angle - PI / 2.0);
                    corner4.y = all[j].y + width * sin(angle - PI / 2.0);

                    if (isInsideRectangle(all[z], corner1, corner2, corner3, corner4)) {
                        //1= covered
                        covered_fires[i][j][z] = 1;
                    }

                    else {
                        covered_fires[i][j][z] = 0;
                    }


                }
                else {
                    covered_fires[i][j][z] = 0;
                }


            }
        }
     }


    vector<Point> end;
    vector<Point> check;
    vector<Point> fast;

    int sum = 0;
    clock_t shortt = clock();

    do {
        dist = 0;
        distance1 = 0;
        vector<Point> cover;
        check = {};
        vector<Point> temp;
        fast = {};
        check.push_back(airport);
        temp.push_back(airport);
        atomic<bool> continues = false;
        int counter = 0;
        int counter1 = 0;

        for (int i = 0; i < fires.size(); i++) {
            int index = perm[i];
            Point fire = fires[index];

            //Extract fire according to perm index
            end = {};

            if (i == 0) {
                //get distance from airport to start of fire
                dist += distance_fires[0][perm[i] + 1];

                    //private vector
                    vector<Point> local_fast, local_cover;

                    for (int j = i + 1; j < fires.size(); j++) {
                        {

                            // Check if the point (x, y) is within the bounds of the rectangle and not covered
                       
                            if ((covered_fires[i][perm[i] + 1][perm[j] + 1] == 1)) {
                                if (isNotCovered(fast, fires[perm[j]]) && isNotCovered(local_fast, fires[perm[j]])) {

                                    local_cover.push_back(fires[perm[j]]);
                                    local_fast.push_back(fires[perm[j]]);

                                }
                            }
                        }
                    }
                    
                        //put private vector into the global vector
                        if (local_cover.size() != 0)
                            cover.insert(cover.end(), local_cover.begin(), local_cover.end());

                        if (local_fast.size() != 0)
                            fast.insert(fast.end(), local_fast.begin(), local_fast.end()); 
                    
                


                
            }






            else {

                {
                    dist += distance_fires[perm[i - 1] + 1][perm[i] + 1];


                    clock_t start = clock();
                    Point prevFire = fires[perm[i - 1]];
                    //Find out which is the previous fire
                    previous = perm[i - 1] + 1;



                        vector<Point> local_fast, local_cover;
                        #pragma omp for
                        for (int j = i + 1; j < fires.size(); j++) {
                            // Check if the point (x, y) is within the bounds of the rectangle and not covered
                            
                            if ((covered_fires[previous][perm[i] + 1][perm[j] + 1] == 1)) {

                                {
                                    if (isNotCovered(fast, fires[perm[j]]) && isNotCovered(local_fast, fires[perm[j]])) {

                                        local_cover.push_back(fires[perm[j]]);
                                        local_fast.push_back(fires[perm[j]]);
                                    }
                                }
                            }
                            




                        }


                            //put private vector into the global vector

                            if (local_cover.size() != 0)
                                cover.insert(cover.end(), local_cover.begin(), local_cover.end());

                            if (local_fast.size() != 0)
                                fast.insert(fast.end(), local_fast.begin(), local_fast.end()); 
                        
                    

                }




            }
   


            for (int j = i + 1; j < fires.size(); j++) {

                {
                    // Check if plane go back to airport cover any points

                    if ((covered_fires[0][perm[i] + 1][perm[j] + 1] == 1) && isNotCovered(fast, fires[perm[j]])) {
                        {

                            end.push_back(fires[perm[j]]);
                        }


                    }
                }
            }
            //if total distance exceed minDist ,skip    

            if (dist > minDist) {

                break;


            }


            {
                if (isNotCovered(cover, fire))
                    check.push_back(fire);





                //Save path
                temp.push_back(fire);
                fast.push_back(fire);
            }



            covered = false;

            //Check if covered every fires and point
            if (((check.size() + cover.size()) + end.size()) == (fires.size() + 1)) {

                covered = true;
                Point prevFire = temp[temp.size() - 1];
                temp.push_back(airport);
                //Find out which is the previous fire
                dist += distance(airport, prevFire);






            }



            if (covered) {
                //New minium distance value
                if (dist < minDist) {



                    minDist = dist;
                    //New shortest path
                    path = temp;





                }
                break;








            }




        }
    } while (next_permutation(perm.begin(), perm.end()));
    ofstream outfile("solution.txt");
    // Output the result to the "solution.txt" file
    for (int i = 0; i < path.size(); i++) {
     outfile << path[i].type << endl;

    }


    outfile.close();
    infile.close();
    return 0;
}