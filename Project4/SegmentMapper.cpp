#include "provided.h"
#include <vector>
#include "MyMap.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
    MyMap<string, vector<StreetSegment>> data;
	vector<string> destructIndex;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    for (size_t i = 0; i < ml.getNumSegments(); i++)
    {
        //Load each start and end coordinate of StreetSegments
		StreetSegment tmps;
        GeoSegment* geoSeg;
        GeoCoord* start, *end;
        vector<StreetSegment>* value;
        string coord;
        ml.getSegment(i, tmps);
        geoSeg = &(tmps.segment);
        start = &(geoSeg->start);
        end = &(geoSeg->end);
        
        //Load Start Coordinate
        coord = start->latitudeText + start->longitudeText;
        value = data.find(coord);
        if (value == nullptr) // if this coordinate is not associated, we add a new node in tree
        {
            vector<StreetSegment> tmpVec1;
            tmpVec1.push_back(tmps);
            data.associate(coord, tmpVec1);
			destructIndex.push_back(coord);
        }
        else
            value->push_back(tmps); //if this coordinate is already exist in the tree, we push
                                     //this StreetSegment into the vector but not replace the
                                     //whole node in the tree
        
        //Load end Coordinate
        coord = end->latitudeText + end->longitudeText;
        value = data.find(coord);
        if (value == nullptr)
        {
            vector<StreetSegment> tmpVec2;
            tmpVec2.push_back(tmps);
            data.associate(coord, tmpVec2);
			destructIndex.push_back(coord);
        }
        else
            value->push_back(tmps);
        
        for (size_t j = 0; j < tmps.attractions.size(); j++) 
        {
            coord = tmps.attractions[j].geocoordinates.latitudeText + tmps.attractions[j].geocoordinates.longitudeText;
            value = data.find(coord);
            if (value == nullptr)  //same idea as above
            {
                vector<StreetSegment> tmpVec3;
                tmpVec3.push_back(tmps);
                data.associate(coord, tmpVec3);
				destructIndex.push_back(coord);
            }
            else
                value->push_back(tmps);
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    string coord = gc.latitudeText + gc.longitudeText;
    vector<StreetSegment> v;
    if (data.find(coord) == nullptr)
        return v;
    vector<StreetSegment> s = *(data.find(coord));
    for(size_t i = 0; i < s.size(); i++)
    {
        v.push_back(s[i]);
    }
    return v;
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
