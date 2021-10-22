#include "Clipper.h"

#include "ScanLineAlgo.h"
#include "RenderableObject.h"
#include "Exception.hpp"

#include <functional>
#include <InputHandler.h>

Point::Point() : x(0), y(0) {}

Point::Point(int xCord, int yCord) : x(xCord), y(yCord) {}
Point& Point::operator=(const Point& p) {
    this->x = p.x;
    this->y = p.y;
    return *this;
}

Point::~Point() {}

Edge::Edge() : next(nullptr) {}

Edge::Edge(int ym, float xpos, float xstep, Edge* nxt) : 
    ymax(ym), x(xpos), dx(xstep), next(nxt)
{}

Edge::~Edge() {}


Clipper::Clipper(int l, int r, int t, int b, unsigned int rsrc) : 
    L(l), R(r), T(t), B(b), realPort(rsrc)
{
    workStat = false;
    edges.resize(rsrc + 1,nullptr);
    pointLoop = {
        Point(450, 800), 
        Point(200, 100), 
        Point(850, 100)
    };
}

Clipper::~Clipper() {
}

bool Clipper::clipLine(float p, float q, float& u1, float & u2) {
    float r;
    if(p < 0) {
        r = q/p;
        if(r > u2) {
            return false;
        }
        if(r > u1) {
            u1 = r;
        }
    }
    else if(p > 0) {
        r = q/p;
        if(r < u1) {
            return false;
        }
        if(r < u2) {
            u2 = r;
        }
    } else {
        return q >= 0;
    }

    return true;
}

void Clipper::clip() {
    auto ed = (--pointLoop.end());
    edgePoints.clear();
    for(auto p = pointLoop.begin(); p!=ed; ++p) {
        auto nxt = std::next(p,1);
        edgePoints.push_back({Point(p->x,p->y),Point(nxt->x,nxt->y)});
    }
    edgePoints.push_back({Point(pointLoop.front().x,pointLoop.front().y),
                          Point(pointLoop.back().x,pointLoop.back().y)});

    float p,q,dx,dy,u1 = 0,u2 = 1;
    std::list<std::pair<Point,Point>> nEdgePoints;
    std::pair<Point,Point>* last_cp_index = nullptr;
    Point corssPoint;

    auto clipResProcessor = [&](std::pair<Point,Point>& edgePoint) {
        nEdgePoints.push_back({Point(edgePoint.first.x + u1*dx,edgePoint.first.y + u1*dy),
                               Point(edgePoint.first.x + u2*dx,edgePoint.first.y + u2*dy)});
        if(u1 != 0) {
            corssPoint = Point(edgePoint.first.x + u1*dx,edgePoint.first.y + u1*dy);
            if(last_cp_index == nullptr) {
                nEdgePoints.push_back({corssPoint,Point()});
                last_cp_index = &nEdgePoints.back();
            } else {
                last_cp_index->second = corssPoint;
                last_cp_index = nullptr;
            }
        } else if(u2 != 1) {
            corssPoint = Point(edgePoint.first.x + u2*dx,edgePoint.first.y + u2*dy);
            if(last_cp_index == nullptr) {
                nEdgePoints.push_back({corssPoint,Point()});
                last_cp_index = &nEdgePoints.back();
            } else {
                last_cp_index->second = corssPoint;
                last_cp_index = nullptr;
            }
        }
    };

    for(auto& edgePoint : edgePoints) {
        dx = edgePoint.second.x - edgePoint.first.x;
        dy = edgePoint.second.y - edgePoint.first.y;
        p = -dx;  q = edgePoint.first.x - L;
        bool clipRes = clipLine(p,q,u1,u2);
        if(clipRes) {
            clipResProcessor(edgePoint);
        }
        u1 = 0; u2 = 1;
    }
    edgePoints.swap(nEdgePoints);
    nEdgePoints.clear();

    for(auto& edgePoint : edgePoints) {
        dx = edgePoint.second.x - edgePoint.first.x;
        dy = edgePoint.second.y - edgePoint.first.y;
        p = dx;  q = R - edgePoint.first.x;
        bool clipRes = clipLine(p,q,u1,u2);
        if(clipRes) {
            clipResProcessor(edgePoint);
        }
        u1 = 0; u2 = 1;
    }
    edgePoints.swap(nEdgePoints);
    nEdgePoints.clear();

    for(auto& edgePoint : edgePoints) {
        dx = edgePoint.second.x - edgePoint.first.x;
        dy = edgePoint.second.y - edgePoint.first.y;
        p = -dy;  q = edgePoint.first.y - B;
        bool clipRes = clipLine(p,q,u1,u2);
        if(clipRes) {
            clipResProcessor(edgePoint);
        }
        u1 = 0; u2 = 1;
    }
    edgePoints.swap(nEdgePoints);
    nEdgePoints.clear();

    for(auto& edgePoint : edgePoints) {
        dx = edgePoint.second.x - edgePoint.first.x;
        dy = edgePoint.second.y - edgePoint.first.y;
        p = dy;  q = T - edgePoint.first.y;
        bool clipRes = clipLine(p,q,u1,u2);
        if(clipRes) {
            clipResProcessor(edgePoint);
        }
        u1 = 0; u2 = 1;
    }
    edgePoints.swap(nEdgePoints);
    nEdgePoints.clear();
}

void Clipper::spanEdge(int x0, int y0, int x1, int y1) {
        int ymin = std::min(y1,y0);
        int ymax = std::max(y1,y0);

        if(ymin < bot) bot = ymin;
        if(ymax > top) top = ymax;

        int ex = ymin == y1 ? x1 : x0;
        float dx = (float)(ymin == y1 ? 
                 (x0 - x1) : 
                 (x1 - x0));
        dx = dx / (float)(ymax - ymin);
        edges[ymin] = new Edge(ymax,(float)ex,dx,edges[ymin]);
}

std::list<Point>* Clipper::getPointLoop() {
    return &pointLoop;
}

void Clipper::update() {
    if(InputHandler::getKeyDown(GLFW_KEY_C)) {
        workStat = !workStat;
    }

    if(InputHandler::getMouseDownR()) {
        workStat = false;
    }

    if(workStat) {
        clip();
        bot = 2147483647; top = 0;
        for(auto& edgePoint : edgePoints) {
        spanEdge(edgePoint.first.x, edgePoint.first.y,
                 edgePoint.second.x,edgePoint.second.y);
        }
        scanLineAlgo->setupEdges(edges,bot,top);
        for(auto& ptr : edges) {
            ptr = nullptr;
        }
    }
}

void Clipper::onActivate() {
    Component* scanLine = attachedObject->getComponent("ScanLineAlgo");
    if(!scanLine) {
        throw Throwable("Can not get scanline program of this object");
    }
    scanLineAlgo = dynamic_cast<ScanLineAlgo*>(scanLine);
}

std::string Clipper::componentType() const {
    return "Clipper";
}