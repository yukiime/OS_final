#include <iostream>
#include <queue>
#include <unordered_map>

#define MAX_PAGE_NUMBER 18
#define SEQUENCE_LEN 54
#define M 6

class LRU_mainMemory
{
private:
    struct Node
    {
        int value;
        int key;
        Node* prevPtr;
        Node* nextPtr;
        Node(): prevPtr(nullptr), nextPtr(nullptr) {}
        Node(int nodeValue): value(nodeValue), prevPtr(nullptr), nextPtr(nullptr) {}
    };
    
    const int frame_num;
    Node* head;
    Node* tail;
    std::unordered_map<int, Node*> frames;

public:
    LRU_mainMemory(int num):frame_num(num)
    {
        head = new Node();
        tail = new Node();
        head->nextPtr = tail;
        tail->prevPtr = head;
    }

    // Is the memory full
    bool isFrameFull()
    {
        return (frames.size() == frame_num);
    }

    // Get whether the page number is in a certain frame memory
    int getPage_frame(int pageNo)
    {
        for (const auto& pair : frames) 
        {
            if (pair.second->value == pageNo) 
            {
                return pair.first;
            }
        }
        return -1;
    }

    // add a new page
    void addFrame(int pageNo)
    {
        int len = frames.size();
        Node* n = new Node(pageNo);

        n->nextPtr = head->nextPtr;
        head->nextPtr->prevPtr = n;
        head->nextPtr = n;
        n->prevPtr = head;
        frames[len] = n;
        n->key = len;
    }

    // make recently (let it on head)
    void makeRecently(int pageNo) 
    {
        int key = getPage_frame(pageNo);
        Node* x = frames[key];
        putInHead(x);
    }

    // put the node in the head
    void putInHead(Node* x)
    {
        // First delete this node from the linked list
        x->prevPtr->nextPtr = x->nextPtr;
        x->nextPtr->prevPtr = x->prevPtr;

        // reinsert at the end of the queue
        head->nextPtr->prevPtr = x;
        x->prevPtr = head;
        x->nextPtr = head->nextPtr;
        head->nextPtr = x;
    }

    // page replacement
    void replacement(int pageNo)
    {
        // tail pointer to the previous one of the x
        Node* x = tail->prevPtr;
        tail->prevPtr = x->prevPtr;

        // get the key and replace page number
        int key = x->key;
        int old_page_number = x->value;
        x->value = pageNo;

        // put the node in the head
        putInHead(x);
        
        // print replace information
        print_replacement(key,old_page_number,pageNo);
    }

    void print_replacement(int key, int old_page_number, int pageNo)
    {
        std::cout<<"frame:"<<key<<",Replace page number "<<old_page_number<<" with page number "<<pageNo<<std::endl;
    }

};

// Initialize the page queue
std::queue<int> init_sqeuence(int num,int max)
{
    std::queue<int> sqeuence;
    srand((unsigned)time(NULL));
    for(int i = 0;i < num; i++)
    {
        sqeuence.push( std::rand() % (max+1) );
    }

    return sqeuence;
}

void print_sqe(std::queue<int>sqeuence)
{
    int len =sqeuence.size();
    for(int i = 0;i < len; i++){
        std::cout<<sqeuence.front();
        sqeuence.pop();
        if(!sqeuence.empty())
            std::cout<<",";
        else
            std::cout<<std::endl;
    }
}

// Execute the LRU algorithm and return the number of page faults
int lru_algorithm(std::queue<int>sqeuence, LRU_mainMemory mainMemory)
{
    int pageFault = 0;

    while (!sqeuence.empty())
    {
        int pageNo = sqeuence.front();

        // if frame is inside
        if(mainMemory.getPage_frame(pageNo) != -1)
        {
            mainMemory.makeRecently(pageNo);
        }
        else 
        {
            // page fault occurred
            pageFault++;

            // Memory is full and page replacement is required
            if(mainMemory.isFrameFull())
            {
                mainMemory.replacement(pageNo);
            }
            else
            {
                mainMemory.addFrame(pageNo);
            }
        }
        sqeuence.pop();
    }
    
    return pageFault;
}

void print_pageFault_rate(int pageFault, int total)
{
    float rate = float(pageFault) / float(total) * 100;
    std::cout<<"page fault rate: "<<rate<<"%"<<std::endl;
}

int main() {
    std::queue<int> sqeuence = init_sqeuence(SEQUENCE_LEN,MAX_PAGE_NUMBER);

    std::cout<<"-------------------------------a page access sequence-------------------------------"<<std::endl;
    print_sqe(sqeuence);

    std::cout<<"----------------LRU algorithm for page replacement on the above access sequence-------------"<<std::endl;
    LRU_mainMemory mainMemory = LRU_mainMemory(M);
    int pageFault = lru_algorithm(sqeuence, mainMemory);

    std::cout<<"---------------------------------------------page fault rate--------------------------------------------"<<std::endl;
    print_pageFault_rate(pageFault, SEQUENCE_LEN);

    return 0;
}
