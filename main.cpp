/* @Author
Student Name: Faruk Orak		
Student ID: 150180058
Date: 19.12.2019 */

#include<iostream>
#include<cstdio>
#include<string.h>
#include<fstream>
#include<string>
#include<stdbool.h>
using namespace std;

struct Node_MH{ //This structure holds mobile hosts
	int id;
	Node_MH *next;
};

struct Node_BS{  //This structure holds base stations
	Node_BS *next;
	Node_BS *children;
	Node_MH *mobile_hosts; //is created to keep mobile hosts address' which is connected to spesific base station
	int parent_id;
	int id;
};

struct BS_MultiLinkedList{ //This structure is used to create general tree
	Node_BS *head;
	void create();
	void add_BS(Node_BS* root,int parent_id, int id);
	Node_BS* search_BS(Node_BS* root,int id);
	void send_message(int id);
};

struct MH_List{ //list structure which holds mobile hosts
	void add(Node_BS* root,int parent_id,int id);
};

void BS_MultiLinkedList::create(){ //creates the root
	head = new Node_BS;
	head->next = NULL;
	head->children = NULL;
	head->mobile_hosts = NULL;
	head->parent_id = -1; // assigned to -1 since, there will never be a base station whose parent is -1
	head->id = 0;
}
void BS_MultiLinkedList::add_BS(Node_BS* root,int parent_id, int id) // adds base stations
{
	Node_BS* traverse;
	Node_BS* newnode = new Node_BS;
	newnode->next = NULL;
	newnode->children = NULL;
	newnode->mobile_hosts = NULL;
	newnode->parent_id = parent_id;
	newnode->id = id;
	traverse = search_BS(root,newnode->parent_id); //finds the base station's parent which is being added
	if(traverse->id == newnode->parent_id)
	{
		if(traverse->children == NULL) //if there is no children then make it children 
		{
			traverse->children = newnode;
			return;
		}
		else if(traverse->children)//if a children already exists, then add the base station as left most node in the same level
		{
			traverse = traverse->children;
			while(traverse->next)
			{
				traverse = traverse->next;
			}
			traverse->next = newnode;
			return;
		}
	}	
}
Node_BS* BS_MultiLinkedList::search_BS(Node_BS* root,int id) //finds a base station and returns its address
{
	Node_BS* traverse;
	Node_BS* traverse_children;
	Node_BS* traverse_next;
	if(root == NULL) //checks whether tree is created or not
	{
		return NULL;
	}
	else if(root && root->id != id) //recursive part of the function
	{
		traverse_children = search_BS(root->children,id); //always checks childrens first
		traverse_next = search_BS(root->next,id); //After checks same levels
	}
	else if(root->id == id) //if you find the base station return its address
	{
		traverse = root;
		return traverse;
	}
	if(traverse_children) //traverse_children and traverse_next are used to return address of base station when base station is found at the recursive part of function
	{
		return traverse_children;
	}
	else if(traverse_next) //traverse_children and traverse_next are used to return address of base station when base station is found at the recursive part of function
	{
		return traverse_next;
	}
	else
	{
		return NULL;
	}
}
void MH_List::add(Node_BS* root,int parent_id,int id) //adds mobile hosts
{
	BS_MultiLinkedList tree;
	Node_BS* traverse;
	Node_MH* newhost = new Node_MH;
	newhost->next = NULL;
	newhost->id = id;
	
	traverse = tree.search_BS(root,parent_id); //finds the mobile host's parent base station
	if(traverse->mobile_hosts == NULL)//if there is no mobile host, then add it as first
	{
		traverse->mobile_hosts = newhost;
	}
	else//if not,add it as last
	{
		Node_MH* traverse_mobile;
		traverse_mobile = traverse->mobile_hosts;
		while(traverse_mobile->next)
		{
			traverse_mobile = traverse_mobile->next;
		}
		traverse_mobile->next = newhost;
	}
}
void print_path(Node_BS* root,int id, int* path)//prints the path of mobile host like "0 5 2 7 9" (this function is a recursive function)
{												//this function starts from the base station and lasts at root. Adds the visited base station IDs to path array.
	Node_BS* traverse;							//Function adds the path as reverse of the true path because we start from bs to root. path array is used to reverse the 
	BS_MultiLinkedList tree;					//added path and makes it actual path 
	int i = 0;
	if(id != -1)//if we are not at the root
	{
		traverse = tree.search_BS(root,id);
		while(path[i] != -1)
		{
			i++;
		}
		path[i] = traverse->id;
		print_path(root,traverse->parent_id,path); //recursive part 
	}
	if(id == -1)//if we are at the root(on the other hand, if searching is finished)
	{
		i = 0;
		while(path[i] != -1) //print the path in reverse order
		{
			i++;
		}
		i--;
		while(i!=-1)
		{
			printf("%d ",path[i]);
			i--;
		}
	}
}
void traverse_tree(Node_BS* root,Node_BS* ptr, int id, char message[], int* flag) //this function traverses tree, finds host, sends message and prints the path
{																				//root variable is used at print_path() function
	Node_MH* traverse;															//ptr variable holds current base station
	if(ptr && *flag == 0)														//id holds id of host and message array holds message which will be sent to mobile host
	{																			//flag holds 1 or 0. flag is sent to function with 0 as default. If function finds the host,then flag is assigned to 1 and recursive function do not continue to search anymore.
		cout << ptr->id << " ";//prints traversed IDs
		traverse = ptr->mobile_hosts;
		while(traverse)//traverses current base station's mobile hosts
		{
			if(traverse->id == id)//if function finds the target host, sends message and prints path
			{   
				int path[64];//is created to send an array to print_path() function
				int* path_ptr;
				path_ptr = path;
				for(int i = 0;i<64;i++)//fill -1's array because we don't know the path's length. print_path() function performs until -1 and prints path correctly
				{
					path[i] = -1;
				}
				cout << "\nMessage:"<<message <<" To:";
				print_path(root,ptr->id,path_ptr);
				cout << "mh_" << id << "\n";
				*flag = 1; //assign flag to 1 since host is found
				break;
			}
			traverse = traverse->next;
		}
		if(*flag==1)//if host is found, do not continue to search
		{
			return;
		}
		traverse_tree(root,ptr->children, id, message,flag); //else continue to traverse as pre-order
		traverse_tree(root,ptr->next, id, message,flag);
	}
}
int main(int argc, char *argv[]){
	
	FILE *fPtr;
	struct BS_MultiLinkedList tree;
	struct MH_List mobile_list;
	char type[5]; //holds the type (BS or MH)
	int number1; //holds the id
	int number2; //holds the parent id
    char mh[] = "MH"; //is used at strcmp()
	char bs[] = "BS"; //is used at strcmp()
	tree.create(); //create tree
	fPtr = fopen(argv[1],"r+");
	if(fPtr){
		while(!feof(fPtr)) //add all BS's and MH's
		{
			fscanf(fPtr,"%s %d %d",type,&number1,&number2);
			if(strcmp(bs,type) == 0)
			{
				tree.add_BS(tree.head,number2,number1);
			}
			else if(strcmp(mh,type)==0)
			{
				mobile_list.add(tree.head,number2,number1);
			}
		}
	}
	fclose(fPtr);
	ifstream file(argv[2]);
	string line_f;
	while(getline(file,line_f))//(*)
	{
		int i = 0;
		char message[50];
		char id[10];
		while(line_f[i]!= '>')
		{
			message[i] = line_f[i];
			i++;
		}
		message[i] = '\0';
		i++;
		int j = 0;
		while(line_f[i] != '\0')
		{
			id[j] = line_f[i];
			i++;
			j++;
		}
		id[j] = '\0';
		int mobile_host_id;
		sscanf(id,"%d",&mobile_host_id);//(**)
		cout << "Traversing:";//part of code from (*) to (**), splits the line with respect to > and converts numeric part of string to int
		int flag = 0; //is created to hold whether MH is found or not
		traverse_tree(tree.head,tree.head,mobile_host_id,message,&flag); //traverse tree, search mh and if you find, send message and print path
		if(flag == 0)//unless you find
		{
			cout << "\nCan not be reached the mobile host mh_"<<mobile_host_id<<" at the moment\n";
		}
	}
	return 0;
}















































