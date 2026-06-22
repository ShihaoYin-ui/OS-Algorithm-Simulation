#include<iostream>
#include<vector>
#include<cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

typedef struct PageTable{
    int page_number;            
    int block_number;           
} PageTable;
int page_size;
int page_table_size;

typedef struct SegmentTable{
    int segment_number;         
    int base_address;           
    int limit;                  
} SegmentTable;
int segment_table_size;

void method_select();
void page_shortage();
void segment_shortage();
void print_method(int choice);
vector<PageTable> page_table;
vector<SegmentTable> segment_table;

void method_select(){
    cout << "请输入要选择的地址变化方式(0-分页;1-分段):" ;
    int choice;
    while (cin >> choice){
        if(choice==0){
            page_shortage();
            break;
        }
        else if(choice==1){
            segment_shortage();
            break;
        }
        else{
            cout << "输入无效，请重新输入(0-分页;1-分段):" ;
        }
    }
    print_method(choice);
    if(choice==0){
        int logic_addr, num, offset;
        int real_addr;
        cout<<"请输入逻辑地址:";
        while(cin>>logic_addr){
            num = logic_addr / page_size;
            offset = logic_addr % page_size;
            if(num>=page_table.size()){
                cout << "逻辑地址溢出，请重新输入" << endl;
                continue;
            }
            else{
                real_addr = page_table[num].block_number * page_size + offset;
                break;
            }
        }
        cout << "您选择了分页方式,输入的逻辑地址为" << logic_addr << ",对应的页号为" << num << ",页内偏移为" << offset << ";" << endl
             << "该地址对应的块号为" << page_table[num].block_number << ",块内偏移为" << offset << ",实际物理地址为" << real_addr << endl;
    }
    else{
        int seg, offset;
        int real_addr;
        cout<<"您选择了分段方式,请输入逻辑地址的段号和段内偏移:" << endl;
        while(cin>>seg>>offset){
            if(seg>=segment_table_size){
                cout << "段号超出范围,请重新输入" << endl;
                continue;
            }
            if(offset>=segment_table[seg].limit){
                cout << "段内偏移超出范围,请重新输入" << endl;
                continue;
            }
            break;
        }
        real_addr = segment_table[seg].base_address + offset;
        cout << "您选择了分段方式,输入的逻辑地址为" << seg<<","<<offset << ",其对应的段号为" << seg << ",段内偏移为" << offset << ";"<<endl
             <<"该段对应的基址为"<< segment_table[seg].base_address << ",段内偏移为" << offset << ",实际物理地址为" << real_addr << endl;
    }
    cout<<"是否继续输入? (Y/N)" << endl;
    char ch;
    cin>>ch;
    if(ch=='N'||ch=='n'){
        return;
    }
    else{
        page_table.clear();
        segment_table.clear();
        method_select();
    }
}

void page_shortage(){
    cout << "请输入每页大小:(0-1K;1-2K;2-4K)";
    while(cin>>page_size){
        if(page_size==0){
            page_size = 1024;
            break;
        }
        else if(page_size==1){
            page_size = 2048;
            break;
        }
        else if( page_size==2){
            page_size = 4096;
            break;
        }
        else{
            cout << "输入无效，请重新输入(0-1K;1-2K;2-4K):" ;
        }
    }
    cout << "请输入页表项个数(5到10之间整数,含端点):";
    while(cin>>page_table_size){
        if(page_table_size>=5 && page_table_size<=10){
            break;
        }
        else{
            cout << "输入无效，请重新输入(5到10之间整数,含端点):";
        }
    }
    vector<int> p(11, -1);
    cout << "请输入页表中每页对应的块号(0-手动输入;1-随机生成0至10之间的非重复整数作为块号):";
    int block;
    while (cin >> block){
        PageTable page;
        if(block==0){
            cout << "从第0号页面开始输入相应块号,块号不能重复,用空格间隔: ";
            for (int i = 0; i < page_table_size;i++){
                page.page_number = i;
                while(cin>>page.block_number){
                    if(page.block_number>10||page.block_number<0){
                        cout << "您输入的块号超出范围，请重新输入页面"<<page.page_number<<"对应的块号: ";
                    }
                    else if(p[page.block_number]==-1){
                        p[page.block_number] = page.page_number;
                        break;
                    }
                    else{
                        cout << "您输入的块号与页面"<<p[page.block_number]<<"重复，请重新输入页面"<<page.page_number<<"对应的块号: ";
                    }
                }
                page_table.push_back(page);
            }
            break;
        }
        else if (block==1){
            vector<bool> used(11, false);
            for (int i = 0; i < page_table_size; i++){
                unsigned rd=rand()%11;
                if(used[rd]){
                    i--;
                }
                else{
                    used[rd]=true;
                    page.page_number=i;
                    page.block_number=rd;
                    page_table.push_back(page);
                }
            }
            break;
        }
        else{
            cout << "输入错误，请重新输入:" ;
        }
    }
    cout << "您选择了分页方式,输入的数据如下:" << endl;
    cout << "每页大小为" << page_size << "页表为" << endl;
}

void segment_shortage(){
    cout << "请输入段表项个数(5到10之间整数,含端点):";
    cin >> segment_table_size;
    cout << "请输入页表中每段对应的基址和段长(0-手动输入;1-随机生成):";
    int choice;
    while(cin>>choice){
        if(choice==0){
            SegmentTable seg;
            vector<bool> used_base(101, false);
            for (int i = 0; i < segment_table_size;i++){
                seg.segment_number = i;
                cout << "基址从1K至100K,K的整数倍手动输入,不得重复(仅需输入数字即可):";
                int base;
                while(cin>>base){
                    if(base<1 || base>100){
                        cout << "基址超出范围，请重新输入:";
                    }
                    else if(used_base[base]){
                        cout << "基址重复，请重新输入:";
                    }
                    else{
                        used_base[base] = true;
                        seg.base_address = base*1024;
                        break;
                    }
                }
                int size;
                cout << "段长从1K,2K,4K,8K中随机生成或手动输入(仅需输入数字即可):";
                while(cin>>size){
                    if(size==1 || size==2 || size==4 || size==8){
                        seg.limit = size*1024;
                        break;
                    }
                    else{
                        cout << "数据有误请重新输入：";
                    }
                }
                segment_table.push_back(seg);
            }
            break;
        }
        else if(choice==1){
            SegmentTable seg;
            vector<bool> used_base(101, false);
            for (int i = 0;i<segment_table_size;i++){
                seg.segment_number = i;
                unsigned bs;
                while(true){
                    bs = rand() % 101;
                    if(!used_base[bs]){
                        used_base[bs] = true;
                        seg.base_address = bs*1024;
                        break;
                    }
                }
                unsigned limit=pow(2,rand()%4);
                seg.limit = limit*1024;
                segment_table.push_back(seg);
            }
            break;
        }
        else{
            cout << "输入无效，请重新输入(0-手动输入;1-随机生成):";
        }
    }
    cout << "您选择了分段方式,输入的数据如下" << endl;
}

void print_method(int choice){
    if(choice==0){
        cout << left << setw(8) << "页号" << setw(8) << "块号" << endl;
        for (size_t i = 0; i < page_table.size();i++){
            cout << left
                 << setw(8) << page_table[i].page_number
                 << setw(8) << page_table[i].block_number
                 << endl;
        }
    }
    else if(choice==1){
        cout << left << setw(8) << "段号" << setw(8) << "基址" << setw(8) << "限长" << endl;
        for (size_t i = 0; i < segment_table.size();i++){
            cout << left
                 << setw(8) << segment_table[i].segment_number
                 << setw(4) << segment_table[i].base_address / 1024
                 << setw(4) << "K"
                 << setw(8) << segment_table[i].limit / 1024 << "K"
                 << endl;
        }
    }
}

void address_translation_main(){
    method_select();
}