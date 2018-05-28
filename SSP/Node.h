#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXLINE 10000
#define BITNUM 104

#define ALPHA 0.8
#define END_THRESHOLD 0.3

typedef struct IP {
    uint32_t p1; /// MSB
    uint32_t p2;
    uint32_t p3;
    uint32_t p4; /// LSB
} IP;

typedef struct Rule {
    uint32_t src_addr_from;
    //uint32_t src_addr_to;
    uint32_t src_mask;
    //uint32_t src_binary[32];

    uint32_t dest_addr_from;
    //uint32_t dest_addr_to;
    uint32_t dest_mask;
   //uint32_t dest_binary[32];

    uint32_t src_port_to;
    uint32_t src_port_from;
    uint32_t dest_port_from;
    uint32_t dest_port_to;
    uint32_t src_port_binary[16];
    uint32_t dest_port_binary[16];

    uint32_t data_protocol;
    uint32_t protocol_mask;
    uint32_t protocol_binary[32];

    IP  srcIPBinary;    // no use
    IP  dstIPBinary;    // no use
    uint32_t ruleBinary[BITNUM];
    int priority;
} Rule;

typedef struct Node {
    Rule rule;
    int id;
    struct Node* nextNode;
} Node;

typedef struct Table {
    Node* child;
    struct Table* nextTable;
    int ruleNum;
    int index;

} Table;

Node* CreateNode(Rule rule);

void AddNextNode(Node** head, Node* newAdd);

int TableNodeNum(Node* headNode);

void TableNodeIni(Table** head, Node* newAdd, int ruleNum);

void AddAllTable(Table** head, int cut);

void TableAddNewNode2(Table** head, Node* newAdd, int ruleNum, int minTableIndex);

void DecimalBinary(Node* node);

int PivotBitChoose(Table *headTable, int targetTableIndex,int maxTableSize, int minTableSize, int averageTableSize);

void Decomposition2(Table *headTable, int pivotBit, int targetIndex, int minTableIndex);

int DoneDecomposition(Table *headTable, double averageTableSize, int *maxIndex, int *minIndex, int *maxTableSize, int *minTableSize);

int TableStatistics(Table *headTable);

uint32_t parseIPV4string(char* ipAddress);

Node* LoadRuleFromFile(int *rule_sum, char filename[]);

#endif
