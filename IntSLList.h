//************************  intSLList.h  **************************
//           singly-linked list class to store integers

#ifndef INT_LINKED_LIST
#define INT_LINKED_LIST
#include <iostream>
#include<stack>
using namespace std;

struct for_undo{
    int linenumber;         // used for the line number and n(in move function)
    string text;            // for storing the text
    int mline;              //for m in move function
    int command;            //for undo
    int difference;         // to delete the empty nodes
};


class IntSLLNode {
public:
    IntSLLNode() {
        next = 0;
    }
    IntSLLNode(string el, IntSLLNode *ptr = 0) {
        info = el; next = ptr;
    }
    string info;
    IntSLLNode *next;
};

class IntSLList {
public:
	int currentpagenum=1;
    IntSLList() {
        head = tail = 0;
		size = 0;
    }
    ~IntSLList();
    int isEmpty() {
        return head == 0;
    }

    IntSLLNode* addToHead(string);
    void deleteFromHead(); 
    void  deleteFromTail(); 
    void printAll(int) const;
    int get_size();
    void open();
    void menu();
    void insert(int,string,int); 
    void next(int);
    void prev(int);
    void delete_line(int,int); 
    void move(int,int,int);
    void replace(int,string,int);
    void save(string);
    void undo();
private:
    IntSLLNode *head, *tail;
	int size;
    stack<for_undo> mystack;
};

#endif
