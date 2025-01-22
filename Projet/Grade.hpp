#pragma once

#include <string>
#include <sstream>
#include <vector>
#include "Generic_mapper.hpp"

namespace travel {
    class Grade {
    public:
        Grade(bool use_small_database); // true: small database, false: ratp database
        void stations(Generic_station_parser& parser);
        void connections(Generic_connection_parser& parser);
        void dijkstra(Generic_mapper& mapper, bool input = false);

        std::string stations_filename;
        std::string connections_filename;
    };

    static travel::Grade evaluate_small(true);
    static travel::Grade evaluate(false);


    Grade::Grade(bool use_small_database)
    {
        if (use_small_database) {
            stations_filename = "data/stations.csv";
            connections_filename = "data/connections.csv";

        }
        else {
            stations_filename = "data/s.csv";
            connections_filename = "data/c.csv";
        }
    }

    inline void Grade::stations(Generic_station_parser& parser)
    {
        parser.read_stations(stations_filename);
    }

    inline void Grade::connections(Generic_connection_parser& parser)
    {
        parser.read_connections(connections_filename);
    }

    inline void Grade::dijkstra(Generic_mapper& mapper, bool input)
    {
        mapper.read_connections(connections_filename);
		mapper.read_stations(stations_filename);
        uint64_t input_num=1;
        if (input) {
            while (true) {
                std::cout << "=================>Grade "<< input_num++ << "<==================" << std::endl;
                std::cout << "Enter start station: ";
                std::string start;
                //std::cin >> start;
                std::getline(std::cin, start);

                std::cout << "Enter end station: ";
                std::string end;
                //std::cin >> end;


                std::getline(std::cin, end);
                mapper.compute_and_display_travel(start, end);
                std::cin.clear();
            }
        }
        else {
            while (true) {
                std::cout << "=================>Grade " << input_num++ << "<==================" << std::endl;
                std::cout << "Enter start station id: ";
                uint64_t start;
                std::cin >> start;
                std::cout << "Enter end station id: ";
                uint64_t end;
                std::cin >> end;
                mapper.compute_and_display_travel(start, end);
                // Refresh some incorrect input
                std::cin.clear();
                std::cin.ignore(INT16_MAX, '\n');
            }
        }

    }
}
