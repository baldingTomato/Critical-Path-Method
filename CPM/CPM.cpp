#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>

struct Node
{
    std::string name;
    int duration;
    int es, ef, ls, lf;
    bool isCritical = false;
    bool isStarting = false;
    bool isEnding = false;

    std::vector<std::string> precedents;
    std::vector<std::string> consequents;

};

Node CreateNode(std::string label, int duration, std::vector<std::string> precedents) {

    Node node;

    node.name = label;
    node.duration = duration;
    
    if (!precedents.empty()) {

        for(std::string p : precedents){

            node.precedents.push_back(p);

        }

    }

    return node;
}

void ReadNetwork(std::vector<Node> &nodes){

    std::ifstream input_file("network.txt");
    std::string line;
    std::string label;
    std::string prec;
    std::vector<std::string> precedents;
    std::vector<std::string> consequents;
    int value = 0;

    if(!input_file.is_open()){

        std::cout << "Could not open the file \"network.txt\"" << std::endl;
        std::exit(1);

    }

    while(std::getline(input_file, line)){

        std::stringstream lineStream(line);
        precedents.clear();
        consequents.clear();

        lineStream >> label;
        lineStream >> value;

        // read precedents if you can and after reading each one, add it to the matching element of main vector as consequent
        while(lineStream >> prec)
        {

            precedents.push_back(prec);
            
            auto match = std::find_if(nodes.begin(), nodes.end(), [prec](const Node& obj) {return obj.name == prec; });

                if(match != nodes.end())
                {
                    // found element, 'match' is an iterator to the first matching element
                    auto index = std::distance(nodes.begin(), match);

                    nodes.at(index).consequents.push_back(label);
                }

        }

        nodes.push_back(CreateNode(label, value, precedents));

    }

}

void EstablishOrder(std::vector<Node>& nodes) {

    for(Node node : nodes){

        if(node.precedents.empty()){

            node.isStarting = true;
            //std::cout << node.name;

        }

        if(node.consequents.empty()){

            node.isEnding = true;
            //std::cout << node.name;

        }

        //std::cout << node.name << " " << node.isStarting << " " << node.isEnding << std::endl;

    }

}

void PrintNetwork(std::vector<Node> &nodes){

    for (Node n : nodes) {

        std::cout << n.name << " -> duration: " << n.duration;

        if (!n.precedents.empty()) {

            std::cout << ", precedents: ";

            for (std::string p : n.precedents) {

                std::cout << p << " ";

            }

        }

        if (!n.consequents.empty()){

            std::cout << ", consequents: ";

            for (std::string c : n.consequents) {

                std::cout << c << " ";

            }

        }

        std::cout << std::endl;

    }

}

int main(){

    std::vector<Node> nodes;

    ReadNetwork(nodes);

    PrintNetwork(nodes);

    EstablishOrder(nodes);

    return 0;
}