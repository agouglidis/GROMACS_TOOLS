/***************************************************************************
 *   Copyright (C) 2017 by Antonios Gouglidis			   			       *
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
#include <cmath>
#include <sstream>
#include <string>


using namespace std;

void rotate(const double, const double, const double, const string);

int main(int argc, char *argv[]) {
    
    // Check arguments
    if (argc != 5) {
        cout << "\nObjective: Calculate the degrees of X or Y while rotating in 3D, such that the given point will almost fit on the Z = 0 plane, i.e.  Z value of the point will be 0 or close to 0." << endl;
        cout << "\nUsage: groRotate X_VALUE Y_VALUE Z_VALUE ROTATION_AXIS" << endl ;
        cout << "\t* X_VALUE, Y_VALUE, Z_VALUE are real numbers;" << endl;
        cout << "\t* ROTATION_AXIS is either X or Y." << endl;
        cout << "\nExample: groRotate 0.543 -1.434 0.724 X\n" << endl;
        
        return 1;
    }
    
    if ((*argv[4] != 'X') && (*argv[4] != 'x') && (*argv[4] != 'Y') && (*argv[4] != 'y')) {
        cout << "Invalid axis! Rotation performs only on X or Y axis." << endl;
        
        return -1;
    }
    
    double x, y, z;
    
    istringstream sDOUBLE_X(argv[1]);
    sDOUBLE_X >> x;
    
    istringstream sDOUBLE_Y(argv[2]);
    sDOUBLE_Y >> y;
    
    istringstream sDOUBLE_Z(argv[3]);
    sDOUBLE_Z >> z;
    
    if ((*argv[4] == 'X') || (*argv[4] == 'x'))
        rotate(x, y, z, "X");
    else if ((*argv[4] == 'Y') || (*argv[4] == 'y'))
        rotate(x, y, z, "Y");
    
    cout << endl;
	
	return 0;
}


void rotate(const double x, const double y, const double z, const string axis)
{
    double xp = x;
    double yp = y;
    double zp = z;
    
    if (axis == "X")
        cout << "\n--------- X rotation -----------\n";
    else if (axis == "Y")
        cout << "\n--------- Y rotation -----------\n";
    
    // Set the step for degrees
    double step=0.0;
    
    for (int i=0; i<360000; ++i) {
        if (axis == "X") {
            // Perform X rotation
            yp = y*cos(step * M_PI / 180) - z*sin(step * M_PI / 180);
            zp = y*sin(step * M_PI / 180) + z*cos(step * M_PI / 180);
        } else if (axis == "Y") {
            // Perform Y rotation
            zp = z*cos(step * M_PI / 180) - x*sin(step * M_PI / 180);
            xp = z*sin(step * M_PI / 180) + x*cos(step * M_PI / 180);
        }
        
        // Check when Z is close to 0
        if ( fabs(zp) < 1e-4 ) {
            cout << "\nValue of degrees: " << step;
            cout << "\nX = " << xp << " Y = " << yp << " Z = " << zp << endl;
        }
        
        // Increment degrees (step)
        step += 1e-3;
    }
}
