#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <iostream>
#include <vector>

const int HEIGTH = 1000;
const int WIDTH = 1400;

const int Y_num_sqrs = 250;
const int X_num_sqrs = 350;

const int X_size = WIDTH/X_num_sqrs;
const int Y_size = HEIGTH/Y_num_sqrs;

const int numCells = X_num_sqrs * Y_num_sqrs;

int world_1[Y_num_sqrs][X_num_sqrs];
int world_2[Y_num_sqrs][X_num_sqrs];

int (*ptr_1)[X_num_sqrs] = world_1; //Pointer to the main world
int (*ptr_2)[X_num_sqrs] = world_2; //Ptr to second world

int startBlack = numCells/7;                                                       //Change number to select initial density of cells

int seed;

void initWorld(){
    for(int i = 0; i < startBlack; i++){
       int row = rand() % Y_num_sqrs;
       int col = rand() % X_num_sqrs;

       world_1[row][col] = 1;
       world_2[row][col] = 1;
    }

    // //Generate a Blinker
    // world_1[100][100] = 1;
    // world_2[100][100] = 1;
    // world_1[100][101] = 1;
    // world_2[100][101] = 1;
    // world_1[100][102] = 1;
    // world_2[100][103] = 1;

}

int getNeighbours(int r, int c){
    int n = 0;
    if(r > 0){
        if(c > 0){
            n += ptr_1[r-1][c-1];   //1
        }
        n += ptr_1[r-1][c];         //2
        if(c < X_num_sqrs-1){
            n += ptr_1[r-1][c+1];   //3
        }
    }
    if(c > 0){
        n += ptr_1[r][c-1];         //4
    }
    if(c < X_num_sqrs-1){
        n += ptr_1[r][c+1];         //5
    }

    if(r < Y_num_sqrs-1){
        if(c > 0){
            n += ptr_1[r+1][c-1];   //6
        }
        n += ptr_1[r+1][c];         //7
        if(c < X_num_sqrs-1){
            n += ptr_1[r+1][c+1];   //8
        }
    }

    // if(n > 0){
    //     std::cout<<"Cell ("<<r<<", "<<c<<") has "<<n<<" neighbours"<<std::endl;
    // }

    return n;
}

/*
    update ptr_2 using ptr_1, then swap

*/
int updateWorld(){
    int numAlive = 0;

    //Update ptr_2
    for(int r = 0 ; r < Y_num_sqrs; r++){
        for(int c = 0; c < X_num_sqrs; c++){

            //Get number of neighboors
            int neigh = getNeighbours(r,c);

            int alive = 0;
            
            if(ptr_1[r][c]){    //If cell is alive
                if(neigh == 2 || neigh == 3){
                    alive = 1;
                }
            }
            else{   //If cell is dead
                if(neigh == 3){
                    alive = 1;
                }
            }

            ptr_2[r][c] = alive;
            numAlive += alive;
        }
    }

    //Swap ptr_1 and ptr_2
    int (*ptr_temp)[X_num_sqrs];

    ptr_temp = ptr_2;
    ptr_2 = ptr_1;
    ptr_1 = ptr_temp;

    return numAlive;
}

int main()
{
    std::cout<<"Enter seed:  "<<std::endl;
    std::cin>>seed;
    std::cout<<""<<std::endl;
    srand(seed);

    std::cout<<"Initializing..........";
    initWorld();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGTH), "Conway's Game of Life!", sf::Style::Close | sf::Style::Titlebar);
    sf::Clock clock;
    sf::Time waitTime = sf::seconds(0.05f);

    std::vector<std::vector<sf::RectangleShape>> theSquares;

    for(int r = 0; r < Y_num_sqrs; r++){
        std::vector<sf::RectangleShape> temp;

        for(int c = 0; c < X_num_sqrs; c++){
            sf::RectangleShape square(sf::Vector2f(X_size,Y_size));
            square.setFillColor(sf::Color::Black);
            square.setPosition(c*(WIDTH/X_num_sqrs),r*(HEIGTH/Y_num_sqrs));
            temp.push_back(square);
        }
        theSquares.push_back(temp);
    }
    std::cout<<"Done"<<std::endl;
    std::cout<<"Vector size is: "<<theSquares.size()<<std::endl;


    int numDays = 0;
    int numAlive = 1; 
    while (window.isOpen() && numAlive)
    {
        //std::cout<<"Day: "<<numDays<<std::endl;
        numDays++;
        clock.restart().asSeconds();

        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);


        for(int r = 0; r < Y_num_sqrs; r++){
            for(int c = 0; c < X_num_sqrs; c++){
                if(ptr_1[r][c]){
                    window.draw(theSquares[r][c]);
                }
            }
        }

        window.display();
        
        numAlive = updateWorld();

        while(clock.getElapsedTime() <= waitTime){
            //wait
        }
    }

    std::cout<<"All cells are dead after day "<<numDays<<std::endl;
    while (window.isOpen()){
        clock.restart().asSeconds();

        sf::Event evnt;
        while (window.pollEvent(evnt)){
            if (evnt.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.display();       
        
        while(clock.getElapsedTime() <= sf::seconds(0.5f)){
            //wait
        }
        window.close();
    }


    return 0;
}