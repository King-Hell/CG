#pragma once
struct mPoint{
	float x;
	float y;
	int type;
	mPoint(){}
	mPoint(float x,float y,int type){
		this->x=x;
		this->y=y;
		this->type=type;
	}
};