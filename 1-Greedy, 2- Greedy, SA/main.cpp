#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include <iomanip>
#include <windows.h>

#include "Topology.h"

using namespace std;

int main()
{
    srand(time(NULL));
    char fileOutput[]="Output.txt";
    fstream fout;

    ofstream outstream;

    int pathnum = PATHNUM;
    int switchnum = SWITCHNUM;
    int slen = SLEN;
    int llen = LONGESTPATH;

    char outfileName[100];
    sprintf(outfileName, "./output/%d_%d_%d_%d.txt", switchnum, pathnum, slen, llen);


    outstream.open(outfileName);


    int cnt[4][LONGESTPATH];    /// Number of colors from 1-Greedy, 2-Greedy, SA, and Optimal (0: 1-Greedy, 1: 2-Greedy, 2: SA, 3: Optimal)
    int m_counter[3][MAX_M+1];

    int tempCount = 0;          /// Number of topologies
    int tempCount2 = 0;

    LARGE_INTEGER startTime,endTime,fre;
    double times;

    /// Initialize
    for(int i = 0; i <= LONGESTPATH; i++) {
        cnt[0][i] = 0;
        cnt[1][i] = 0;
        cnt[2][i] = 0;
        cnt[3][i] = 0;
    }

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < MAX_M+1; j++){
            m_counter[i][j] = 0;
        }
    }


    while(1) {

        int topology[PATHNUM][SWITCHNUM];   /// Store 0 or 1 : topology[i][j] = 1 represents path i passes switch j, otherwise 0.
        int temp1, temp2, temp3, temp4;

        GenerateTopologyX(topology);


        for (int i = 0; i < 3; i++) {       /// 0: 1-Greedy, 1: 2-Greedy, 2: SA, 3: Optimal
            Path path[PATHNUM];
            Switch switchInfo[SWITCHNUM + 1];
            int switchPathRelation[SWITCHNUM][PATHNUM]; /// Equal to topology
            int sLen;

            sLen = InitialTopology(path, switchInfo, topology, switchPathRelation);

            if(sLen != SLEN) {
                break;
            }

            if(sLen == SLEN && i == 0) {
                tempCount++;
            }

            if(i == 0) {
                QueryPerformanceFrequency(&fre);
                QueryPerformanceCounter(&startTime);

                temp1 = OneGreedyColor(path, switchInfo, sLen); /// 1-Greedy

                QueryPerformanceCounter(&endTime);
                times=((double)endTime.QuadPart - (double)startTime.QuadPart)/fre.QuadPart;
                //cout << "1-Greedy times: " << fixed << setprecision(10) << times << 's' << endl;
                cnt[0][sLen] += temp1;

                m_counter[0][temp1]++;

            } else if (i == 1) {
                QueryPerformanceFrequency(&fre);
                QueryPerformanceCounter(&startTime);

                temp2 = TwoGreedyColor(path, switchInfo, sLen, switchPathRelation); /// 2-Greedy

                QueryPerformanceCounter(&endTime);
                times=((double)endTime.QuadPart - (double)startTime.QuadPart)/fre.QuadPart;
                //cout << "2-Greedy times: " << fixed << setprecision(10) << times << 's' << endl;
                cnt[1][sLen] += temp2;
                m_counter[1][temp2]++;

            } else if (i == 2){
                QueryPerformanceFrequency(&fre);
                QueryPerformanceCounter(&startTime);

                temp3 = SA(path, switchInfo, sLen, switchPathRelation); /// SA

                QueryPerformanceCounter(&endTime);
                times=((double)endTime.QuadPart - (double)startTime.QuadPart)/fre.QuadPart;
                //cout << "SA times: " << fixed << setprecision(10) << times << 's' << endl;

                cnt[2][sLen] += temp3;
                m_counter[2][temp3]++;

            } else if (i == 3) {
                temp4 = OptimalSolution(path, switchPathRelation, sLen);
                cnt[3][sLen] += temp4;
            }

            /// Release all path and switch
            for (int k = 0; k < PATHNUM; k++) {
                Switch* ptr = path[k].switchNode;
                while(ptr != NULL){
                    Switch* prev = ptr;
                    ptr = ptr->nextSwitch;
                    free(prev);
                }
            }
        }


        if(cnt[0][SLEN] > 0 && tempCount2 < tempCount) {
            //cnt[3][SLEN] = 1;

            tempCount2 = tempCount;
            if(tempCount == FILENUM){
                cout << "Total topologies = " << tempCount << endl;
                printf("slen %d: %d %d %d %d\n", SLEN, cnt[0][SLEN], cnt[1][SLEN], cnt[2][SLEN], cnt[3][SLEN]);
                //printf("optimal %.3f %.3f %.3f %.3f ", (double)cnt[0][SLEN]/cnt[3][SLEN], (double)cnt[1][SLEN]/cnt[3][SLEN], (double)cnt[2][SLEN]/cnt[3][SLEN], (double)cnt[3][SLEN]/cnt[3][SLEN]);
                cout << fixed << setprecision(2) << "Switch: " << SWITCHNUM << " Flow: " << PATHNUM << " Average number of colors:\t" << (double)cnt[0][SLEN]/tempCount << "\t" << (double)cnt[1][SLEN]/tempCount << "\t" << (double)cnt[2][SLEN]/tempCount << endl;
                //cout << fixed << setprecision(2) << "Number of colors/optimal colors:\t" << (double)cnt[0][SLEN]/cnt[3][SLEN] << " " << (double)cnt[1][SLEN]/cnt[3][SLEN] << " " << (double)cnt[2][SLEN]/cnt[3][SLEN] << " ";

                cout << endl << endl;


                for(int j = 1; j < MAX_M+1; j++){
                        if(j == 1)
                            printf("%3d", j);
                        else
                            printf(" %3d", j);
                }
                cout << endl;
                for(int i = 0; i < 3; i++){
                    for(int j = 1; j < MAX_M+1; j++){
                        if(j == 1){
                            printf("%3d", m_counter[i][j]);
                            //outstream << m_counter[i][j];
                        }
                        else{
                            printf(" %3d", m_counter[i][j]);
                            //outstream << "\t" << m_counter[i][j];
                        }
                    }
                    cout << endl;
                    //outstream << endl;
                }

            }
            //system("pause");
        }

        if(tempCount == FILENUM) {
            break;
        }
    }

    return 0;
}
