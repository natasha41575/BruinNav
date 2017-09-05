#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <cctype>
using namespace std;


class NavigatorImpl {
private:
    AttractionMapper am;
    SegmentMapper sm;
    
    struct Node {
        Node(GeoCoord gc, double ds, double p, Node* ptr, string name) {
            this->gc = gc;
            distance_start = ds;
            priority = p;
            parent = ptr;
            m_name = name;
        }
        string m_name;
        GeoCoord gc;
        double distance_start;
        double priority;
        Node* parent;
    };
    
    string set_direction(double angle) const {
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
    
    void get_directions(Node* node, vector<NavSegment>& directions) const {
        vector<NavSegment> rev_dir;
        Node* ptr = node;
        while (node != nullptr) {
            Node* node_ptr = node;
            if (node_ptr->parent == nullptr) {
                break;
            }
            
            GeoSegment geo_seg(node_ptr->parent->gc, node_ptr->gc);
            rev_dir.push_back(NavSegment(set_direction(angleOfLine(geo_seg)),
                              node_ptr->m_name,
                              distanceEarthMiles(node_ptr->parent->gc, node_ptr->gc),
                              geo_seg));
            
            if (!(node_ptr->parent->parent == nullptr) && !(node_ptr->m_name == node_ptr->parent->m_name))  {
                Node* pr = node_ptr->parent->parent;
                GeoSegment first(pr->gc, node_ptr->parent->gc);
                GeoSegment next(node_ptr->parent->gc, node_ptr->gc);
                double angle = angleBetween2Lines(first, next);
                string dir;
                if (angle < 180) {
                    dir = "left";
                }
                else {
                    dir = "right";
                }
                rev_dir.push_back(NavSegment(dir, node_ptr->m_name));
                
            }
            node = node->parent;
        }

        if (ptr != nullptr) {
            Node* ptr_parent = ptr->parent;
            while (ptr != nullptr && ptr_parent != nullptr) {
                ptr = ptr_parent;
                ptr_parent = ptr->parent;
            }
        }
        for (int i = (int) rev_dir.size() - 1; i >= 0; i--) {
            directions.push_back(rev_dir[i]);
        }
    }

    static bool compareNodes(const Node* one, const Node* two) {
        return one->priority > two->priority;
    }
    
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
};

NavigatorImpl::NavigatorImpl() {}

NavigatorImpl::~NavigatorImpl() {}

bool NavigatorImpl::loadMapData(string mapFile) {
    MapLoader ml;
    if (!ml.load(mapFile)) {
        return false;
    }
    am.init(ml);
    sm.init(ml);
    return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const {
    for (int i = 0; i < start.size(); i++) {
        start[i] = tolower(start[i]);
    }
    for (int i = 0; i < end.size(); i++) {
        end[i] = tolower(end[i]);
    }
    GeoCoord begin;
    GeoCoord finish;
    
    if (!am.getGeoCoord(start, begin)) {
        return NAV_BAD_SOURCE;
    }
    if (!am.getGeoCoord(end, finish)) {
        return NAV_BAD_DESTINATION;
    }
    
    double de = distanceEarthMiles(begin, finish);
    vector<StreetSegment> s = sm.getSegments(begin);
    Node* potato = new Node(begin, 0, de, nullptr, s[0].streetName);
    
    priority_queue<Node*, vector<Node*>, decltype(&compareNodes)> open(&compareNodes);
    vector<Node*> node_holder;
    open.push(potato);
    node_holder.push_back(potato);
    
    MyMap<GeoCoord, Node*> closed;
    
    while (!open.empty()) {
        Node* current = open.top();
        open.pop();
        
        if (closed.find(current->gc) != nullptr) {
            continue;
        }
        closed.associate(current->gc, current);
        vector<StreetSegment> segs = sm.getSegments(current->gc);
        
        for (int i = 0; i < segs.size(); i++) {
            StreetSegment current_seg = segs[i];
            for (int j = 0; j < current_seg.attractions.size(); j++) {
                
                for (int k = 0; k < current_seg.attractions[j].name.size(); k++) {
                    current_seg.attractions[j].name[k] = tolower(current_seg.attractions[j].name[k]);
                }
                
                if (current_seg.attractions[j].name == end) {
                    Node* dest = new Node(current_seg.attractions[i].geocoordinates,
                                          0, 0, current, current_seg.streetName);
                    node_holder.push_back(dest);
                    directions.clear();
                    get_directions(dest, directions);
                    for (int k = 0; k < node_holder.size(); k++) {
                        delete node_holder[k];
                    }
                    return NAV_SUCCESS;
                }
            }
            GeoCoord st = current_seg.segment.start;
            GeoCoord en = current_seg.segment.end;
            if (closed.find(st) == nullptr) {
                double ds = current->distance_start + distanceEarthMiles(current->gc, st);
                double priority = ds + distanceEarthMiles(st, finish);
                Node* rotato = new Node(st, ds, priority, current, current_seg.streetName);
                open.push(rotato);
                node_holder.push_back(rotato);
            }
            if (closed.find(en) == nullptr) {
                double ds = current->distance_start + distanceEarthMiles(current->gc, en);
                double priority = ds + distanceEarthMiles(en, finish);
                Node* totato = new Node(en, ds, priority, current, current_seg.streetName);
                open.push(totato);
                node_holder.push_back(totato);
            }
        }
    }
    for (int k = 0; k < node_holder.size(); k++) {
        delete node_holder[k];
    }
	return NAV_NO_ROUTE;
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
