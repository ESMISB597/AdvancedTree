#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include "fundamental_tree.h"
#include "stack.h"
//driver routine
int main() 
{
  //A sample use of these functions.  Start with the empty tree
  //insert some stuff into it, and then delete it
  
  Tree * root;
  int i;
  int value;
  int select;
  root = NULL;
  
  // clrscr();
  make_empty(root);
  
  root = initialTree(root);

  do{
    printf("\n\nMENU: 1: Insert, 2: Delete, 3: Undo, 4. History 5: Exit\n");
    printf("Select: ");
    fflush(stdin);
    scanf("%d", &select);
    switch(select){
      case 1: printf("Insert value: \n");
		scanf("%d", &value);
		root = insert(value, root);

    /* HISTORY ADDED */
    push(value,'I');
    /* HISTORY ADDED */

		print_ascii_tree(root);
		break;
      case 2: printf("Delete value: \n");
		scanf("%d", &value);

    /* HISTORY ADDED */
    push(value,'D');
    /* HISTORY ADDED */

		root = deletenode(value, root);
		print_ascii_tree(root);
		break;
      case 3: printf("Undo: \n");
      
    	/* UNDO ADDED */
    	if(stp[top].indicate == 'I')
		{
    		printf("Delete %d\n",stp[top].stack);
    		root = deletenode(stp[top].stack, root);
    		pop();
		}else if(stp[top].indicate == 'D'){
			printf("Insert %d\n",stp[top].stack);
			root = insert(stp[top].stack, root);
			pop();
		}else{
			
		}
    	/* UNDO ADDED */
    	
		print_ascii_tree(root);
		break;
      case 4: printf("History: \n");
      	if(top == -1)
      	{
      		printf("--- HISTORY EMPTY !! ---\n");
		}else{
			
		}
        peek();
		break;
      default:  break;
    }
  }while(select != 5);

  make_empty(root);
  return 0;
}
