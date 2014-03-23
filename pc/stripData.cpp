#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

static string getM(string line){
    istringstream iss(line, istringstream::in);
    string bla;
    iss >> bla;
    iss >> bla;
    iss >> bla;
    return bla;
}

static string getY(string line){
    istringstream iss(line, istringstream::in);
    string bla;
    iss >> bla;
    iss >> bla;
    return bla;
}

static string getX(string line){
    istringstream iss(line, istringstream::in);
    string bla;
    iss >> bla;
    return bla;
}

static bool getValues(const std::string& line, double& x, double& y, double& z){
    istringstream iss(line, istringstream::in);
    return (iss >> x) && (iss >> y) && (iss >> z);
}


static double string_to_double( const std::string& s ){
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    return 0;
  return x;
}

static int string_to_int( const std::string& s ){
  std::istringstream i(s);
  int x;
  if (!(i >> x))
    return 0;
  return x;
}

void help(){
    cerr << "Params: infile outfile compression [xlow] [xhigh] [ylow] [yhigh] [zLow] [zHigh] [invert]" << endl;
    cerr << "Use '-' to leave a parameter unset" << endl;
    //exit(1);
}

int main(int argc, char** argv){

    if(argc < 4){
        help();
        return 1;
    }

    double xlow = 0;
    double xhigh = 100;
    double ylow = 0;
    double yhigh = 100;
    double zlow = -1000;
    double zhigh = 1000;
    int compression = 0;
    bool invert = 0;

    compression = int(string_to_int(argv[3]));
    if(argc > 4 && string(argv[4]) != string("-")){
        xlow = string_to_double(argv[4]);
    }
    if(argc > 5 && string(argv[5]) != string("-")){
        xhigh = string_to_double(argv[5]);
    }
    if(argc > 6 && string(argv[6]) != string("-")){
        ylow = string_to_double(argv[6]);
    }
    if(argc > 7 && string(argv[7]) != string("-")){
        yhigh = string_to_double(argv[7]);
    }
    if(argc > 8 && string(argv[8]) != string("-")){
        zlow = string_to_double(argv[8]);
    }
    if(argc > 9 && string(argv[9]) != string("-")){
        zhigh = string_to_double(argv[9]);
    }
    if(argc > 10 && string(argv[10]) != string("-")){
        invert = string_to_int(argv[10]);
    }

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

    cout << "Parameters: " << endl;
    cout << "\tcompression: " << compression << endl;
    cout << "\tX[" << xlow << "," << xhigh << "]" << endl;
    cout << "\tY[" << ylow << "," << yhigh << "]" << endl;
    cout << "\tZ[" << zlow << "," << zhigh << "]" << endl;
    if(invert){
        cout << "Removing specified values" << endl;
    }else{
        cout << "Keeping only specified values" << endl;
    }

    int pos = 0;

    int skippedLines = 0;
    int compressFactor = compression;
    double x,y,z;

    if(invert){
        while(in){
            string line;
            getline(in, line);

            bool ok = getValues(line, x, y, z);
            if(!ok){
                cerr << "ignoring malformed line: " << line << endl;
                continue;
            }

            if(     y >= ylow && y <= yhigh &&
                    x >= xlow && x <= xhigh &&
                    z >= zlow && z <= zhigh){
                continue;
            }

            /*skippedLines++;
            if(skippedLines >= compressFactor){
                skippedLines = 0;
                continue;
            }*/

            out << line << endl;

        }

    }else{
        while(in){
            string line;
            getline(in, line);

            bool ok = getValues(line, x, y, z);
            if(!ok){
                cerr << "ignoring malformed line: " << line << endl;
                continue;
            }

            if(     y < ylow || y > yhigh ||
                    x < xlow || x > xhigh ||
                    z < zlow || z > zhigh){
                continue;
            }

            /*skippedLines++;
            if(skippedLines >= compressFactor){
                skippedLines = 0;
                continue;
            }*/

            out << line << endl;

        }

    }


    in.close();
    out.close();
}


