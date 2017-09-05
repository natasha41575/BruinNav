//
//  support.cpp
//  Project 4
//
//  Created by Natasha Sarkar on 3/13/17.
//  Copyright © 2017 Natasha Sarkar. All rights reserved.
//

#include "support.h"

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs) {
    return lhs.latitudeText == rhs.latitudeText && lhs.longitudeText == rhs.longitudeText;
}

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs) {
    if (lhs.latitudeText < rhs.latitudeText) {
        return true;
    }
    else if (lhs.latitudeText == rhs.latitudeText && lhs.longitudeText < rhs.longitudeText) {
        return true;
    }
    else return false;
}
