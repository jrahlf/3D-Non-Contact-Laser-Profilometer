#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

string getM(string line){
    istringstream iss(line, istringstream::in);
    string bla;
    iss >> bla;
    iss >> bla;
    iss >> bla;
    return bla;
}

string getY(string line){
    istringstream iss(line, istringstream::in);
    string bla;
    iss >> bla;
    iss >> bla;
    return bla;
}

double string_to_double( const std::string& s ){
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    return 0;
  return x;
}

int string_to_int( const std::string& s ){
  std::istringstream i(s);
  int x;
  if (!(i >> x))
    return 0;
  return x;
}

int main(int argc, char** argv){
    if(argc < 1){
        cerr << "No input specified" << endl;
        return 1;
    }
    if(argc < 2){
        cerr << "No output file specified" << endl;
        return 1;
    }

    int threshold = 30;
    vector<string> lastMeasurements;

    ifstream in;
    in.open(argv[1]);
    ofstream out;
    out.open(argv[2]);

    if(!in){
        cerr << "input file no good" << endl;
        return 1;
    }
    if(!out){
        cerr << "output file no good" << endl;
        return 1;
    }

    int pos = 0;

    for(int i = 0; i < threshold; i++){
        lastMeasurements.push_back("9");
    }

    int oldY = -1;
    int skippedLines = 0;
    int compressFactor = 2;

    while(in){
        string line;
        getline(in, line);
        bool found = true;
        string m = getM(line);
        /*double mm = string_to_double(m);
        if(mm > 300 || mm < -300){
            continue;
        }*/
        string y = getY(line);
        int yy = string_to_int(y);
        if(skippedLines == 0){
            out << line << endl;
        }
        skippedLines++;
        skippedLines %= compressFactor;
        /*for(int i = 0; i < threshold; i++){
            if(m != lastMeasurements[i]){
                found = false;
                break;
            }
        }

        lastMeasurements[pos] = m;
        pos++;
        pos %= threshold;

        if(!found){
            out << line << endl;
        }*/
    }

    in.close();
    out.close();
}


