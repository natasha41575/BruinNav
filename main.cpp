#include "support.h"

#include <iostream>

#include <string>

#include <vector>

#include <cstring>

using namespace std;



// START OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)

// If you want the turn-by-turn directions to give a real direction like

// east or southwest instead of IN_SOME_DIRECTION, you'll need to

// implement the ordinary function

//    string directionOfLine(const GeoSegment& gs)

// to return a string like "east" or "southwest" based on the angle of the

// GeoSegment gs according to the table at the bottom of page 20 of the spec.

// When you do that, you can delete this comment and the template function

// below that appears here solely to allow this main.cpp to build.

// Why didn't we just write the real function for you?  Because it's also

// a function you'd find useful in producing the NavSegments in the navigate()

// method.  Since it's useful in more than one .cpp file, its declaration

// should go in support.h and its implementation in support.cpp.



string directionOfLine(const GeoSegment& gs)

{
    
    double angle = angleOfLine(gs);
    
    string direction;
    
    if (angle >= 0 && angle <= 22.5) {
        
        direction = "east";
        
    }
    
    else if (angle <= 67.5) {
        
        direction = "northeast";
        
    }
    
    else if (angle <= 112.5) {
        
        direction = "north";
        
    }
    
    else if (angle <= 157.5) {
        
        direction = "northwest";
        
    }
    
    else if (angle <= 202.5) {
        
        direction = "west";
        
    }
    
    else if (angle <= 247.5) {
        
        direction = "southwest";
        
    }
    
    else if (angle <= 292.5) {
        
        direction = "south";
        
    }
    
    else if (angle <= 337.5) {
        
        direction = "southeast";
        
    }
    
    else {
        
        direction = "east";
        
    }
    
    return direction;
    
}

// END OF WHAT YOU CAN REMOVE ONCE YOU'VE IMPLEMENTED string directionOfLine(const GeoSegment& gs)



void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments);

void printDirections(string start, string end, vector<NavSegment>& navSegments);



int main()

{
    
    
    
    
    
    Navigator nav;
    
    
    
    if ( ! nav.loadMapData("/Users/natasha/Downloads/mapdata.txt") )
        
    {
        
        cout << "Map data file was not found or has bad format: " << endl;
        
        return 1;
        
    }
    
    
    
    
    
    string start = "Murdock Plaza";
    
    string end = "The Playboy Mansion";
    
    vector<NavSegment> navSegments;
    
    
    
    NavResult result = nav.navigate(start, end, navSegments);
    
    
    
    
    
    switch (result)
    
    {
            
        case NAV_NO_ROUTE:
            
            cout << "No route found between " << start << " and " << end << endl;
            
            break;
            
        case NAV_BAD_SOURCE:
            
            cout << "Start attraction not found: " << start << endl;
            
            break;
            
        case NAV_BAD_DESTINATION:
            
            cout << "End attraction not found: " << end << endl;
            
            break;
            
        case NAV_SUCCESS:
            
            printDirections(start, end, navSegments);
            
            
            
            
    }
    
}



void printDirectionsRaw(string start, string end, vector<NavSegment>& navSegments)

{
    
    cout << "Start: " << start << endl;
    
    cout << "End:   " << end << endl;
    
    cout.setf(ios::fixed);
    
    cout.precision(4);
    
    for (auto ns : navSegments)
        
    {
        
        switch (ns.m_command)
        
        {
                
            case NavSegment::PROCEED:
                
                cout << ns.m_geoSegment.start.latitudeText << ","
                
                << ns.m_geoSegment.start.longitudeText << " "
                
                << ns.m_geoSegment.end.latitudeText << ","
                
                << ns.m_geoSegment.end.longitudeText << " "
                
                << ns.m_direction << " "
                
                << ns.m_distance << " "
                
                << ns.m_streetName << endl;
                
                break;
                
            case NavSegment::TURN:
                
                cout << "turn " << ns.m_direction << " " << ns.m_streetName << endl;
                
                break;
                
        }
        
    }
    
}



void printDirections(string start, string end, vector<NavSegment>& navSegments)

{
    
    cout.setf(ios::fixed);
    
    cout.precision(2);
    
    
    
    cout << "You are starting at: " << start << endl;
    
    
    
    double totalDistance = 0;
    
    string thisStreet;
    
    GeoSegment effectiveSegment;
    
    double distSinceLastTurn = 0;
    
    
    
    for (auto ns : navSegments)
        
    {
        
        switch (ns.m_command)
        
        {
                
            case NavSegment::PROCEED:
                
                if (thisStreet.empty())
                    
                {
                    
                    thisStreet = ns.m_streetName;
                    
                    effectiveSegment.start = ns.m_geoSegment.start;
                    
                }
                
                effectiveSegment.end = ns.m_geoSegment.end;
                
                distSinceLastTurn += ns.m_distance;
                
                totalDistance += ns.m_distance;
                
                break;
                
            case NavSegment::TURN:
                
                if (distSinceLastTurn > 0)
                    
                {
                    
                    cout << "Proceed " << distSinceLastTurn << " miles "
                    
                    << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
                    
                    thisStreet.clear();
                    
                    distSinceLastTurn = 0;
                    
                }
                
                cout << "Turn " << ns.m_direction << " onto " << ns.m_streetName << endl;
                
                break;
                
        }
        
    }
    
    
    
    if (distSinceLastTurn > 0)
        
        cout << "Proceed " << distSinceLastTurn << " miles "
        
        << directionOfLine(effectiveSegment) << " on " << thisStreet << endl;
    
    cout << "You have reached your destination: " << end << endl;
    
    cout.precision(1);
    
    cout << "Total travel distance: " << totalDistance << " miles" << endl;
    
}
