#include <iostream>
#include "Grade.hpp"
#include "Generic_my_connection_parser.hpp"
#include "Generic_my_station_parser.hpp"
#include "Generic_my_mapper.hpp"
#include <windows.h>  // Contains Windows API header files

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	Generic_my_connection_parser connection_parser;
	Generic_my_station_parser station_parser;
	Generic_my_mapper mapper;

	// Determine whether to apply small data implementation
	std::string use_small_data;
	std::cout << "use small data to calculate route(yes or no):";
	std::cin >> use_small_data;
	bool use_small = (use_small_data == "yes");
	travel::Grade grade(use_small);
	//Verify that the read data is correct. If no error output is printed, it means that the read data is correct.
	grade.stations(station_parser);
	//Verify that the read data is correct. If no error output is printed, it means that the read data is correct.
	grade.connections(connection_parser);
	//Calculate the shortest path using heap-optimized dijstra's algorithm
	std::string use_name;
	std::cout << "use name to calculate route(yes or no):";
	std::cin >> use_name;
	if (use_name == "yes") {
		grade.dijkstra(mapper, true);
	}
	else {
		grade.dijkstra(mapper, false);
	}

	return 0;
}
