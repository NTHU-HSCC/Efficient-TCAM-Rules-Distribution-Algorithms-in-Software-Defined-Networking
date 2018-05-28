#include <iostream>
#include <fstream>
#include "Topology.h"

using namespace std;

Switch* AddSwitchToPath(Switch *head, int ID) {

    Switch* s = (Switch*) malloc(sizeof(Switch));
    s->ID = ID;
    s->color = 0;
    s->pathNum = 0;
    s->shortestPathNum = 0;
    s->shortestPathLen = 0;
    s->nextSwitch = NULL;

    if(head == NULL) {
        head = s;
    } else {
        Switch *ptr = head;
        while(ptr->nextSwitch != NULL) {
            ptr = ptr->nextSwitch;
        }
        ptr->nextSwitch = s;
    }
    return head;
}


void GenerateTopologyX(int topology[PATHNUM][SWITCHNUM]){

    for (int i = 0; i < PATHNUM; i++) {
        for(int j = 0; j < SWITCHNUM; j++) {
            topology[i][j] = 0;
        }
    }

    int count = 0;
    while(1){

        int record[SWITCHNUM] = {0};

        for (int i = 0; i < PATHNUM; i++) {

            int switches[SWITCHNUM];
            for(int j = 0; j < SWITCHNUM; j++){
                switches[j] = j;
            }

            // Shuffle the switches array
            for(int j = 0; j < 10000; j++){

                int t1 = rand()%SWITCHNUM;
                int t2 = rand()%SWITCHNUM;
                int temp = switches[t1];
                switches[t1] = switches[t2];
                switches[t2] = temp;
            }

            int length = SLEN + rand()%(LONGESTPATH-SLEN+1);

            for(int t = 0; t < length; t++){
                int target = switches[t];
                topology[i][target] = 1;
                record[target] = 1;
            }

        }


        bool repeat = false;
        for(int i = 0; i < SWITCHNUM; i++) {
            if(record[i] == 0) {
                count++;
                printf("failed %d\n", count);
                repeat = true;
                break;
            }
        }

        if(repeat)
            continue;
        else
            break;


    }
    printf("Topology created!\n");

}




void GenerateTopology(int topology[PATHNUM][SWITCHNUM]) {

    int i, j;
    int probability;
    int sum;

    for (int i = 0; i < PATHNUM; i++) {
        for(int j = 0; j < SWITCHNUM; j++) {
            topology[i][j] = 0;
        }
    }

    while(true) {
        int record[SWITCHNUM] = {0};
        for(i = 0; i < PATHNUM; i++) {
            for(j = 0; j < SWITCHNUM; j++) {
                topology[i][j] = 0;
                probability = rand() % SWITCHNUM;
                if(probability <= PROBA) {
                    topology[i][j] = 1;
                    record[j] = 1;
                }
            }
        }

        bool repeat = false;
        for(i = 0; i < SWITCHNUM; i++) {
            if(record[i] == 0) {
                repeat = true;
                break;
            }
        }

        if(repeat)
            continue;
        else
            break;
    }


    /*for (i = 0; i < PATHNUM; i++) {
        for(j = 0; j < SWITCHNUM; j++) {
            if(topology[i][j] == 1) {
                cout << j << " " ;
            }
        }
        cout << endl;
    }
    cout << endl;*/

}

void SortPathWithLength(Path path[], int switchPathRelation[SWITCHNUM][PATHNUM]) {
    int i, j;
    for(i = 0; i < PATHNUM - 1; i++) {
        int minIndex = i;
        for(j = i + 1; j < PATHNUM; j++) {
            if(path[j].length < path[minIndex].length) {
                minIndex = j;
            }
        }
        if(i != minIndex) {
            Path temp;
            temp = path[i];
            path[i] = path[minIndex];
            path[minIndex] = temp;
        }
    }

    /// Update switchPathRelation
    for(i = 0; i < PATHNUM; i++) {
        Switch* ptr = path[i].switchNode;
        while(ptr != NULL) {
            switchPathRelation[ptr->ID][i] = 1;
            ptr = ptr->nextSwitch;
        }
    }
}

void FindShortestPathInfo(Path path[], Switch switchInfo[], int sLen) {

    int i;
    /// Calculate the number of the shortest flow paths that pass each switch
    for(i = 0; i < PATHNUM ; i++) {
        if(path[i].length == sLen) {
            Switch* ptr = path[i].switchNode;
            while(ptr != NULL){
                switchInfo[ptr->ID].shortestPathNum++;
                ptr = ptr->nextSwitch;
            }
        }
    }

    /// Find the length of the shortest flow paths that pass each switch
    for(i = 0; i < PATHNUM; i++) {
        Switch* ptr = path[i].switchNode;
        while(ptr != NULL){
            if(switchInfo[ptr->ID].shortestPathLen == 0) {
                switchInfo[ptr->ID].shortestPathLen = path[i].length;
            }
            ptr = ptr->nextSwitch;
        }
    }

}

int InitialTopology(Path path[], Switch switchInfo[], int topology[PATHNUM][SWITCHNUM],int switchPathRelation[SWITCHNUM][PATHNUM]) {

    int i, j;
    int sLen;

    /// Initial path
    for (i = 0; i< PATHNUM; i++) {
        path[i].color = 0;
        path[i].switchNode = NULL;
        path[i].length = 0;
    }

    /// Initial switchInfo
    for (i = 0; i < SWITCHNUM; i++) {
        switchInfo[i].color = 0;
        switchInfo[i].ID = i;
        switchInfo[i].pathNum = 0;
        switchInfo[i].shortestPathLen = 0;
        switchInfo[i].shortestPathNum = 0;
        switchInfo[i].nextSwitch = NULL;
    }

    /// Initial switchPathRelation
    for(i = 0; i < SWITCHNUM; i++) {
        for(j = 0; j < PATHNUM; j++) {
            switchPathRelation[i][j] = 0;
        }
    }

    /// Add switch to path
    for(i = 0; i < PATHNUM; i++) {
        for(j = 0; j < SWITCHNUM; j++) {
            if(topology[i][j] == 1) {
                path[i].switchNode = AddSwitchToPath(path[i].switchNode, j);
                path[i].length++;
                switchInfo[j].pathNum++;
            }
        }
    }

    SortPathWithLength(path, switchPathRelation);

    /// sLen: Length of the shortest flow path
    sLen = path[0].length;
    FindShortestPathInfo(path, switchInfo, sLen);

    return sLen;
}

bool CheckColor(Path path[], int targetColor) { /// Check if each path passes "targetColor"

    int i;
    for (i = 0; i < PATHNUM; i++) {
        if (path[i].color != targetColor) {
            return false;
        }
    }
    return true;
}

void UpdatePathInfo(Path path[], int colorID, int targetColor) { /// Update the color of each path

    int i;
    for(i = 0; i < PATHNUM; i++) {
        Switch* ptr = path[i].switchNode;
        while(ptr != NULL) {
            if(ptr->ID == colorID) {
                ptr->color = targetColor;
                if(path[i].color != targetColor) {
                    path[i].color = targetColor;
                }
            }
            ptr = ptr->nextSwitch;
        }
    }
}

int OneGreedyFindMaxID(int switchPathNum[], Switch switchInfo[]) { /// 1-Greedy find target uncolored switch

    int i;
    int maxIndex = -1;
    int maxNum = -1;

    for (i = 0; i < SWITCHNUM; i++) {
        if(switchInfo[i].color == 0 && switchPathNum[i] > maxNum) {
            maxNum = switchPathNum[i];
            maxIndex = i;
        }
    }

    return maxIndex;
}

void TwoGreedyFindMaxID(Path path[], Switch switchInfo[], int *color1ID, int *color2ID,
                        int switchPathRelation[SWITCHNUM][PATHNUM], int targetColor) { /// 2-Greedy find target uncolored switch
    int i, j, p;
    int maxNum;

    maxNum = -1;
    *color1ID = -1;
    *color2ID = -1;

   /* for(i = 0; i < SWITCHNUM; i++ ) {
        cout << i << " " << switchInfo[i].color << " " << endl;
    }
    for (p = 0; p <PATHNUM; p++) {
        cout << p << " " << path[p].color << endl;
    }

    for(i = 0; i < SWITCHNUM; i++ ) {
        for (p = 0; p <PATHNUM; p++) {
            cout << switchPathRelation[i][p] << " ";
        }
        cout << endl;
    }*/

    for (i = 0; i < SWITCHNUM; i++) {
        for (j = 0; j < SWITCHNUM; j++) {
            int cnt = 0;
            if(i != j && switchInfo[i].color == 0 && switchInfo[j].color == 0 ) {
                for (p = 0; p <PATHNUM; p++) {
                    if(path[p].color != targetColor && (switchPathRelation[i][p] | switchPathRelation[j][p])) {
                        cnt++;
                    }
                }
                if(cnt > maxNum && cnt > 0) {
                    maxNum = cnt;
                    *color1ID = i;
                    *color2ID = j;
                }
            }
        }
    }

    for (i = 0; i < SWITCHNUM; i++) {
        int cnt = 0;
        if(switchInfo[i].color == 0) {
            for (p = 0; p <PATHNUM; p++) {
                if(switchPathRelation[i][p] && path[p].color != targetColor) {
                    cnt++;
                }
            }
            if(switchInfo[i].color == 0 && cnt >= maxNum && cnt > 0) {
                maxNum = cnt;
                *color1ID = i;
                *color2ID = -1;
                //printf("c1 %d, cnt %d\n", *color1ID, cnt);
            }
        }
    }
}

int OneGreedyColor(Path path[], Switch switchInfo[], int sLen) {

    int targetColor;
    int i;

    for(targetColor = 1; targetColor <= sLen; targetColor++) {
        while(!CheckColor(path, targetColor)) {
            int switchPathNum[SWITCHNUM] = {0};
            for(i = 0; i < PATHNUM; i++) {
                if(path[i].color != targetColor) {
                    Switch* ptr = path[i].switchNode;
                    while(ptr != NULL && ptr->color == 0) {
                        switchPathNum[ptr->ID]++;
                        ptr = ptr->nextSwitch;
                    }
                }
            }
            int colorID = OneGreedyFindMaxID(switchPathNum, switchInfo);
            if(colorID == -1) {
                return targetColor - 1;
            }

            switchInfo[colorID].color = targetColor;
            UpdatePathInfo(path, colorID, targetColor);
        }
    }
    return sLen;
}

int TwoGreedyColor(Path path[], Switch switchInfo[], int sLen,
                   int switchPathRelation[SWITCHNUM][PATHNUM]) {

    int targetColor;
    int color1, color2;

    for(targetColor = 1; targetColor <= sLen; targetColor++) {
        //printf("\nColor %d\n", targetColor);
        while(!CheckColor(path, targetColor)) {
            TwoGreedyFindMaxID(path, switchInfo, &color1, &color2, switchPathRelation, targetColor);
            if(color1 == -1 && color2 == -1) {
                //printf("TwoGreedy error find color !");
                return targetColor - 1;
            } else if(color1 != -1 && color2 == -1) {
                //printf("color1 id = %d\n", color1);
                switchInfo[color1].color = targetColor;
                UpdatePathInfo(path, color1, targetColor);
            } else if(color1 != -1 && color2 != -1) {
                //printf("color1 id = %d\t", color1);
                //printf("color2 id = %d\n", color2);
                switchInfo[color1].color = targetColor;
                switchInfo[color2].color = targetColor;
                UpdatePathInfo(path, color1, targetColor);
                UpdatePathInfo(path, color2, targetColor);
            }
        }
    }
    return sLen;
}

int SA(Path path[], Switch switchInfo[], int sLen, int switchPathRelation[SWITCHNUM][PATHNUM]) {

    int i, j;
    int colorNum = 1;
    int ifColored[SWITCHNUM] = {0};
    int currentPath[PATHNUM] = {0};
    bool ifStart = true;


    while (colorNum < sLen + 1) {
        if(ifStart) {

            /// Find an uncolored switch with condition 1
            for(j = 0; j < PATHNUM; j++) {
                currentPath[j] = 0;
            }
            int maxPath = 0;
            int maxIndex = -1;
            for(i = 0; i < SWITCHNUM; i++)  {
                if(switchInfo[i].color == 0) {
                    if(switchInfo[i].pathNum > maxPath) {
                        maxIndex = i;
                        maxPath = switchInfo[i].pathNum;
                    }
                }
            }

            //printf("maxIndex ====== %d\n", maxIndex);

            if(maxIndex != -1) {
                switchInfo[maxIndex].color = colorNum;
                //maxPath = switchInfo[i].pathNum;
                if(maxPath == PATHNUM) {
                    ifStart = true;
                    //printf("1-1 color: %d (%d)\n", maxIndex, colorNum);
                    colorNum++;
                    continue;
                } else {
                    for(j = 0; j < PATHNUM; j++) {
                        currentPath[j] = switchPathRelation[maxIndex][j];
                    }
                    ifStart = false;
                    //printf("1-2 color: %d\n", maxIndex);
                }

            } else {   /// Not find any switches
                return colorNum - 1;
            }
        } else {

            int maxPath = 0;
            int maxIndex = -1;

            for(i = 0; i < SWITCHNUM; i++) {
                int cnt = 0;
                if(switchInfo[i].color == 0) {
                    for(j = 0; j < PATHNUM; j++) {
                        if(currentPath[j] | switchPathRelation[i][j])
                            cnt++;
                    }

                    if(cnt > maxPath) { /// condition 1
                        maxPath = cnt;
                        maxIndex = i;
                    } else if (maxIndex != -1 && cnt == maxPath && switchInfo[i].shortestPathNum < switchInfo[maxIndex].shortestPathNum) { /// condition 2
                        maxPath = cnt;
                        maxIndex = i;
                    } else if (maxIndex != -1 && cnt == maxPath && switchInfo[i].shortestPathNum == switchInfo[maxIndex].shortestPathNum){
                        if(switchInfo[i].shortestPathLen > switchInfo[maxIndex].shortestPathLen) { /// condition 3
                            maxPath = cnt;
                            maxIndex = i;
                        }else if(switchInfo[i].shortestPathLen == switchInfo[maxIndex].shortestPathLen
                                && switchInfo[i].pathNum < switchInfo[maxIndex].pathNum) { /// condition 4
                                maxPath = cnt;
                                maxIndex = i;
                        }
                    }
                }
            }

            if(maxIndex != -1) {
                switchInfo[maxIndex].color = colorNum;
                if(maxPath == PATHNUM) {
                    ifStart = true;
                    colorNum++;
                    //printf("2 color: %d\n", maxIndex);
                    continue;
                } else {
                    for(j = 0; j < PATHNUM; j++) {
                        currentPath[j] = switchPathRelation[maxIndex][j] | currentPath[j];
                    }
                    ifStart = false;
                    //printf("2 color: %d\n", maxIndex);
                }

            } else {    /// Not find any switches
                return colorNum - 1;
            }
        }
    }
    //printf("colorNum = %d\n", colorNum);
    return colorNum - 1;
}

/// Optimal solution

ColorSwitch* CreateColor(int switchIndex, int switchPathNum[SWITCHNUM][PATHNUM], int length, int parentColorSwitch[SWITCHNUM]) {

    int colorPath[PATHNUM] = {0};
    ColorSwitch* colorswitch = (ColorSwitch*) malloc(sizeof(ColorSwitch));
    colorswitch->length = length;
    colorswitch->pathNum = 0;
    colorswitch->id = -1;
    colorswitch->duplicate = false;

    int i, j;
    for(i = 0; i < SWITCHNUM; i++) {
        colorswitch->sameColorSwitch[i] = parentColorSwitch[i];
    }
    colorswitch->sameColorSwitch[switchIndex] = 1;
    for(i = 0; i < SWITCHNUM; i++) {
        if(colorswitch->sameColorSwitch[i]) {
            for(j = 0; j < PATHNUM; j++) {
                if(switchPathNum[i][j]) {
                    colorPath[j] = 1;
                }
            }
        }
    }
    for(j = 0; j < PATHNUM; j++) {
        if(colorPath[j]) {
            colorswitch->pathNum++;
        }
    }
    colorswitch->nextNode = NULL;
    return colorswitch;
}

ColorSwitch* NodeCombination(int switchPathNum[SWITCHNUM][PATHNUM], int parentColorSwitch[SWITCHNUM], ColorSwitch child,
                             int length, int parentId, int parentColor) {

    int colorPath[PATHNUM] = {0};
    ColorSwitch* colorswitch = (ColorSwitch*) malloc(sizeof(ColorSwitch));
    colorswitch->length = length;
    colorswitch->pathNum = 0;
    colorswitch->id = child.id;
    colorswitch->duplicate = false;
    colorswitch->color = 0;
    int i, j;
    for(i = 0; i < SWITCHNUM; i++) {
        if(parentColorSwitch[i] & child.sameColorSwitch[i]) {
            colorswitch->duplicate = true;
        }
        colorswitch->sameColorSwitch[i] = parentColorSwitch[i] | child.sameColorSwitch[i];
    }
    for(i = 0; i < SWITCHNUM; i++) {
        if(colorswitch->sameColorSwitch[i]) {
            for(j = 0; j < PATHNUM; j++) {
                if(switchPathNum[i][j]) {
                    colorPath[j] = 1;
                }
            }
        }
    }
    for(j = 0; j < PATHNUM; j++) {
        if(colorPath[j]) {
            colorswitch->pathNum++;
        }
    }
    colorswitch->nextNode = NULL;

    if(!colorswitch->duplicate) {
        colorswitch->color += parentColor;
        colorswitch->color++;
        //printf("parent id = %d, child id = %d\n", parentId, child.id);
    } else {
        colorswitch->color = parentColor;
    }
    return colorswitch;
}

void AddNextColor(ColorSwitch** head, ColorSwitch* newAdd) {
    if(*head == NULL){
        *head = newAdd;
    } else {
        ColorSwitch *ptr = *head;
        while(ptr->nextNode != NULL)
            ptr = ptr->nextNode;
        ptr->nextNode = newAdd;
    }
}

ColorSwitch* InitilizeColorSwitch(ColorSwitch* head ,int switchPathNum[SWITCHNUM][PATHNUM]) {
    int i;
    int parentColorSwitch[SWITCHNUM] = {0};
    for(i = 0; i < SWITCHNUM; i++) {
        AddNextColor(&head, CreateColor(i, switchPathNum, 1, parentColorSwitch));
    }

    return head;
}

ColorSwitch* RemoveLowerPathNumNode(ColorSwitch* head) {
    ColorSwitch* frontNode;
    ColorSwitch* currentNode;
    frontNode = NULL;
    currentNode = head;

    while(currentNode != NULL) {
        if(currentNode->pathNum < PATHNUM) {
            if(frontNode == NULL) {
                head = currentNode->nextNode;
                currentNode = head;
            } else {
                frontNode->nextNode = currentNode->nextNode;
                currentNode = frontNode->nextNode;
            }
        } else {
            frontNode = currentNode;
            currentNode = currentNode->nextNode;
        }
    }
    return head;
}

int SetId(ColorSwitch* head) {
    int id = 0;
    int i;
    ColorSwitch* currentNode = head;
    while(currentNode != NULL) {
        //printf("id = %d, ", id);
        for(i = 0; i < SWITCHNUM; i++) {
            //if(currentNode->sameColorSwitch[i])
                //printf("%d", i);
        }
        currentNode->id = id++;
        currentNode->length = 1;
        currentNode->color = 1;
        currentNode = currentNode->nextNode;
        //printf("\n");
    }
    return id;
}

int OptimalSolution (Path path[], int switchPathNum[SWITCHNUM][PATHNUM], int sLen) {

    int i, j, k;
    int CombineNum;
    ColorSwitch* head;
    head = NULL;
    head = InitilizeColorSwitch(head, switchPathNum);


    ColorSwitch* ptr;
    ptr = head;
    while(ptr != NULL) {
        //printf("pathNum = %d %d\n", ptr->pathNum, ptr->length);
        ptr = ptr->nextNode;
    }

    for(i = 2; i <= SWITCHNUM; i++) {
        ptr = head;
        while(ptr != NULL) {
            if(ptr->length == (i - 1)) {
                int start;
                for(j = SWITCHNUM - 1; j >= 0; j--) {
                    if(ptr->sameColorSwitch[j] != 0) {
                        start = j + 1;
                        break;
                    }
                }
                int parentColorSwitch[SWITCHNUM] = {0};
                for(k = 0; k < SWITCHNUM; k++) {
                    parentColorSwitch[k] = ptr->sameColorSwitch[k];
                }

                for(j = start; j < SWITCHNUM; j++) {
                    for(k = 0; k < SWITCHNUM; k++) {
                        //if(ptr->sameColorSwitch[k])
                            //printf("%d", k);
                    }
                    //printf("%d", j);
                    AddNextColor(&head, CreateColor(j, switchPathNum, i, parentColorSwitch));
                    //CreateColor(j, switchPathNum, i);
                }
            }
            ptr = ptr->nextNode;
        }

    }

    head = RemoveLowerPathNumNode(head);
    CombineNum = SetId(head);
    printf("CombineNum = %d\n", CombineNum);

    ColorSwitch arr[CombineNum];

    i = 0;
    ptr = head;
    while(ptr != NULL) {
        /*ColorSwitch* temp = (ColorSwitch*) malloc(sizeof(ColorSwitch));
        temp->duplicate = ptr->duplicate;printf("d");
        temp->id = ptr->id;
        temp->length = ptr->length;
        temp->nextNode = NULL;
        temp->pathNum = ptr->pathNum;
        temp->color = 1;
        AddNextColor(&combineHead, temp);*/
        arr[i].duplicate = ptr->duplicate;
        arr[i].id = ptr->id;
        arr[i].length = ptr->length;
        arr[i].nextNode = NULL;
        arr[i].pathNum = ptr->pathNum;
        arr[i].color = 1;
        for(j = 0; j < SWITCHNUM; j++)
            arr[i].sameColorSwitch[j] = ptr->sameColorSwitch[j];
        i++;
        ptr = ptr->nextNode;
    }
    //printf("CombineNum = %d\n", CombineNum);

    for(i = 2; i <= sLen; i++) {

        ptr = head;
        while(ptr != NULL) {
            if(ptr->length == (i - 1)) {
                int start = ptr->id + 1;
                int parentColorSwitch[SWITCHNUM] = {0};
                for(k = 0; k < SWITCHNUM; k++) {
                    parentColorSwitch[k] = ptr->sameColorSwitch[k];
                }

                /*ColorSwitch *jNode;
                jNode = combineHead;
                while (jNode != NULL) {
                    if(jNode->id == start) {
                        bool addNew = true;
                        for(k = 0; k < SWITCHNUM; k++) {
                            if(parentColorSwitch[k] & jNode->sameColorSwitch[k]) {
                                addNew = false;
                                break;
                            }
                        }

                        if(addNew)
                            AddNextColor(&head, NodeCombination(switchPathNum, parentColorSwitch, jNode, i, ptr->id, ptr->color));

                        start++;
                    }
                    jNode = jNode->nextNode;
                }*/
                ColorSwitch* tail;
                tail = head;
                while (tail->nextNode != NULL) {
                    tail = tail->nextNode;
                }

                for(j = start; j < CombineNum; j++) {
                    bool addNew = true;
                    for(k = 0; k < SWITCHNUM; k++) {
                        if(parentColorSwitch[k] & arr[j].sameColorSwitch[k]) {
                            addNew = false;
                            break;
                        }
                    }
                    if(addNew) {
                        //tail->nextNode = NodeCombination(switchPathNum, parentColorSwitch, arr[j], i, ptr->id, ptr->color);
                        //tail = tail->nextNode;
                        AddNextColor(&head, NodeCombination(switchPathNum, parentColorSwitch, arr[j], i, ptr->id, ptr->color));
                    }
                    //printf("%d %d\n", ptr->id, j);
                    //AddNextColor(&combineHead, NodeCombination(switchPathNum, parentColorSwitch, arr[j], i, ptr->id, ptr->color));
                }

                //if(combineHead != NULL) {
                //    AddNextColor(&head, combineHead);
                //}
            }
            //printf("i = %d\n", i);
            ptr = ptr->nextNode;
        }
    }

    int maxColor = 0;
    ptr = head;
    while(ptr != NULL) {
        if(!ptr->duplicate && ptr->color > maxColor) {
            maxColor = ptr->color;
        }
        ptr = ptr->nextNode;
    }

    /// Release linked-list
    ptr = head;
    while(ptr != NULL) {
        ColorSwitch* prev = ptr;
        ptr = ptr->nextNode;
        free(prev);
    }
    //printf("Max color = %d.\n", maxColor);
    //fprintf(f_out, "Max color = %d.\n", maxColor);
    //printf("r1\n");
    return maxColor;

}

