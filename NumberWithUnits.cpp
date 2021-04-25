#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "NumberWithUnits.hpp"
using namespace std;
namespace ariel {

    
        NumberWithUnits::NumberWithUnits(double v,const string& u) {
                if(data.count(u) != 0) {this->value = v; this->unit = u;}
                else {throw invalid_argument {"This unit does not match the units in the file"};}
        }

        /* This function read from file and insert the records to map data structure.
           The map saves the data in the way it is written in the file e.g. <km : < m : 1000>>.
           In addition all the pairs are saved on a vector of pairs data structure. */
        void NumberWithUnits::read_units(ifstream & file){
                string from;
                string equal;
                string to;
                double n = 0;
                double value = 0;
                while(file >> n >> from >> equal >> value >> to) {
                        data[from][to] = value;
                        // vice versa
                        data[to][from] = n/value;
                        // insert all the pairs to vector data structure
                        pairs.emplace_back(from,to);
                        pairs.emplace_back(to,from);
                }
                update();
                // showData();
        }

        /* This method uses the vector of pairs data structure,
           the meaning behind its to perform a "brute force search" to match all measurement units.
           @Note the time complexity of this function is O(n^2).*/
        void NumberWithUnits::update() {
                for (auto & pair : pairs) {
                      string unit1 = pair.first;
                      string unit2 = pair.second;
                      for (auto & current : data[unit2]) {
                            string unit3 = current.first;
                            if(unit3 == unit1) {continue;}
                            data[unit1][unit3] = data[unit2][unit3] * data[unit1][unit2];
                            data[unit3][unit1] = 1/data[unit1][unit3];
                }
        }
}

        /* This function converts the appropriate value based on the map data structure.
           @return the converted value.*/
        static double conversionOf(const string& from ,const string& to ,double value) {
               if(from == to) {return value;}
               try {
                 return data.at(from).at(to) * value;
               }
               catch(...) {throw invalid_argument {"Units do not match - ["+from+"]"+" cannot be converted to ["+to+"]"};}
        }


        /* Overloading arithmetic operators */

        NumberWithUnits NumberWithUnits::operator + (const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,this->unit,n2.value);
                return NumberWithUnits {this->value + converted , this->unit};
        }
        NumberWithUnits & NumberWithUnits::operator += (const NumberWithUnits& n2) {
                double converted = conversionOf(n2.unit,this->unit,n2.value);
                this->value += converted;
                return *this;
        }
        NumberWithUnits NumberWithUnits::operator - (const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,this->unit,n2.value);
                return NumberWithUnits {this->value - converted , this->unit};
        }
        NumberWithUnits & NumberWithUnits::operator -= (const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,this->unit,n2.value);
                this->value -= converted;
                return *this;
        }
        NumberWithUnits NumberWithUnits::operator - (){
                return NumberWithUnits {-this->value , this->unit};
        }
        NumberWithUnits NumberWithUnits::operator + (){return *this;}


        /* Overloading increment and decrement operators */

        NumberWithUnits & NumberWithUnits::operator ++ (){ //prefix return by reference
                this->value ++;
                return *this;
        }
        NumberWithUnits NumberWithUnits::operator ++ (int n){ //postfix return by value
                NumberWithUnits ret = *this;
                this->value ++;
                return ret;
        }
        NumberWithUnits & NumberWithUnits::operator -- (){ //prefix return by reference
                this->value --;
                return *this;
        }
        NumberWithUnits NumberWithUnits::operator -- (int n){ //postfix return by value
                NumberWithUnits ret = *this;
                this->value --;
                return ret;
        }


        /* Overloading multiplication operations */

        NumberWithUnits operator * (const NumberWithUnits& n, double num){
                return NumberWithUnits {n.value * num , n.unit};
        }
        NumberWithUnits operator * (double num, const NumberWithUnits& n){
                return NumberWithUnits {n.value * num , n.unit};
        }

        /* Overloading the comparison operators */

        bool operator > (const NumberWithUnits &n1 , const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,n1.unit,n2.value);
                return (n1.value > converted);
        }
        bool operator >= (const NumberWithUnits& n1 , const NumberWithUnits& n2){
                return ((n1 == n2) || (n1 > n2));
        }
        bool operator <  (const NumberWithUnits& n1 , const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,n1.unit,n2.value);
                return (n1.value < converted);
        }
        bool operator <=  (const NumberWithUnits& n1 , const NumberWithUnits& n2){
                return ((n1 == n2) || (n1 < n2));
        }
        bool operator == (const NumberWithUnits& n1 , const NumberWithUnits& n2){
                double converted = conversionOf(n2.unit,n1.unit,n2.value);
                return (abs(n1.value - converted) < epsilon);
        }

        bool operator != (const NumberWithUnits& n1 , const NumberWithUnits& n2){
                return (!(n1 == n2));
        }

        /* Overloading in/out stream operators */

        ostream& operator << (ostream& out, const NumberWithUnits& t){
                return out << t.value << "[" << t.unit << "]";
        }

        istream& operator >> (istream& in, NumberWithUnits& t){
                string input;
                getline (in, input,']');
                validation(t,input);
                if(data.count(t.unit) != 0){return in;}
                throw invalid_argument {"This unit does not match the units in the file"};
        }


        /*This function checks the integrity of the input while ignoring spaces and parentheses */
        void validation(NumberWithUnits& num , const string & input) {
                string value; string unit;
                uint left = input.find('[');
                uint right = input.length();
                for (uint i = 0; i < left; i++) {
                        if (input.at(i) != ' ') {value += input.at(i);}
                }
                for (uint i = left + 1; i < right; i++) {
                        if(input.at(i) != ' ' ) {unit += input.at(i);}
                }
                num.value = stod(value);
                num.unit = unit;
        } 

        /* This function print the records of the map data structure */
        void NumberWithUnits::showData(){
                for (auto & key1 : data) {
                        cout << "<" << key1.first << ": {";
                        for (auto & key2 : data[key1.first]) {
                                cout <<"<" << key2.first << " : " <<  key2.second << "> ";
                        }
                        cout << "}\n";
                }
        }
}