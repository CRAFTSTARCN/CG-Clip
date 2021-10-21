#ifndef CLIPPER_H
#define CLIPPER_H

#include "Component.h"
#include "EdgeAndPoint.h"

#include <vector>
#include <list>

class ScanLineAlgo;

class Clipper : public Component {

    protected:
    std::vector<Edge*> edges;
    std::list<std::pair<Point,Point>> edgePoints;
    int L, R, T, B;
    unsigned int realPort;
    bool workStat;
    ScanLineAlgo* scanLineAlgo;
    int bot,top;

    public:
    Clipper(int l, int r, int t, int b, unsigned int rsrc);
    ~Clipper();

    void spanEdge(int x0, int y0, int x1, int y1);

    void clip();

    /*By Liang-Barsky Clipping Algorithm*/
    bool clipLine(float p, float q, float& u1, float & u2);

    virtual void update() override;
    virtual void onActivate() override;
    virtual std::string componentType() const override;
};

#endif