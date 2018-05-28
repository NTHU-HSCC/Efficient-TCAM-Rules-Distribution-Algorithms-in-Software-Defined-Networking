#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SWITCHNUM 150
#define LONGESTPATH 18 /// Must same as SWITCHNUM
#define PATHNUM 50
#define SLEN 10
#define FILENUM 100     /// Number of topologies


#define PROBA  50       /// Must lower than SWITCHNUM
#define MAX_M 15

struct Switch {
    int ID;
    int color;
    int pathNum;            /// Number of flow paths that pass switch i
    int shortestPathLen;    /// Length of shortest flow path that pass switch i
    int shortestPathNum;    /// Number of shortest flow paths that pass switch i
    Switch* nextSwitch;
};

struct Path {
    Switch* switchNode;
    int color;
    int length;
};

struct ColorSwitch {
    int sameColorSwitch[SWITCHNUM];
    int length;
    int pathNum;
    int id;
    bool duplicate;
    int color;
    ColorSwitch *nextNode;
};

Switch* AddSwitchToPath(Switch *head, int ID);

void GenerateTopologyX(int topology[PATHNUM][SWITCHNUM]);

void GenerateTopology(int topology[PATHNUM][SWITCHNUM]);

void SortPathWithLength(Path path[], int switchPathRelation[SWITCHNUM][PATHNUM]);

void FindShortestPathInfo(Path path[], Switch switchInfo[], int sLen);

int InitialTopology(Path path[], Switch switchInfo[], int topology[PATHNUM][SWITCHNUM], int switchPathRelation[SWITCHNUM][PATHNUM]);

bool CheckColor(Path path[], int targetColor);

void UpdatePathInfo(Path path[], int colorID, int targetColor);

int OneGreedyFindMaxID(int switchPathNum[], Switch switchInfo[]);

void TwoGreedyFindMaxID(Path path[], Switch switchInfo[], int *color1ID, int *color2ID,
                        int switchPathRelation[SWITCHNUM][PATHNUM], int targetColor);

int OneGreedyColor(Path path[], Switch switchInfo[], int sLen);

int TwoGreedyColor(Path path[], Switch switchInfo[], int sLen, int switchPathRelation[SWITCHNUM][PATHNUM]);

int SA(Path path[], Switch switchInfo[], int sLen, int switchPathRelation[SWITCHNUM][PATHNUM]);



ColorSwitch* CreateColor(int switchIndex, int switchPathNum[SWITCHNUM][PATHNUM], int length, int parentColorSwitch[SWITCHNUM]);

ColorSwitch* NodeCombination(int switchPathNum[SWITCHNUM][PATHNUM], int parentColorSwitch[SWITCHNUM], ColorSwitch child,
                             int length, int parentId, int parentColor);

void AddNextColor(ColorSwitch** head, ColorSwitch* newAdd);

ColorSwitch* InitilizeColorSwitch(ColorSwitch* head ,int switchPathNum[SWITCHNUM][PATHNUM]);

ColorSwitch* RemoveLowerPathNumNode(ColorSwitch* head);

int SetId(ColorSwitch* head);

int OptimalSolution (Path path[], int switchPathNum[SWITCHNUM][PATHNUM], int sLen);

#endif // TOPOLOGY_H_INCLUDED
