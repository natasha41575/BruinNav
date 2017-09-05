#ifndef MAP_CPP
#define MAP_CPP

#include "provided.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class MapLoaderImpl {
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
    vector<StreetSegment> segments;
    
};

MapLoaderImpl::MapLoaderImpl() {}

MapLoaderImpl::~MapLoaderImpl() {}

bool MapLoaderImpl::load(string mapFile) {
    ifstream file(mapFile);
    if (!file) {
        return false;
    }
    string s;
    while (getline(file, s)) {
        StreetSegment current;
        current.streetName = s;
        
        string lat1, lon1, lat2, lon2;
        getline(file, s, ',');
        lat1 = s;
        getline(file, s, ' ');
        if (s.size() <= 1) {
            getline(file, s, ' ');
        }
        lon1 = s;

        getline(file, s, ',');
        lat2 = s;
        getline(file, s);
        lon2 = s;
        
        GeoCoord start(lat1, lon1);
        GeoCoord end(lat2, lon2);
        
        GeoSegment seg(start, end);
        current.segment = seg;
        
        int k;
        file >> k;
        // If you want to consume and ignore the rest of the line the
        // number is found on, follow this with
        file.ignore(10000, '\n');
        
        vector<Attraction> a;
        for (int i = 0; i < k; i++) {
            Attraction attr;
            getline(file, s, '|');
            attr.name = s;
            
            getline(file, s, ',');
            string lat = s;
            getline(file, s);
            string lon = s;
            GeoCoord c(lat, lon);
            attr.geocoordinates = c;
            
            a.push_back(attr);
        }
        current.attractions = a;
        segments.push_back(current);
    }
    return true;
}

size_t MapLoaderImpl::getNumSegments() const {
    return segments.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const {
    if (segNum >= 0 && segNum < segments.size()) {
        seg = segments[segNum];
        return true;
    }
    return false;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
#endif
