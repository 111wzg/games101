#include <iostream>
// #include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigen>
#include <cmath>

using namespace std;
using namespace Eigen;

int main(){

    // Basic Example of cpp

    cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    cout << a << endl;
    cout << a/b << endl;
    cout << sqrt(b) << endl;
    cout << acos(-1) << endl;
    cout << sin(30.0/180.0*acos(-1)) << endl;

    // Example of vector
    cout << "Example of vector \n";
    // vector definition
    Vector3f v(1.0f,2.0f,3.0f);
    Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    cout << "Example of output \n";
    cout << v << endl;
    // vector add
    cout << "Example of add \n";
    cout << v + w << endl;
    // vector scalar multiply
    cout << "Example of scalar multiply \n";
    cout << v * 3.0f << endl;
    cout << 2.0f * v << endl;

    // Example of matrix
    cout << "Example of matrix \n";
    // matrix definition
    Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    cout << "Example of output \n";
    cout << i << endl;
    // matrix add i + j
    // matrix scalar multiply i * 2.0
    // matrix multiply i * j
    // matrix multiply vector i * v


    /* 
    * PA 0
    */
    // TO DO: Define point P
    // TO DO: Define rotation matrix M
    // TO DO: M * P

    Vector3f P(2.0f, 1.0f, 1.0f);
    cout << "P:" << endl;
    cout << P << endl;

    Matrix3f rot;
    rot << 1/sqrt(2.0f), -1/sqrt(2.0f), 0.0, 1/sqrt(2.0f), 1/sqrt(2.0f), 0.0, 0.0, 0.0, 1.0;
    cout << "rot:" << endl << rot << endl;

    Matrix3f parr;
    parr << 1.0, 0.0, 1.0, 0.0, 1.0, 2.0, 0.0, 0.0, 1.0;
    cout << "parr:" << endl << parr << endl;
    
    Vector3f res;
    res = parr * rot * P;
    cout << "res:" << endl;
    cout << res << endl;

    return 0;
}