/***************************************************************************
 *   Copyright (C) 2016-2017 by Antonios Gouglidis			   			   *
 *   e-mail: agouglidis@gmail.com   					   				   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

string FloatToFixedString(float);

int main(int argc, char *argv[]) {
    
    // Check of arguments
    if (argc != 9) {
        cout << "\nObjective: Transforms all X Y Z coordinates of a .gro topology file.";
        cout << "\nUsage: groTransform SOURCE.gro TARGET.gro OPERATION X_VALUE OPERATION Y_VALUE OPERATION Z_VALUE \n" << endl ;
        cout << "\tOPERATION: (p)lus, (m)inus, (t)imes, (d)ivision." << endl;
        cout << "\nExample: groTransform SOURCE.gro TARGET.gro p 0.543 m 1.434 t 1.0\n" << endl;
        
        return 1;
    }
    
    // Set the source file and open for reading
    ifstream ifile_source;
    ifile_source.open(argv[1]);
    
    string OPERATION_X(argv[3]);
    string OPERATION_Y(argv[5]);
    string OPERATION_Z(argv[7]);
    
    // Perform some basic sanity checks
    if ((OPERATION_X != "p") && (OPERATION_X != "m") && (OPERATION_X != "t") && (OPERATION_X != "d")) {
        cout << "\nInvalid operation!" << endl;
        return 1;
    }
    
    if ((OPERATION_Y != "p") && (OPERATION_Y != "m") && (OPERATION_Y != "t") && (OPERATION_Y != "d")) {
        cout << "\nInvalid operation!" << endl;
        return 1;
    }
    
    if ((OPERATION_Z != "p") && (OPERATION_Z != "m") && (OPERATION_Z != "t") && (OPERATION_Z != "d")) {
        cout << "\nInvalid operation!" << endl;
        return 1;
    }
    
    cout << "Reading from file:" << argv[1];
    
    // Set the target file and open for writing
    ofstream ofile_target;
    ofile_target.open(argv[2]);
    
    cout << "\nWriting to file:" << argv[2];
    
    string sLine, sDummy, sPosition;
    float x, y, z;
    float FLOAT_X, FLOAT_Y, FLOAT_Z;
    
    istringstream sFLOAT_X(argv[4]);
    sFLOAT_X >> FLOAT_X;
    istringstream sFLOAT_Y(argv[6]);
    sFLOAT_Y >> FLOAT_Y;
    istringstream sFLOAT_Z(argv[8]);
    sFLOAT_Z >> FLOAT_Z;
    
     // Count the number of lines copied
    int iLines = 0;
    
    if (ifile_source.is_open()) {
        
        getline(ifile_source, sLine); // Get first line and copy it to target
        ofile_target << sLine << '\n';
        
        getline(ifile_source, sLine); // Get second line and copy it to target
        ofile_target << sLine << '\n';
        
        while (!ifile_source.eof()) {
            
            // Initialise everytime to be able to find last line
            z=99999;
            
            getline(ifile_source, sLine);
            
            
            // residue number (5 positions, integer); residue name (5 characters)
            // atom name (5 characters); atom number (5 positions, integer)
            
            // Copy first 4 fields
            sDummy = sLine.substr(0, 20);
            
            // Position (in nm, x y z in 3 columns, each 8 positions with 3 decimal places)
            sPosition = sLine.substr(20, 24);
            
            istringstream ssLine(sPosition);
            ssLine >> x >> y >> z;

            // This is for copying and writing the last line
            if (z == 99999) {
                ofile_target << sDummy << sPosition << '\n';
                break;
            }
            
            // Perform the operation
            if (OPERATION_X == "p") x += FLOAT_X;
            else if (OPERATION_X == "m") x -= FLOAT_X;
            else if (OPERATION_X == "t") x *= FLOAT_X;
            else if (OPERATION_X == "d") x /= FLOAT_X;
            
            if (OPERATION_Y == "p") y += FLOAT_Y;
            else if (OPERATION_Y == "m") y -= FLOAT_Y;
            else if (OPERATION_Y == "t") y *= FLOAT_Y;
            else if (OPERATION_Y == "d") y /= FLOAT_Y;
            
            if (OPERATION_Z == "p") z += FLOAT_Z;
            else if (OPERATION_Z == "m") z -= FLOAT_Z;
            else if (OPERATION_Z == "t") z *= FLOAT_Z;
            else if (OPERATION_Z == "d") z /= FLOAT_Z;
            
            // C format
            //"%5d%-5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f"
            //Copy 0 - 20 for 4 first columns 8.4 for the last 3 x y z
            
            if (!sLine.empty() && z != 99999) {
                ofile_target << sDummy << FloatToFixedString(x) << FloatToFixedString(y) << FloatToFixedString(z) << '\n';
                ++iLines;
            }
        }
            
    }
    
    // Close input and output files
    ifile_source.close();
    ofile_target.close();
    
    cout << "\nNumber of lines converted:" << iLines << "\nEnjoy your new topology file!\n";
    
    return 0;
}

string FloatToFixedString(const float num){
    
    ostringstream sNum;
    sNum << fixed << setprecision(3) << num;
    
    string s(sNum.str());
    string sFixedFormatNumber = "";
    
    for (int i=0; i < (10-s.length()-2); ++i) {
        sFixedFormatNumber += " ";
    }
    
    sFixedFormatNumber += s;
    
    return sFixedFormatNumber;
}
