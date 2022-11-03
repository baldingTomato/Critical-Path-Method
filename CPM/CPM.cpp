#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
//#include <algorithm>

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

void EstablishOrder(std::vector<Node>& nodes){

    int index = 0;

    for(Node node : nodes){

        if(node.precedents.empty()){

            nodes.at(index).isStarting = true;
            //std::cout << node.name;

        }

        if(node.consequents.empty()){

            nodes.at(index).isEnding = true;
            //std::cout << node.name;

        }

        index++;

    }

}

void PrintNetwork(std::vector<Node> &nodes){

    for(Node n : nodes){

        std::cout << n.name << " -> duration: " << n.duration;

        if(!n.precedents.empty()){

            std::cout << ", precedents: ";

            for (std::string p : n.precedents) {

                std::cout << p << " ";

            }

        }

        if(!n.consequents.empty()){

            std::cout << ", consequents: ";

            for(std::string c : n.consequents){

                std::cout << c << " ";

            }

        }

        std::cout << std::endl;

    }

}

int MaxEF(std::vector<std::string> precs, std::vector<Node>& nodes){

    int max = 0;

    for(std::string label : precs){

        for(Node n : nodes){

            if(n.name == label && n.ef > max){

                max = n.ef;

            }

        }

    }

    return max;
}

void ForwardPass(std::vector<Node>& nodes){

    int index = 0;

    // calculate earliest start and earliest finish for starting nodes
    for(Node n : nodes){

        if (n.isStarting == true) {

            nodes.at(index).es = 0;
            nodes.at(index).ef = nodes.at(index).duration;

        }

        index++;

    }

    index = 0;

    // calculate earliest start and earliest finish for ending nodes
    for(Node n : nodes){

        if (n.isStarting == false) {

            nodes.at(index).es = MaxEF(n.precedents, nodes);
            nodes.at(index).ef = nodes.at(index).es + nodes.at(index).duration;

        }

        index++;

    }

}

int MaxLF(std::vector<std::string> cons, std::vector<Node>& nodes) {

    int max = 0;

    for(std::string label : cons){

        for(Node n : nodes){

            if(n.name == label && n.ls > max){

                max = n.ls;

            }

        }

    }

    return max;
}

void BackwardPass(std::vector<Node>& nodes){

    int max_ef = 0;

    // calculate latest start and latest finish for starting nodes
    for(Node n : nodes){

        if(n.isEnding == true && n.ef > max_ef){

            max_ef = n.ef;

        }

    }

    int index = nodes.size()-1;

    for(Node n : nodes){

        if(n.isEnding == true){

            nodes.at(index).lf = max_ef;
            nodes.at(index).ls = nodes.at(index).lf - nodes.at(index).duration;

        }

        index++;

    }

    for(int index = nodes.size() - 1; index >= 0; index--){



    }

    index = nodes.size()-1;

    // calculate latest start and latest finish for ending nodes
    for(Node n : nodes){

        if(n.isEnding == false){

            nodes.at(index).lf = MaxLF(n.consequents, nodes);
            nodes.at(index).ls = nodes.at(index).lf - nodes.at(index).duration;

        }

        index--;


    }

}

int main(){

    std::vector<Node> nodes;

    ReadNetwork(nodes);

    PrintNetwork(nodes);

    EstablishOrder(nodes);

    ForwardPass(nodes);
    BackwardPass(nodes);

    for (Node n : nodes) {

        std::cout << n.name << " -> duration: " << n.duration << " es: " << n.es << " ef: " << n.ef << " ls: " << n.ls << " lf: " << n.lf;;// << "--- " << n.isEnding << n.isStarting;

        if (!n.precedents.empty()) {

            std::cout << ", precedents: ";

            for (std::string p : n.precedents) {

                std::cout << p << " ";

            }

        }

        if (!n.consequents.empty()) {

            std::cout << ", consequents: ";

            for (std::string c : n.consequents) {

                std::cout << c << " ";

            }

        }

        std::cout << std::endl;

    }

    return 0;
}