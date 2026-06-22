#include <iostream>
#include "address_translation.h"
#include "banker_algorithm.h"
#include "disk_seek.h"
#include "page_request.h"
#include "process_scheduling.h"

using namespace std;

void printMainMenu() {
    cout << "\n";
    cout << "╔═══════════════════════════════════════════════════════════════\n";
    cout << "║                      操作系统课程设计                          \n";
    cout << "╠═══════════════════════════════════════════════════════════════\n";
    cout << "║  1. 地址转换（分页/分段）                                       \n";
    cout << "║  2. 银行家算法（死锁避免）                                      \n";
    cout << "║  3. 磁盘调度算法（FCFS/SSTF/SCAN/CSCAN）                     \n";
    cout << "║  4. 页面置换算法（OPT/FIFO/LRU/CLOCK）                       \n";
    cout << "║  5. 进程调度算法（FCFS/SJF/Priority/RR）                    \n";
    cout << "║  0. 退出程序                                                  \n";
    cout << "╚════════════════════════════════════════════════════════════════\n";
    cout << "请输入您的选择（0-5）：";
}

int main() {
    int choice;
    while (true) {
        printMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                system("cls");
                address_translation_main();
                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");
                banker_algorithm_main();
                system("pause");
                system("cls");
                break;
            case 3:
                system("cls");
                disk_seek_main();
                system("pause");
                system("cls");
                break;
            case 4:
                system("cls");
                page_request_main();
                system("pause");
                system("cls");
                break;
            case 5:
                system("cls");
                process_scheduling_main();
                system("pause");
                system("cls");
                break;
            case 0:
                cout << "\n感谢使用！程序已退出。\n";
                return 0;
            default:
                cout << "\n输入无效，请重新输入！\n";
                system("pause");
                system("cls");
                break;
        }
    }
}