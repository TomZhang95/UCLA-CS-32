#include "provided.h"
#include <string>
#include "MyMap.h"
#include <algorithm>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
	
private:
	MyMap<string, GeoCoord> data;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml) 
{
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment tmp;
		ml.getSegment(i, tmp);
		for (size_t j = 0; j < tmp.attractions.size(); j++)
        {
            transform(tmp.attractions[j].name.begin(), tmp.attractions[j].name.end(), tmp.attractions[j].name.begin(), ::toupper);
            data.associate(tmp.attractions[j].name, tmp.attractions[j].geocoordinates);
            
            //associate all attractions in a MyMap with name as key, and geoCoord as value
        }
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    transform(attraction.begin(), attraction.end(), attraction.begin(), ::toupper);
	if (data.find(attraction) == nullptr)
		return false;
	GeoCoord const tmp = *data.find(attraction);
	gc = tmp;
	return true;
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
