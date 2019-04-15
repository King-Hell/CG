#pragma once

#include "LTPoint.h"
#include <vector>

typedef std::pair<int, int> Point;

class LTPolygon {
    //自定义多边形图元类
public:
    LTPolygon() {
        close = false;//是否闭合
/*        xl=INT_MAX;
        xr=0;
        yt=0;
        yb=INT_MAX;*/
    }

    bool add(Point v) {
/*        if(v.first<xl)
            xl=v.first;
        if(v.first>xr)
            xr=v.first;
        if(v.second<yb)
            yb=v.second;
        if(v.second>yt)
            yt=v.second;*/
        if (has(v) && v == getFirst()) {
            close = true;
            return true;
        } else if (!has(v)) {
            vertex.push_back(v);
            return true;
        }
        return false;
    }

    Point& getFirst() {
        //获取起始定点
        return vertex[0];
    }

    Point& getLast() {
        return vertex.back();
    }

    bool has(Point &v) {
        for (auto i = vertex.begin(); i != vertex.end(); i++) {
            if (*i == v)
                return true;
        }
        return false;
    }

    bool isClose() { return close; }
    auto begin(){return vertex.begin();}
    auto end(){return vertex.end();}
    void set(int index,int x,int y){
        vertex[index].first=x;
        vertex[index].second=y;
    }
    Point& operator[](int i){
        return vertex[(i+vertex.size())%vertex.size()];
    }

    std::vector<Point>& getVertex(){return vertex;}
    //int xl,xr,yt,yb;
private:
    std::vector<Point> vertex;
    bool close;
    //std::vector<int> borderVertex;
};
