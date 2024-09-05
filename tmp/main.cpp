#include <iostream>
#include <sstream>

int main(){
	std::string tmp = "tes te ttt";
	std::stringstream   stream(tmp);
	std::string trim;
	stream >> trim;
	std::cout << trim << '\n';
	stream >> trim;
	std::cout << trim << '\n';
	stream >> trim;
	std::cout << trim << '\n';
}