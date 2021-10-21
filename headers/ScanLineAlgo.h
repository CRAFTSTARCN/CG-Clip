#ifndef SCAN_LINE_ALGO_H
#define SCAN_LINE_ALGO_H

#include "Component.h"
#include "EdgeAndPoint.h"

#include <vector>
#include <list>

class Mesh;

class ScanLineAlgo : public Component {
    protected:
    std::vector <Edge*> edges;
    int bot, top;
    Mesh* ownerMesh;

    bool workStat;

    public:


    ScanLineAlgo();
    ~ScanLineAlgo();

    virtual void update() override;

    virtual std::string componentType() const override;
    virtual void onActivate() override;

    void setupEdges(const std::vector<Edge*>& edgeArr, int mbot, int mtop);
    void runScanLineAlgo();
    void appendPoint(int x, int y, int pc);
};

#endif