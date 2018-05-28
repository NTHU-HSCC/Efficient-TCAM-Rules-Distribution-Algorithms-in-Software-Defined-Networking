#include "Node.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <windows.h>

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
    ofstream outfile;
    outfile.open("ipc1_10k_1.5_-1_-1.txt");
    LARGE_INTEGER startTime,endTime,fre;
    double times;

    for (int cnt = 2; cnt < 4; cnt++) {

        Table *headTable = NULL;
        Node *headNode;
        int cut = 0;
        int ruleSum = 0;
        int maxIndex;
        int minIndex;
        int maxTableSize;
        int minTableSize;
        int pivot;
        int sum;
        double partitionSize;

        cout << "************** m = " << cnt << " **************"<< endl;
        cut = cnt;
        headNode = LoadRuleFromFile(&ruleSum, filename);        /// Load rule from file
        ruleSum = TableNodeNum(headNode);
        cout << "Rule number = " << ruleSum << endl;

        TableNodeIni(&headTable, headNode, ruleSum);

        Node *ptr = headTable->child;
        while(ptr != NULL){
            DecimalBinary(ptr);
            ptr = ptr->nextNode;
        }

        partitionSize = (double)ruleSum / cut;
        //cout << "partitionSize = " << partitionSize << endl;

        AddAllTable(&headTable, cut);

        /// START -----------------------------------
        QueryPerformanceFrequency(&fre);
        QueryPerformanceCounter(&startTime);
        while (!DoneDecomposition(headTable, partitionSize, &maxIndex, &minIndex, &maxTableSize, &minTableSize)) {
            pivot = PivotBitChoose(headTable, maxIndex, maxTableSize, minTableSize, partitionSize);
            //TableStatistics(headTable);
            if(pivot == -1) {
                break;
            }
            Decomposition2(headTable, pivot, maxIndex, minIndex);
            //TableStatistics(headTable);
        }
        QueryPerformanceCounter(&endTime);
        times=((double)endTime.QuadPart - (double)startTime.QuadPart)/fre.QuadPart;
        //cout << fixed << setprecision(20) << times << 's' << endl;
        /// END -----------------------------------
        TableStatistics(headTable);
        sum = 0;
        Table *ptrTable = headTable;
        while(ptrTable != NULL) {
            sum += ptrTable->ruleNum;
            ptrTable = ptrTable->nextTable;
        }

        cout << "Total partition rule: " << sum << endl;
        cout << "Rule Overhead: " << (double)(sum - ruleSum)/(ruleSum)*100 << "% \n";
        //outfile << "Total partition rule : " << sum << endl;
        //outfile << cut << " " << sum << " " << ruleSum  << " " << (double)(sum - ruleSum)/(ruleSum)*100 << " " << TableStatistics(headTable);
        //outfile << " " << maxTableSize << endl;
        outfile << cut << " " << fixed << setprecision(20) << times << endl;
    }

    return 0;
}
