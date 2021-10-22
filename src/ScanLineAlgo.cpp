#include "ScanLineAlgo.h"

#include "RenderableObject.h"
#include "InputHandler.h"

#include <algorithm>

ScanLineAlgo::ScanLineAlgo() {
    edges.resize(901);
    workStat = false;
}

ScanLineAlgo::~ScanLineAlgo() {}

void ScanLineAlgo::setupEdges(const std::vector<Edge*>& edgeArr, int mbot, int mtop) {
    bot = 2147483647; top = 0;
    for(int i=0; i<edgeArr.size(); ++i) {
        edges[i] = edgeArr[i];
    }
    bot = mbot; top = mtop;
}

void ScanLineAlgo::runScanLineAlgo() {
    Edge* AT = nullptr;
    std::vector<float> xPoints;
    int pointCount = 0;
    for(int index = bot; index <= top; ++index) {
        xPoints.clear();
        Edge* cur = AT, *pre = nullptr;
        //scan
        while(cur) {
            pre = cur;
            xPoints.push_back(cur->x);
            cur = cur->next;
        }
        if(!pre) {
            AT = edges[index];
            cur = AT;
        } else {
            pre->next = edges[index];
            cur = pre->next;
        }

        
        std::sort(xPoints.begin(),xPoints.end());
        for(int i=1; i<xPoints.size(); i += 2) {
            int l = int(xPoints[i-1]+0.5), r = int(xPoints[i] + 0.5);
            for(int xcoord=l; xcoord<r; ++xcoord) {
                appendPoint(xcoord, index, ++pointCount);
            }
        }
        

        cur = AT, pre = nullptr;
        while(cur) {
            if(cur->ymax == index) {
                if(cur == AT) {
                    cur = cur->next;
                    delete AT;
                    AT = cur;
                } else {
                    cur = cur->next;
                    delete pre->next;
                    pre->next = cur;
                }
            } else {
                cur->x += cur->dx;
                pre = cur;
                cur = cur->next;
            }
        }
    }
    while(pointCount < (int)ownerMesh->vertexSize() / 3) {
        ownerMesh->popVertex();
    }
    return;
}

void ScanLineAlgo::appendPoint(int x, int y, int pc) {
    if(pc > ((int)(ownerMesh->vertexSize())) / 3) {
        ownerMesh->addVertex((float)x, float(y), -1.0f);
    } else {
        ownerMesh->updateVertex((float)x,(float)y,-1.0f, pc-1);
    }
}

void ScanLineAlgo::update() {
    if(InputHandler::getKeyDown(GLFW_KEY_C)) {
        workStat = !workStat;
    }

    if(InputHandler::getMouseDownR()) {
        workStat = false;
        ownerMesh->clearVertex();
    }

    if(workStat) {
        runScanLineAlgo();
    }
}

void ScanLineAlgo::onActivate() {
    ownerMesh = attachedObject->getMesh();
}

std::string ScanLineAlgo::componentType() const {
    return "ScanLineAlgo";
}
