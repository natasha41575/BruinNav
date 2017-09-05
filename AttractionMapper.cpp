#include "provided.h"
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl {
private:
    MyMap<string, GeoCoord> attraction_mapper;
    
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
};

AttractionMapperImpl::AttractionMapperImpl() {}

AttractionMapperImpl::~AttractionMapperImpl() {}

void AttractionMapperImpl::init(const MapLoader& ml) {
    int i = 0;
    StreetSegment seg;
    while (ml.getSegment(i, seg)) {
        for (int j = 0; j < seg.attractions.size(); j++) {
            for (int k = 0; k < seg.attractions[j].name.size(); k++) {
                seg.attractions[j].name[k] = tolower(seg.attractions[j].name[k]);
            }
            attraction_mapper.associate(seg.attractions[j].name, seg.attractions[j].geocoordinates);
        }
        i++;
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const {
    for (int k = 0; k < attraction.size(); k++) {
        attraction[k] = tolower(attraction[k]);
    }
    const GeoCoord* ptr = (attraction_mapper.find(attraction));
    if (ptr == nullptr) {
        return false;
    }
    else {
        gc = *ptr;
        return true;
    }
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
