#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef char** Huffcode;
typedef struct Node{
	int freq;
	int value;
	struct Node *lchild;
	struct Node *rchild;
	struct Node *parent;
	struct Node *left;
	struct Node *right;
}Node;

typedef struct Heap{
	int size;
	Node *root;
	Node *last;
}Heap;

//create a new node
Node *newNode(int freq,int value,Node *p,Node *l,Node *r){
	Node *new;
	new = malloc(sizeof(Node));
	assert(new!=NULL);
	new->freq = freq;
	new->value = value;
	new->parent = p;
	new->left =l;
	new->right = r;
	new->lchild =NULL;
	new->rchild =NULL;
	return new;
}

//create a new heap
Heap *newHeap(){
	Heap *H;
	H = malloc(sizeof(Heap));
	assert(H!=NULL);
	H->size = 0;
	H->last = NULL;
	H->root = NULL;
	return H;
}

//Find the position of next node after insertion
 Node *lastForInsert(Node *node)
{
	Node *parents,*new;
	parents = node->parent ;
	if(parents!= NULL){     //not the root node
		while(parents!=NULL && parents->left!=node)          //go up until a left child or the root is reached
		{
			node = node->parent;
			parents = parents->parent;

		}
		if(parents==NULL){                          //if root is reached,node is point to the root node
			new=node;
		}
		else{        					  			//if a left child is reached
			new = parents->right;		  			//go to the right child
			if (new==NULL)  return parents;         //if right child does not exist,it's the position of next node
		}
		while (new->left!=NULL)  new = new->left;   //go down left until a leave is reached
		return new;
	}
	new = node;  //if node is the root
	return new;
 }

 //insert new node into heap
void insert(Heap *H,Node *new){
	Node *parents,*grand,*lefts,*rights,*last;
	if (H->size==0){
		H->root = new;
		H->last = new;
		H->size =1;
		return;
	}
	last = lastForInsert(H->last);
	new->parent =last;
	if (last->left==NULL) last->left = new;
	else last->right = new;
	H->size +=1;
	if(new->freq >= last->freq){
		H->last = new;
		return;
	}
	parents = last;

	// upheap until reached root or parent has a freq smaller than or equal to node
	while (parents!=NULL && new->freq < parents->freq ){
		if(parents->parent==NULL){               //if the root node is reached
			new->parent = NULL;
			H->root = new;
		}
		else{
			grand = parents->parent;
			if(grand->right == parents)  grand->right = new; //decide whether it's a left child or right child of the grand node
			else grand->left =new;
			new->parent = grand;
		}

		rights= new->right;           //record the left and right children of current node
		lefts= new->left;
		if(parents->right == new) {   //swap current node with its parent node
			new->right =parents;
			if (parents->left !=NULL){
				new->left = parents->left;
				parents->left->parent = new;
			}
		}
		else{
			new->left = parents;
			if (parents->right !=NULL){
				new->right = parents->right;
				parents->right->parent = new;
			}
		}

		parents->left = lefts;                          //link with new children
		parents->right = rights;
		if (rights!=NULL) rights->parent = parents;
		if (lefts!=NULL) lefts->parent = parents;
		parents->parent = new;
		parents = new->parent;                          //compare with new parent node

	}
	H->last = last;
}

//Find the position of next node after deletion
Node *LastNodeForRemove(Node *node)
 {
 	Node *parents,*new;
 	parents = node->parent;
 	if(parents!= NULL){		                            //not the root node
		while(parents!=NULL && parents->right!=node)  	//go up until a right child or the root is reached
		{
			parents = parents->parent;
			node = node->parent;
		}
		if(parents== NULL) new=node;                    //if root is reached,node is point to the root node
		else new = parents->left;                       //if a right child is reached, go to the left child

		while (new->right!=NULL){
			new = new->right;                           //go down right until a leave is reached
		}
		return new;
	}
	new = node;  //if node is the root
	return new;
 }

//remove min-node from heap,and return the frequency and value.
Node *RemoveMin(Heap *T)
{
	Node *node,*lefts,*rights,*last,*temp,*record;
	node=NULL;
	last=NULL;
	if (T->size ==1){                                    // if only one node left.
		node = T->last;
		T->root=NULL;
		T->last =NULL;
		T->size = 0;
		return node;
	}

	last = T->last;
	node = T->root;
	record = newNode(node->freq, node->value,NULL,NULL,NULL);    //record the value of removed node
	record->lchild = node->lchild;
	record->rchild = node->rchild;
	node->freq = last->freq;                              //replace the root freq and attributes with last node.
	node->value = last->value;
	node->lchild = last->lchild;
	node->rchild =last->rchild;
	T->last = LastNodeForRemove(last);                //update the last node
	if (last->parent->left == last) last->parent->left =NULL;       //remove last node;
	else last->parent->right =NULL;
	last->parent = NULL ;
	T->size -= 1;                                        //decrease the heap size

	//downheap until reached a leaf or children have freqs greater than or equal to node.
	while((node->left!=NULL && node->freq > node->left->freq) || (node->right!=NULL && node->freq > node->right->freq )){
		if (node->right==NULL || (node->right!=NULL && node->left->freq <= node->right->freq)){    //swap the root with child which has smaller freq
			temp = node->left;
			lefts =temp->left;
			rights = temp->right;
			temp->right = node->right;
			if (node->right !=NULL)node->right->parent = temp;
			temp->left = node;
		}
		else{
			temp = node->right;
			lefts = temp->left;
			rights = temp->right;
			temp->left = node->left;
			if(node->left !=NULL) node->left->parent = temp;
			temp->right = node;
		}
		if (node->parent!=NULL){
			if (node->parent->left==node) node->parent->left = temp;
			else node->parent->right =temp;
		}
		temp->parent = node->parent;
		node->parent = temp;
		node->left = lefts;
		node->right = rights;
		if(lefts!=NULL) lefts->parent = node;
		if(rights!=NULL) rights ->parent = node;
		if (node==T->root) {
			T->root = node->parent;
			T->root->parent =NULL;
		}
		if (temp==T->last){
			T->last = node;
		}
	}
	return record;
}


void add_bit(char* block,int offset){
	*block = ((0x80>>offset))|*block;
}

//concat two string into one
char *join(char *a, char *b) {
    char *c = (char *) malloc(strlen(a) + strlen(b) + 1);
    if (c == NULL) exit (1);
    char *tempc = c;
    while (*a != '\0') {
        *c++ = *a++;
    }
    while ((*c++ = *b++) != '\0') {
        ;
    }
    return tempc;
}

//get huffman codes for each character.
void GetCodes(Huffcode HC,char* prefix, Node* node,int len){
	if(node->lchild==NULL && node->rchild ==NULL){  //lead node
		HC[node->value] = (char*)malloc(sizeof(char)*(256));
		strcpy(HC[node->value],prefix);
	}
	len++;
	char * temp=NULL;
	if (node->lchild!=NULL) {
		temp = join(prefix,"0");
		GetCodes(HC,temp,node->lchild,len);
	}
	if (node->rchild !=NULL) {
		temp = join(prefix,"1");
		GetCodes(HC,temp,node->rchild,len);
	}
	free(temp);
}

//construct a huffman tree using min heap
void HuffmanTree(Heap* H){
	Node *temp1,*temp2,*parent;
	while(H->size>1){
		temp1 = RemoveMin(H);
		temp2 = RemoveMin(H);
		parent = newNode((temp1->freq)+(temp2->freq),-1,NULL,NULL,NULL);
		parent->lchild = temp1;
		parent->rchild = temp2;
		temp1->parent =parent;
		temp2->parent = parent;
		insert(H,parent);
	}
}

//free node in tree
void freenode(Node *node){
	if(node==NULL) return;
	freenode(node->lchild);
	freenode(node->rchild);
}

//encode a file
void encode(char* input_path,char* output_path){
	//create frequency table
	int *frequency =(int*)calloc(256,sizeof(int));
	FILE* in_fp =  fopen(input_path,"r");
	int i,c,j;
	while((c=fgetc(in_fp))!=EOF){
		++frequency[c];
	}

    //Huffman coding
	Heap *H = newHeap();
	char *bitarr=(char*)calloc(1024,sizeof(char));
	for (i=0;i<256;i++){
		if (frequency[i]==0) continue;
		insert(H,newNode(frequency[i],i,NULL,NULL,NULL));
		for (j=31;j>=0;j--){
			if (frequency[i]&(1<<j)) add_bit(bitarr+i*4+(j/8),7-(j%8));
		}
	}
	free(frequency);
	HuffmanTree(H);

	//Get Huffman codes
	Huffcode HC =(Huffcode)calloc(256,sizeof(char*));   //huffman code array
	char *prefix=(char*)calloc(256,sizeof(char));
	prefix[255]='\0';
	GetCodes(HC,prefix,H->root,0);

	//create encoded file
	FILE* fp =  fopen(output_path,"wb");
	if(fp==NULL) return;
	fwrite(bitarr,sizeof(char),1024,fp);
	char offset=0;
	unsigned char write=0;
	fseek(in_fp,0,SEEK_SET);
	char *p;

	while((c=fgetc(in_fp))!=EOF){
		p=HC[c];
		while(*p!='\0') {
			if (*p++=='1') write=(write|(0x80>>offset));
			if(offset++==7) {
				fwrite(&write,sizeof(char),1,fp);
				offset=0;
				write=0;
			}
		}
	}
	fwrite(&write,sizeof(char),1,fp);
	fclose(fp);
	fclose(in_fp);
	free(bitarr);
	freenode(H->root);
	free(H);
}

//decode a file
void decode(char* input_path,char* output_path){
	//get header information
	FILE *infp = fopen(input_path,"rb");
	if(infp==NULL){
		return;
	}
	int *frequency =(int*)calloc(256,sizeof(int));
	fread(frequency,sizeof(int),256,infp);
	int fileLength=0;
	int i;
	for(i=0;i<256;i++){
		if (frequency[i]==0) continue;
		fileLength+=frequency[i];
	}

	//huffman coding
	Heap *H = newHeap();
	for (i=0;i<256;i++){
		if (frequency[i]==0) continue;
		insert(H,newNode(frequency[i],i,NULL,NULL,NULL));
	}
	HuffmanTree(H);

	Huffcode HC =(Huffcode)calloc(256,sizeof(char*));   //huffman code array
	char *prefix=(char*)calloc(256,sizeof(char));
	prefix[255]='\0';
	GetCodes(HC,prefix,H->root,0);

	//decode
	char temp=0;
	int count=0;
	FILE *output = fopen(output_path,"wb");
	Node *node = H->root;
	if(node->lchild==NULL && node->rchild==NULL){
		for(i=0;i<fileLength;i++){
			fwrite(&node->value,sizeof(char),1,output);
		}
	}
	else{
		while(fread(&temp,sizeof(char),1,infp)){
			for(i=0;i<8;i++){
				if ((temp&(0x80>>i) )== 0) node=node->lchild;
				else node=node->rchild;
				if (node->lchild==NULL || node->rchild==NULL ){
					if(count==fileLength) break;
					fwrite(&node->value,sizeof(char),1,output);
					count++;
					node=H->root;
				}
			}
		}
	}
	fclose(output);
	fclose(infp);
	freenode(H->root);
	free(H);
}

//Failure function for kMP algorithm.
void FailureFunction(char* p,int F[]){
	F[0]=0;
	int i=1,j=0;
	while(i<strlen(p)){
		if(p[i]==p[j]){
			F[i]=j+1;
			i++;
			j++;
		}
		else if(j>0) j=F[j-1];
		else F[i++]=0;
	}
}

//kMP algorithm for pattern matching.
int KMP(char s[], char p[], int n, int patt_len,int next[])
{
    int ans = 0;
    int i = 0;
    int j = 0;
    while (i<n){
        if (p[j] == s[i]){
        	if (j==patt_len-1){
                ans++;
                if(j>0) j = next[j-1];
                else i++;
            }
        	else{
				i++;
				j++;
			}
        }
        else {
            if(j>0) j = next[j-1];
            else i++;
        }
    }
    return ans;
}

int search(char* query,char* filepath){
	//get header information
	FILE *infp = fopen(filepath,"rb");
	if(infp==NULL) {
		return 0;
	}
	int *frequency =(int*)calloc(256,sizeof(int));
	fread(frequency,sizeof(int),256,infp);
	int fileLength=0;
	int i,j;
	for(i=0;i<256;i++){
		if (frequency[i]==0) continue;
		fileLength+=frequency[i];
	}
	if (fileLength==0) return 0;   //if the encoded file is empty,return 0
	int  m= strlen(query);    
	if(m>fileLength) return 0;     //if search term is longer than the whole file,no need of comparison.

	for(i=0;i<m;i++){              //if a character in search term is not exist in file.
		j = query[i];
		if (frequency[j]==0 ) {
			return 0;
		}
	}

	//huffman coding
	Heap *H = newHeap();
	for (i=0;i<256;i++){
		if (frequency[i]==0) continue;
		insert(H,newNode(frequency[i],i,NULL,NULL,NULL));
	}
	HuffmanTree(H);

	//get huffman codes
	Huffcode HC =(Huffcode)calloc(256,sizeof(char*));   //huffman code array
	char *prefix=(char*)calloc(256,sizeof(char));
	prefix[255]='\0';
	GetCodes(HC,prefix,H->root,0);

	//decode and KMP pattern matching
	int count=0;
	char temp;
	int SIZE=1024;     //cut whole file into blocks
	free(prefix);
	free(frequency);
	char* s=(char*)calloc(SIZE,sizeof(char)); //array of decoded string
	Node *node = H->root;
	char *temps=(char*)calloc(m-1,sizeof(char));
	int ans=0;
	int *next=(int*)calloc(1024,sizeof(int));
	if(node->lchild==NULL && node->rchild==NULL){     // if only one kind of character
			for(i=0;i<fileLength;i++){
				ans++;
			}
		}
	else{
		while (fread(&temp,sizeof(char),1,infp)){
			for(i=0;i<8;i++){
				if ((temp&(0x80>>i) )== 0) node=node->lchild;
				else node=node->rchild;
				if (node->lchild==NULL || node->rchild==NULL ){
					if(fileLength==0) {
						break;
					}
					s[count]=node->value;
					count++;
					fileLength--;
					node=H->root;
					if(count>=SIZE){       //do comparison (using KMP) for every #SIZE of characters
						FailureFunction(query,next);
						ans += KMP(s, query,SIZE,m,next);
						for(j=0;j<m-1;j++){         //keep the last m-1 characters for consistency.
							strcpy(&temps[j],&s[SIZE+1-m+j]);
						}
						s =memset(s,0,SIZE*sizeof(char));
						for(j=0;j<m-1;j++){
							strcpy(&s[j],&temps[j]);
							temps[j]=0;
							fileLength++;
						}
						count=m-1;
					}
				}
			}
		}
		ans += KMP(s,query,count,m,next);
		free(s);
		free(temps);
	}


	return ans;
}

int main(int argc, char* argv[]){
	 if (argc !=4) {
		 return 0;
	 }
	 if (strcmp(argv[1],"-e")==0) encode(argv[2],argv[3]);
	 if (strcmp(argv[1],"-d")==0) decode(argv[2],argv[3]);
	 if (strcmp(argv[1],"-s")==0) {
		 int ans =search(argv[2],argv[3]);
		 printf("%d\n",ans);
	 }
	 return 0;
 }


