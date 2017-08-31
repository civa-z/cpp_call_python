#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cpp_call_python.hpp"

#define random(x) (rand()%x)


int main(){
	srand((int)time(0));

    PythonCaller<double, double> pc("./", "py_test", "plus_one");

	std::vector<double> input;
	for (int i = 0; i < 10; ++i){
        int item = random(10);
		input.push_back(item);
        std::cout<<item<<" ";
	}
    std::cout<<std::endl;

    auto output = pc.call(input);
    for (auto item : output)
        std::cout<<item<<" ";
    std::cout<<std::endl;

	return 0;
}
