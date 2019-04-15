#pragma once
struct LTPoint{
	int x;
	int y;
	LTPoint(){}
	LTPoint(int x,int y){
		this->x=x;
		this->y=y;
	}
	bool operator==(const LTPoint &b){
	    return (x==b.x && y==b.y);
	}
};