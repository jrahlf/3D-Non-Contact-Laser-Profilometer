
#include "slantCorrection.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <algorithm>

using namespace std;

namespace{

vector<vector<double> > lut;
int xLength = 1;
int yLength = 1;
double xmin, xmax;
double ymin, ymax;

bool getData(string line, double& x, double& y, double& m){
    std::istringstream i(line);
    return (i >> x) && (i >> y) && (i >> m);
}
}

int getXIndex(double x){
    if(x < xmin){
        x = xmin;
    }
    if(x > xmax){
        x = xmax;
    }

    int xPos = int((x/xmax)*xLength + 0.5f);
    //cout << "x " << xPos << " y " << yPos << endl;

    return xPos;
}

int getYIndex(double y){
    if(y < ymin){
        y = ymin;
    }
    if(y > ymax){
        y = ymax;
    }

    int yPos = int((y/ymax)*yLength + 0.5f);
    //cout << "x " << xPos << " y " << yPos << endl;

    return yPos;
}

bool SlantCorrect::init(string file){
    cout << "[SlantCorrect] init: ";
    if(file.length() == 0 || file == ""){
        cout << "not correcting values" << endl;
        xLength = 1;
        yLength = 1;
        lut.push_back(vector<double>());
        lut.at(0).push_back(0);
        return true;
    }
    cout << "using " << file << endl;
    return true;
    xmin = INT_MAX;
    xmax = INT_MIN;
    ymin = INT_MAX;
    ymax = INT_MIN;
    xLength = 1;
    yLength = 1;


    std::ifstream in;
    in.open(file);
    int c = 0;
    double x,y,m;
    vector<double> yValues;
    while(in){
        string line;
        getline(in, line);
        //cout << line << endl;
        bool ok = getData(line, x, y, m);
        if(!ok){
            cerr << "failed to parse " << line << endl;
            continue;
        }
        auto const itery = std::upper_bound(yValues.begin(), yValues.end(), y);
        yValues.insert(itery, y);
        if(itery == yValues.end()){
            lut.push_back(vector<double>());
        }
        int yIndex = itery - yValues.begin();
        vector<double> lutx = lut.at(yIndex);
        auto const iterx = std::upper_bound(lutx.begin(), lutx.end(), x);
        lutx.insert(iterx, m);
        c++;
    }
    cout << " read " << c << " lines" << endl;

    return true;
}

void SlantCorrect::correct(double x, double y, double& z){

    double corr = y*9.103;
    z -= corr;
    return;

    int xi = getXIndex(x);
    int yi = getYIndex(y);
    cout << "for x " << x << " y " << y << " z " << z << " yi " << yi << " -> ";
    z -= lut.at(yi).at(xi);
    cout << z << endl;

}
