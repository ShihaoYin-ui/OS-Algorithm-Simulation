#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

class PageReplacement {
protected:
    vector<int> seq;
    int blockNum;
    vector<int> blocks;
    int faultCnt;
public:
    PageReplacement(const vector<int>& s, int n)
        : seq(s), blockNum(n), faultCnt(0) {
        blocks.assign(blockNum, -1);
    }
    virtual void run() = 0;
    virtual ~PageReplacement() {}
    int getFault() const { return faultCnt; }
    int getLen() const { return seq.size(); }
    double getRate() const {
        return 100.0 * faultCnt / seq.size();
    }
};

class OPT : public PageReplacement {
public:
    OPT(const vector<int>& s, int n) : PageReplacement(s, n) {}
    void run() override {
        cout << "\n您选择了OPT算法，执行结果如下：\n";
        cout << left << setw(12) << "访问页面"
            << setw(24) << "物理块"
            << "缺页中断\n";
        for (int i = 0; i < seq.size(); ++i) {
            int page = seq[i];
            bool hit = false;
            for (int b : blocks) if (b == page) { hit = true; break; }
            if (!hit) {
                faultCnt++;
                int idx = -1, farthest = i;
                for (int j = 0; j < blockNum; ++j) {
                    if (blocks[j] == -1) { idx = j; break; }
                    int pos = seq.size();
                    for (int k = i + 1; k < seq.size(); ++k)
                        if (seq[k] == blocks[j]) { pos = k; break; }
                    if (pos > farthest) { farthest = pos; idx = j; }
                }
                blocks[idx] = page;
            }
            cout << left << setw(12) << page;
            for (int b : blocks) cout << setw(8) << (b == -1 ? "&" : to_string(b));
            cout << (hit ? "F" : "T") << "\n";
        }
        cout << "页面访问序列总长" << getLen()
            << "，OPT算法共发生缺页中断" << getFault()
            << "次，缺页率为" << fixed << setprecision(2) << getRate() << "%\n";
    }
};

class FIFO : public PageReplacement {
    int ptr;
public:
    FIFO(const vector<int>& s, int n) : PageReplacement(s, n), ptr(0) {}
    void run() override {
        cout << "\n您选择了FIFO算法，执行结果如下：\n";
        cout << left << setw(12) << "访问页面"
            << setw(24) << "物理块"
            << "缺页中断\n";
        for (int page : seq) {
            bool hit = false;
            for (int b : blocks) if (b == page) { hit = true; break; }
            if (!hit) {
                faultCnt++;
                blocks[ptr] = page;
                ptr = (ptr + 1) % blockNum;
            }
            cout << left << setw(12) << page;
            for (int b : blocks) cout << setw(8) << (b == -1 ? "&" : to_string(b));
            cout << (hit ? "F" : "T") << "\n";
        }
        cout << "页面访问序列总长" << getLen()
            << "，FIFO算法共发生缺页中断" << getFault()
            << "次，缺页率为" << fixed << setprecision(2) << getRate() << "%\n";
    }
};

class LRU : public PageReplacement {
public:
    LRU(const vector<int>& s, int n) : PageReplacement(s, n) {}
    void run() override {
        cout << "\n您选择了LRU算法，执行结果如下：\n";
        cout << left << setw(12) << "访问页面"
            << setw(24) << "物理块"
            << "缺页中断\n";
        for (int i = 0; i < seq.size(); ++i) {
            int page = seq[i];
            bool hit = false;
            int hitIdx = -1;
            for (int j = 0; j < blockNum; ++j)
                if (blocks[j] == page) { hit = true; hitIdx = j; break; }
            if (!hit) {
                faultCnt++;
                int idx = 0, lru = i;
                for (int j = 0; j < blockNum; ++j) {
                    if (blocks[j] == -1) { idx = j; break; }
                    int last = -1;
                    for (int k = i - 1; k >= 0; --k)
                        if (seq[k] == blocks[j]) { last = k; break; }
                    if (last < lru) { lru = last; idx = j; }
                }
                blocks[idx] = page;
            }
            cout << left << setw(12) << page;
            for (int b : blocks) cout << setw(8) << (b == -1 ? "&" : to_string(b));
            cout << (hit ? "F" : "T") << "\n";
        }
        cout << "页面访问序列总长" << getLen()
            << "，LRU算法共发生缺页中断" << getFault()
            << "次，缺页率为" << fixed << setprecision(2) << getRate() << "%\n";
    }
};

class CLOCK : public PageReplacement {
    vector<bool> useBit;
    int ptr;
public:
    CLOCK(const vector<int>& s, int n)
        : PageReplacement(s, n), ptr(0) {
        useBit.assign(blockNum, false);
    }
    void run() override {
        cout << "\n您选择了CLOCK算法，执行结果如下：\n";
        cout << left << setw(12) << "访问页面"
            << setw(24) << "物理块"
            << "缺页中断\n";
        for (int page : seq) {
            bool hit = false;
            for (int j = 0; j < blockNum; ++j) {
                if (blocks[j] == page) {
                    hit = true;
                    useBit[j] = true;
                    break;
                }
            }
            if (!hit) {
                faultCnt++;
                while (true) {
                    if (!useBit[ptr]) break;
                    useBit[ptr] = false;
                    ptr = (ptr + 1) % blockNum;
                }
                blocks[ptr] = page;
                useBit[ptr] = true;
                ptr = (ptr + 1) % blockNum;
            }
            cout << left << setw(12) << page;
            for (int j = 0; j < blockNum; ++j) {
                if (blocks[j] == -1) cout << setw(8) << "&";
                else cout << setw(8) << to_string(blocks[j]) + (useBit[j] ? "*" : "");
            }
            cout << (hit ? "F" : "T") << "\n";
        }
        cout << "页面访问序列总长" << getLen()
            << "，CLOCK算法共发生缺页中断" << getFault()
            << "次，缺页率为" << fixed << setprecision(2) << getRate() << "%\n";
    }
};

int page_inputInt(int low, int high, const string& tip) {
    int x;
    while (true) {
        cout << tip;
        cin >> x;
        if (x >= low && x <= high) break;
        cout << "您输入的数字超出范围，请重新输入" << low << "-" << high << "：\n";
    }
    return x;
}

vector<int> page_randSeq(int len) {
    vector<int> res;
    srand(time(0));
    int last = -1;
    for (int i = 0; i < len; ++i) {
        int r;
        do {
            r = rand() % 6;
        } while (r == last && (rand() % 3) == 0);
        res.push_back(r);
        last = r;
        this_thread::sleep_for(chrono::milliseconds(50));
    }
    return res;
}

vector<int> userSeq(int len) {
    string s;
    while (true) {
        cout << "请输入0-5数字组成的无规律字符串作为页面访问序列，长度为" << len << "：\n";
        cin >> s;
        bool ok = true;
        for (char c : s) if (c < '0' || c > '5') ok = false;
        if (!ok) {
            cout << "您输入的序列有误，请重新输入0-5数字组成的字符串！\n";
            continue;
        }
        if (s.size() < len) {
            cout << "您已输入" << s.size() << "长度，还需输入" << len - s.size() << "：\n";
            continue;
        }
        if (s.size() > len) {
            char c;
            cout << "您输入的字符串超长，自动截取Y或重新输入N（Y/N）：";
            cin >> c;
            if (c == 'Y' || c == 'y') s = s.substr(0, len);
            else continue;
        }
        break;
    }
    vector<int> res;
    for (char c : s) res.push_back(c - '0');
    return res;
}

void page_request_main() {
    srand(time(0));
    while (true) {
        int len = page_inputInt(15, 25, "请输入页面访问序列长度（15-25）：");
        int block = page_inputInt(3, 5, "请输入物理块个数（3-5）：");
        int mode = page_inputInt(1, 2, "请选择序列生成方式：1用户输入 2随机生成：");
        vector<int> seq = (mode == 1) ? userSeq(len) : page_randSeq(len);

        cout << "\n页面访问序列为：";
        for (int x : seq) cout << x << " ";
        cout << "\n物理块个数为：" << block << "\n";

        while (true) {
            int alg = page_inputInt(1, 4, "请选择算法：1OPT 2FIFO 3LRU 4CLOCK：");
            PageReplacement* p = nullptr;
            if (alg == 1) p = new OPT(seq, block);
            else if (alg == 2) p = new FIFO(seq, block);
            else if (alg == 3) p = new LRU(seq, block);
            else if (alg == 4) p = new CLOCK(seq, block);

            p->run();
            delete p;

            int op = page_inputInt(1, 3, "\n请选择：1继续换算法 2更换数据 3结束程序：");
            if (op == 2 || op == 3) {
                if (op == 3) cout << "程序结束！\n";
                break;
            }
        }
        if (page_inputInt(1, 2, "\n是否重新开始？1是 2否：") == 2) break;
    }
}