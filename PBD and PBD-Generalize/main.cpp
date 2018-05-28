#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <time.h>

#include <iomanip>
#include <windows.h>

#include "Node.h"


using namespace std;

int main()
{

    //fw1_2k_0.7_-1_-1
    //acl2_3.5k_1_-1_-1
    //ipc1_5k_1.5_-1_-1

    //fw1_4k_0.7_-1_-1
    //acl2_7k_1_-1_-1
    //ipc1_10k_1.5_-1_-1

    //fw1_6k_0.7_-1_-1
    //acl2_10.5k_1.5_-1_-1
    //ipc1_15k_1.5_-1_-1

    //
    //acl2_14k_1_-1_-1
    //ipc1_20k_1.5_-1_-1


    char filename[] = "./New/ipc1_10k_1.5_-1_-1";
    char *fileOpen;
    ofstream outfile;
    outfile.open("ipc1_10k_1.5_-1_-1_G.txt");
    LARGE_INTEGER startTime,endTime,fre;
    double times;


    for (int cnt = 2; cnt < 8; cnt++) {

        Table *hadTable = NULL;
        Node *headNode;
        int cut = 0;
        int ruleSum = 0;
        int pivot;
        int targetTableIndex;
        int i;
        int maxTableSize = 0;

        cut = cnt;
        headNode = LoadRuleFromFile(&ruleSum, filename);        /// Load rule from file
        ruleSum = TableNodeNum(headNode);
        //ruleSum = RemoveRedundant(headNode, ruleSum);         /// Return actual rule sum
        TableAddNewNode(&hadTable, headNode, ruleSum);

        int sum = 0;
        Node *ptr = hadTable->child;
        while(ptr != NULL){
            DecimalBinary(ptr);
            sum++;
            ptr = ptr->nextNode;
            if(ptr->nextNode->nextNode == NULL) {

            }
        }

        /// Generate cut number
        double cutNum[cut*3];
        int ifCut[cut*3];
        int maxNum;
        int maxIndex;
        int addIndex = 1;
        int j;
        double part1 = 0;
        double part2 = 0;
        cutNum[0] = (double)cut;
        maxNum = cutNum[0];
        ifCut[0] = 0;
        maxIndex = 0;


        for(i = 1; i < cut*3 - 1; i++) {
            cutNum[i] = 0;
            ifCut[i] = 0;
        }

        while(1) {
            for(i = 0; i < cut*3 - 1; i++) {
                if(cutNum[i] > maxNum && ifCut[i] == 0) {
                    maxNum = cutNum[i];
                    maxIndex = i;
                }
            }

            if(maxNum > 2) {
                for(j = 1; ;j++) {
                    if(pow(2, j) < maxNum && pow(2, j + 1) >= maxNum) {
                        part1 = pow(2, j);
                        part2 = (double)maxNum - part1;
                        break;
                    }
                }
                cutNum[addIndex++] = part1;
                cutNum[addIndex++] = part2;
                ifCut[maxIndex] = 1;
            } else {
                break;
            }
            maxNum = 0;
        }

        /// Sort
        for(i = 0; i < addIndex - 1; i++) {
            maxIndex = i;
            for(j = i + 1; j < addIndex; j++) {
                if(cutNum[j] > cutNum[maxIndex]) {
                    maxIndex = j;
                }
            }
            if(i != maxIndex) {
                double temp;
                temp = cutNum[i];
                cutNum[i] = cutNum[maxIndex];
                cutNum[maxIndex] = temp;
            }
        }


        /// START -----------------------------------
        QueryPerformanceFrequency(&fre);
        QueryPerformanceCounter(&startTime);

        for (i = 0; i < cut - 1; i++) {
            //cout << "Cut -------- " << i+1 << endl;
            targetTableIndex = ChooseTargetTable(hadTable);
            //cout << "targetTableIndex = " << targetTableIndex << endl;
            pivot = PivotBitChoose(hadTable, targetTableIndex, i, cutNum[i]);
            //cout << "pivot = " << pivot << endl;
            hadTable = Decomposition(hadTable, pivot, targetTableIndex);
            //TableStatistics(hadTable, &maxTableSize);

        }
        QueryPerformanceCounter(&endTime);
        times=((double)endTime.QuadPart-(double)startTime.QuadPart)/fre.QuadPart;
        //cout << fixed << setprecision(20) << times << 's' << endl;
        /// END -----------------------------------



        Table *ptrTable = hadTable;
        sum = 0;
        i = 0;
        while(ptrTable != NULL) {
           sum += ptrTable->ruleNum;
           ptrTable = ptrTable->nextTable;
        }

        //cout << "Total partition sum: " << sum << endl;
        //cout << ", Original rule :" << ruleSum;
        TableStatistics(hadTable, &maxTableSize);
        cout << "Rule Overhead : " << (double)(sum - ruleSum) << " = " << (double)(sum - ruleSum)/(ruleSum)*100 << "%" << endl << endl;
        //outfile << cut << " " << sum << " " << ruleSum << " " << (double)(sum - ruleSum)/(ruleSum)*100 <<  " " << TableStatistics(hadTable,  &maxTableSize);
        //outfile << " " << maxTableSize << endl;
        //outfile << ", D = " << TableStatistics(hadTable) << "\n";
        outfile << cut << " " << fixed << setprecision(20) << times << endl;

    }

    return 0;
}
