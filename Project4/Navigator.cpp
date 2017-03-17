#include "provided.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
    AttractionMapper attractionMap;
    SegmentMapper SegmentMap;
	void rearrangeSegment(vector<StreetSegment>& segVec) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
    MapLoader ml;
    if (!ml.load(mapFile))
        return false;
    attractionMap.init(ml);
    SegmentMap.init(ml);
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	for (auto iter = directions.begin(); iter != directions.end();)
		iter = directions.erase(iter);

	double totalDistance = 0;
    GeoCoord startCoord;
    GeoCoord endCoord;
    MyMap<string, GeoCoord> visitedPoints;
    if(!attractionMap.getGeoCoord(start, startCoord)) //make sure source and destination
        return NAV_BAD_SOURCE;                         //are valid
    if(!attractionMap.getGeoCoord(end, endCoord))
        return NAV_BAD_DESTINATION;
	if (startCoord.latitudeText == endCoord.latitudeText && startCoord.longitudeText == endCoord.longitudeText)
		return NAV_SUCCESS;
    queue<GeoCoord> coordQueue;
	vector <StreetSegment> startingSeg = SegmentMap.getSegments(startCoord); //find coordinates
	vector <StreetSegment> endingSeg = SegmentMap.getSegments(endCoord);
	if (startingSeg.size() != 1)
		return NAV_BAD_SOURCE;

	startCoord = startingSeg[0].segment.start; //set starting coordinate and ending coordinate
	endCoord = endingSeg[0].segment.start;     //as the start coordinate of the StreetSegment
                                                //they belong to
    coordQueue.push(startCoord);
	GeoCoord current;
	GeoCoord previous("00", "00");
	visitedPoints.associate(startCoord.latitudeText + startCoord.longitudeText, previous);

    while (!coordQueue.empty()) //if coordQueue is empty, it means all points on map are
    {                            //searched and didn't find route, so return No_Route
		current = coordQueue.front();
        coordQueue.pop();
		string currentText = current.latitudeText + current.longitudeText;
		
		if ((current.latitudeText == endCoord.latitudeText) && (current.longitudeText == endCoord.longitudeText))
		{
            //When this part is called, it means the algorithm already found the route
            //Therefore transfer the coordinates we got to a route vector of NavSegment
            
			GeoCoord tmpCurrent = endCoord;
			GeoCoord* prevPtr = visitedPoints.find(endCoord.latitudeText + endCoord.longitudeText);
			queue<GeoCoord> coordQueue;
			stack<StreetSegment> segmentStack;
			while ((tmpCurrent.latitudeText != "00") || (tmpCurrent.longitudeText != "00"))
			{
                //Find the previous coordinate from the ending coordinate
                //Push them into a queue
                
				coordQueue.push(tmpCurrent);
				tmpCurrent = *prevPtr;
				prevPtr = visitedPoints.find(tmpCurrent.latitudeText + tmpCurrent.longitudeText);
			}
			
            //transfer the queue of geoCoords to a stack of StreetSegments
            GeoCoord first;
			GeoCoord second = coordQueue.front();
			coordQueue.pop();
            size_t segmentCounter = 1;
            size_t segmentsEnd = coordQueue.size();
            attractionMap.getGeoCoord(start, startCoord);
            attractionMap.getGeoCoord(end, endCoord);
			while (!coordQueue.empty())
			{
				first = second;
				second = coordQueue.front();
				coordQueue.pop();
				vector<StreetSegment> firstSeg = SegmentMap.getSegments(first);
				vector<StreetSegment> secondSeg = SegmentMap.getSegments(second);
				for (size_t i = 0; i < firstSeg.size(); i++)
					for (size_t j = 0; j < secondSeg.size(); j++)
						if ((firstSeg[i].streetName == secondSeg[j].streetName) && (firstSeg[i].segment.start.latitudeText == secondSeg[j].segment.start.latitudeText))
						{
                            //The starting coordinate and ending coordinate is the coordinates on
                            //attractions, so assign them to the first and the last segment.
                            if (segmentCounter == 1)
                                firstSeg[i].segment.end = endCoord;
                            else
                                firstSeg[i].segment.end = first; //if this is not the first segment
                                                                  //assign it normally
                            if (segmentCounter == segmentsEnd)
                                firstSeg[i].segment.start = startCoord;
                            else
                                firstSeg[i].segment.start = second;
							segmentStack.push(firstSeg[i]);
							i = firstSeg.size();
							j = secondSeg.size();
                            segmentCounter++;
						}
			}
            
            //Last step, transfer the stack of StreetSegments to a vector of NavSegments
			NavSegment tmpNav;
			StreetSegment curr = segmentStack.top();
			StreetSegment prev = curr;
			while (!segmentStack.empty())
			{
				curr = segmentStack.top();
				segmentStack.pop();
                //If the street name of current StreetSegment and previous StreetSegment,
                //it means there is no turn occure, so creat PROCEED type NavSegment
				if (curr.streetName == prev.streetName)
				{
					tmpNav.m_command = NavSegment::NavCommand::PROCEED;
					double angle = angleOfLine(GeoSegment(curr.segment.start, curr.segment.end));
					if (0 <= angle && angle <= 22.5)
						tmpNav.m_direction = "east";
					else if (22.5 < angle && angle <= 67.5)
						tmpNav.m_direction = "northeast";
					else if (67.5 < angle && angle <= 112.5)
						tmpNav.m_direction = "north";
					else if (112.5 < angle && angle <= 157.5)
						tmpNav.m_direction = "northwest";
					else if (157.5 < angle && angle <= 202.5)
						tmpNav.m_direction = "west";
					else if (202.5 < angle && angle <= 247.5)
						tmpNav.m_direction = "southwest";
					else if (247.5 < angle && angle <= 292.5)
						tmpNav.m_direction = "south";
					else if (292.5 < angle && angle <= 337.5)
						tmpNav.m_direction = "southeast";
					else if (337.5 < angle && angle <= 360)
						tmpNav.m_direction = "east";
				}
				else //Otherwise, there is a turn occure
				{
					tmpNav.m_command = NavSegment::NavCommand::TURN;
					
                    //When a turn occure, the user acctually won't move location,
                    //therefore I add current segment back, and later on 'prev' will
                    //be assigned by curr, then it will do the PROCEED in next loop
                    segmentStack.push(curr);
					double angle = angleBetween2Lines(curr.segment, prev.segment);
					if (angle < 180)
						tmpNav.m_direction = "right";
					else
						tmpNav.m_direction = "left";
				}
				
				tmpNav.m_streetName = curr.streetName;
				tmpNav.m_distance = distanceEarthMiles(curr.segment.start, curr.segment.end);
				tmpNav.m_geoSegment = GeoSegment(curr.segment.start, curr.segment.end);
				directions.push_back(tmpNav);
				prev = curr;
				totalDistance += tmpNav.m_distance;
			}
			return NAV_SUCCESS;
		}
        
        vector<StreetSegment> segment = SegmentMap.getSegments(current);
		rearrangeSegment(segment);
        for (size_t i = 0; i < segment.size(); i++)
        {
            //Since we don't know algorithm will go from GeoCoord 'start' to 'end' or
            //from 'end' to 'start', we check that if current GeoCoord is on 'start', then
            //'end' of current segment will be the next GeoCoord that push into queue
            
			string nextText;
			if (current.latitudeText == segment[i].segment.start.latitudeText && current.longitudeText == segment[i].segment.start.longitudeText)
			{
				nextText = segment[i].segment.end.latitudeText + segment[i].segment.end.longitudeText;
				if (visitedPoints.find(nextText) == nullptr) //if this coordinate isn't visited
					if (nextText != currentText)//and also if next coordinate we going is not
                                                //where we stand right now
					{
                        //push this coordinate into queue, record the previous coordinate,
                        //and mark this coordinate visited
						coordQueue.push(segment[i].segment.end);
						previous = current;
						visitedPoints.associate(nextText, previous);
					}
			}
			else if (current.latitudeText == segment[i].segment.end.latitudeText && current.longitudeText == segment[i].segment.end.longitudeText)
			{
				nextText = segment[i].segment.start.latitudeText + segment[i].segment.start.longitudeText;
				if (visitedPoints.find(nextText) == nullptr)
					if (nextText != currentText)
					{
						coordQueue.push(segment[i].segment.start);
						previous = current;
						visitedPoints.associate(nextText, previous);
					}
			}
        }
    }

    return NAV_NO_ROUTE;
}

void NavigatorImpl::rearrangeSegment(vector<StreetSegment>& segVec) const
{
	if (segVec.size() <= 1)
		return;
    
    //Bubble sort the vecore to make it goes from less to greater
    //since the size of the vector will always be small, I used bubble sort
    
	size_t size = segVec.size() - 1;
	for (size_t i = 0; i < size; i++)
		for (size_t j = i + 1; j < segVec.size(); j++)
		{
			double iDistance = distanceEarthMiles(segVec[i].segment.start, segVec[i].segment.end);
			double jDistance = distanceEarthMiles(segVec[j].segment.start, segVec[j].segment.end);
			StreetSegment tmp;
			if (iDistance > jDistance)
			{
				tmp = segVec[i];
				segVec[i] = segVec[j];
				segVec[j] = tmp;
			}
		}
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
