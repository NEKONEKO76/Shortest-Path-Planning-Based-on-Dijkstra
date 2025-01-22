#pragma once
#include <fstream>
#include "Helper.hpp"
#include "Generic_station_parser.hpp"

using namespace travel;

// This code defines a Generic_my_station_parser class that inherits from the Generic_station_parser class
// and implements a virtual function named read_stations, which is used to read station information from a file
// and store it in a hash table.
class Generic_my_station_parser : public Generic_station_parser {
public:
    virtual void read_stations(const std::string& _filename) override;

};

// Implements the read_stations function with the parameter being the filename of the station information file.
void Generic_my_station_parser::read_stations(const std::string& _filename) {

    // Create an input file stream object infile and open the specified file
    std::ifstream infile(_filename);

    // Check if the file was successfully opened. If the file fails to open, output an error message and return.
    if (!infile) {
        std::cerr << "Cannot open file:" << _filename << std::endl;
        return;
    }
    // Define a string variable line to store the content of each line, and line_num to record the current line number.
    std::string line;
    int line_num = 1;

    try
    {
        // Read the first line, which can be ignored
        std::getline(infile, line);

        // Enter a loop to read the file content line by line. Each time a line is read, the line number increases by 1
        while (std::getline(infile, line)) {
            ++line_num;

            // Use the split_string_by_string(line, ",") function to split the line content into multiple strings by commas and store them in the tokens vector.
            std::vector<std::string> tokens = split_string_by_string(line, ",");

            // Create a Station object and assign the values in tokens to the corresponding member variables.
            Station station;
            station.name = tokens[0];
            station.line_id = tokens[1];
            station.address = tokens[2];
            station.line_name = tokens[3];

            // Use station.line_id as the key and store the station object in the stations_hashmap hash table.
            // std::stoull is used to convert a string to an unsigned long long integer.
            this->stations_hashmap[std::stoull(station.line_id)] = station;
        }
    }

    // Catch any possible exceptions. If an exception occurs during the reading process, clear the stations_hashmap hash table and output an error message with the filename, line number, and exception description.
    catch (const std::exception& e)
    {
        this->stations_hashmap.clear();
        std::cerr << "Read file[" << _filename << "] error at line[" << line_num << "]:" << e.what() << std::endl;
    }

    return;
}
