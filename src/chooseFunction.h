#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

#ifndef CHOOSEFUNCTION_H_
#define CHOOSEFUNCTION_H_

int chooseFunction()
{
	int A, B, C, D;
	float func_A, func_B, func_C, func_D, total;
  
/*********************************************
 **** READING NUMBER OF TIMES THE VARIOUS ****
 **** NUMBER OF TIMES THE FUNCTIONS ARE   ****
 **** CALLED                              ****
 ********************************************/
 
	ifstream infile("list.txt");
	while(infile >> A >> B >> C >> D);
	
/*********************************************
 **** CALCULATING THE PROBABILITY OF EACH ****
 **** FUNCTION THAT CAN BE CALLED NEXT    ****
 ********************************************/

	total = A + B + C + D;
  
	func_A = A/total;
	func_B = B/total;
	func_C = C/total;
	func_D = D/total;
	
/*********************************************
 **** RETURNING THE ID OF THE REQUIRED    ****
 **** FUNCTION                            ****
 ********************************************/

	srand (time(NULL));
	if(((func_A<func_B) && (func_A<func_C)) && (func_A<func_D)) return 0;
	else if(((func_B<func_A) && (func_B<func_C)) && (func_B<func_D)) return 1;
	else if(((func_C<func_A) && (func_C<func_B)) && (func_C<func_D)) return 2;
	else if(((func_D<func_A) && (func_D<func_B)) && (func_D<func_C)) return 3;
	else return rand()%4;
}

#endif /* chooseFunction.h */
