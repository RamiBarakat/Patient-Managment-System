/*



Rami Majadbeh_1190611


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define max 100


struct AVLnode;//create a struct for node to construct a tree
typedef struct AVLnode *AVL_T;//type def for a pointer

struct AVLnode
{

    char name[max];
    char gender[max];
    char date[max];
    char birth[max];
    char illness[max];
    char address[max];
    char bloodType[max];
    AVL_T Left;
    AVL_T Right;
    int Height;
};


typedef struct hash//hash table
{

    int flag;
    char name[max];
    char gender[max];
    char date[max];
    char birth[max];
    char illness[max];
    char address[max];
    char bloodType[max];

} hash;
////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
int isprime(int x){//function that checks if a number is prime or not
int flag =1;
 for (int i = 2; i <= sqrt(x); i++) {

        // If n is divisible by any number between
        // 2 and n/2, it is not prime
        if (x % i == 0) {
            flag = 0;
            break;
        }
    }

    if (x <= 1)
        flag = 0;

    if (flag == 1) {
        return 1;
    }
    else {
        return 0;
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////
int getHeight (AVL_T t)//get the height of tree
{
    if(t==NULL)
        return -1;
    else
        return t->Height;
}
/////////////////////////////
int getBalance(AVL_T N)//balance = height for left - height for right
{
    if (N == NULL)
        return 0;
    return getHeight(N->Left) - getHeight(N->Right);
}

////////////////////////////
AVL_T minNode(AVL_T node)//return the min node
{
    AVL_T current=node;
    while(current->Left!=NULL)
        current=current->Left;

    return current;
}
/////////////////////////////
int MAX(int x,int y)//return max of two
{
    if(x>y)
        return x;
    else
        return y;
}
////////////////////////////////////////////////
AVL_T singleRotateLeft(AVL_T x)//single rotate left for the tree to correct the balance factor
{
    AVL_T y;//create a new node
    y = x->Right;
    x->Right  = y->Left;
    y->Left = x;

    x->Height = 1 + MAX(getHeight(x->Left), getHeight(x->Right));
    y->Height = 1 + MAX(getHeight(x), getHeight(y->Right));

    return y;
}
////////////////////////////////////////////////
AVL_T singleRotateRight(AVL_T y)//single rotation to the right
{
    AVL_T x;
    x = y->Left;
    y->Left  = x->Right;
    x->Right = y;

    y->Height = 1 + MAX(getHeight(y->Right), getHeight(y->Left));
    x->Height = 1 + MAX(getHeight(y), getHeight(x->Left));

    return x;
}
////////////////////////////////////////////////
AVL_T doubleRotateLeft(AVL_T K3)//double rotate left is one single left and one single right
{
    K3->Left  = singleRotateLeft(K3->Left);
    return singleRotateRight(K3);
}
////////////////////////////////////////////////
AVL_T doubleRotateRight(AVL_T K3)//double rotate right is one single right and one single left
{
    K3->Right  = singleRotateRight(K3->Right);
    return singleRotateLeft(K3);
}
////////////////////////////////////////////////
AVL_T insert (AVL_T t1, char *name,char *gender,char *date,char *birth,char *illness,char *address,char *bloodType)
{//insert a new node to the tree
    if(t1==NULL)//if it is null then malloc and copy data
    {
        t1=(AVL_T)malloc(sizeof (struct AVLnode));
        strcpy(t1->name,name);
        strcpy(t1->gender,gender);
        strcpy(t1->date,date);
        strcpy(t1->birth,birth);
        strcpy(t1->illness,illness);
        strcpy(t1->address,address);
        strcpy(t1->bloodType,bloodType);
        t1->Height=0;
        t1->Left=NULL;
        t1->Right=NULL;
        return t1;

    }
    //I used strcasecmp() which compares two names without giving importance to lower or upper case
    else if(strcasecmp(t1->name,name) > 0)//if not null and root is bigger than the name
    {
        t1->Left = insert(t1->Left,name,gender,date,birth,illness,address,bloodType);
        if(getHeight(t1->Left) - getHeight(t1->Right) >= 2)//see if balance is got or not(-1,0,1) and do the rotations necessary
        {

            if(strcasecmp(name,t1->Left->name)<0)
                t1=singleRotateRight(t1);
            else
                t1=doubleRotateLeft(t1);
        }
    }

    else if(strcasecmp(t1->name,name) < 0)//if not null and root is smaller than the name
    {
        t1->Right = insert(t1->Right,name,gender,date,birth,illness,address,bloodType);
        if(getHeight(t1->Right) - getHeight(t1->Left) >= 2)//see if balance is got or not(-1,0,1) and do the rotations
        {

            if(strcasecmp(name,t1->Left->name) > 0)
                t1=singleRotateLeft(t1);
            else
                t1=doubleRotateRight(t1);
        }
    }

    t1->Height = 1 + MAX(getHeight(t1->Right), getHeight(t1->Left));//get the height

    return t1;
}
////////////////////////////////////////////////////////////////////////////
AVL_T Delete(AVL_T t,char name[max])//delete a name depending on the name as key
{

    // do the basic BST deletion process
    if (t == NULL)
    {
        printf("Patient does not exist\n");
        return t;

    }
    //Search for the node that needs to be deleted
    if(strcasecmp(name,t->name)<0)
        t->Left = Delete(t->Left, name);

    else if(strcasecmp(name,t->name)>0)
        t->Right = Delete(t->Right, name);

    else
    {

        if( (t->Left == NULL) || (t->Right == NULL) )//no or one child
        {
            AVL_T temp = t->Left ? t->Left : t->Right;


            if (temp == NULL)//no child
            {
                temp = t;
                t = NULL;
            }
            else // one child case
                *t = *temp; // Copy the contents of the not empty child to the empty
                free(temp);

        }

        else//node with two children
        {
            //get the smallest successor and delete
            AVL_T temp = minNode(t->Right);
            strcpy(t->name,temp->name);
            t->Right = Delete(t->Right, temp->name);
        }

    }

    // If one node then return
    if (t == NULL)
        return t;

    //update the height
    t->Height = 1 + MAX(getHeight(t->Left),getHeight(t->Right));

    //get balance factor and check if it is unbalanced
    int balance = getBalance(t);

    //from here under we have for cases

    if (balance > 1 && getBalance(t->Left) >= 0)//Left Left
        return singleRotateRight(t);


    if (balance > 1 && getBalance(t->Left) < 0)  //Left Right
    {
        t->Left = singleRotateLeft(t->Left);
        return singleRotateRight(t);
    }


    if (balance < -1 && getBalance(t->Right) <= 0)//Right Right
        return singleRotateLeft(t);


    if (balance < -1 && getBalance(t->Right) > 0)//Right Left
    {
        t->Right = singleRotateRight(t->Right);
        return singleRotateLeft(t);
    }

    return t;
}
////////////////////////////////////////////////////////////////////////////
void printInOrder(AVL_T t)//print left-root-right
{
    if(t!= NULL)
    {
        printInOrder(t->Left);
        printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);
        printInOrder(t->Right);

    }
}
////////////////////////////////////////////////////////////////////////////
void printPreOrderillness(AVL_T t,char illness[max])//print the same patients with the same illness
{


    if(t!= NULL)
    {

        if(strcasecmp(t->illness,illness) == 0)
        {
            printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);
            printPreOrderillness(t->Left,illness);
            printPreOrderillness(t->Right,illness);
        }
        else
        {
            printPreOrderillness(t->Left,illness);
            printPreOrderillness(t->Right,illness);

        }
    }

}
/////////////////////////////////////////////////////////////////////////////////
int checkNumbers(char *key)
{
    int x=0;
for(int i=0;key[i]!= '\0';i++){
    if(isdigit(key[i])){
        x=1;

    }
}

if(x == 1){
    return 1;
}
else{
    return 0;
}
}
////////////////////////////////////////////////////////////////////////////
AVL_T ReadFile(FILE *in,AVL_T tree)//read data from file patients.txt
{
    char line[max];
    char name[max] = "";
    char gender[max];
    char date[max];
    char birth[max];
    char illness[max] = "";
    char address[max]="";
    char bloodType[max]="";

    int i =1;
    in = fopen("patients.txt","r");

    if(in == NULL)
    {
        printf("there is an error with the file\n");
        exit(0);
    }

    char *token;

    while(fgets(line,max,in) != NULL)//split it by '#'
    {

        token = strtok(line,"#");
        strcpy(name,token);
        if(checkNumbers(name) == 1){
            printf("you cant enter a number in a name in patient number %d\n",i);
        }


        token = strtok(NULL,"#");
        strcpy(gender,token);
        if(checkNumbers(gender) == 1){
            printf("YOU CANT ENTER A NUMBER IN A GENDER IN PATIENT NUMBER %d\n",i);
        }


        token = strtok(NULL,"#");
        strcpy(date,token);


        token = strtok(NULL,"#");
        strcpy(birth,token);


        token = strtok(NULL,"#");
        strcpy(illness,token);


        token = strtok(NULL,"#");
        strcpy(address,token);


        token = strtok(NULL,"#");
        strcpy(bloodType,token);
        if(checkNumbers(bloodType) == 1){
            printf("you cant enter a number in a blood type in patient number %d\n",i);
        }


        tree = insert(tree,name,gender,date,birth,illness,address,bloodType);//insert node by node
        i++;

    }
    fclose(in);
    return tree;
}
////////////////////////////////////////////////////////////////////////////////
AVL_T addPatient(AVL_T t)//add a patient into the tree  using insert
{

    char name[max],gender[max], illness[max],address[max],bloodType[max];
    char date[max],birth[max];

    printf("please enter the patient name\n");
    gets(name);
    printf("please enter the patient gender\n");
    gets(gender);
    printf("enter the patient's date of addmission\n");
    gets(date);
    printf("enter the date of birth\n");
    gets(birth);
    printf("please enter the illness of the patient\n");
    gets(illness);
    printf("please enter the address of the patient\n");
    gets(address);
    printf("please enter the blood type for the patient\n");
    gets(bloodType);

 t = insert(t,name,gender,date,birth,illness,address,bloodType);

    return t;
}
////////////////////////////////////////////////////////////////////////////////
AVL_T searchPatient(AVL_T t,char name[max])//search patient(usually used for deletion)
{

    if (t==NULL)
    {

        return NULL;
    }

    if(strcasecmp(name,t->name) >0)
    {
        return searchPatient(t->Right,name);
    }
    else if(strcasecmp(name,t->name)<0)
    {
        return searchPatient(t->Left,name);
    }
    else
    {
        printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);
        return t;
    }

return t;
}
////////////////////////////////////////////////////////////////////////////////
AVL_T changeInfo(AVL_T tree,char name[max])//change info of a patient
{

    char namee[max],gender[max], illness[max],address[max],bloodType[max];
    char date[max],birth[max],nameNew[max];

    AVL_T temp = NULL;

    temp = searchPatient(tree,name);
    strcpy(nameNew,name);
   if(temp == NULL){
    printf("NO PATIENT WITH SUCH NAME!!\n");

   }


    int x;
    if(temp != NULL){
    printf("1- update name\n2- update gender\n3- update date of addmission\n4- update date of birth\n5- update illness\n6- update address\n7- update bloodType\n");
    scanf("%d",&x);

     if(x > 7 || x < 1){
            printf("Wrong Input\n");
        }


    getchar();


    switch(x)
    {

    case 1://here is most important(if we change the name 'which is key' we delete and re-insert)
        strcpy(gender,temp->gender);
        strcpy(illness,temp->illness);
        strcpy(address,temp->address);
        strcpy(bloodType,temp->bloodType);
        strcpy(date,temp->date);
        strcpy(birth,temp->birth);


        tree = Delete(tree,name);
        printf("enter the updated patient name\n");
        gets(namee);
        strcpy(nameNew,namee);

        tree = insert(tree,namee,gender,date,birth,illness,address,bloodType);
        break;

    case 2://change gender
        printf("enter the updated gender\n");
        gets(gender);
        strcpy(temp->gender,gender);
        break;


    case 3://change date
        printf("enter the updated date of addmission\n");
        gets(date);
        strcpy(temp->date,date);
        break;


    case 4://change birth
        printf("enter the updated date of birth\n");
        gets(birth);
        strcpy(temp->birth,birth);
        break;


    case 5://change illness
        printf("enter the updated illness\n");
        gets(illness);
        strcpy(temp->illness,illness);
        break;

    case 6://change address
        printf("enter the updated address\n");
        gets(address);
        strcpy(temp->address,address);
        break;

    case 7://change blood type
        printf("enter the updated blood type\n");
        gets(bloodType);
        strcpy(temp->bloodType,bloodType);
        break;
    }

    char con[max];//if user likes to change more than one info at a time, they can.
    printf("\nIF You Would Like To Change Another Data Type 'YES' If Not Type 'No' \n");
    gets(con);

    if(strcasecmp(con,"yes") == 0){

       return changeInfo(tree,nameNew);
      }

    else {

        return tree;
}
    }
return tree;
}
////////////////////////////////////////////////////////////////////////////////
AVL_T searchIllness(AVL_T t,char name[max])//search here for illness(to delete)
{

    if (t==NULL)
    {
        printf("EMPTY!\n");
        return NULL;
    }
    if(strcmp(t->name,name) == 0)
    {

    printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);

    }
    if(strcasecmp(name,t->illness) >0)
    {
        return searchIllness(t->Right,name);
    }
    else if(strcasecmp(name,t->illness)<0)
    {
        return searchIllness(t->Left,name);
    }

    else
    {
        printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);
        return t;

    }

}
////////////////////////////////////////////////////////////////////////////////
void fprintFile(AVL_T t,FILE *f)//print to a file so we can read into hash table
{

    if(t!= NULL)
    {

        fprintf(f,"%s#%s#%s#%s#%s#%s#%s",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);

        fprintFile(t->Left,f);
        fprintFile(t->Right,f);

    }


}
//////////////////////////////////////////////////////////////////////////////////////
void fprintFileprime(AVL_T t,FILE *f)//print to the file to preview
{



    if(t!= NULL)
    {
        fprintFileprime(t->Left,f);
        fprintf(f,"Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",t->name,t->gender,t->date,t->birth,t->illness,t->address,t->bloodType);
        fprintFileprime(t->Right,f);

    }
}
//////////////////////////////////////////////////////////////////////////////////////

/*

HASHING
HASHING


*/
////////////////////////////////////////////////////////////////////////////////

int size = 0;
int maxsize;//initliaze the size of the table


double loadFactor()//get the load factor
{
    return (double)size/maxsize ;
}
////////////////////////////////////////////////////////////////////////////////
hash *Rehash(hash *h1)//rehash the table if load factor is >=0.7
{
    //printf("\n\n");
    //display(h);
    hash *ptr;//new table
    int oldsize = maxsize;//modify the size
    maxsize = (2*maxsize) + 1;

    while(isprime == 0)//check if prime or not
    {
        maxsize++;
    }


    ptr = (hash *)malloc(maxsize*sizeof(hash));//malloc in memory for the specified size

    for(int i=0; i<maxsize; i++)
    {
        ptr[i].flag = 0;

    }

    //copy data to the new table
    for(int i=0; i<oldsize; i++)
    {

        if(h1[i].flag != 1) continue;


        insert_hash(ptr,h1[i].name,h1[i].gender,h1[i].date,h1[i].birth,h1[i].illness,h1[i].address,h1[i].bloodType);
        size--;

    }

    return ptr;
}
//////////////////////////////////////////////////////////////////////////////////////////////
int getAscii(char key[])//get ascii code for the different strings
{

    int i=0;
    int index = 0;

    for(i =0;key[i] != '\0';i++){
        if(key[i] == ' '){continue;}
         index +=key[i];
    }
    printf("index is %d\n",index);
    return index;
}

//////////////////////////////////////////////////////////////////////////////////////////////
int getindex(char key[])//get the index to insert in the hash table
{

    return (getAscii(key) % maxsize);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void insert_hash(hash *h2,char *name,char *gender,char *date,char *birth,char *illness,char *address,char *bloodtype)
{//insert to the hash table


    int i = getindex(name);

    while(h2[i].flag == 1)//if occupied increment the index to find an empty one
    {
        //linear probing
        i = (i + 1) % maxsize;

    }
    //copy data
    strcpy(h2[i].name,name);
    strcpy(h2[i].gender,gender);
    strcpy(h2[i].date,date);
    strcpy(h2[i].birth,birth);
    strcpy(h2[i].illness,illness);
    strcpy(h2[i].address,address);
    strcpy(h2[i].bloodType,bloodtype);
    h2[i].flag = 1;
    size++;

}

///////////////////////////////////////////////////////////////////////////////////////////////
void display(hash *h)//diplay on the screen according to the flage
{

    for(int i=0; i<maxsize; i++)
    {

        if( h[i].flag == 0)
        {
            printf("Not Occupied\n");

        }
        else if(h[i].flag == 2){

            printf("Deleted\n");

        }
        else
            printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",h[i].name,h[i].gender,h[i].date,h[i].birth,h[i].illness,h[i].address,h[i].bloodType);

    }


}
/////////////////////////////////////////////////////////////////////////////////////////////
void fillHashtable(FILE *f,hash *hashtable)//fill the hash table by reading from the file patients_hash.data and fill the hash table
{

    int count = 0;
    char line[max];
    char name[max] = "";
    char gender[max] ="";
    char date[max]="";
    char birth[max]="";
    char illness[max] = "";
    char address[max]="";
    char bloodType[max]="";


    f = fopen("patients_hash.data","r");

    if(f == NULL)
    {
        printf("there is an error with the file\n");
        exit(0);
    }

    char *token,*token1,*token2,*token3,*token4,*token5,*token6;



    while(fgets(line,max,f) != NULL)
    {

        token = strtok(line,"#");
        strcpy(name,token);


        token1 = strtok(NULL,"#");
        strcpy(gender,token1);


        token2 = strtok(NULL,"#");
        strcpy(date,token2);


        token3 = strtok(NULL,"#");
        strcpy(birth,token3);


        token4 = strtok(NULL,"#");
        strcpy(illness,token4);


        token5 = strtok(NULL,"#");
        strcpy(address,token5);


        token6 = strtok(NULL,"\n");
        strcpy(bloodType,token6);




        insert_hash(hashtable,name,gender,date,birth,illness,address,bloodType);//insert data to hash line by line

    }

    fclose(f);
}
////////////////////////////////////////////////////////////////////////////////////////////////
void insertnewToHash(hash *h)//insert new element to the hash
{

    char name[max],gender[max], illness[max],address[max],bloodType[max];
    char date[max],birth[max];

    printf("please enter the patient name\n");
    gets(name);
    printf("please enter the patient gender\n");
    gets(gender);
    printf("enter the patient's date of addmission\n");
    gets(date);
    printf("enter the date of birth\n");
    gets(birth);
    printf("please enter the illness of the patient\n");
    gets(illness);
    printf("please enter the address of the patient\n");
    gets(address);
    printf("please enter the blood type for the patient\n");
    gets(bloodType);



    insert_hash(h,name,gender,date,birth,illness,address,bloodType);//insert the local variables that we read from user

}
////////////////////////////////////////////////////////////////////////////////////////////////
void searchHash(hash *h,char name[])//search for a specific patient in hash table takes o(1).
{

int found = 0;
int index = getindex(name) % maxsize;

while(h[index].flag != 0){

    if(strcasecmp(h[index].name,name) == 0 ){

        printf("element found\n");
        printf("Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",h[index].name,h[index].gender,h[index].date,h[index].birth,h[index].illness,h[index].address,h[index].bloodType);
        found = 1;
            }

    index = (index + 1) % maxsize;

}

if(found == 0)//if element is not found tell the user
{

printf("Element not found\n");

}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void deleteHash(hash *h,char name[]){//delete from the hash table

int index = getindex(name) % maxsize;//find index according to table size
int found = 0;

while(h[index].flag != 0){

    if(strcasecmp(h[index].name,name) == 0){

         h[index].flag = 2;
         size--;
        //printf("%s  and   %s",h[index].name,h[index].address);
        found = 1;
            }

    index = (index + 1) % maxsize;//if collision happens use linear probing

}

if(found == 1){

    printf("item deleted successfully\n");
}
else{

    printf("item not found to be deleted\n");
}

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void saveHashFile(hash *h){//save data back to the file

FILE *in;
in = fopen("patients_hash.data","a");
fprintf(in,"////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
fprintf(in,"\n\nHash Table : \n");
for(int i=0; i<maxsize; i++)
    {

        if( h[i].flag == 0)
        {
            fprintf(in,"Occupied\n");

        }
        else if(h[i].flag == 2){

            fprintf(in,"Deleted\n");

        }
        else
            fprintf(in,"Name : %s||Gender : %s||Date : %s||Birth : %s||Illness : %s||Address : %s||Blood Type : %s\n",h[i].name,h[i].gender,h[i].date,h[i].birth,h[i].illness,h[i].address,h[i].bloodType);

    }

fclose(in);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void HashCases(hash *h)//hashing cases to choose from
{
    char name[max];
    char name1[max];
    char x[1];
    int choice = 0;
    printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////////");
    printf("\n HASH TABLE MENU : \n");

    while(1)
    {


        printf("1- Print out table size\n2- Print the used hashing function\n3- Insert a new record into the hash table\n4- Search for a specific patient\n5- Delete a specific record for a patient\n6- Save the hash table back to file.\n7- Exit The Program\n");


        gets(x);
        choice = atoi(x);

        if(choice > 7 || choice < 1){
            printf("Wrong Input\n");
        }



        switch(choice)
        {

        case 1://table size
            printf("table size is %d\n",maxsize);//print size
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");

            break;

        case 2://the used function
            printf("used hashing function for collision resolution is Linear Probing\n ");
            printf(" h(key) = (key + i) mod TableSize    where i = 0,1,2,...(when collision increment i)\n");//print used function
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            break;

        case 3://insert new nodes
            if(loadFactor() >= 0.7)//check for rehashing
                {

                h = Rehash(h);

                }
            insertnewToHash(h);
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            display(h);
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            break;


        case 4://search for a patient
            printf("searching\n");
            gets(name);
            searchHash(h,name);
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            break;


        case 5://delete a patient
            printf("deleting\n");
            gets(name1);
            deleteHash(h,name1);
            display(h);
            printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            break;


        case 6://save to file
           saveHashFile(h);
           printf("Saved To File!\n");
           printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
            break;


        case 7://exit
            printf("exiting\n");
            exit(0);
            break;

    }

}
}
////////////////////////////////////////////////////////////////////////////////////////////////
int main()//main function
{

    int count =0;//variables, tree nodes,hash table are initliazed.
    char name[max],line[max];
    char name1[max],name2[max];
    FILE *in;
    AVL_T t =NULL;
    AVL_T tree = NULL;
    char c[1];
    int choice = 0;
    hash *h;

     printf("\t\t\t\t\t\tWelcome!\n");
     printf("\t\t\t\t-------------------------------------------");
     printf("\n\t\t\t\t\t\tProject 3\n");
     printf("\t\t\t\t-------------------------------------------");

    while(1){


        printf("\n1- Read Patients From File\n2- Add Patient TO The Hospital\n3- Update Info For a Specific Patient\n4- List Patients In Lexicographical order\n5- Search Patients By Illness\n6- Delete a Patient From The Hospital\n7- Print Patients to file And Load Hash Table\n8- Exit The Program\n");


       gets(c);
       choice = atoi(c);

        if(choice > 8 || choice < 1){
            printf("Wrong Input\n");
        }

       switch(choice){//a list to choose from


       case 1://load data into the tree
         tree=ReadFile(in,tree);
         count = 1;
         printInOrder(tree);
         printf("file read succesfully\n");
         printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
          break;


       case 2://add patient to the tree
           if(count == 0){
            printf("\nRead From File First!\n");
           }

        else{
        tree = addPatient(tree);
        printf("///////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
        printInOrder(tree);
        printf("///////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
        }
        break;


       case 3://change info for a patient
            if(count == 0){
            printf("\nRead From File First!\n");
           }

        else{
        printf("enter the patient name that you want to change his name\n");
        gets(name);
        tree = changeInfo(tree,name);
        printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
        printInOrder(tree);
        printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
       }
        break;

       case 4://print in alphabetical order
        if(count == 0){
        printf("\nRead From File First!\n");
           }
         else{
        printf("printing in lexicographical order\n");
        printInOrder(tree);
        printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
          }
        break;

       case 5://search for illness
         if(count == 0){
            printf("\nRead From File First!\n");
           }
        else{
        printf("enter the illness to search for all patients\n");
        gets(name1);
        printPreOrderillness(tree,name1);
        printf("////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
         }
        break;


       case 6:
         if(count == 0){
            printf("\nRead From File First!\n");
           }
           else{
       printf("please enter the patient name you wish to delete\n");
       gets(name2);
       tree = Delete(tree,name2);
       printInOrder(tree);
       printf("//////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");
           }
       break;

       case 7:
     if(count == 0){
            printf("\nRead From File First!\n");
           }

      else{
    printInOrder(tree);
    printf("\n\nprint to file\n\n");

    FILE *f;
    f = fopen("patients_hash.data","w");
    fprintFile(tree,f);
    fclose(f);

/////////////////////////////////////////////////////////////
    f = fopen("patients_hash.data","r");
    while(fgets(line,max,f) != NULL)
    {
        count++;
    }

    maxsize = ceil((double)count/0.7);



    while(isprime(maxsize) == 0)
    {
        maxsize++;
    }

    fclose(f);
/////////////////////////////////////////////////////////
    //printf("%d\n",maxsize);

    h = (hash *)malloc(maxsize*sizeof(hash));

    for(int i=0; i<maxsize; i++)
    {
        h[i].flag = 0;
    }

    fillHashtable(f,h);

    f = fopen("patients_hash.data","w");
    fprintf(f,"");
    fclose(f);



    ///////////////////////////////////////////////////////
    f = fopen("patients_hash.data","w");
    fprintf(f,"AVL Tree Data :\n\n");
    fprintFileprime(tree,f);
    fclose(f);


    printf("/////////////////////////////////////////////////////////////////////////////////////////////////////////////////'n");
    printf("\n\n");
    printf("welcome to Hashing!\n");
    printf("*NOTE: desired load factor is 0.7\n");
    printf("/////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n");

    display(h);
    HashCases(h);
      }
    break;


       case 8:

        exit(0);
        break;
       }

     }

///////////////////////////////////////////////////////////////////////////////////
    return 0;
}


