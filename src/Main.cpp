#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Teleop_updated.h"


////////----------------------------//////////
using namespace std;
// functions
void write_dist(double);
void functionCall();
double drive();

//void generate_perf_dists();
// double read_artag();  returns distance between human and robot
// double write_dist_moved();

// number of conditions
int n_comfort = 6;
int n_perf_exp = 6;
int min_n_pref = 2;


// return positions
double dist_near = 0.25;
double dist_far = 5.0;
double dist_choice = 0.0;

// selected distances
const int n_dists = 10;
//float dist[n_dists] = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25,4.75};
//float REQ_POS_Arr[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
//function definitions
void write_dist(double vale) {
  // move to this position
  cout << "\n Writing distance to file";
  ofstream myfile;
  myfile.open("/home/robotlab/File Read/write.csv", ios::ate | ios::app);
  if (myfile.is_open()) {
    myfile << vale << ",";
    myfile.close();
  } else {
    cout << "Unable to open file!";
  }
}


///////////////*****FUNCTION CALL*****/////////////////////////
void functionCall()
{
  int y;
  ifstream myfile;
  myfile.open("/home/robotlab/File Read/UserID.txt");
  if (myfile.is_open()) {
    myfile >> y;   // change it to READ SYSTEM TIME
  } else {
    cout << "\nUnable to open file!";
  }
  y = y + 1;
  ofstream out;
  out.open("/home/robotlab/File Read/UserID.txt", ios::ate);
  out << y;
  out.close();
  int uid = y;
  ofstream in;
  in.open("/home/robotlab/File Read/write.csv", ios::ate | ios::app);
  if (in.is_open()) {
    in << "\n" << uid << ",";
    in.close();
  } else {
    cout << "\nUnable to open file!";
  }
}

/////////-----DRIVE-----------//////////////
double drive()
{
  double d;
  d = (rand()%10+1);
  return d;
}

////////--------MAIN----------/////////////////

int main(int argc, char** argv)
{
  functionCall();
  ros::init(argc, argv, "teleop");
  ROS_INFO("In the Main");	  
  TeleopTurtle teleop;
  
  /////////////////////////////////////////////
  // int count = 0;
  // TeleopPioneer Test[6];
  double comf_dist[n_comfort];
  //int len = int(sizeof(comf_dist)/sizeof(double));
  teleop.init_func();
  // comf_dist = drive();
  // comf_dist[i] = read_artag();
  //write_dist(comf_dist);	
  ros::spin();
}

