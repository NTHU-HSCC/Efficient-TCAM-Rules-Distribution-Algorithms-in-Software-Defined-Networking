#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAXLINE 10000
#define BITNUM 104
#define GENERALIZE 1 /// 0: PBD, 1: PBD-Generalize

typedef struct IP {
    uint32_t p1; /// MSB
    uint32_t p2;
    uint32_t p3;
    uint32_t p4; /// LSB
} IP;

struct Rule {
    uint32_t src_addr_from;
    uint32_t src_addr_to;
    uint32_t src_mask;
    uint32_t src_binary[32];

    uint32_t dest_addr_from;
    uint32_t dest_addr_to;
    uint32_t dest_mask;
    uint32_t dest_binary[32];

    uint32_t src_port_to;
    uint32_t src_port_from;
    uint32_t dest_port_from;
    uint32_t dest_port_to;
    uint32_t src_port_binary[32];
    uint32_t dest_port_binary[32];

    uint32_t data_protocol;
    uint32_t protocol_mask;
    uint32_t protocol_binary[32];

    IP  srcIPBinary;
    IP  dstIPBinary;
    uint32_t ruleBinary[BITNUM];
    int priority;

};

struct Node {
    Node* nextNode;
    Rule rule;
    int id;
};

struct Table {
    int ruleNum;
    int index;
    Node* child;
    Table* nextTable;

};

Node* CreateNode(Rule rule);

void AddNextNode(Node** head, Node* newAdd);

void TableAddNewNode(Table** head, Node* newAdd, int ruleNum);

void DecimalBinary(Node* node);

int ChooseTargetTable(Table *headTable);

int PivotBitChoose(Table *headTable, int targetTableIndex, int cut, double cutNum);

Table* Decomposition(Table *headTable, int pivotBit, int targetIndex);

int TableStatistics(Table *headTable, int *maxTableSize);

uint32_t parseIPV4string(char* ipAddress);

int TableNodeNum(Node* headNode);

Node* LoadRuleFromFile(int *rule_sum, char filename[]);

#endif
