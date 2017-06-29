/***************************************************************************
 *   Copyright (C) 2017 by Antonios Gouglidis			   			   *
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
#include <algorithm>


using namespace std;

int main(int argc, char *argv[]){
    
    // Check of arguments
    if (argc != 5) {
        
        cout << "\nObjective: Removes solvent atoms within a range of Z-axis values.";
        cout << "\nUsage: groRemoveSolvent SOURCE.gro TARGET.gro Z_VALUE_MIN Z_VALUE_MAX\n";
        cout << "\nExample: groRemoveSolvent SOURCE.gro TARGET.gro 0.543 1.434\n" << endl;
        
        return 1;
    }
    
    // Set the source file and open for reading
    ifstream ifile_source;
    ifile_source.open(argv[1]);
    
    cout << "Reading from file:" << argv[1];
    
    // Set the target file and open for writing
    ofstream ofile_target;
    ofile_target.open(argv[2]);
    
    cout << "\nWriting to file:" << argv[2];
    
    string sZMin = argv[3];
    string sZMax = argv[4];
    
    std::string::size_type sz;     // alias of size_t
    
    float z_min = stof (sZMin, &sz);
    float z_max = stof (sZMax, &sz);
    
    string sLine;

    int iDeleted = 0;
    
    if (ifile_source.is_open()){
        
        while (!ifile_source.eof()){
            
            getline(ifile_source, sLine);
            
            auto pos = sLine.find("SOL");
            
            if (pos == string::npos)
                ofile_target << sLine << '\n';
            else {
                // Keep the 3 SOL lines
                string s1 = sLine;
                string s2, s3;
                getline(ifile_source, s2);
                getline(ifile_source, s3);
                
                // copy the 3 Z-values
                string s1Z = s1.substr(37, 8);
                float Z1 = stof (s1Z, &sz);
                
                string s2Z = s2.substr(37, 8);
                float Z2 = stof (s2Z, &sz);
                
                string s3Z = s3.substr(37, 8);
                float Z3 = stof (s3Z, &sz);
                
                // Check if within limits or not
                bool deleteSOL = false;
                
                if ( ((Z1 >= z_min) && (Z1 <= z_max)) ||
                     ((Z2 >= z_min) && (Z2 <= z_max)) ||
                     ((Z3 >= z_min) && (Z3 <= z_max))
                    )
                    deleteSOL = true;
                    
                if (deleteSOL)
                    iDeleted+=3;
                else
                    ofile_target << s1 << '\n' << s2 << '\n' << s3 << '\n';
            }
        }
    }
    
    // Close input and output files
    ifile_source.close();
    ofile_target.close();
    
    cout << "\nNumber of removed lines:" << iDeleted << "\nEnjoy your new topology file!\n";
    
    return 0;
}
