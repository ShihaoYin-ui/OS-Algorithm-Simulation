#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#include<algorithm>
#include<queue>
#include<iomanip>
using namespace std;

typedef struct PCB{
    string name;
    int arrival_time = 0;
    int service_time = 0;
    int start_time = 0;
    int finish_time = 0;
    int turn_around_time = 0;
    double weighted_turn_around_time = 0;
    int static_priority = 1;
}PCB;

vector<PCB> proce(5);
vector<PCB> roundQueue;

void initProcess_auto();
void initProcess_manual();
void printMainMenu();
void mainMenu();
void FCFS();
void SJF();
void Priority();
void PPS();
void NPPS();
void RR();
void printByTurnaroundTime();
bool isAll_0(vector<int>&temp);

void initProcess_auto(){
    proce[0].name = "A";
    proce[0].arrival_time = 0;
    proce[0].service_time = 4;
    proce[0].static_priority = 4;
    proce[1].name="B";
    proce[1].arrival_time=1;
    proce[1].service_time=3;
    proce[1].static_priority=2;
    proce[2].name="C";
    proce[2].arrival_time=2;
    proce[2].service_time=5;
    proce[2].static_priority=3;
    proce[3].name="D";
    proce[3].arrival_time=3;
    proce[3].service_time=2;
    proce[3].static_priority=5;
    proce[4].name="E";
    proce[4].arrival_time=4;
    proce[4].service_time=4;
    proce[4].static_priority=1;
}

void initProcess_manual(){
    cout<<"依次输入五个进程的进程名、到达时间、服务时间、静态优先级（以空格间断，进程名不得多于八个字节）"<<endl;
    for (int i = 0; i < 5; i++){
        string name;
        int arrive_time, service_time, static_priority;
        cin>>name>>arrive_time>>service_time>>static_priority;
        proce[i].name=name;
        proce[i].arrival_time=arrive_time;
        proce[i].service_time=service_time;
        proce[i].static_priority=static_priority;
    }
}

void ps_printMainMenu(){
    cout << "====进程调度算法模拟====" << endl;
    cout << "    0. 退出" << endl;
    cout << "    1. 先来先服务(FCFS)" << endl;
    cout << "    2. 短作业优先(SJF)" << endl;
    cout << "    3. 优先权算法(Priority)" << endl;
    cout << "    4. 时间片轮转算法(RR)" << endl;
    cout << "请输入你的选择: ";
}

void ps_mainMenu(){
    int choice;
    while(1){
        ps_printMainMenu();
        cin >> choice;
        if(choice==0){
            cout << "程序结束，已退出!" << endl;
            return;
        }
        cout << "初始化数据(0.自动/1.手动): ";
        int temp;
        cin >> temp;
        if(temp == 1){
            if(choice!=3){
                cout << "静态优先权可随意输入" << endl;
            }
            initProcess_manual();
        }
        else if(temp==0){
            initProcess_auto();
        }
        switch(choice){
            case 1:
                FCFS();
                break;
            case 2:
                SJF();
                break;
            case 3:
                Priority();
                break;
            case 4:
                RR();
                break;
            default:
                cout << "无效的选择，请重新输入" << endl;
                break;
        }
        
    }
}

void FCFS(){
    cout << "你选择的是：先来先服务(FCFS)调度算法" << endl;
    roundQueue.clear();
    sort(proce.begin(), proce.end(),
         [](const PCB &a, const PCB &b)
         {
             return a.arrival_time < b.arrival_time;
         });
    int current_time = 0;
    for (size_t i = 0; i < proce.size();i++){
        if(current_time<proce[i].arrival_time){
            current_time = proce[i].arrival_time;
        }
        proce[i].start_time = current_time;
        current_time += proce[i].service_time;
        proce[i].finish_time = current_time;
        proce[i].turn_around_time = proce[i].finish_time-proce[i].arrival_time;
        proce[i].weighted_turn_around_time=(double)proce[i].turn_around_time / proce[i].service_time;
        roundQueue.push_back(proce[i]);
    }
    printByTurnaroundTime();
}

void SJF(){
    cout << "你选择的是：最短作业优先(SJF)调度算法" << endl;
    roundQueue.clear();
    vector<PCB> temp(proce);
    int current_time = 0;
    while(roundQueue.size() < proce.size()){
        size_t selected = SIZE_MAX;
        int shortest_service_time = INT_MAX;
        for (size_t i = 0; i < temp.size(); i++){
            if(current_time >= temp[i].arrival_time && temp[i].service_time < shortest_service_time){
                shortest_service_time = temp[i].service_time;
                selected = i;
            }
        }
        if(selected != SIZE_MAX){
            temp[selected].start_time = current_time;
            current_time += temp[selected].service_time;
            temp[selected].finish_time = current_time;
            temp[selected].turn_around_time = temp[selected].finish_time - temp[selected].arrival_time;
            temp[selected].weighted_turn_around_time = (double)temp[selected].turn_around_time / temp[selected].service_time;
            roundQueue.push_back(temp[selected]);
            temp.erase(temp.begin() + selected);
        }
        else{
            current_time++;
        }
    }
    for(size_t i = 0; i < roundQueue.size(); i++){
        proce[i] = roundQueue[i];
    }
    printByTurnaroundTime();
}

void Priority(){
    int snatch;
    cout << "是否为抢占式: 0-非抢占优先权算法; 1-抢占优先权算法" << endl;
    cin >> snatch;
    if(snatch == 1){
        PPS();
    }
    else{
        NPPS();
    }
}

bool isAll_0(vector<int>&temp){
    for(size_t i = 0; i < temp.size(); i++){
        if(temp[i] != 0){
            return false;
        }
    }
    return true;
}

void PPS(){
    cout << "你选择的是：抢占式优先权算法" << endl;
    roundQueue.clear();
    int current_time = 0;
    vector<int> temp;
    vector<int> start(proce.size(),-1);
    int sum_time = 0;
    for (size_t i = 0;i<proce.size();i++){
        temp.push_back(proce[i].service_time);
        sum_time += proce[i].service_time;
    }
    while(!isAll_0(temp)){
        int min_priority = INT_MAX;
        size_t selected = SIZE_MAX;
        for (size_t i = 0;i<proce.size();i++){
            if(temp[i] != 0 && proce[i].static_priority < min_priority&&current_time>=proce[i].arrival_time){
                min_priority = proce[i].static_priority;
                selected = i;
            }
        }
        if(selected != SIZE_MAX){
            if(start[selected] == -1){
                proce[selected].start_time = current_time;
                start[selected] = current_time;
            }
            temp[selected]--;
            current_time++;
            roundQueue.push_back(proce[selected]);
            if(temp[selected]==0){
                proce[selected].finish_time = current_time;
                proce[selected].turn_around_time = proce[selected].finish_time - proce[selected].arrival_time;
                proce[selected].weighted_turn_around_time = (double)proce[selected].turn_around_time / proce[selected].service_time;
            }
        }
        else{
            current_time++;
        }
    }
    printByTurnaroundTime();
}

void NPPS(){
    cout << "你选择的是：非抢占式优先权算法" << endl;
    roundQueue.clear();
    vector<PCB>temp(proce);
    int current_time = 0;
    while(roundQueue.size()<proce.size()){
        int min_priority = INT_MAX;
        size_t selected = SIZE_MAX;
        for (size_t i = 0;i<temp.size();i++){
            if(temp[i].arrival_time <= current_time && temp[i].static_priority < min_priority){
                min_priority = temp[i].static_priority;
                selected = i;
            }
        }
        if(selected != SIZE_MAX){
            temp[selected].start_time = current_time;
            current_time += temp[selected].service_time;
            temp[selected].finish_time = current_time;
            temp[selected].static_priority = INT_MAX;
            temp[selected].turn_around_time = temp[selected].finish_time - temp[selected].arrival_time;
            temp[selected].weighted_turn_around_time = (double)temp[selected].turn_around_time / temp[selected].service_time;
            roundQueue.push_back(temp[selected]);
            temp.erase(temp.begin() + selected);
        } else {
            current_time++;
        }
    }
    for(size_t i = 0; i < roundQueue.size(); i++){
        proce[i] = roundQueue[i];
    }
    printByTurnaroundTime();
}

void RR(){
    cout << "你选择的是：时间片轮转调度算法" << endl;
    int time_slice;
    cout << "请输入时间片大小(1~10): ";
    cin >> time_slice;
    roundQueue.clear();
    vector<int> remaining_time(proce.size());
    vector<bool> first_run(proce.size(), true);
    queue<size_t> ready_queue;
    
    int current_time = 0;
    size_t next_arrival_index = 0;
    
    sort(proce.begin(), proce.end(), [](const PCB &a, const PCB &b){
        return a.arrival_time < b.arrival_time;
    });
    
    for(size_t i = 0; i < proce.size(); i++){
        remaining_time[i] = proce[i].service_time;
    }
    
    while(next_arrival_index < proce.size() && proce[next_arrival_index].arrival_time <= current_time){
        ready_queue.push(next_arrival_index);
        next_arrival_index++;
    }
    
    while(!isAll_0(remaining_time)){
        if(ready_queue.empty()){
            if(next_arrival_index < proce.size()){
                current_time = proce[next_arrival_index].arrival_time;
                ready_queue.push(next_arrival_index);
                next_arrival_index++;
            }
            else{
                current_time++;
            }
            continue;
        }
        size_t current_process = ready_queue.front();
        if(first_run[current_process]){
            first_run[current_process] = false;
            proce[current_process].start_time = current_time;
        }
        ready_queue.pop();
        if(remaining_time[current_process] > time_slice){
            current_time += time_slice;
            while(next_arrival_index < proce.size() && proce[next_arrival_index].arrival_time <= current_time){
                ready_queue.push(next_arrival_index);
                next_arrival_index++;
            }
            remaining_time[current_process] -= time_slice;
            ready_queue.push(current_process);
        }
        else{
            current_time += remaining_time[current_process];
            while(next_arrival_index < proce.size() && proce[next_arrival_index].arrival_time <= current_time){
                ready_queue.push(next_arrival_index);
                next_arrival_index++;
            }
            remaining_time[current_process] = 0;
            proce[current_process].finish_time = current_time;
            proce[current_process].turn_around_time = proce[current_process].finish_time - proce[current_process].arrival_time;
            proce[current_process].weighted_turn_around_time = (double)proce[current_process].turn_around_time / proce[current_process].service_time;
        }
        roundQueue.push_back(proce[current_process]);
    }
    
    printByTurnaroundTime();
}

void printByTurnaroundTime(){
    sort(proce.begin(), proce.end(), [](const PCB &a, const PCB &b)
         { return a.weighted_turn_around_time < b.weighted_turn_around_time; });
    cout << left
         << setw(12) << "Process"
         << setw(10) << "Arrive"
         << setw(10) << "Service"
         << setw(10) << "Start"
         << setw(10) << "Finish"
         << setw(12) << "TurnAround"
         << setw(16) << "WeightedTA"
         << endl;
         int sum_turn_around_time = 0;
         double sum_weighted_turn_around_time = 0;
    for(size_t i = 0; i < proce.size(); ++i){
        cout << left
             << setw(12) << proce[i].name
             << setw(10) << proce[i].arrival_time
             << setw(10) << proce[i].service_time
             << setw(10) << proce[i].start_time
             << setw(10) << proce[i].finish_time
             << setw(12) << proce[i].turn_around_time
             << setw(16) << proce[i].weighted_turn_around_time
             << endl;
        sum_turn_around_time += proce[i].turn_around_time;
        sum_weighted_turn_around_time += proce[i].weighted_turn_around_time;
    }
    cout << left
         << setw(12 + 10 + 10 + 10 + 10) << "Average"
         << setw(12) << (double)sum_turn_around_time / proce.size()
         << setw(16) << (double)sum_weighted_turn_around_time / proce.size()
         << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "轮转顺序：";
    for (size_t i = 0;i<roundQueue.size()-1;i++){
        cout << roundQueue[i].name << "-";
    }
    cout<<roundQueue[roundQueue.size()-1].name<<endl;
    cout << endl
         << endl;
}

void process_scheduling_main(){
    ps_mainMenu();
}