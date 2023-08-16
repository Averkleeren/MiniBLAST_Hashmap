#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include "hashy.h"

class hash_string_obj {
public:
    unsigned int operator() ( const std::string &key ) const {
    
    unsigned int hash = 0;
    for(unsigned int i = 0; i<key.length(); i++){
        // hash += int(key[i]);
        //hash += (int(key[i]) * (i+1));
        if(key[i] == 'A'){
            hash+=(1*i)^12312 ;
        }
        else if(key[i] == 'C'){
            hash+=(2*i)^664252;
        }
        else if(key[i] == 'T'){
            hash +=(3*i)^3122490;
        }
        else{
            hash+=(4*i)^123901824;
        }
    }

    return hash;
  }   
  
};

typedef hashy< hash_string_obj> hash_type;

std::vector< std::pair< std::pair<std::string, int>, int> > \
similars(hash_type &set1, const std::string &stream, int length, int offset){
    // this command returns a vector of all the strings that fit the
    // criteria alond with the starting location and how much they
    // differ by.
    const char letters[4] = {'A', 'C', 'T', 'G'};
    std::string main, none, product;
    std::vector< std::pair< std::pair<std::string, int>, int> > finals;
    std::vector<int> positions, temp;
    int dif = 0;
    // start by creating the first parts of stream
    // then find all the locations of that string
    main = stream.substr(0, length);
    positions = set1.starts(main);

    //loop through all those positions
    for(int j = 0; j < positions.size(); j++){
        dif = 0;

        //safety
        for(int s = 0; s<main.size(); s++){
            if(main[s] != stream[s]){
                dif++;
            }
        }

        //first few bases get added to string
        product = main;
        
        //loop through rest of the input query
        for(int i =1; i < (stream.size()-(length-1)); i++){
            
            //create string of the previous letters or 1 less than length
            //this is so you can determine what the next letter should be
            none = product.substr(i, length-1);

            //loop through all the possible base pairs that could be inputed
            //and if the pair fits, add it to the string
            for(int L =0; L<4; L++){
                temp = set1.starts(none+letters[L]);
                std::vector<int>::iterator it = std::find(temp.begin(), temp.end(), positions[j]+i);
                if(it != temp.end()){
                    none += letters[L];

                    //if the letter that gets added is different from the
                    //requested letter, increase the difference count
                    if(stream[i+length-1] != letters[L]){
                        dif++;
                    }
                }
            }

            //stop condition
            if(dif > offset){
                break;
            }
            //safety
            if(none.size()!=length){
                break;
            }
            
            //if it passes the cases, add the letter to the product
            product += none[length-1];
            
            //check if done
            if(product.size() == stream.size()){
                int w = 0;
                //safety
                for(; w < finals.size(); w++){
                    if(finals[w].first.first == product && finals[w].first.second == positions[j]){
                        break;
                    }
                    
                }
                //add to return list
                if(w == finals.size()){
                    finals.push_back(std::make_pair(std::make_pair(product, positions[j]), dif));
                }
            }
        }
    }
    return finals;
}

void main_commands(hash_type &set1, int length){
    std::string command, stream;
    int number;
    std::vector< std::pair< std::pair<std::string, int>, int> > match;
    while(std::cin >> command){
        if(command == "query"){
            std::cin >> number >> stream;
            std::cout << "Query: " << stream << std::endl;
            match = similars(set1, stream, length, number);
            if(match.size() == 0){
                std::cout << "No Match" << std::endl; 
            }
            else{
                for(int i = 0; i<match.size(); i++){
                    std::cout << match[i].first.second << ' ' <<\
                    match[i].second << ' ' << match[i].first.first << std::endl;
                }
            }
        }
        else{
            return;
        }
    }
}

void table(std::ifstream& gene_file, int length, int size, double occupancy){
    hash_type set1(size, occupancy);
    std::string genome, filler;
    while(gene_file >> filler){
        genome += filler;
    }
    for(int i = 0; i<genome.size()-length; i++){
        set1.insert(genome.substr(i, length), i);
    }
    
    //set1.print(std::cout);
    main_commands(set1, length);
}


int main(int argc, char* argv[]){
    std::string file, command;
    int number = 0, size = 100;
    double occupancy = 0.5; 
    while(std::cin >> command){
        if(command == "genome"){
            std::cin >> file;
        }
        else if(command == "table_size"){
            std::cin >> size;
        }
        else if(command == "occupancy"){
            std::cin >> occupancy;
        }
        else if(command == "kmer"){
            std::cin >> number;
            std::ifstream gene_file(file);
            if (!gene_file) {
                std::cerr << "Could not open " << file << " to read\n";
                break;
            }
            table(gene_file, number, size, occupancy);
            break;
        }
        
    }


}