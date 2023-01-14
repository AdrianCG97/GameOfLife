
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>


int main(){
	int num;
	std::cout<<"Enter seed:  "<<std::endl;
	std::cin>>num;
	std::cout<<""<<std::endl;
	srand(num);
	int num1 = rand() % 100;
	int num2 = rand() % 100;
	std::cout<<num1<<"   "<<num2<<std::endl;
}