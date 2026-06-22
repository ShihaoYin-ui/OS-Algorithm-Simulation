#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

using namespace std;

class DiskSchedule {
protected:
    vector<int> seq;
    int head;
    double totalLen;
    double avgLen;
public:
    DiskSchedule(const vector<int>& s, int h)
        : seq(s), head(h), totalLen(0), avgLen(0) {
    }
    virtual void run() = 0;
    virtual ~DiskSchedule() {}
    double getAvg() const { return avgLen; }
    double getTotal() const { return totalLen; }
};

class FCFS : public DiskSchedule {
public:
    FCFS(const vector<int>& s, int h) : DiskSchedule(s, h) {}
    void run() override {
        cout << "\n您选择了FCFS算法，执行结果如下：\n";
        cout << left << setw(16) << "当前磁道号"
            << setw(16) << "下一磁道号"
            << "绝对差\n";
        int cur = head;
        for (int next : seq) {
            int diff = abs(next - cur);
            totalLen += diff;
            cout << left << setw(16) << cur
                << setw(16) << next
                << diff << "\n";
            cur = next;
        }
        avgLen = totalLen / seq.size();
        cout << "寻道序列总长" << seq.size()
            << "，FCFS算法总寻道长度：" << totalLen
            << "，平均寻道长度：" << fixed << setprecision(2) << avgLen << "\n";
    }
};

class SSTF : public DiskSchedule {
public:
    SSTF(const vector<int>& s, int h) : DiskSchedule(s, h) {}
    void run() override {
        cout << "\n您选择了SSTF算法，执行结果如下：\n";
        cout << left << setw(16) << "当前磁道号"
            << setw(16) << "下一磁道号"
            << "绝对差\n";
        vector<int> temp = seq;
        int cur = head;
        while (!temp.empty()) {
            int idx = 0, minDiff = abs(temp[0] - cur);
            for (int i = 1; i < temp.size(); ++i) {
                int d = abs(temp[i] - cur);
                if (d < minDiff) { minDiff = d; idx = i; }
            }
            int next = temp[idx];
            totalLen += minDiff;
            cout << left << setw(16) << cur
                << setw(16) << next
                << minDiff << "\n";
            cur = next;
            temp.erase(temp.begin() + idx);
        }
        avgLen = totalLen / seq.size();
        cout << "寻道序列总长" << seq.size()
            << "，SSTF算法总寻道长度：" << totalLen
            << "，平均寻道长度：" << fixed << setprecision(2) << avgLen << "\n";
    }
};

class SCAN : public DiskSchedule {
private:
    bool up;
public:
    SCAN(const vector<int>& s, int h, bool dir)
        : DiskSchedule(s, h), up(dir) {
    }
    void run() override {
        cout << "\n您选择了SCAN算法，当前磁头运动方向："
            << (up ? "由低到高" : "由高到低") << "，执行结果如下：\n";
        cout << left << setw(16) << "当前磁道号"
            << setw(16) << "下一磁道号"
            << "绝对差\n";
        vector<int> temp = seq;
        sort(temp.begin(), temp.end());
        int cur = head;
        vector<int> order;
        if (up) {
            auto it = upper_bound(temp.begin(), temp.end(), cur);
            for (; it != temp.end(); ++it) order.push_back(*it);
            for (auto it = temp.rbegin(); it != temp.rend(); ++it)
                if (*it < cur) order.push_back(*it);
        }
        else {
            auto it = lower_bound(temp.begin(), temp.end(), cur);
            for (auto r = temp.rbegin(); r != temp.rend(); ++r)
                if (*r >= cur) order.push_back(*r);
            for (; it != temp.end(); ++it)
                if (*it < cur) order.push_back(*it);
        }
        for (int next : order) {
            int diff = abs(next - cur);
            totalLen += diff;
            cout << left << setw(16) << cur
                << setw(16) << next
                << diff << "\n";
            cur = next;
        }
        avgLen = totalLen / seq.size();
        cout << "寻道序列总长" << seq.size()
            << "，SCAN算法总寻道长度：" << totalLen
            << "，平均寻道长度：" << fixed << setprecision(2) << avgLen << "\n";
    }
};

class CSCAN : public DiskSchedule {
private:
    bool up;
public:
    CSCAN(const vector<int>& s, int h, bool dir)
        : DiskSchedule(s, h), up(dir) {
    }
    void run() override {
        cout << "\n您选择了CSCAN算法，当前磁头运动方向："
            << (up ? "由低到高" : "由高到低") << "，执行结果如下：\n";
        cout << left << setw(16) << "当前磁道号"
            << setw(16) << "下一磁道号"
            << "绝对差\n";
        vector<int> temp = seq;
        sort(temp.begin(), temp.end());
        int cur = head;
        vector<int> order;
        if (up) {
            auto it = upper_bound(temp.begin(), temp.end(), cur);
            for (; it != temp.end(); ++it) order.push_back(*it);
            for (int x : temp)
                if (x <= cur) order.push_back(x);
        }
        else {
            auto it = lower_bound(temp.begin(), temp.end(), cur);
            for (auto r = temp.rbegin(); r != temp.rend(); ++r)
                if (*r >= cur) order.push_back(*r);
            for (; it != temp.end(); ++it)
                if (*it < cur) order.push_back(*it);
        }
        for (int next : order) {
            int diff = abs(next - cur);
            totalLen += diff;
            cout << left << setw(16) << cur
                << setw(16) << next
                << diff << "\n";
            cur = next;
        }
        avgLen = totalLen / seq.size();
        cout << "寻道序列总长" << seq.size()
            << "，CSCAN算法总寻道长度：" << totalLen
            << "，平均寻道长度：" << fixed << setprecision(2) << avgLen << "\n";
    }
};

int disk_inputInt(int low, int high, const string& tip) {
    int x;
    while (true) {
        cout << tip;
        cin >> x;
        if (x >= low && x <= high) break;
        cout << "您输入的数字超出范围，请重新输入" << low << "-" << high << "：\n";
    }
    return x;
}

vector<int> disk_randSeq(int len) {
    vector<int> res;
    srand(time(0));
    int last = -1;
    for (int i = 0; i < len; ++i) {
        int r;
        do {
            r = rand() % 201;
        } while (r == last && rand() % 3 == 0);
        res.push_back(r);
        last = r;
    }
    return res;
}

void printSeq(const vector<int>& seq) {
    cout << "磁盘访问序列为：";
    for (size_t i = 0; i < seq.size(); ++i) {
        if (i) cout << ",";
        cout << seq[i];
    }
    cout << "\n";
}

void disk_seek_main() {
    srand(time(0));
    while (true) {
        int len = disk_inputInt(10, 20, "请输入磁盘寻道序列长度（10-20）：");
        vector<int> seq = disk_randSeq(len);
        int head = rand() % 201;
        printSeq(seq);

        while (true) {
            int alg = disk_inputInt(1, 4, "请选择算法：1FCFS 2SSTF 3SCAN 4CSCAN：");
            DiskSchedule* p = nullptr;
            if (alg == 1) {
                p = new FCFS(seq, head);
            }
            else if (alg == 2) {
                p = new SSTF(seq, head);
            }
            else if (alg == 3 || alg == 4) {
                int dir = disk_inputInt(1, 2, "请选择磁头方向：1由低到高 2由高到低：");
                bool up = (dir == 1);
                if (alg == 3) p = new SCAN(seq, head, up);
                else p = new CSCAN(seq, head, up);
            }
            p->run();
            delete p;

            int op = disk_inputInt(1, 3, "\n请选择：1继续换算法 2更换数据 3结束程序：");
            if (op == 2 || op == 3) {
                if (op == 3) cout << "程序结束！\n";
                break;
            }
        }
        if (disk_inputInt(1, 2, "\n是否重新开始？1是 2否：") == 2) break;
    }
}