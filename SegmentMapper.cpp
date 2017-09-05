#include "provided.h"
#include "MyMap.h"
#include <vector>
#include <cctype>
#include "support.h"
using namespace std;

class SegmentMapperImpl {
private:
    MyMap<GeoCoord, vector<StreetSegment>> segment_mapper;
    
    void pushSeg(GeoCoord& gc, StreetSegment& seg) {
        vector<StreetSegment>* temp;
        temp = segment_mapper.find(gc);
        if (temp == nullptr) {
            vector<StreetSegment> v;
            v.push_back(seg);
            segment_mapper.associate(gc, v);
        }
        else {
            temp->push_back(seg);
        }
    }
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
};

SegmentMapperImpl::SegmentMapperImpl() {}

SegmentMapperImpl::~SegmentMapperImpl() {}

void SegmentMapperImpl::init(const MapLoader& ml) {
    StreetSegment seg;
    for (int i = 0; i < ml.getNumSegments(); i++) {
        ml.getSegment(i, seg);
        
        GeoCoord start = seg.segment.start;
        pushSeg(start, seg);
        
        GeoCoord end = seg.segment.end;
        pushSeg(end, seg);
        
        for (int j = 0; j < seg.attractions.size(); j++) {
            GeoCoord attr = seg.attractions[j].geocoordinates;
            if (!(attr == start || attr == end)) {
                pushSeg(attr, seg);
            }
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const {
    const vector<StreetSegment>* temp;
    temp = segment_mapper.find(gc);
    if (temp != nullptr) {
        return *temp;
    }
    else {
        vector<StreetSegment> p;
        return p;
    }
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
