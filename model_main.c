/*
This code models the effect of insertions on traffic flow.
It recieves values of I and F in pairs and outputs 4 values (I, avgspeed in km/h, spacing, F)
More detail on this in the main function below (I & F correspond to ins and freq respectively)
*/


/*--------- Packages ---------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


/*--------- Parameters ---------*/

// Length of road in cells
#define LENGTH 10000

// Number of iterations
#define ITERATIONS 300

// Maximum allowed speed in cells per iteration
#define VMAX 5

// Probability of random slowing
#define P 0.1

/*--------- Measurements ---------*/

// Number of cars that go out of range
int out = 0;
//Succesful insertions
int ins = 0;
// Sum of the speeds of all the cars on road
int tot = 0;
// Total number of cars on road
int cars = 0;


/*--------- Functions ---------*/

/* 'initialize' initialises the road */

void initialize(int road[LENGTH], int freq){

    /* 
    Create an empty road.
    By default all cells are worth -1, this means that there is no car present on the cell.
    */

    for (int i=0; i<LENGTH; i++){
        road[i]=-1;
    }

    /* 
    Populate the road according to the requested traffic density.
    Every FREQ cells we insert a car at VMAX.
    */

    for (int i = 0; i<LENGTH; i++){
        if (i%freq==0)road[i]=VMAX;
    }
}


/* 'keepaddingcars' maintains the flow of cars at the given FREQ rate */

void keepaddingcars(int road[LENGTH], int freq){

    /* Find the position of the first car on the road */

    int x = 0;
    for (int i=0; i< LENGTH; i++){
        if (road[i]!=-1){
            x=i;
            break;
        }
    }

    /* If it is greater than FREQ, insert a car at i = x-FREQ with speed VMAX */

    if (x>freq)road[x-freq]=VMAX;
}


/* 'updateroad' moves each car according to the rules explicited */

void updateroad(int road[LENGTH]){

    /* Create copy of road */

    int newroad [LENGTH];
    for (int i=0; i<LENGTH; i++){
        newroad[i]=-1;
    }

    /* i is the position of the car */

    for (int i = 0; i< LENGTH; i++){

        /* v is the speed of that car */

        int v=road[i];

        /* 
        Only move cars
        By design if the speed is -1 there is no car
        */

        if (v != -1){

        // 1. Acceleration    
            /* 
            Find the position of the car immediately in front.
            If this value ('next') minus the position i of the car is larger than it's speed, the car can accelerate (speed increases by one cell per time-step).
            In other words, if there is enough space in front the car accelerates.
            By default 'next' is very large. This is so that the last car on the road can accelerate and exit the simulation with no problems.
            If a car is already going at VMAX, it does not accelerate.
            */

            if (v<VMAX){
                int next=LENGTH+10000;
                for (int j=i+1; j<LENGTH; j++){
                    if (road[j]!=-1){
                        next=j;
                        break;
                    }
                }

                if (next-i>v) v=v+1;
            }

        // 2. Security deceleration
        /*
        If there is a vehicle at less than 'v' places ahead, the car slows down to the distance j between the two cars minus 1.
        &&&&&&&&__---__ write smth ??
        */

            for (int j=1; j<v+1; j++){
                if (i+j<LENGTH && road[i+j]!=-1){
                    v=j-1;
                    break;
                }
            }

        // 3. Stochastic deceleration
        /*
        Any given vehicle slows down (by one cell per time step) with probability P at each iteration.
        This is the human element of this model.
        Note: This only applies to vehicles going above v=2. 
        The assumption is that if you are that slow on a motorway your speed is not affected by stochastic aspects.
        */
        
            if ((float)rand() / RAND_MAX < P && v>2) {
                v=v-1;
            }

        // 4. Move
        /* Car moves from position i to position i+v (provided it is still in the simulation) */

            if (i+v< LENGTH) newroad[i+v]=v;
            
        /* 
        The variable 'out' keeps track of the cars that have exited the simulation.
        ie. the cars were i+v is greater than the lenght of the road 
        */

            else out++;
        }
    }

    /* Copy new road onto main road */

    for (int i=0; i<LENGTH; i++){
        road[i] = newroad[i];
    }
}


/* 'insertcar' inserts IN cars to a given road */

void insertcar(int road[LENGTH], int IN){
    int r = 0;
    ins=0;
    for (int i= 0; i<IN; i++){

        /*
        To keep a stochastic element in the model, we randomly generate a cell and try to insert.
        If insertion fails, increase the position by 1 and try again.
        This process is repeated IN times.
        Note: we avoid insertions in the first and last 20 cells of the road.
        */

        r=rand() % (LENGTH - 40) + 20; /* r is a random number between 20 and LENGTH-20*/

        int f = 0; /* Space to front car */
        int b = 0; /* Space to rear car */

        for (int n=r; n<LENGTH-20; n++){

            /* Find the position of the car in front */

            for (int bb=0; bb<LENGTH; bb++){
                if (road[n-bb]!=-1){
                    b=bb;
                    break;
                }
            }

            /* Find the position of the car behind */

            for (int ff=1; ff<LENGTH; ff++){
                if (road[n+ff]!=-1){
                    f=ff;
                    break;
                }
            }

            /*
            We now insert a car according to the positions and speeds of the cars immidiately in front and behind of the insertion candidate.

            A car is succesfully inserted if the following conditions are satisfied:
            - Let b be the distance to the car behind and vb the speed of that car -
            - Let f be the distance to the car ahead -
            If (b>=4 and vb= 5) or (b>=3 and vb=4) or (b>=2 and vb<4):
                If f>=5 insert with speed 4
                If f=4 insert with speed 3
                If f=3 insert with speed 2
                If f=2 insert with speed 1

            Note: Theses implementation rules are valid when VMAX = 5. They should be modified if VMAX increases (not necessarily by adding one everywhere).
            */

            if ((b>=4 && road[n-b]==5) || (b>=3 && road[n-b]==4) || (b>=2 && road[n-b]<4)){
                if (f>=5){
                    road[n]=4;
                    ins++;
                    break;
                }
                else if (f==4 || f==3 || f==2){
                    road[n]=road[n+f]-1;
                    ins++;
                    break;
                }

                /* With each insertion we add one to 'ins', the value that keeps track of the number of successful insertions */

            }

            /* 
            If we still have not inserted a car by the time we get to 20 cells before the end, no insertions are made. 
            Again, this is a legitimate stochastic element to the model.
            */

            if (n==LENGTH-20) break;
        }
    }
}


/* 'totalspeed' calculates the number of cars on a road and the total speed of all the cars */
/* These two values are later used to calculate the average speed of cars on a given road by computing tot/cars */

void totalspeed(int road[LENGTH]){
    tot=0;
    cars=0;
    for (int i=0; i<LENGTH; i++){
        if (road[i]!=-1){
            tot=tot+road[i];
            cars++;
        }
    }
}


/* 'printroad' prints a given road */

void printroad(int road[LENGTH]){

    for (int i=0; i< LENGTH; i++){

        /* If there is no car (v=-1), a point '.' is displayed */

        if (road[i]==-1){
            printf("%c", '.');

        /* If there is a car, it's speed is displayed */

        }else{
            printf("%d", road[i]);
        }
    }
    printf("\n");
}


/* 'main' coordinates all functions */

int main() {

    /* The while loop and the result and value variables are used for the piping process, where our C code uses the values provided by feed.py */

    int result = 0 ;

    /* Number of insertions per time step */
    int ins = 0;

    /* On the road there is a car every freq spaces */
    int freq = 0;

    while ( (result = scanf( "%d,%d", &ins, &freq)) != EOF ) {

        /* 'road' is a grid 1 by length */
        int road[LENGTH];

        /* Initialize road */
        initialize(road, freq);

        /* Setup for random number*/
        srand(time(NULL));

        for (int n=0; n<ITERATIONS; n++){

            /* Update road */
            updateroad(road);

            /* Maintain continuous flux of cars */
            keepaddingcars(road,freq);

            /* Insert a car every 10 time-steps */
            if (n%10==0){
                insertcar(road, ins);
            }
        }

        /* After simulation is over (n iterations have been performed), compute the number of cars on the road and their combined speed */
        totalspeed(road);

        /* Average speed of car in cells per iteration */
        float avgspeed = (float)tot/cars;

        /* Spacing, after full simulation run there on average is a car every 'spac' cells */
        float spac = (float)10000/cars;

        /* 
        Print the number of insertions per 10 time-steps, the average speed of cars and car density after all the time-steps.
        They will then be treated by analyze.py using pipe.
        */
        printf("%d,%.2f,%.2f,%d\n", ins, avgspeed*7*3.6, spac, freq);

        /* Reset the number of cars that have exited to 0 */
        out=0;
    }
    return 0;
}
