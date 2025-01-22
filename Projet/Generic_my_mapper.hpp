// This code defines a class named Generic_my_mapper, which inherits from Generic_mapper,
// Implemented methods for calculating and displaying travel paths. This class also implements methods for reading station and connection information.
// And provides the function of obtaining station information by name.


#pragma once
#include "Generic_mapper.hpp"
#include <queue>
#include <unordered_set>
#include "Helper.hpp"
using namespace travel;


class Generic_my_mapper : public Generic_mapper {
public:

    virtual std::vector<std::pair<uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end);
    virtual std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end);

    // Use names to detect stations
    virtual std::vector<std::pair<uint64_t, uint64_t> > compute_travel(const std::string& _start, const std::string& _end);
    virtual std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(const std::string& _start, const std::string& _end);
public:
    // Virtual functions need to be overridden, otherwise compilation errors
    virtual void read_connections(const std::string& _filename) override;
    virtual void read_stations(const std::string& _filename) override;
    std::unique_ptr<Station> get_station_by_name(const std::string& _name) const {
        if (this->stations_id_hashmap_by_name.find(_name) == this->stations_id_hashmap_by_name.end()) {
            return nullptr;
        }
        uint64_t id = this->stations_id_hashmap_by_name.at(_name);
        return unique::make_unique<Station>(this->stations_hashmap.at(id));
    }
protected:
    std::unordered_map<std::string, uint64_t> stations_id_hashmap_by_name;
};

std::vector<std::pair<uint64_t, uint64_t>> Generic_my_mapper::compute_travel(uint64_t _start, uint64_t _end)
{
    // If the start or end point does not exist, return empty
    // This function takes two parameters _start and _end, representing the IDs of the start and end points respectively.
    // First, check if the start or end point exists in connections_hashmap. If not, return an empty std::vector.
    if (this->connections_hashmap.find(_start) == this->connections_hashmap.end() || this->connections_hashmap.find(_end) == this->connections_hashmap.end()) {
        return std::vector<std::pair<uint64_t, uint64_t>>();
    }
    // Priority queue for storing the shortest path, elements in the priority queue are pairs (distance, node)
    // Declare a priority queue q for storing shortest path information. Elements in the priority queue are pairs, 
    // with the first element being the distance and the second element being the node ID. The queue is sorted by distance in ascending order.
    // std::vector<std::pair<uint64_t, uint64_t>> specifies the storage element type of our priority queue, 
    // std::greater<std::pair<uint64_t, uint64_t>> specifies the sorting method of our priority queue
    // Priority queue supports three operations: top to access the highest priority element, pop to remove the highest priority element, push to insert an element
    std::priority_queue<std::pair<uint64_t, uint64_t>,
        std::vector<std::pair<uint64_t, uint64_t>>,
        std::greater<std::pair<uint64_t, uint64_t>>> q;

    // Used to mark whether the node has been visited
    std::unordered_set<uint64_t> vis;

    // Used to store the shortest path to each node
    std::unordered_map<uint64_t, uint64_t> dis;

    // Used to store the predecessor of the shortest path to each node
    std::unordered_map<uint64_t, uint64_t> pre;

    // Initialize, put the starting point into the priority queue, set the distance to the starting point to 0
    q.push(std::make_pair(0, _start));
    dis[_start] = 0;

    // Main loop processes each node in the queue until the queue is empty.
    while (!q.empty()) {

        // Take out the shortest path from the priority queue
        uint64_t curdis = q.top().first;
        uint64_t u = q.top().second;
        q.pop();

        // Check if the current node u has been visited, if yes, skip the current iteration. Otherwise, mark it as visited.
        if (vis.find(u) != vis.end()) {
            continue;
        }
        vis.insert(u);

        // If the current node is the end point, end the loop.
        if (u == _end) {
            break;
        }

        // Traverse the adjacent nodes and update the shortest path of the adjacent nodes
        for (auto it = this->connections_hashmap[u].begin(); it != this->connections_hashmap[u].end(); it++) {
            uint64_t next_dist = curdis + it->second;
            uint64_t v = it->first;
            // If the adjacent node has not been visited or the path to the adjacent node is shorter, update the shortest path of the adjacent node
            if (dis.find(v) == dis.end() || next_dist < dis[v]) {
                q.push(std::make_pair(next_dist, v));
                dis[v] = next_dist;
                // Record the predecessor of the adjacent node
                pre[v] = u;
            }
        }
    }

    // Initialize res for saving the path
    std::vector<std::pair<uint64_t, uint64_t>> res;
    // Confirm unreachable, return empty
    if (vis.find(_end) == vis.end()) {
        return res;
    }
    // Confirm reachable, backtrack the path from the end point
    uint64_t cur = _end;
    while (cur != _start) {
        res.push_back(std::make_pair(cur, dis[cur]));
        cur = pre[cur];
    }
    res.push_back(std::make_pair(cur, dis[cur]));

    // Reverse the path, from end to start, to start to end
    std::reverse(res.begin(), res.end());
    return res;
}

// Used to detect the shortest path and then print it out
std::vector<std::pair<uint64_t, uint64_t>> Generic_my_mapper::compute_and_display_travel(uint64_t _start, uint64_t _end)
{
    std::vector<std::pair<uint64_t, uint64_t>> travel_cost = this->compute_travel(_start, _end);
    // Print path
    for (size_t i = 0; i < travel_cost.size(); i++) {
        uint64_t id = travel_cost[i].first;
        uint64_t cost = travel_cost[i].second;
        std::cout << this->stations_hashmap[id] << std::endl;
        std::cout << "travel cost: " << cost << std::endl;
        if (i != travel_cost.size() - 1) {
            std::cout << "  | \n<" << travel_cost[i + 1].second - cost << ">\n" << std::endl << "  | " << std::endl << "  V" << std::endl;
        }
    }
    if (travel_cost.size() == 0) {
        std::cout << "Unreachable!" << std::endl;
    }
    return travel_cost;
}
// Used to detect the shortest path
std::vector<std::pair<uint64_t, uint64_t>> Generic_my_mapper::compute_travel(const std::string& _start, const std::string& _end)
{
    std::unique_ptr<Station> station_start = this->get_station_by_name(_start);
    std::unique_ptr<Station> station_end = this->get_station_by_name(_end);
    if (station_start == nullptr || station_end == nullptr) {
        return std::vector<std::pair<uint64_t, uint64_t>>();
    }
    return this->compute_travel(std::stoull(station_start->line_id), std::stoull(station_end->line_id));
}

std::vector<std::pair<uint64_t, uint64_t>> Generic_my_mapper::compute_and_display_travel(const std::string& _start, const std::string& _end)
{
    std::unique_ptr<Station> station_start = this->get_station_by_name(_start);
    std::unique_ptr<Station> station_end = this->get_station_by_name(_end);
    if (station_start == nullptr || station_end == nullptr) {
        std::cout << "Unreachable!" << std::endl;
        return std::vector<std::pair<uint64_t, uint64_t>>();
    }
    return this->compute_and_display_travel(std::stoull(station_start->line_id), std::stoull(station_end->line_id));
}

// Used to read connection data
void Generic_my_mapper::read_connections(const std::string& _filename)
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
        // Read the first line, which can be ignored
        std::getline(infile, line);
        while (std::getline(infile, line)) {
            ++line_num;
            std::vector<std::string> tokens = split_string_by_string(line, ",");
            this->connections_hashmap[std::stoull(tokens[0])].emplace(std::stoull(tokens[1]), std::stoull(tokens[2]));
        }
    }
    catch (const std::exception& e)
    {
        this->connections_hashmap.clear();
        std::cerr << "Read file[" << _filename << "] error at line[" << line_num << "]:" << e.what() << std::endl;
    }
}

// Used to read station data
void Generic_my_mapper::read_stations(const std::string& _filename)
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
        // Read the first line, which can be ignored
        std::getline(infile, line);
        while (std::getline(infile, line)) {
            ++line_num;
            std::vector<std::string> tokens = split_string_by_string(line, ",");
            Station station;
            station.name = tokens[0];
            station.line_id = tokens[1];
            station.address = tokens[2];
            station.line_name = tokens[3];
            uint64_t id = std::stoull(station.line_id);
            // Store the corresponding station information, accessible by id
            this->stations_hashmap[id] = station;
            // Also accessible by station name
            this->stations_id_hashmap_by_name[station.name] = id;
        }
    }
    catch (const std::exception& e)
    {
        this->stations_hashmap.clear();
        std::cerr << "Read file[" << _filename << "] error at line[" << line_num << "]:" << e.what() << std::endl;
    }

    return;
}
