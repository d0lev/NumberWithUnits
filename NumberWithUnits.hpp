#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>
using namespace std;

namespace ariel {

    static map <string,map<string,double>> data;
    static vector<pair<string,string>> pairs;
    static const double epsilon = 0.001;
    class NumberWithUnits {

        double value;
        string unit;
   
    public:

        NumberWithUnits(){}
        NumberWithUnits(double,const string&);
        static void read_units(ifstream& units_file);
        static void update();
        static void showData();

        /* Overloading the arithmetic operators */
        NumberWithUnits operator + (const NumberWithUnits& n2);
        NumberWithUnits operator + ();
        NumberWithUnits & operator += (const NumberWithUnits& n2);
        NumberWithUnits operator - (const NumberWithUnits& n2);
        NumberWithUnits operator - ();
        NumberWithUnits & operator -= (const NumberWithUnits& n2);

        /* Increment operations overiding */
        NumberWithUnits & operator ++ ();
        NumberWithUnits   operator ++ (int);    
        NumberWithUnits & operator -- ();
        NumberWithUnits   operator -- (int);

        /* Multiplication operations overiding */
        friend NumberWithUnits operator*(const NumberWithUnits& n, double num);
        friend NumberWithUnits operator*(double num, const NumberWithUnits& n);

        /* Overloading the comparison operators - Declerations */
        friend bool operator >  (const NumberWithUnits &n1 , const NumberWithUnits& n2);
        friend bool operator >= (const NumberWithUnits& n1 , const NumberWithUnits& n2);
        friend bool operator <  (const NumberWithUnits& n1 , const NumberWithUnits& n2);
        friend bool operator <= (const NumberWithUnits& n1 , const NumberWithUnits& n2);
        friend bool operator == (const NumberWithUnits& n1 , const NumberWithUnits& n2);
        friend bool operator != (const NumberWithUnits& n1 , const NumberWithUnits& n2);

        friend void validation(NumberWithUnits& num , const string & input);
        friend ostream& operator << (ostream& out, const NumberWithUnits& t);
        friend istream& operator >> (istream& in, NumberWithUnits& t);

    };
}