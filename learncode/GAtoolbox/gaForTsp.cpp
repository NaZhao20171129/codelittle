		  /*************************************************/
		  /*  Applied Neural Networks IV Assignment 95/96  */
		  /*                                               */
		  /*  Travelling Salesman Problem                  */
		  /*                                               */
		  /*  Solved by Simulated Annealing                */
		  /*                                               */
		  /*  Programmer : Mr. K. H. Ang,  Copyright 1995  */
		  /*  Modified by Dr. Y. Li,  1996, Y.Li@elec.gla  */
		  /*  for Neural and Evolutionary Computing IV     */
		  /*************************************************/

// (1) You need the egavga.obj file during compiling.  This way
//     no egavga.bgi is needed at run-time in nec4tsp.exe directory
// (2) Or slightly modify this file to use egavga.bgi at run-time

#include <stdio.h>
#include <stdlib.h>
#include <iostream.h>
#include <conio.h>
#include <math.h>
#include <graphics.h>
#include <dos.h>

#define Temperature_Factor 0.2
#define Compute_Path_Length(a,b,c,d) sqrt(((b)-(a))*((b)-(a))+((d)-(c))*((d)-(c)))

#define BIT_1 1
#define BIT_2 2
#define BIT_5 16
#define BIT_18 131072
#define MASK (BIT_1+BIT_2+BIT_5)

#define HUGE_VALUE 1000000000
#define HUGE_SEED 176234574     // Change the value of HUGE_SEED for others configurations
#define FACTOR (1.0/HUGE_VALUE)

char repeat_yes;
long int No_Of_City = 30;



/* Procedure to initialise the graphics display */
void initialise_graph()
{

  int gdriver=DETECT, gmode, errorcode;

#ifndef DJGPP
  if (registerbgidriver(EGAVGA_driver) < 0) exit(1);
#endif
  // initialize graphics & local variables
  initgraph(&gdriver,&gmode,"");

  // read result of initialization
  errorcode = graphresult();
  if (errorcode != grOk)
  {
	 printf("Graphics error : %s\n",grapherrormsg(errorcode));
	 printf("Press any key to halt.");
	 getch();
	 exit(0);
  }
} /* End of initialise_graph() */


/* Display information */
void display_info()
{
  int maxx, maxy;

  maxx = (getmaxx() - 405)/2;
  maxy = (getmaxy()/2) + 190;

  setbkcolor(WHITE);

  /* Draw display boxes */
  setcolor(BLUE);
  rectangle(maxx-5,maxy-405,maxx+405,maxy+5);	   // Main display
  rectangle(maxx+410,maxy-337,maxx+515,maxy-323);  // Path length display
  rectangle(maxx+410,maxy-287,maxx+515,maxy-273);  // Temperature display
  rectangle(maxx+410,maxy-229,maxx+515,maxy-215);  // Successful Moves display
  rectangle(maxx-100,maxy-300,maxx-20,maxy-260);   // Legend Display

  /* Display items in the Legend box */
  setfillstyle(SOLID_FILL,GREEN);
  setcolor(GREEN);
  circle(maxx-85,maxy-289,2);
  fillellipse(maxx-85,maxy-289,2,2);

  setcolor(BROWN);
  moveto(maxx-95,maxy-269);
  lineto(maxx-75,maxy-269);

  settextstyle(TRIPLEX_FONT,HORIZ_DIR,1);
  settextjustify(CENTER_TEXT,CENTER_TEXT);
  setcolor(BLUE);
  outtextxy(maxx+150,maxy+20,"NEC4: Travelling Salesmans Problem Solved by Simulated Annealing");
  outtextxy(maxx+150,maxy+30,"(C) Programmed by: Mr. K.H. Ang, ANN4 student in 1995/96 session");
  outtextxy(maxx+150,maxy+40,"Modified: Dr. Y. Li, ANN4/NEC4 Lecturer 95-, Y.Li@elec.gla.ac.uk");


  do {

	  setcolor(RED);
	  setfillstyle(SOLID_FILL, WHITE);
	  bar(maxx-150,maxy-413, maxx+525,maxy-433);
	  outtextxy(maxx+150,maxy-423,"Continue to optimise your itinerary using SA?  (Y/N):           ");

	  repeat_yes = getch();

	  setfillstyle(SOLID_FILL, LIGHTGREEN);
	  bar(maxx-150,maxy-413, maxx+525,maxy-433);

	  delay(100);

  }while (repeat_yes != 'n' && repeat_yes != 'N' && repeat_yes != 'y' && repeat_yes != 'Y');


  /* Display title */
  setcolor(MAGENTA);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  outtextxy(maxx-65,maxy-390,"UNIVERSITY");
  outtextxy(maxx-65,maxy-370,"    of    ");
  outtextxy(maxx-65,maxy-350," GLASGOW ");

  setcolor(LIGHTGRAY);
  outtextxy(maxx+460,maxy-350,"  SA Temp.  ");
  outtextxy(maxx+460,maxy-300,"Route Length");
  outtextxy(maxx+460,maxy-250," Attempted  ");
  outtextxy(maxx+460,maxy-242,"   Moves    ");
  outtextxy(maxx-70,maxy-292,"    city");
  outtextxy(maxx-70,maxy-272,"    path");
  if (repeat_yes != 'n' && repeat_yes != 'N')
  {

	  outtextxy(maxx-75,maxy-200,"See if  ");
	  outtextxy(maxx-75,maxy-190,"you can ");
	  outtextxy(maxx-75,maxy-180,"figure  ");
	  outtextxy(maxx-75,maxy-170,"out the ");
	  outtextxy(maxx-75,maxy-160,"shortest");
	  outtextxy(maxx-75,maxy-150,"route...");

	  setcolor(LIGHTMAGENTA);
	  outtextxy(maxx-65,maxy-90,"Press a   ");
	  outtextxy(maxx-65,maxy-80,"key to    ");
	  outtextxy(maxx-65,maxy-70,"continue &");
	  outtextxy(maxx-65,maxy-60,"refine    ");
	  outtextxy(maxx-65,maxy-50,"learning..");

	  outtextxy(maxx+460,maxy-90,"Why does ");
	  outtextxy(maxx+460,maxy-80,"sometimes");
	  outtextxy(maxx+460,maxy-70,"the route");
	  outtextxy(maxx+460,maxy-60,"becomes  ");
	  outtextxy(maxx+460,maxy-50,"longer ? ");

  }

} /* End of display_info() */


/* Procedure performing simulated annealing schedule */
void simulated_annealing(float x[], float y[], int visit_order[])
{
  int max_trial, max_limit, i1, i2, value, approve;
  int i, j, k, successful_moves, missing_city, decision;
  float path_length, difference, temperature;
  unsigned long int seed;
  static int segment[20];

  int random_bits(unsigned long &seed);
  int oracle(float difference, float temperature);
  float random_number(int &value);
  float compute_cost(float x[], float y[], int visit_order[], int segment[]);
  float compute_reverse_cost(float x[], float y[], int visit_order[], int segment[]);
  void reverse_path_connection(int visit_order[], int segment[]);
  void connect_new_path(int visit_order[], int segment[]);
  void display_result(float x[], float y[], float temperature, float path_length, int successful_moves, int visit_order[]);

  max_trial = 30*No_Of_City;		// Max. no. of paths tried at any temperature
  max_limit = 5*No_Of_City;		// Max. no. of successful path changes before continuing
  path_length = 0.0;
  temperature = 1;
  value = -1;
  seed = 111;

  /* Calculate initial path length */
  for (i=1; i<No_Of_City; i++){
	 i1 = visit_order[i];
	 i2 = visit_order[i+1];
	 path_length += Compute_Path_Length(x[i1],x[i2],y[i1],y[i2]);
  }

  /* Close the loop by tying path ends together */
  i1 = visit_order[No_Of_City];
  i2 = visit_order[1];
  path_length += Compute_Path_Length(x[i1],x[i2],y[i1],y[i2]);

  for (j=1; j<=100; j++){	// Try up to 100 temperature steps
	 successful_moves = 0;
	 for (k=1; k<=max_trial; k++){
		do{
	segment[1] = 1+((long int)(No_Of_City*random_number(value)));	// Choose start of segment
	segment[2] = 1+(long int)((No_Of_City-1)*random_number(value));	// and end of segment
	if (segment[2] >= segment[1])
	  ++segment[2];
	missing_city = 1+((segment[1]-segment[2]+No_Of_City-1)%No_Of_City);	// No. of cities not on segment
		}while (missing_city < 3);

		decision = random_bits(seed);	// Decide whether to do a segment reversal or transport

		if (decision == 0){		// Do a transport
	segment[3] = segment[2] + (int)(abs(missing_city-2)*random_number(value))+1;
	segment[3] = 1+((segment[3]-1)%No_Of_City);	   // Transport to a location not on the path
	difference = compute_cost(x,y,visit_order,segment);	   // Calculate the cost
	approve = oracle(difference,temperature);	   // Check whether to continue with connection
	if (approve){
	  ++successful_moves;
	  path_length += difference;
	  connect_new_path(visit_order,segment);	// Carry out the connection
	}
		}
		else{				// Do a path reversal
	difference = compute_reverse_cost(x,y,visit_order,segment);	// Calculate the cost
	approve = oracle(difference,temperature);          // Check whether to continue with connection
	if (approve){
	  ++successful_moves;
	  path_length += difference;
	  reverse_path_connection(visit_order,segment);	// Carry out the reverse path connection
	}
		}

		/* Finish early if have enough successful changes */
		if (successful_moves >= max_limit) break;
	 }

	 display_result(x,y,temperature,path_length,successful_moves,visit_order);

	 temperature *= Temperature_Factor;

	 getch();

	 if (successful_moves == 0)
	 return;
  }
} /* End of simulated_annealing() */


/* Function computing the cost of the proposed connection */
float compute_cost(float x[], float y[], int visit_order[], int segment[])
{
  float xx[7], yy[7], difference;
  int i, n;

  segment[4] = 1+(segment[3]%No_Of_City);         // Find the city following segment[3]
  segment[5] = 1+((segment[1]+No_Of_City-2)%No_Of_City);  // and the city preceeding segment[1]
  segment[6] = 1+(segment[2]%No_Of_City);         // and the city following segment[2]

  /* Find the coordinates of the 6 cities involved */
  for (i=1; i<=6; i++){
	 n = visit_order[segment[i]];
	 xx[i] = x[n];
	 yy[i] = y[n];
  }

  /* Compute the cost of the proposed connection */
  difference = -Compute_Path_Length(xx[2],xx[6],yy[2],yy[6]);
  difference -= Compute_Path_Length(xx[1],xx[5],yy[1],yy[5]);
  difference -= Compute_Path_Length(xx[3],xx[4],yy[3],yy[4]);
  difference += Compute_Path_Length(xx[1],xx[3],yy[1],yy[3]);
  difference += Compute_Path_Length(xx[2],xx[4],yy[2],yy[4]);
  difference += Compute_Path_Length(xx[5],xx[6],yy[5],yy[6]);

  return(difference);
} /* End of compute_cost() */


/* Procedure performing the proposed connection */
void connect_new_path(int visit_order[], int segment[])
{
  int m1, m2, m3, city, i, n, *new_order;
  int *allocate(int min, int max);
  void free_allocation(int *v, int min);

  new_order = allocate(1,No_Of_City);

  /* Find no. of cities from segment[1] to segment[2] */
  m1 = 1+((segment[2]-segment[1]+No_Of_City)%No_Of_City);

  /* Find no. of cities from segment[4] to segment[5] */
  m2 = 1+((segment[5]-segment[4]+No_Of_City)%No_Of_City);

  /* Find no. of cities from segment[5] to segment[6] */
  m3 = 1+((segment[3]-segment[6]+No_Of_City)%No_Of_City);

  city = 1;

  /* Transfer the new order of city visit into new_order array */
  for (i=1; i<=m1; i++){
	 n = 1+((i+segment[1]-2)%No_Of_City);
	 new_order[city++] = visit_order[n];
  }

  if (m2>0){
	 for (i=1; i<=m2; i++){
		n = 1+((i+segment[4]-2)%No_Of_City);
		new_order[city++] = visit_order[n];
	 }
  }

  if (m3>0){
	 for (i=1; i<=m3; i++){
		n = 1+((i+segment[6]-2)%No_Of_City);
		new_order[city++] = visit_order[n];
	 }
  }

  /* Overwrite the old order of city visit with the new ones */
  for (i=1; i<No_Of_City+1; i++)
	 visit_order[i] = new_order[i];

  free_allocation(new_order,1);
} /* End of connect_new_path() */


/* Function computing the cost of reverse path connection */
float compute_reverse_cost(float x[], float y[], int visit_order[], int segment[])
{
  float xx[5], yy[5], difference;
  int i, n;

  segment[3] = 1+((segment[1]+No_Of_City-2)%No_Of_City);  // Find city before segment[3]
  segment[4] = 1+(segment[2]%No_Of_City);                 // and after segment[4]

  /* Find the coordinates of the 4 cities involved */
  for (i=1; i<=4; i++){
	 n = visit_order[segment[i]];
    xx[i] = x[n];
	 yy[i] = y[n];
  }

  /* Compute the cost of reverse connection */
  difference = -Compute_Path_Length(xx[1],xx[3],yy[1],yy[3]);
  difference -= Compute_Path_Length(xx[2],xx[4],yy[2],yy[4]);
  difference += Compute_Path_Length(xx[1],xx[4],yy[1],yy[4]);
  difference += Compute_Path_Length(xx[2],xx[3],yy[2],yy[3]);

  return(difference);
} /* End of compute_reverse_cost() */


/* Procedure performing the reverse path connection */
void reverse_path_connection(int visit_order[], int segment[])
{
  int swap_city, i, j, k, temp;

  /* Compute how many cities to swap */
  swap_city = (1+((segment[2]-segment[1]+No_Of_City)%No_Of_City))/2;

  /* Swapping process */
  for (i=1; i<=swap_city; i++){
	 j = 1+((segment[1]+i-2)%No_Of_City);
	 k = 1+((segment[2]-i+No_Of_City)%No_Of_City);
	 temp = visit_order[j];
	 visit_order[j] = visit_order[k];
	 visit_order[k] = temp;
  }
} /* End of reverse_path_connection() */


/* Function generating random numbers between 0 and 1 */
float random_number(int &value)
{
  static int inext, inextp;
  static long ma[56];
  static int iff=0;
  long mj, mk;
  int i, k, n;

  if (value < 0 || iff == 0){
	 iff = 1;
	 mj = HUGE_SEED-(value<0 ? -value : value);
	 mj %= HUGE_VALUE;
	 ma[55] = mj;
	 mk = 1;

	 for (i=1; i<=54; i++){
		n = (21*i)%55;
		ma[n] = mk;
		mk = mj - mk;
		if (mk < 0)
	mk += HUGE_VALUE;
		mj = ma[n];
	 }

	 for (k=1; k<=4; k++){
		for (i=1; i<=55; i++){
	ma[i] -= ma[1+(i+30)%55];
	if (ma[i] < 0)
	  ma[i] += HUGE_VALUE;
		}
	 }

	 inext = 0;
    inextp = 31;
	 value = 1;
  }

  if (++inext == 56)
	 inext = 1;

  if (++inextp == 56)
	 inextp = 1;

  mj = ma[inext] - ma[inextp];

  if (mj < 0)
	 mj += HUGE_VALUE;

  ma[inext] = mj;

  return(mj*FACTOR);
} /* End of random_number() */


/* Function deciding whether to do a segment reversal or transport */
int random_bits(unsigned long &seed)
{
  /* Change all masked bits, shift, and put 1 into bit 1 */
  if (seed & BIT_18){
	 seed = ((seed^MASK) << 1) | BIT_1;
	 return 1;
  }
  else{		    	// Shift and put 0 into bit 1
    seed <<= 1;
	 return 0;
  }
} /* End of random_bits() */


/* Function deciding whether to approve the proposed connection */
int oracle(float difference, float temperature)
{
  static int oracle_value=1;
  float random_number(int &value);

  return(difference < 0.0 || random_number(oracle_value) < exp(-difference/temperature));
} /* End of oracle() */


/* Procedure to display the results */
void display_result(float x[], float y[], float temperature, float path_length,
			 int successful_moves, int visit_order[])
{
  int maxx, maxy, i, n;
  char path_length_buffer[101], temperature_buffer[101], successful_moves_buffer[101];
  float xcord, ycord, xx[101], yy[101], top_x, top_y;

  /* Store temperature into temperature_buffer */
  sprintf(temperature_buffer,"%12.10f",temperature);
  temperature_buffer[20] = '\0';

  /* Store path length into path_length_buffer */
  sprintf(path_length_buffer,"%10.3f",path_length*1e3);
  path_length_buffer[20] = '\0';

  /* Store successful moves into successful_moves_buffer */
  sprintf(successful_moves_buffer,"%4.2d",successful_moves);
  successful_moves_buffer[20] = '\0';

  /* Re-arrange the order properly */

	top_y = -1;
	top_x = -1;

  for (i=1; i<=No_Of_City; i++){
	 n = visit_order[i];
	 xx[i] = x[n];
	 yy[i] = y[n];
	 if (yy[i] > top_y)
	 {
		top_y = yy[i];
		top_x = xx[i];
	 }
  }

  maxx = (getmaxx() - 400)/2;
  maxy = getmaxy()/2 + 190;

  /* To clear displays */
  setfillstyle(SOLID_FILL,WHITE);
  bar(maxx,maxy-400,maxx+400,maxy);	         // Main display
  bar(maxx+410,maxy-335,maxx+510,maxy-325);  // Temperature display
  bar(maxx+410,maxy-285,maxx+510,maxy-275);  // Path Length display
  bar(maxx+410,maxy-227,maxx+510,maxy-217);  // Successful Moves display

  /* Display data*/
  setcolor(BLUE);
  settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
  outtextxy(maxx+460,maxy-330,temperature_buffer);       // Temperature
  outtextxy(maxx+455,maxy-280,path_length_buffer);       // Path Length
  outtextxy(maxx+455,maxy-222,successful_moves_buffer);  // Successful Moves

  /* Move to the start city */
  xcord = maxx + (top_x * 400);
  ycord = maxy - (top_y * 400);
  moveto(xcord,ycord);

  setfillstyle(SOLID_FILL, GREEN);
  setcolor(GREEN);
  circle(xcord,ycord,4);
  fillellipse(xcord,ycord,2,2);
  outtextxy(xcord-50, ycord+3, "Glasgow");


  /* Move to the first city to optimise */
  xcord = maxx + (xx[1] * 400);
  ycord = maxy - (yy[1] * 400);
  moveto(xcord,ycord);

  /* Create a red circle to represent a city */
  setfillstyle(SOLID_FILL, GREEN);
  setcolor(GREEN);
  circle(xcord,ycord,2);
  fillellipse(xcord,ycord,2,2);

  /* Draw link to other cities in green */
  for (i=2; i<=No_Of_City; i++){
	 setcolor(BROWN);
	 xcord = maxx+(xx[i] * 400);
	 ycord = maxy-(yy[i] * 400);
	 lineto(xcord,ycord);
	 setfillstyle(SOLID_FILL,GREEN);
	 setcolor(GREEN);
	 circle(xcord,ycord,2);
	 fillellipse(xcord,ycord,2,2);
  }

  /* Close up the loop by connecting path ends together */
  setcolor(BROWN);
  xcord = maxx + (xx[1] * 400);
  ycord = maxy - (yy[1] * 400);
  lineto(xcord,ycord);

//  delay(1);
} /* End of display_result() */


/* Function to allocate a dynamic array[min..max] */
int *allocate(int min, int max)
{
  int *v;

  v = (int*)malloc((unsigned)(max-min+1)*sizeof(int));

  if (!v)
	 exit(1);

  return(v-min);
} /* End of *allocate() */


/* Procedure to free the memory allocation allocate by *allocate() */
void free_allocation(int *v, int min)
{
  free(v+min);
} /* End of free_allocation() */


/* Main Function */
void main()
{
  float x[100], y[100];
  int visit_order[100], i, value;
  void initialise_graph(), display_info();
  void simulated_annealing(float x[], float y[], int visit_order[]);

  clrscr;

  cout << "\n\n\n\n\nNEC4: Travelling Salesmans Problem Solved by Simulated Annealing\n"
		 << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n"
		 << "Input the number of cities you need to visit (4-50): ";
  cin >> No_Of_City;

  if (No_Of_City < 1)
  {
  No_Of_City = 30;
  cout << "\n\n\nLet's try 30 cities ...";
  delay(2000);
  }
  else if (No_Of_City < 4)
  {
  No_Of_City = 4;
  cout << "\n\n\nYou can determine such an easy route without a computer;"
	<< "\n\nLet's try 4 cities now ...";
  delay(2000);
  }
  else if (No_Of_City > 100)
  {
  No_Of_City = 100;
  cout << "\n\n\nIt's not fair for you to visit more than 100 cities;"
	<< "\n\nLet's have a look at 100 cities now ...";
  delay(2000);
  }

  clrscr;

  initialise_graph();


	do{

//	  cleardevice();
	  display_info();

	  /* Randomly generates coordinates for the cities */
	  value=-1;
	  for (i=1; i<=No_Of_City; i++)
	  {
		 x[i] = random_number(value);
		 y[i] = random_number(value);
		 visit_order[i] = i;
	  }
	  if (repeat_yes == 'n' || repeat_yes == 'N') closegraph();
	  else simulated_annealing(x,y,visit_order);

	}while (repeat_yes != 'n' && repeat_yes != 'N');

} /* End of Main Function */


