#include "provided.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;

private:
	ifstream ifs;
	vector<StreetSegment*> segs;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	for (auto i = segs.begin(); i != segs.end();)
	{
		delete *i;
		i = segs.erase(i);
	}
}

bool MapLoaderImpl::load(string mapFile)
{
	ifs.open(mapFile);
    if(!ifs)
        return false;
	string line;

	while (getline(ifs, line))
	{
		StreetSegment* currSeg = new StreetSegment;
		int attNum = 0;

		//First line is street name
		//transform(line.begin(), line.end(), line.begin(), towupper);
        unsigned long p = line.find('\r', 0);
		if (p<line.size())
			line.erase(p, 1);
		p = line.find('\n', 0);
		if (p<line.size())
			line.erase(p, 1);
		currSeg->streetName = line;
		getline(ifs, line);

		//Second line it is GeoCoordinates

		unsigned long p1 = 0, p2 = 0;
		string tmpLat, tmpLon;
		GeoCoord start, end;

		//separate line to 4 coordinates
        p = line.find('\r', 0);  //remove \r and \n just in case
		if (p<line.size())
			line.erase(p, 1);
		p = line.find('\n', 0);
		if (p<line.size())
			line.erase(p, 1);
		p1 = line.find(',', 0);
		tmpLat = line.substr(0, p1);
		p1++;
        while (line[p1] == ' ') //skip spaces
            p1++;
		p2 = line.find(' ', p1);
		tmpLon = line.substr(p1, p2 - p1);
		start = GeoCoord(tmpLat, tmpLon);

		p2++;
		p1 = line.find(',', p2);
		tmpLat = line.substr(p2, p1 - p2);
		p1++;
        while (line[p1] == ' ')
            p1++;
		p2 = line.find(' ', p1);
		tmpLon = line.substr(p1, p2 - p1);
		end = GeoCoord(tmpLat, tmpLon);

		currSeg->segment = GeoSegment(start, end);
		getline(ifs, line);

		
        //read the number of attraction, then decide what to do
		if (line == "0")
		{
			segs.push_back(currSeg);
			continue;
		}
		else
		{
			if (attNum == 0) //transfer string type attraction number in 'line' into int
			{
				stringstream ss;
				ss << line;
				ss >> attNum;
			}
			while (attNum > 0) //when there is one or more attractions, get those attractions
			{
				getline(ifs, line);
				Attraction tmpAtt;
				string tmpLat, tmpLon, name;
				unsigned long p1 = 0, p2 = 0;

                p = line.find('\r', 0);
				if (p<line.size())
					line.erase(p, 1);
				p = line.find('\n', 0);
				if (p<line.size())
					line.erase(p, 1);
				p1 = line.find('|', 0);
				name = line.substr(0, p1);
				tmpAtt.name = name;
				p1++;
				p2 = line.find(',', p1);
				tmpLat = line.substr(p1, p2 - p1);
				p2++;
                while (line[p2] == ' ')
                    p2++;
				p1 = line.find(' ', p2);
				tmpLon = line.substr(p2, p1 - p2);

				tmpAtt.geocoordinates = GeoCoord(tmpLat, tmpLon);
				currSeg->attractions.push_back(tmpAtt);
				attNum--;
			}
			segs.push_back(currSeg);
		}
		
	}
    ifs.close();
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return segs.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= segs.size())
		return false;
	else
	{
		seg = *(segs[segNum]);
		return true;
	}
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
