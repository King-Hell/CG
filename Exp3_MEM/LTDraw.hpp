#pragma once

#include <iostream>
#include <set>
#include <algorithm>
#include "LTPolygon.hpp"

typedef std::pair<int, int> Point;

struct newLine {
    float x;
    float dx;
    float ymax;
    int type;//用以表示上下顶点类型,10：顶点在线上方,01：顶点在线下方,00：顶点在线之间

    newLine() {}

    newLine(int x, float dx, int ymax, int type) : x(x), dx(dx), ymax(ymax), type(type) {}

    //newLine(int x, float dx, int ymax) : x(x), dx(dx), ymax(ymax) {}
    bool operator<(newLine &a) { return x < a.x; }
};

class LTDraw {
    //绘图类
public:
    LTDraw(int width, int height) : width(width), height(height) {}

    void init() {
        pixelNum = width * height;
        bufferSize = pixelNum * sizeof(int);
        buffer = new int[pixelNum];
        clear();
        glGenVertexArrays(1, VAOs);
        glGenBuffers(1, VBOs);
        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_DYNAMIC_DRAW);
        glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void *) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //允许修改点大小
        glEnable(GL_PROGRAM_POINT_SIZE);
    }

    ~LTDraw() {
        glDeleteVertexArrays(1, VAOs);
        glDeleteBuffers(1, VBOs);
        delete buffer;
    }

    void drawPixel(int x, int y, int direct = 0, int color = 2) {
        //绘制坐标点
        switch (direct) {
            case 2:
                set(y, x, color);
                break;
            case -1:
                set(x, -y, color);
                break;
            case -2:
                set(y, -x, color);
                break;
            default:
                set(x, y, color);
        }
    }


/*    void drawLine(float x0,float y0, float x1,float y1){
        IntegerBresenhamline(std::round(x0),std::round(y0),std::round(x1),std::round(y1));
    }*/
    void drawLine(int x0, int y0, int x1, int y1) {
        IntegerBresenhamline(x0, y0, x1, y1);
        //FloatLine(x0,y0,x1,y1);
        set(x0, y0, 1);
        set(x1, y1, 1);
    }

    void clear() {
        memset(buffer, 0, bufferSize);
    }

    void updateBuffer() {
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, buffer);
    }

    void loop() {
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_POINTS, 0, GLsizei(pixelNum));
    }

    int get(int x, int y) { return buffer[y * width + x]; }

    void fill(LTPolygon &polygon) {
        std::vector<Point> vertex = polygon.getVertex();
        std::map<int, std::vector<newLine>> net;
        std::vector<newLine> aet;
        int n = vertex.size();
        int ymin=INT_MAX;
        int ymax=0;
        for (int i = 0; i < n; i++) {
            //计算新边表
            Point a = vertex[i];//当前点
            Point b = vertex[(i + 1) % n];//下一个点
            //int ymin=std::min(a.second,b.second);
            int bottomIndex = i;
            int topIndex = (i + 1) % n;
            if (a.second > b.second)
                std::swap(a, b), std::swap(topIndex, bottomIndex);
            if(a.second<ymin)
                ymin=a.second;
            if(b.second>ymax)
                ymax=b.second;
            if (net.find(a.second) == net.end()) {
                //向net中插入
                net.insert(std::pair<int, std::vector<newLine>>(a.second, std::vector<newLine>()));
            }
            float dx;
            if (a.second - b.second == 0)
                continue;//dx=0;dx=0的边不计算
            else
                dx = (float(a.first - b.first)) / (a.second - b.second);
            net[a.second].push_back(newLine(a.first, dx, b.second, getVertexType(vertex, topIndex, bottomIndex)));
        }
        for (int i = ymin; i <= ymax; i++) {
            if (net.find(i) != net.end()) {
                std::vector<newLine> lines = (*net.find(i)).second;
                for (auto j = lines.begin(); j != lines.end(); j++) {
                    aet.push_back(*j);
                }
            }

            for (size_t j = 0; j < aet.size(); j++) {
                if (aet[j].ymax == i) {
                    aet.erase(aet.begin() + j);
                    j--;
                } else {
                    aet[j].x += aet[j].dx;
                }
            }
            std::sort(aet.begin(), aet.end());
            for (size_t j = 0; j < aet.size(); j += 2) {
                drawXLine(aet[j].x,aet[j + 1].x, i);
            }
        }
    }

    void drawBorder(LTPolygon &polygon){
        //绘制边缘
        auto vertex=polygon.getVertex();
        int n=vertex.size();
        for(int i=0;i<n;i++){
            drawLine(vertex[i].first,vertex[i].second,vertex[(i+1)%n].first,vertex[(i+1)%n].second);
        }
    }
private:
    void FloatLine(int x0,int y0,int x1,int y1){
        if(y1<y0){
            std::swap(y0,y1);
            std::swap(x0,x1);
        }
        if(y0-y1==0){
            for(int i=std::min(x0,x1);i<=std::max(x0,x1);i++)
                set(i,y0,2);
        }
        float dx=float(x0-x1)/(y0-y1);
        float x=x0;
        for(int i=y0;i<=y1;i++,x+=dx)
            set(int(x+0.5),i,2);
    }

    void IntegerBresenhamline(int x0, int y0, int x1, int y1) {
        int direct = 0;
        if (x0 > x1) {
            //保证x0总小于x1
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        int x, y, dx, dy;
        dx = x1 - x0, dy = y1 - y0;
        x = x0, y = y0;
        if (dx == 0) {
            //斜率为无穷情况
            if (y0 > y1)
                std::swap(y0, y1);
            for (; y0 <= y1; y0++)
                drawPixel(x, y0, direct);
            return;
        }
        float k = float(dy) / dx;
        if (k > 1) {
            std::swap(x, y);
            std::swap(dx, dy);
            direct = 2;
        } else if (k < -1) {
            dy *= -1;
            y *= -1;
            std::swap(x, y);
            std::swap(dx, dy);
            direct = -2;
        } else if (k < 0 && k >= -1) {
            dy *= -1;
            y *= -1;
            direct = -1;
        }
        int e = -dx;
        for (int i = 0; i <= dx; i++) {
            drawPixel(x, y, direct);
            x++, e = e + 2 * dy;
            if (e >= 0) {
                y++;
                e = e - 2 * dx;
            }
        }
    }

    void set(int x, int y, int type) {
        buffer[y * width + x] = type;
    }
    void drawXLine(float x0,float x1, int y) {
        int a=int(x0+0.5);
        int b=int(x1+0.5);
        if(a>b)
            std::swap(a,b);
        for(int i=a;i<=b;i++)
            set(i,y,3);
    }
    int getVertexType(std::vector<Point> &v, int t, int b) {
        int type = 0;
        int n = v.size();
        if (v[(t + n - 1) % n].second > v[t].second && v[(t + 1) % n].second > v[t].second)
            type += 0b0100;
        else if (v[(t + n - 1) % n].second < v[t].second && v[(t + 1) % n].second < v[t].second)
            type += 0b1000;
        if (v[(b + n - 1) % n].second > v[b].second && v[(b + 1) % n].second > v[b].second)
            type += 0b0001;
        else if (v[(b + n - 1) % n].second < v[b].second && v[(b + 1) % n].second < v[b].second)
            type += 0b0010;
        return type;
    }

    int width, height;
    int *buffer;//视频缓冲区，缓冲区内容是坐标的类型
    int bufferSize;
    int pixelNum;
    unsigned int VAOs[1], VBOs[1];
};


