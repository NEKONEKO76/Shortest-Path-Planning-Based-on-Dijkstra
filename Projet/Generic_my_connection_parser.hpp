// 1. This class Generic_my_connection_parser is used to read and parse files containing station and connection information, and store this information in a hash table.
// 2. The read_connections function reads the connection information file and stores the parsed connection information in the connections_hashmap hash table.
// 3. The read_stations function reads the station information file and stores the parsed station information in the stations_hashmap hash table.
// 4. If an exception occurs during the reading process, the corresponding hash table will be cleared and an error message will be output.

#pragma once
#include <fstream>
#include "Helper.hpp"
#include "Generic_connection_parser.hpp"
#include "Generic_my_station_parser.hpp"
using namespace travel;

class Generic_my_connection_parser : public Generic_connection_parser {
public:
    // Declare two virtual functions, read_connections and read_stations, for reading connection information and station information.
    virtual void read_connections(const std::string& _filename) override;
    
    virtual void read_stations(const std::string& _filename) override;
};

// Implement the read_connections function, the parameter is the file name containing connection information
void Generic_my_connection_parser::read_connections(const std::string& _filename) {

    //Create an input file stream object infile and open the specified file. If the file fails to open, an error message is output and returned.
    std::ifstream infile(_filename);
    if (!infile) {
        std::cerr << "Cannot open file:" << _filename << std::endl;
        return;
    }

    //Define a string variable line to store the content of each line, and line_num to record the current line number. Line numbers are not added to the hash table
    std::string line;
    int line_num = 1;
    try
    {
        //Read the first line first, the first line can be ignored
        std::getline(infile, line);

        // getline is used to read infile file information and store it in line. If the reading fails, False will be returned.
        while (std::getline(infile, line)) {
            // Enter a loop to read the file contents line by line. Each time a line is read, the line number is incremented by 1.
            ++line_num;
            // Use the split_string_by_string(line, ",") function to separate the line content into multiple strings by commas and store them in the tokens vector.
            std::vector<std::string> tokens = split_string_by_string(line, ",");
            // Use std::stoull to convert the string in tokens to unsigned long long type,
                         // And store these values in the connections_hashmap hash table. The key is the origin station ID and the value is a hash table representing other stations connected to the origin station and their weights or distances.
            this->connections_hashmap[std::stoull(tokens[0])].emplace(std::stoull(tokens[1]), std::stoull(tokens[2]));
        }
    }
    catch (const std::exception& e)
    {
        this->connections_hashmap.clear();
        std::cerr << "Read file[" << _filename << "] error at line[" << line_num << "]:" << e.what() << std::endl;
    }
}


void Generic_my_connection_parser::read_stations(const std::string& _filename)
{
    std::ifstream infile(_filename);
    if (!infile) {
        std::cerr << "Cannot open file:" << _filename << std::endl;
        return;
    }
    std::string line;
    int line_num = 1;
    try
    {

        std::getline(infile, line);
        while (std::getline(infile, line)) {
            ++line_num;
            std::vector<std::string> tokens = split_string_by_string(line, ",");
            Station station;
            station.name = tokens[0];
            station.line_id = tokens[1];
            station.address = tokens[2];
            station.line_name = tokens[3];
            this->stations_hashmap[std::stoull(station.line_id)] = station;
        }
    }
    catch (const std::exception& e)
    {
        this->stations_hashmap.clear();
        std::cerr << "Read file[" << _filename << "] error at line[" << line_num << "]:" << e.what() << std::endl;
    }

    return;
}