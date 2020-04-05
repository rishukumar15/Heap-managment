#include<stdio.h>
#include<stdlib.h> 

int count;

typedef struct Memory_tag
{
	int lb;
	int ub;
}memory;
 
typedef struct Node_tag
{
	int size;
	memory m;
 	struct Node_tag* next;
}Node;


int poweroftwo(int n)
{
	int prod=1,i=0;
	while(prod<n)
	{
		prod=prod*2;
		i++;
	}
	return i;
}
  
void AddAlloclist(Node **Allocl,Node *r,int x)
{
	Node *lptr;
	lptr=(Allocl[x]);
	r->next=NULL;
	if(lptr==NULL)
	{
		(Allocl[x])=r;
		
	}
	else
	{
		while(lptr->next!=NULL)
		{
		  lptr=lptr->next;	
	    }
		lptr->next=r;   
	}
  } 
  
 void RemoveFreelist(Node **freel,int x)
 {
 	Node *ptr;
 	ptr=(freel[x]);
 	free(ptr);
 	(freel[x])=NULL;
 
  } 
  
void Allocate(Node **Allocl,Node **freel,int n,int size)
{
    Node *ptr,*p,*q,*r;
   
		int i,x,flag=0;
		x=poweroftwo(size);
	 
		ptr=(freel[x]);
		if(ptr!=NULL)
		{
			
			printf("Memory allocated from %d to %d \n",ptr->m.lb,ptr->m.ub);
			count=count-(ptr->size);
			printf(" Available Memory is %d \n",count);
			r=(Node*)malloc(sizeof(Node));
			r->size=ptr->size;
			r->m.lb=ptr->m.lb;
			r->m.ub=ptr->m.ub;
			r->next=NULL;
			AddAlloclist(Allocl,r,x);
			RemoveFreelist(freel,x);
		}
		else
		{
			int d,a;
            for(i=x+1;i<n && flag==0;i++)
			{
			    ptr=(freel[i]);
			  
				if(ptr!=NULL)
				{
				  flag=1;
				}
		    }
			if(flag==0)
			{
			  printf("Sorry, failed to allocate memory \n");	
			}
			else
			{
				i--;
				a=i;
				i--;
				for(;i>=x;i--)
				{
				  p=(Node*)malloc(sizeof(Node));
				  p->size=(ptr->size)/2;
			
				  p->m.ub=ptr->m.ub;
			
				  d=((ptr->m.ub)-(ptr->m.lb))+1;
				  d=d/2;
				  p->m.lb=(ptr->m.lb)+d;
	             p->next=NULL;
				   (freel[i])=p;
				   q=(Node*)malloc(sizeof(Node));
				  q->size=(ptr->size)/2;
			
				  q->m.lb=ptr->m.lb;
				 
				  d=(ptr->m.ub)-(ptr->m.lb);
				  d=d/2;
				  q->m.ub=(ptr->m.lb)+d;
				  q->next=NULL;
				  ptr=q;
				}
				printf("Memory allocated from %d to %d \n",ptr->m.lb,ptr->m.ub);
				count=count-(ptr->size);
			    printf(" Available Memory is %d \n",count);
				AddAlloclist(Allocl,ptr,i+1);
				RemoveFreelist(freel,a);
				
			}			
     	}
   
}

void AddFreelist(Node **freel,Node *ptr,int x)
{
	Node *prev,*curr,*next;
	int i=0,prod=1;
	while(i<x)
	{
		prod=prod*2;
		i++;
	}
	curr=freel[x];
	if(curr==NULL)
	{
	Node* p=(Node*)malloc(sizeof(Node));
	p->size=ptr->size;
	p->m.lb=ptr->m.lb;
	p->m.ub=ptr->m.ub;
	p->next=NULL;
	freel[x]=p;
	printf("Memory added from %d to %d in freelist of %d \n",p->m.lb,p->m.ub,prod);
	}
	else
	{
	 int a=0;
	 prev=NULL;
	 while(a==0 && curr!=NULL)
	 {
	 	next=curr->next;
	    if(curr->m.lb==(ptr->m.ub+1))
		a=1;
		else
		{
		  prev=curr;
		  curr=next;
		  	
		}	
	 }
	  if(a==0)
	  {
	   Node* p=(Node*)malloc(sizeof(Node));
	   p->size=ptr->size;
	   p->m.lb=ptr->m.lb;
	   p->m.ub=ptr->m.ub;
	   p->next=NULL;
	   prev->next=p;
	   printf("Memory added from %d to %d in freelist of %d \n",p->m.lb,p->m.ub,prod);	
	  }
	  else
	  {
			Node* q=(Node*)malloc(sizeof(Node));
	        q->size=2*(ptr->size);
	        q->m.lb=ptr->m.lb;
	        q->m.ub=curr->m.ub;
	        q->next=NULL;
	        AddFreelist(freel,q,x+1);
	        if(prev==NULL)
	        {
	        	freel[x]=next;
			}
	        else
	        {
	        	prev->next=next;
			}
			free(curr);
	        free(q);
	   }
   }
}



void Remove(Node **Allocl,Node **freel,int n,int size)
{
	int x;
	x=poweroftwo(size);
	Node* ptr,*prev;
	ptr=Allocl[x];
	if(ptr==NULL)
	{
		printf("Memory of such size is not allocated \n");
	}
	else
	{
		prev=NULL;
		while(ptr->next!=NULL)
		{
			prev=ptr;
			ptr=ptr->next;
		}
		AddFreelist(freel,ptr,x);
		count=count+(ptr->size);
		printf("Available memory is %d \n",count);
		printf("Memory free from %d to %d in alloclist \n",ptr->m.lb,ptr->m.ub);
		if(prev==NULL)
		{
			Allocl[x]=NULL;
		}
		else
		{
		prev->next=NULL;
		}
		free(ptr);
	}
}


void main()
{
	int initialsize,n,f=0,size,i,choice;
	printf("Enter the initial memory size \n");
	scanf("%d",&initialsize);
	n=poweroftwo(initialsize);
	printf("%d is value of power to 2 equivalent to %d \n",n,initialsize);
	Node *freel[n+1];
	Node *Allocl[n+1];
	count=initialsize;
	for(i=0;i<n+1;i++)
	{
		freel[i]=NULL;
		Allocl[i]=NULL;
	}
	Node* ptr=(Node*)malloc(sizeof(Node));
	ptr->size=initialsize;
	ptr->m.lb=0;
	ptr->m.ub=initialsize-1;
	ptr->next=NULL;
	(freel[n])=ptr;
	printf("%d is the free available memory \n",(freel[n])->size);
	while(f==0)
	{
		printf("Enter 1 for allocate ,2 for free and 0 for stop");
		scanf("%d",&choice);
		if(choice==0)
		{
		f=1;
		printf("Stopped");
	    }
		else if(choice==1)
		{
		 printf("Enter the size to be allocated \n");
		 scanf("%d",&size);
		 if(size>initialsize)
		 {
			printf("No such space available \n");
		 }
		 else
		  {
		  Allocate(Allocl,freel,n+1,size);
	      } 
	    }
	    else
		{
			printf("Enter the size to be free \n");
		    scanf("%d",&size);
			if(size>initialsize)
			{
				printf("No such memory allocated \n");
			}
			else
			{
				Remove(Allocl,freel,n+1,size);
			}
		}
    }
    
}
