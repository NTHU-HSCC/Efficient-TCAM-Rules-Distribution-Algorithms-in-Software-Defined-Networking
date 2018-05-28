#include "Node.h"

Node* CreateNode(Rule rule) { /// Create a Node and initialize it
    Node* n = (Node*) malloc(sizeof(Node));
    n->nextNode = NULL;
    n->rule = rule;
    n->id = 0;
    return n;
}

void AddNextNode(Node** head, Node* newAdd) { /// Add a Node to the Node link-list
    if(*head == NULL){
        *head = newAdd;
    } else {
        Node *ptr = *head;
        while(ptr->nextNode != NULL)
            ptr = ptr->nextNode;
        ptr->nextNode = newAdd;
    }
}

int TableNodeNum(Node* headNode) {
    Node* node = headNode;
    int num = 0;
    while (node != NULL) {
        num++;
        node = node->nextNode;
    }
    return num;
}

void TableNodeIni(Table** head, Node* newAdd, int ruleNum) { /// Create the first sub-table
    Table* t = (Table*) malloc(sizeof(Table));
    if(*head == NULL){
        (*head) = t;
        (*head)->child = newAdd;    /// Add all nodes(rules) to the first sub-table
        (*head)->ruleNum = ruleNum;
        (*head)->index = 1;
        (*head)->nextTable = NULL;
    }
}

void AddAllTable(Table** head, int cut) { /// Create remaining m-1 sub-tables
    Table *ptr;
    int i;
    for (i = 0; i < cut - 1; i++) {
        int cnt = 1;
        ptr = *head;
        while(ptr->nextTable != NULL) {
            cnt++;
            ptr = ptr->nextTable;
        }
        cnt++;
        Table* t = (Table*) malloc(sizeof(Table));
        ptr->nextTable = t;
        ptr->nextTable->child = NULL;
        ptr->nextTable->ruleNum = 0;
        ptr->nextTable->index = cnt;
        ptr->nextTable->nextTable = NULL;

    }
}

void TableAddNewNode2(Table** head, Node* newAdd, int ruleNum, int minTableIndex) { /// Add rules to Tmin

    Table *ptr = *head;
    while(ptr != NULL) {
        if(ptr->index == minTableIndex) {
            break;
        }
        ptr = ptr->nextTable;
    }

    Node *node = ptr->child;
    if(node == NULL) {  /// Tmin is empty
        ptr->child = newAdd;
        ptr->ruleNum = ruleNum;
    } else {            /// Tmin is not empty
        while(node->nextNode != NULL) {
            node = node->nextNode;
        }
        node->nextNode = newAdd;
        ptr->ruleNum += ruleNum;
        //printf("%d ", ptr->ruleNum);
    }
}

void DecimalBinary(Node* node){ /// Transfer the decimal value of each rule(in the first sub-table) to a 104-bits rule

    int ptr_temp[32];
    uint32_t convert, base10;
    int k, i, j;
    int mask;

    /// 0 means storing src address while 1 means dest address
    for (k = 0; k <= 1; k++) {
        if(k == 0) {
            base10 = node->rule.src_addr_from;
            mask = 32 - node->rule.src_mask;
        } else {
            base10 = node->rule.dest_addr_from;
            mask = 32 - node->rule.dest_mask;
        }

        for(i = 0; i < 32; i += 8){
            convert = base10 % 256;
            base10 = base10 / 256;
            for(j = 0; j < 8; j++){
                ptr_temp[i + j] = convert % 2;
                convert = convert / 2;
            }
        }
        if(mask == 32) {
            for(j = 31; j >= 0; j--) {
                ptr_temp[j] = 2;
            }
        } else {
            for(j = mask - 1; j >= 0; j--) {
                ptr_temp[j] = 2;
            }
        }

        for(j = 31; j >= 0; j--) {
            node->rule.ruleBinary[j + (2-k)*32 + 8] =  ptr_temp[j];
        }
    }

    int src_port_temp[16] = {0};
    int dest_port_temp[16] = {0};

    /// source port from
    if(node->rule.src_port_from == 0 && node->rule.src_port_to == 65535) {
        for (i = 15; i >= 0; i--)
            src_port_temp[i] = 2;
    } else if (node->rule.src_port_from == 20 && node->rule.src_port_from == 23) {
        int port[16] = {0,0,0,0, 0,0,0,0, 0,0,0,1, 0,1,2,2};
        for (i = 15; i >= 0; i--)
            src_port_temp[i] = port[i];
    } else if ((node->rule.src_port_to - node->rule.src_port_from > 1))  {
        base10 = node->rule.src_port_from;
        for(i = 0; i < 16; i += 8){
            convert = base10 % 256;
            base10 = base10 / 256;
            for(j = 0; j < 8; j++){
                src_port_temp[i + j] = convert % 2;
                convert = convert / 2;
            }
        }
        ///int port1[6] = {1024, 2048, 4096, 8192, 16384, 32768};
        ///int port2[6] = {2047, 4095, 8191, 16383, 32767, 65535};
        if(node->rule.src_port_from >= 1024) {
            int find_ = 0;
            for (i = 15; i >= 0; i--) {
                if(src_port_temp[i] == 1) {
                    find_ = 1;
                } else {
                    if(find_) {
                        src_port_temp[i] = 2;
                    }
                }
            }
        } else {
            //printf("src Priority = %d, ", node->rule.priority);
            if(node->rule.src_port_from <512) {
                //printf("%d\n", node->rule.src_port_from);
                for(i = 0; i < 15; i++) {
                    if(src_port_temp[i] == 0) {
                        src_port_temp[i] = 2;
                    } else {
                        break;
                    }
                }
            } else {
                if (node->rule.src_port_from == 512) {
                    for(i = 0; i < 7; i++) {
                        src_port_temp[i] = 2;
                    }
                } else if (node->rule.src_port_from == 640) {
                    for(i = 0; i < 4; i++) {
                        src_port_temp[i] = 2;
                    }
                } else if (node->rule.src_port_from == 656) {
                    for(i = 0; i < 2; i++) {
                        src_port_temp[i] = 2;
                    }
                }
            }

            /*for(i = 15; i >= 0; i--) {
                printf("%d", src_port_temp[i]);
            }
            printf("\n");*/
        }

    } else {
        base10 = node->rule.src_port_from;
        for(i = 0; i < 16; i += 8){
            convert = base10 % 256;
            base10 = base10 / 256;
            for(j = 0; j < 8; j++){
                src_port_temp[i + j] = convert % 2;
                convert = convert / 2;
            }
        }
    }

    for(i = 15; i >= 0; i--) {
        node->rule.src_port_binary[i] = src_port_temp[i];
    }

    /// destination port from
    if(node->rule.dest_port_from == 0 && node->rule.dest_port_to == 65535) {
        for (i = 15; i >= 0; i--)
            dest_port_temp[i] = 2;
    } else if (node->rule.dest_port_from == 20 && node->rule.dest_port_to == 23) {
        int port[16] = {0,0,0,0, 0,0,0,0, 0,0,0,1, 0,1,2,2};
        for (i = 15; i >= 0; i--)
            dest_port_temp[i] = port[i];
    } else if ((node->rule.dest_port_to - node->rule.dest_port_from > 1)) {
        base10 = node->rule.dest_port_from;
        for(i = 0; i < 16; i += 8){
            convert = base10 % 256;
            base10 = base10 / 256;
            for(j = 0; j < 8; j++){
                dest_port_temp[i + j] = convert % 2;
                convert = convert / 2;
            }
        }
        if(node->rule.dest_port_from >= 1024) {
            if ( (node->rule.dest_port_from == 1024 && node->rule.dest_port_to == 2047) || (node->rule.dest_port_from == 2048 && node->rule.dest_port_to == 4095)
                 || (node->rule.dest_port_from == 4096 &&  node->rule.dest_port_to == 8191)  || (node->rule.src_port_from == 8192 && node->rule.dest_port_to == 16383)
                 || (node->rule.dest_port_from == 16384 &&  node->rule.dest_port_to == 32767) || (node->rule.dest_port_from == 32768 && node->rule.dest_port_to == 65535))
            {
                int find_ = 0;
                for (i = 15; i >= 0; i--) {
                    if(dest_port_temp[i] == 1) {
                        find_ = 1;
                    } else {
                        if(find_) {
                            dest_port_temp[i] = 2;
                        }
                    }
                }
            } else { /// 1026 ~ 2047
                for(i = 0; i < 15; i++) {
                    if(dest_port_temp[i] == 0) {
                        dest_port_temp[i] = 2;
                    } else {
                        break;
                    }
                }
            }
        } else {
            //printf("dest Priority = %d, ", node->rule.priority);
            if(node->rule.dest_port_from <512) {
                //printf("%d\n", node->rule.src_port_from);
                for(i = 0; i < 15; i++) {
                    if(dest_port_temp[i] == 0) {
                        dest_port_temp[i] = 2;
                    } else {
                        break;
                    }
                }
            } else {
                if (node->rule.dest_port_from == 512) {
                    for(i = 0; i < 7; i++) {
                        dest_port_temp[i] = 2;
                    }
                } else if (node->rule.dest_port_from == 640) {
                    for(i = 0; i < 4; i++) {
                        dest_port_temp[i] = 2;
                    }
                } else if (node->rule.dest_port_from == 656) {
                    for(i = 0; i < 2; i++) {
                        dest_port_temp[i] = 2;
                    }
                }
            }
            /*for(i = 15; i >= 0; i--) {
                printf("%d", dest_port_temp[i]);
            }
            printf("\n");*/
        }
    } else {
        base10 = node->rule.dest_port_from;
        for(i = 0; i < 16; i += 8){
            convert = base10 % 256;
            base10 = base10 / 256;
            for(j = 0; j < 8; j++){
                dest_port_temp[i + j] = convert % 2;
                convert = convert / 2;
            }
        }
    }

    for(i = 15; i >= 0; i--) {
        node->rule.dest_port_binary[i] = dest_port_temp[i];
    }

    for(k = 0; k <= 1; k++) {
        if(k == 0) {
            for(j = 15; j >= 0; j--) {
                node->rule.ruleBinary[j + 16 + 8] =  node->rule.src_port_binary[j];
            }
        } else {
            for(j = 15; j >= 0; j--) {
                node->rule.ruleBinary[j + 8] =  node->rule.dest_port_binary[j];
            }
        }
    }

    /// protocol
    int protocol_temp[8] = {0};
    base10 = node->rule.data_protocol;
    for(i = 0; i < 8; i += 8){
        convert = base10 % 256;
        base10 = base10 / 256;
        for(j = 0; j < 8; j++){
            protocol_temp[i + j] = convert % 2;
            convert = convert / 2;
        }
    }

    if(node->rule.protocol_mask == 0) {
        for(i = 7; i >= 0; i--)
            node->rule.ruleBinary[i] = 2;
    } else {
        j = 7;
        for(i = 7; i >= 0; i--) {
            //printf("%d ", protocol_temp[j]);
            node->rule.ruleBinary[i] = protocol_temp[j--];

        } //system("pause");
    }
}

int PivotBitChoose(Table *headTable, int targetTableIndex,int maxTableSize, int minTableSize, int averageTableSize) {

    int i;
    int bitType[BITNUM][3] = {0}; /// 0 = 0, 1 = 1, 2 = *
    int maxP[BITNUM] = {0};
    int minP[BITNUM] = {0};
    int tempPivot = -1;
    int pivot2;
    int temp;

    int bitNumDiff[BITNUM] = {0}; /// d(i)
    int bitNumSum[BITNUM] = {0}; /// s(i)
    double pValue[BITNUM] = {0}; /// v(i)

    Table *targetTable = headTable;
    if(targetTableIndex != 0) {
        while(targetTable != NULL) {
            if(targetTableIndex == targetTable->index) {
                break;
            }
            targetTable = targetTable->nextTable;
        }
    }

    for(i = BITNUM - 1; i >= 0; i--) {
        Node *topNode = targetTable->child;
        while(topNode != NULL) {
            bitType[i][topNode->rule.ruleBinary[i]]++;
            topNode = topNode->nextNode;
        }
        maxP[i] = (bitType[i][0] > bitType[i][1])? bitType[i][0]:bitType[i][1];
        minP[i] = (bitType[i][0] < bitType[i][1])? bitType[i][0]:bitType[i][1];
        bitNumDiff[i] = abs(minP[i] + minTableSize - maxP[i]);                 /// d(i)
        bitNumSum[i] = (maxP[i] + minP[i] + minTableSize + 2 * bitType[i][2]); /// s(i)
        pValue[i] = ALPHA * (bitNumSum[i]) + (1 - ALPHA) * (bitNumDiff[i]);    /// v(i)

        /*printf("%d, 0=%d, 1=%d, *=%d, %d(%d), %d, %.3f, (%.2f, %.2f)\n", i, bitType[i][0], bitType[i][1], bitType[i][2],
                bitNumDiff[i], minP[i] + minTableSize - maxP[i]
                , bitNumSum[i], pValue[i], ((double)(bitType[i][2]+maxP[i])-(minP[i]+bitType[i][2]+minTableSize))
               , (1-END_THRESHOLD)*averageTableSize);*/
    }

    temp = 100000;
    int temp2 = maxP[BITNUM - 1] + bitType[BITNUM - 1][2];
    pivot2 = BITNUM - 1;
    for(i = BITNUM - 1; i >= 0; i--) {
		if(temp > pValue[i]  && (minP[i] + bitType[i][2] + minTableSize) < (maxP[i] + minP[i] + bitType[i][2])
		   && bitNumDiff[i] < abs((maxP[i] + minP[i] + bitType[i][2]) - minTableSize)) {
			temp = pValue[i];
			tempPivot = i;
		}

        // PBD
        /*if(temp2 > maxP[i] + bitType[i][2]) {
            temp2 = maxP[i] + bitType[i][2];
            pivot2 = i;
        }*/
    }
    //printf("Pivot bit (our) = %d\n", tempPivot);
    //printf("Pivot bit (PBD) = %d\n", pivot2);

    return tempPivot;

}

void Decomposition2(Table *headTable, int pivotBit, int targetIndex, int minTableIndex) { /// Move nodes to Tmin and delete nodes from Tmax

    Table *ptrTable = headTable;
    Table *targetTable;         /// Tmax
    Node *newAdd = NULL;

    int change;     /// bit value: Bmin
    int stay;       /// bit value: Bmax
    int one = 0;
    int zero = 0;
    int ruleNum = 0;

    while(ptrTable != NULL) {
        if (ptrTable->index == targetIndex) {
            targetTable = ptrTable;
            break;
        }
        ptrTable = ptrTable->nextTable;
    }

    Node *topNode = targetTable->child;
    while(topNode != NULL) {
        if(topNode->rule.ruleBinary[pivotBit] == 1) {
            one++;
        } else if(topNode->rule.ruleBinary[pivotBit] == 0){
            zero++;
        }
        topNode = topNode->nextNode;
    }

    change = (one < zero)? 1 : 0;
    stay = (one > zero)? 1 : 0;

    Node *frontNode = NULL;
    topNode = targetTable->child;

    while(topNode != NULL) {
        if(topNode->rule.ruleBinary[pivotBit] == change) {      /// Bmin
            /// Add change's node
            Node *n = CreateNode(topNode->rule);
            n->id = topNode->id;
            AddNextNode(&newAdd, n);
            ruleNum++;
            /// Remove node
            if(frontNode == NULL) {
                targetTable->child = topNode->nextNode;
                topNode = targetTable->child;
            } else {
                frontNode->nextNode = topNode->nextNode;
                topNode = frontNode->nextNode;
            }
        } else if(topNode->rule.ruleBinary[pivotBit] == 2) {    /// wild-card
            /// Add change's node
            Node *n = CreateNode(topNode->rule);
            n->id = topNode->id;
            n->rule.ruleBinary[pivotBit] = change;              /// Modify wild-card to change
            AddNextNode(&newAdd, n);
            ruleNum++;

            topNode->rule.ruleBinary[pivotBit] = stay;          /// Modify wild-card to stay
            frontNode = topNode;
            topNode = topNode->nextNode;
        } else {                                                /// Bmax
            frontNode = topNode;
            topNode = topNode->nextNode;
        }
    }

    TableAddNewNode2(&headTable, newAdd, ruleNum, minTableIndex);
    //printf("%d table ruleNum = %d\n", minTableIndex,  ruleNum);

    ruleNum = 0;    /// update rule number of Tmax
    topNode = targetTable->child;
    while(topNode != NULL) {
        ruleNum++;
        topNode = topNode->nextNode;
    }
    targetTable->ruleNum = ruleNum;
    //printf("Target ruleNum = %d\n", ruleNum);
}

int DoneDecomposition(Table *headTable, double averageTableSize, int *maxIndex, int *minIndex,
                      int *maxTableSize, int *minTableSize) { /// Check if SSP meets termination condition

    Table *ptr;
    ptr = headTable;
    *maxTableSize = ptr->ruleNum;
    *minTableSize = ptr->ruleNum;
    *maxIndex = 1;
    *minIndex = 1;

    while(ptr != NULL) {
        if(ptr->ruleNum > *maxTableSize) {
            *maxTableSize = ptr->ruleNum;
            *maxIndex = ptr->index;
        }

        if(ptr->ruleNum < *minTableSize) {
            *minTableSize = ptr->ruleNum;
            *minIndex = ptr->index;
        }
        ptr = ptr->nextTable;
    }

    if((*maxTableSize - *minTableSize) > averageTableSize * END_THRESHOLD ) {
        return 0;       /// not terminate
    } else {
        return 1;       /// terminate
    }
}

int TableStatistics(Table *headTable) { /// print each sub-tables information

    Table *ptr = headTable;
    int maxTable = ptr->ruleNum;
    int minTable = ptr->ruleNum;
    int i = 0;
    while(ptr != NULL) {
        if(ptr->ruleNum > maxTable) {
            maxTable = ptr->ruleNum;
        }

        if(ptr->ruleNum < minTable) {
            minTable = ptr->ruleNum;
        }

        printf("Table %d = %d.\n", ++i, ptr->ruleNum);
        ptr = ptr->nextTable;
    }

    printf("Max = %d, Min = %d, D = %d\n", maxTable, minTable, (maxTable - minTable));
    return (maxTable - minTable);

}

uint32_t parseIPV4string(char* ipAddress) { /// no use function
    uint32_t ipbytes[4];
    sscanf(ipAddress, "%u.%u.%u.%u",&ipbytes[3], &ipbytes[2],&ipbytes[1], &ipbytes[0]);
    return ipbytes[0] | ipbytes[1] << 8 | ipbytes[2] << 16 | ipbytes[3] << 24;
}

Node* LoadRuleFromFile(int *rule_sum, char filename[]) {

    FILE *fp;
    Node* headNode;
    char str[MAXLINE], *rule_parameters[5];
    char *pch;
    int i;
    int data_protocol;
    int protocol_mask;


    fp = fopen(filename ,"r");
    if(fp == NULL){
        printf("Error opening the file");
        exit(EXIT_FAILURE);
    }

    headNode = NULL;

    while(fgets(str, MAXLINE, fp)!=NULL ){  // only get 1 line a time
        pch = strtok(str,"\t");
        for(i = 0; i < 5; i++){                  // rule_parameter store the typical 5 tuples from ClassBench
            rule_parameters[i] = pch;
            if(rule_parameters[i] != NULL) {
                pch=strtok(NULL,"\t");
            }else {
                printf("Error in strtok procedure");
                exit(EXIT_FAILURE);
            }
        }
        Rule addRule;
        for (i = 0; i < 5; i++){
            switch (i){
                case 0:
                    /// source ip address and mask
                    rule_parameters[i]++;   // skip '@' character
                    pch = strtok(rule_parameters[i], "/");
                    //printf("%s\n" , pch);
                    if (pch != NULL){
                        uint32_t ip1, ip2, ip3, ip4;
                        sscanf(pch, "%u.%u.%u.%u", &ip1, &ip2, &ip3, &ip4);
                        addRule.srcIPBinary.p1 = ip1;
                        addRule.srcIPBinary.p2 = ip2;
                        addRule.srcIPBinary.p3 = ip3;
                        addRule.srcIPBinary.p4 = ip4;
                        addRule.src_addr_from = parseIPV4string(pch);
                        pch = strtok(NULL, "/");
                        if (pch != NULL){
                            addRule.src_mask = (uint32_t)strtol(pch, NULL, 10); // convert string to long in base 10
                        }
                    }
                    int range;
                    range = 32 - addRule.src_mask;
                    //addRule.src_addr_to = addRule.src_addr_from + (uint32_t)pow(2.0, range) - 1;
                    //printf("/%d %u~%u\n", addRule.src_mask, addRule.src_addr_from, addRule.src_addr_to);
                    break;

                case 1:
                    /// destination ip address and mask
                    //rule_parameters[i]++;   //skip '@' character
                    pch = strtok(rule_parameters[i], "/");
                    if (pch != NULL){
                        uint32_t d1, d2, d3, d4;
                        sscanf(pch, "%u.%u.%u.%u", &d1, &d2, &d3, &d4);
                        addRule.dstIPBinary.p1 = d1;
                        addRule.dstIPBinary.p2 = d2;
                        addRule.dstIPBinary.p3 = d3;
                        addRule.dstIPBinary.p4 = d4;
                        //printf("%d.%d.%d.%d", d1, d2, d3, d4);
                        //printf(" %d.%d.%d.%d\n", addRule.dstIPBinary.p1, addRule.dstIPBinary.p2, addRule.dstIPBinary.p3, addRule.dstIPBinary.p4);
                        addRule.dest_addr_from = parseIPV4string(pch);
                        //system("pause");
                        pch = strtok(NULL, "/");
                        if (pch != NULL){
                            addRule.dest_mask = (uint32_t)strtol(pch, NULL, 10); // convert string to long in base 10
                        }
                    }

                    range = 32 - addRule.dest_mask;
                    //addRule.dest_addr_to = addRule.dest_addr_from + (uint32_t)pow(2.0, range) - 1;
                    //printf("/%d %u~%u\n", addRule.dest_mask, addRule.dest_addr_from, addRule.dest_addr_to);
                    //system("pause");
                    /*Test Start*/
                    //printf("pow(2.0, %d) = (double)%f\n", range, pow(2.0, range));
                    //printf("pow(2.0, %d) = (uint32_t)%u\n", range, (uint32_t)pow(2.0, range)-1);
                    //printf("sizeof(int) = %d\n", sizeof(int));
                    /*Test End*/
                    break;

                case 2:
                    // source port number
                    pch = strtok(rule_parameters[i], " : ");
                    if (pch != NULL){
                        int src_port_from, src_port_to;
                        src_port_from = (uint16_t)strtol(pch, NULL, 10);
                        pch = strtok(NULL, " : ");
                        if (pch != NULL)
                            src_port_to = (uint16_t)strtol(pch, NULL, 10);

                        addRule.src_port_from = src_port_from;
                        addRule.src_port_to = src_port_to;
                        //printf("%d, %d ", src_port_from, src_port_to);

                        if(src_port_from != src_port_to && !(src_port_from ==0 && src_port_to==65535) && (src_port_to-src_port_from)>1
                           && !(src_port_from ==1024 && src_port_to==65535))
                        {
                            //printf("%d, %d\n", src_port_from, src_port_to);
                        }
                    }
                    break;

                case 3:
                    // destination port number
                    pch = strtok(rule_parameters[i], " : ");
                    if (pch != NULL){
                        int dest_port_from, dest_port_to;
                        dest_port_from = (uint16_t)strtol(pch, NULL, 10);
                        pch = strtok(NULL, " : ");
                        if (pch != NULL)
                            dest_port_to = (uint16_t)strtol(pch, NULL, 10);
                        //printf("%d, %d\n", dest_port_from, dest_port_to);
                        //system("pause");
                        addRule.dest_port_from = dest_port_from;
                        addRule.dest_port_to = dest_port_to;

                        if(dest_port_from != dest_port_to && !(dest_port_from ==0 && dest_port_to==65535) && (dest_port_to-dest_port_from)>1 && !(dest_port_from ==1024 && dest_port_to==65535))
                        {
                            //printf("%d, %d\n", dest_port_from, dest_port_to);
                        }
                    }
                    break;

                case 4:

                    pch = strtok(rule_parameters[i], "/");
                    if (pch != NULL){
                        data_protocol = (uint8_t)strtol(pch, NULL, 0);  // convert string with (0xabc...) form to long
                        addRule.data_protocol = data_protocol;
                        //printf("%d\n", data_protocol);
                        //system("pause");
                        pch = strtok(NULL, "/");
                        if (pch != NULL)
                            protocol_mask = (uint8_t)strtol(pch, NULL, 0);
                        addRule.protocol_mask = protocol_mask;
                        //printf("%d\n", protocol_mask);
                        //system("pause");
                    }
                    break;
            }
        }

        /// 1024 ~ 65535
        int port1[6] = {1024, 2048, 4096, 8192, 16384, 32768};
        int port2[6] = {2047, 4095, 8191, 16383, 32767, 65535};

        int port3[8] = {434, 436, 440, 448, 512, 640, 656, 660};
        int port4[8] = {435, 439, 447, 511, 639, 655, 659, 660};

        int port5[10] = {1025, 1026, 1028, 1032, 1040, 1056, 1088, 1152, 1280, 1536};
        int port6[10] = {1025, 1027, 1031, 1039, 1055, 1087, 1151, 1279, 1535, 2047};
        /// Add rule
        addRule.priority = ++(*rule_sum);
        if(addRule.src_port_from == 1024 && addRule.src_port_to == 65535) {
            if(addRule.dest_port_from == 1024 && addRule.dest_port_to == 65535) {
                int i, j;
                for (i = 0; i < 6; i++) {
                    for (j = 0; j < 6; j++) {
                        addRule.src_port_from = port1[i];
                        addRule.src_port_to = port2[i];
                        addRule.dest_port_from = port1[j];
                        addRule.dest_port_to = port2[j];
                        Node* addNode = CreateNode(addRule);
                        AddNextNode(&headNode, addNode);
                    }
                }
            } else if (addRule.dest_port_from == 33434 && addRule.dest_port_to == 33600) {
                 int i, j;
                for (i = 0; i < 6; i++) {
                    for (j = 0; j < 8; j++) {
                        addRule.src_port_from = port1[i];
                        addRule.src_port_to = port2[i];
                        addRule.dest_port_from = port3[j];
                        addRule.dest_port_to = port4[j];
                        Node* addNode = CreateNode(addRule);
                        AddNextNode(&headNode, addNode);
                    }
                }

            } else {
                int i;
                for (i = 0; i < 6; i++) {
                    addRule.src_port_from = port1[i];
                    addRule.src_port_to = port2[i];
                    Node* addNode = CreateNode(addRule);
                    AddNextNode(&headNode, addNode);
                }
            }
        } else if (addRule.src_port_from == 33434 && addRule.src_port_to == 33600) {
            if(addRule.dest_port_from == 1024 && addRule.dest_port_to == 65535) {
                 int i, j;
                for (i = 0; i < 8; i++) {
                    for (j = 0; j < 6; j++) {
                        addRule.src_port_from = port3[i];
                        addRule.src_port_to = port4[i];
                        addRule.dest_port_from = port1[j];
                        addRule.dest_port_to = port2[j];
                        Node* addNode = CreateNode(addRule);
                        AddNextNode(&headNode, addNode);
                    }
                }
            }

        } else if (addRule.dest_port_from == 1024 && addRule.dest_port_to == 65535) {
            int i;
            for (i = 0; i < 6; i++) {
                addRule.dest_port_from = port1[i];
                addRule.dest_port_to = port2[i];
                Node* addNode = CreateNode(addRule);
                AddNextNode(&headNode, addNode);
            }
        }  else if (addRule.dest_port_from == 1025 && addRule.dest_port_to == 65535) {
            int i;
            for (i = 0; i < 10; i++) {
                addRule.dest_port_from = port5[i];
                addRule.dest_port_to = port6[i];
                Node* addNode = CreateNode(addRule);
                AddNextNode(&headNode, addNode);
            }
        }  else {
            Node* addNode = CreateNode(addRule);
            AddNextNode(&headNode, addNode);
        }
    }

    return headNode;
}
