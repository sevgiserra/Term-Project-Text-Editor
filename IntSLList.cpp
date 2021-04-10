#include <iostream>
#include<string>
#include "IntSLList.h"
#include<fstream>
using namespace std;


IntSLList::~IntSLList() 
{
    for (IntSLLNode *p; !isEmpty(); ) 
    {
        p = head->next;
        delete head;
        head = p;
    }
}
int IntSLList::get_size()
{
    int size=0;
    for (IntSLLNode *tmp = head; tmp != 0; tmp = tmp->next)
        size++;
    
    return size;
}

IntSLLNode* IntSLList::addToHead(string el) 
{
    IntSLLNode * temp = new IntSLLNode;
    temp->info = el;
    temp->next = head;
	size++;
    return temp;
}

void IntSLList::deleteFromHead() 
{
    string el = head->info;
    IntSLLNode *tmp = head;
    if (head == tail)     // if only one node on the list;
         head = tail = 0;
    else head = head->next;
    delete tmp;
    tmp = NULL;
}

void IntSLList::deleteFromTail() 
{ 
    if (head->next == NULL) { 
        delete head; 
    } 
    IntSLLNode* iter = head;
    while(iter->next->next!=NULL)
    {
        iter=iter->next;
    }
    IntSLLNode* temp = iter->next;
    iter->next = NULL;
    delete temp;
    temp = NULL;
}

void IntSLList::insert(int x, string text,int for_stack)
{
    for_undo obj;
    obj.linenumber = x;
    if(head==NULL)
    {
        head = new IntSLLNode;
        head->info="";
    }
    if(x==1){
        if(head->info == "\0" && for_stack==1)
        {
            head->info = text; 
            obj.command = 1;
        }
        else {
            head = addToHead(text); 
            obj.command = 2;
        }
    }
    else if(x<1) cout<<"Please enter a valid line.\n";

    else if(x<=get_size()) //when we dont have to open extra empty nodes
    {
        IntSLLNode *temp = head;
        for(int i = 2; i<x; i++,temp=temp->next);
        if(temp->next->info == "\0" && for_stack==1) 
        {
            temp->next->info = text; 
            obj.command = 1;
        }
        else 
        {
            IntSLLNode* newnode = new IntSLLNode;
            newnode->info = text;
            newnode->next = temp->next;
            temp->next = newnode;
            obj.command = 2;
        }
    }
    else //when x is greater than size so we have to open new empty nodes after size
    {
        int s = get_size();
        IntSLLNode* temp = head;
        while(temp->next!=NULL)
        {
            temp = temp->next;
        }
        obj.command = 3; 
        obj.difference = x-s; //to delete the empty nodes when undo is called
        for(int i=0;i<x-s;i++) //creating new empty nodes until x position
        {
            IntSLLNode* newnode = new IntSLLNode;
            temp->next = newnode;
            temp = temp->next;
        }
        temp->info = text;
    }
    if(for_stack==1) mystack.push(obj);   

}
void IntSLList::move(int n,int m,int for_stack)
{
    for_undo obj;
    obj.linenumber = n; //n is stored for undo
    obj.mline = m; // m is stored for undo
    string temp_str;
    IntSLLNode* iter = head;
    for(int i=1;i<n;i++,iter=iter->next);
    temp_str = iter->info;
    obj.text = temp_str;
    int s = get_size();
    obj.difference = s; //stored to delete the the empty nodes when undo is called
    delete_line(n,0);
    iter = head;
    if(m==1)
    {
        head = addToHead(temp_str);
    }
    else
    {
        insert(m,temp_str,0);
    }
    obj.command = 5;
    if(for_stack==1) mystack.push(obj);

}

void IntSLList::delete_line(int x, int for_stack){
    for_undo obj;
    obj.linenumber = x;
    obj.command = 4;
    if(x==1)  // deleting from head
    {
        obj.text = head->info;
        deleteFromHead();
    }
    else if(x<1 || x>get_size())
    {
        cout<<"Enter a valid line to be deleted.\n";
    }
    else   // deleting a node
    { 
        IntSLLNode* temp = head;
        for(int i=2;i<x;i++,temp=temp->next);
        IntSLLNode* temp1 = temp->next;
        obj.text = temp1->info;
        temp->next = temp->next->next;
        delete temp1;
        temp1 = NULL;
    }
    if(for_stack==1) mystack.push(obj);
    
}

void IntSLList::replace(int x, string text,int for_stack)
{
    for_undo obj;
    obj.linenumber = x;
    IntSLLNode* iter=head;
    if(x>get_size()) 
    {
        cout<<"Please enter a valid line.\n"; 
        return;
    }
    for(int i=1;i<x;i++,iter=iter->next);
    obj.text = iter->info;   //stored for undo
    iter->info = text;
    obj.command = 6;
    if(for_stack==1) mystack.push(obj);

}

void IntSLList::printAll(int pagenum) const {
    int j=1;
    IntSLLNode *tmp = head;
    cout<<endl<<"---------------------------------\n";
    if(pagenum>1) 
        for (tmp; (tmp != 0) && (j<= ((pagenum-1)*10)); tmp = tmp->next,j++); 

    for (tmp; (tmp != 0) && (j<=pagenum*10); tmp = tmp->next,j++)
        cout<<j<< ") "<<tmp->info << endl;

	cout << endl;
    cout << "          -PAGE "<<pagenum<<"-"<<endl;
    cout << endl<<endl;
}

void IntSLList::next(int for_stack)
{
    currentpagenum++;
    /*for_undo obj;
    obj.command = 7;
    if(for_stack==1) mystack.push(obj);*/
}

void IntSLList::prev(int for_stack)
{
    currentpagenum--;
    /*for_undo obj;
    obj.command = 8;
    if(for_stack==1) mystack.push(obj);*/
}

void IntSLList::open()
{
    string file_name;
    cin>>file_name;
    ifstream file;
    file.open(file_name);
    if(!file.is_open())
    {
        cout<<"No such file!";
        return;
    }
    string str;
    getline(file,str);
    if(str!="") 
        head->info = str;
    int i = 2;
    while(!file.eof())
    {
        getline(file,str);
        insert(i,str,0);
        i++;
    }
    currentpagenum=1;
    printAll(currentpagenum);   
    file.close();
}

void IntSLList::save(string filename)
{
    IntSLLNode* temp = head;
    ofstream file;
    file.open(filename);
    while(temp!=NULL){
        file<<temp->info<<endl;
        temp=temp->next;
    }
    file.close();
}

void IntSLList::undo()
{   
    if(!mystack.empty())
    {
        for_undo temp = mystack.top();
        if(temp.command == 1)   // opposite of insert, deleting only the line not the node
        {  
            IntSLLNode* iter = head;
            for(int i=1;i<temp.linenumber;i++,iter=iter->next);
            iter->info = "\0";
            mystack.pop();
        }
        if(temp.command == 2)   // opposite of insert, deleting the node
        {  
            delete_line(temp.linenumber,0);
            mystack.pop();
        }
        if(temp.command == 3)   //opposite of insert, deleting the inserted and empty nodes
        {
            for(int i=0;i<temp.difference;i++)
            {
                delete_line(temp.linenumber,0);
                temp.linenumber--;
            }
            mystack.pop();
        }
        if(temp.command==4)     //opposite of delete, inserting the deleted line
        {
            insert(temp.linenumber,temp.text,0);
            mystack.pop();
        }
        if(temp.command==5)     //opposite of move, temp.linenumber is n; temp.mline is m;
        {
            move(temp.mline,temp.linenumber,0);
            for(int i=get_size();i>temp.difference;i--)
                deleteFromTail();
            mystack.pop();
        }
        if(temp.command==6)     //opposite of replace
        {
            replace(temp.linenumber,temp.text,0);
            mystack.pop();
        }
        /*if(temp.command==7)     //opposite of next     //I wasn't sure if we had to do undo for next and prev so I commented them
        {
            prev(0);
            mystack.pop();
        }
        if(temp.command==8)     //opposite of prev
        {
            next(0);
            mystack.pop();
        }*/
    }
}

void IntSLList::menu()
{
    string opt;
    while(1)
    {
            cout<<"\n                             What Would You Like to Do?\n\n  open || save || insert || delete || move || replace || next || prev || undo || exit \n\n";
            cin>>opt;
            if(opt=="open")
            {
                if(head==NULL)
                {
                    head = new IntSLLNode;
                    head->info="";
                }
                while(head->next!=NULL)
                    deleteFromTail();
                while(!mystack.empty())
                    mystack.pop();
                
                open();         
            }
            else if(opt=="save")
            {
                string text;
                cin>>text;
                save(text);
            }
            else if(opt=="insert")
            {
                int a; string text;
                cin>>a;
                cin.ignore();
                getline(cin,text);
                insert(a,text,1);
                printAll(currentpagenum);
            }
            else if(opt=="delete")
            {
                int x;
                cin>>x;
                delete_line(x,1);
                printAll(currentpagenum);
            }
            else if(opt=="move")
            {
                int a,b;
                cin>>a>>b;
                move(a,b,1);
                printAll(currentpagenum);
            }
            else if(opt=="replace")
            {
                int x; string text;
                cin>>x;
                cin.ignore();
                getline(cin,text);
                replace(x,text,1);
                printAll(currentpagenum);
            }
            else if(opt=="next")
            {
                next(1);
                printAll(currentpagenum);
            }
            else if(opt=="prev")
            {
                prev(1);
                printAll(currentpagenum);
            }
            else if(opt=="undo")
            {
                undo();
                printAll(currentpagenum);
            }
            else if(opt == "exit") break;
            else cout<<"Please enter a valid command\n";
    }
}