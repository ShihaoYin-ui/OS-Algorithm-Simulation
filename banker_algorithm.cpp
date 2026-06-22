#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

const int PROCESS_NUM = 5;      
const int RESOURCE_NUM = 3;     

int Max[PROCESS_NUM][RESOURCE_NUM] = {
    {7, 5, 3},  
    {3, 2, 2},  
    {9, 0, 2},  
    {2, 2, 2},  
    {4, 3, 3}   
};

int Allocation[PROCESS_NUM][RESOURCE_NUM] = {
    {0, 1, 0},  
    {2, 0, 0},  
    {3, 0, 2},  
    {2, 1, 1},  
    {0, 0, 2}   
};

int Need[PROCESS_NUM][RESOURCE_NUM];        
int Available[RESOURCE_NUM] = {3, 3, 2};    

int stepCount = 0;  

void initNeed();
void showAvailable();
void printSecurityTable(const vector<int>& safeSeq, const vector<int*>& workHistory);
bool isSafe(const int available[], const int need[][RESOURCE_NUM],
            const int allocation[][RESOURCE_NUM],
            vector<int>& safeSeq, vector<int*>& workHistory);
void showT0Security();
void resetToT0();
bool requestResource(int pid, int request[]);

void banker_algorithm_main() {
    initNeed();                 
    cout << "========== 银行家算法模拟 ==========\n";
    showT0Security();           

    int pid;
    int request[RESOURCE_NUM];

    while (true) {
        cout << "\n----------------------------------------\n";
        cout << "当前可用资源: ";
        showAvailable();
        cout << "请输入请求资源的进程号(0-4)，输入 -1 退出程序，输入 -2 重置到T0状态: ";
        cin >> pid;

        if (pid == -1) {
            cout << "程序结束。\n";
            break;
        } else if (pid == -2) {
            resetToT0();
            continue;
        }

        if (pid < 0 || pid >= PROCESS_NUM) {
            cout << "进程号无效，请输入 0-4 之间的数字！\n";
            continue;
        }

        cout << "请输入进程 P" << pid << " 请求的 " << RESOURCE_NUM << " 种资源数量 (例如: 1 0 2): ";
        for (int i = 0; i < RESOURCE_NUM; i++) {
            cin >> request[i];
        }

        requestResource(pid, request);
    }
}

void initNeed() {
    for (int i = 0; i < PROCESS_NUM; i++) {
        for (int j = 0; j < RESOURCE_NUM; j++) {
            Need[i][j] = Max[i][j] - Allocation[i][j];
        }
    }
}

void showAvailable() {
    cout << "(";
    for (int i = 0; i < RESOURCE_NUM; i++) {
        cout << Available[i];
        if (i != RESOURCE_NUM - 1) cout << ", ";
    }
    cout << ")\n";
}

void printSecurityTable(const vector<int>& safeSeq, const vector<int*>& workHistory) {
    cout << left << setw(12) << "进程"
         << setw(18) << "WORK"
         << setw(18) << "NEED"
         << setw(18) << "ALLO"
         << setw(18) << "WORK+ALLO"
         << "FINISH" << endl;

    for (size_t idx = 0; idx < safeSeq.size(); idx++) {
        int p = safeSeq[idx];
        int* work = workHistory[idx];

        cout << "P" << p << setw(10) << " ";

        cout << "(";
        for (int j = 0; j < RESOURCE_NUM; j++) {
            cout << work[j];
            if (j != RESOURCE_NUM - 1) cout << " ";
        }
        cout << ")" << setw(12) << " ";

        cout << "(";
        for (int j = 0; j < RESOURCE_NUM; j++) {
            cout << Need[p][j];
            if (j != RESOURCE_NUM - 1) cout << " ";
        }
        cout << ")" << setw(12) << " ";

        cout << "(";
        for (int j = 0; j < RESOURCE_NUM; j++) {
            cout << Allocation[p][j];
            if (j != RESOURCE_NUM - 1) cout << " ";
        }
        cout << ")" << setw(12) << " ";

        cout << "(";
        for (int j = 0; j < RESOURCE_NUM; j++) {
            cout << work[j] + Allocation[p][j];
            if (j != RESOURCE_NUM - 1) cout << " ";
        }
        cout << ")" << setw(12) << " ";

        cout << "TRUE" << endl;
    }
}

bool isSafe(const int available[], const int need[][RESOURCE_NUM],
            const int allocation[][RESOURCE_NUM],
            vector<int>& safeSeq, vector<int*>& workHistory) {
    int work[RESOURCE_NUM];
    for (int i = 0; i < RESOURCE_NUM; i++) {
        work[i] = available[i];
    }

    bool finish[PROCESS_NUM] = {false};
    safeSeq.clear();
    workHistory.clear();

    int count = 0;
    while (count < PROCESS_NUM) {
        bool found = false;
        for (int i = 0; i < PROCESS_NUM; i++) {
            if (!finish[i]) {
                bool canAlloc = true;
                for (int j = 0; j < RESOURCE_NUM; j++) {
                    if (need[i][j] > work[j]) {
                        canAlloc = false;
                        break;
                    }
                }
                if (canAlloc) {
                    int* workCopy = new int[RESOURCE_NUM];
                    for (int j = 0; j < RESOURCE_NUM; j++) {
                        workCopy[j] = work[j];
                    }
                    workHistory.push_back(workCopy);

                    for (int j = 0; j < RESOURCE_NUM; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSeq.push_back(i);
                    found = true;
                    count++;
                    break;
                }
            }
        }
        if (!found) {
            for (size_t i = 0; i < workHistory.size(); i++) {
                delete[] workHistory[i];
            }
            workHistory.clear();
            return false;
        }
    }
    return true;
}

void showT0Security() {
    stepCount = 0;
    vector<int> safeSeq;
    vector<int*> workHistory;

    cout << "\nT0时刻安全性检测中。。。\n";
    bool safe = isSafe(Available, Need, Allocation, safeSeq, workHistory);

    if (safe) {
        printSecurityTable(safeSeq, workHistory);
        cout << "T0时刻安全，存在安全序列: ";
        for (size_t i = 0; i < safeSeq.size(); i++) {
            cout << "P" << safeSeq[i];
            if (i != safeSeq.size() - 1) cout << " -> ";
        }
        cout << endl;
    } else {
        cout << "T0时刻不安全！(这不应发生，请检查初始数据)" << endl;
    }

    for (size_t i = 0; i < workHistory.size(); i++) {
        delete[] workHistory[i];
    }
}

void resetToT0() {
    int initAlloc[PROCESS_NUM][RESOURCE_NUM] = {
        {0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}
    };
    memcpy(Allocation, initAlloc, sizeof(Allocation));

    Available[0] = 3; Available[1] = 3; Available[2] = 2;

    initNeed();

    stepCount = 0;
    cout << "\n***** 系统已重置到T0状态 *****\n";
    showT0Security();
}

bool requestResource(int pid, int request[]) {
    for (int i = 0; i < RESOURCE_NUM; i++) {
        if (request[i] > Need[pid][i]) {
            cout << "您的要求超出了需要范围，导致ERROR，请重新输入！\n";
            return false;
        }
    }

    for (int i = 0; i < RESOURCE_NUM; i++) {
        if (request[i] > Available[i]) {
            cout << "您的要求暂时无法满足，导致BLOCK，请重新输入~\n";
            return false;
        }
    }

    int newAvailable[RESOURCE_NUM];
    int newAllocation[PROCESS_NUM][RESOURCE_NUM];
    int newNeed[PROCESS_NUM][RESOURCE_NUM];

    for (int i = 0; i < RESOURCE_NUM; i++) {
        newAvailable[i] = Available[i];
    }
    for (int i = 0; i < PROCESS_NUM; i++) {
        for (int j = 0; j < RESOURCE_NUM; j++) {
            newAllocation[i][j] = Allocation[i][j];
            newNeed[i][j] = Need[i][j];
        }
    }

    for (int i = 0; i < RESOURCE_NUM; i++) {
        newAvailable[i] -= request[i];
        newAllocation[pid][i] += request[i];
        newNeed[pid][i] -= request[i];
    }

    vector<int> safeSeq;
    vector<int*> workHistory;
    bool safe = isSafe(newAvailable, newNeed, newAllocation, safeSeq, workHistory);

    if (!safe) {
        cout << "未找到安全序列，继续分配将导致系统进入不安全状态，进而可能引发死锁，不予分配资源！！\n";
        for (size_t i = 0; i < workHistory.size(); i++) {
            delete[] workHistory[i];
        }
        return false;
    }

    for (int i = 0; i < RESOURCE_NUM; i++) {
        Available[i] = newAvailable[i];
    }
    for (int i = 0; i < PROCESS_NUM; i++) {
        for (int j = 0; j < RESOURCE_NUM; j++) {
            Allocation[i][j] = newAllocation[i][j];
            Need[i][j] = newNeed[i][j];
        }
    }

    stepCount++;
    cout << "\n************ 分配成功 ************\n";
    cout << "T" << stepCount << "时刻安全性分析：\n";
    printSecurityTable(safeSeq, workHistory);
    cout << "T" << stepCount << "时刻安全，存在安全序列: ";
    for (size_t i = 0; i < safeSeq.size(); i++) {
        cout << "P" << safeSeq[i];
        if (i != safeSeq.size() - 1) cout << " -> ";
    }
    cout << endl;

    for (size_t i = 0; i < workHistory.size(); i++) {
        delete[] workHistory[i];
    }

    return true;
}