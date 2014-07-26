#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Tele_current.h"


const int n_dists = 10;
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
void fCall()
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
  out << y ;
  out.close();
  int uid = y;
  ofstream in;
  in.open("/home/robotlab/File Read/write.csv", ios::ate | ios::app);
  if (in.is_open()) {
    in << "\n" << uid << " ";
    in.close();
  } else {
    cout << "\nUnable to open file!";
  }
}

/*
/////////-----DRIVE-----------//////////////
double drive()
{
  double d;
  d = (rand()%10+1);
  return d;
} */

////////--------MAIN----------/////////////////

int main(int argc, char** argv)
{
  fCall();
  ros::init(argc, argv, "tele_current");
  ROS_INFO("In the Main");	  
  Tele_current tel;
  tel.init_func();
  ros::spin();
}

