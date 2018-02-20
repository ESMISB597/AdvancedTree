// http://web.archive.org/web/20071224095835/http://www.openasthra.com/wp-content/uploads/2007/12/binary_trees1.c
#include <stdio.h>
// #include<conio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Tree Tree;

struct Tree 
{
  Tree * left, * right;
  int element;
};

Tree *make_empty(Tree *t)
{
  if (t != NULL)
  {
    make_empty(t->left);
    make_empty(t->right);
    free(t);
  }

  return NULL;
}

Tree *find_min(Tree *t)
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (t->left == NULL)
  {
    return t;
  }
  else
  {
    return find_min(t->left);
  }
}

Tree *find_max(Tree *t)
{
  if (t == NULL)
  {
    return NULL;
  }
  else if (t->right == NULL)
  {
    return t;
  }
  else
  {
    return find_max(t->right);
  }
}

Tree *find(int elem, Tree *t)
{
  if (t == NULL)
  {
    return NULL;
  }

  if (elem < t->element)
  {
    return find(elem, t->left);
  }
  else if (elem > t->element)
  {
    return find(elem, t->right);
  }
  else
  {
    return t;
  }
}

//Insert i into the tree t, duplicate will be discarded
//Return a pointer to the resulting tree.                 
Tree * insert(int value, Tree * t) 
{
  Tree * new_node;
    
  if (t == NULL) 	// insert the first node
  {
    new_node = (Tree *) malloc (sizeof (Tree));
    if (new_node == NULL) 
    {
	    return t;
    }

    new_node->element = value;

	new_node->left = new_node->right = NULL;
	return new_node;
  }
  
  if (value < t->element) 		// less than
  {
    t->left = insert(value, t->left);
  } 
  else if (value > t->element) 	// greater than
  {
	  t->right = insert(value, t->right);
  } 
  else	// equal
  { 
    //duplicate, ignore it
	  return t;
  }
  return t;
}

Tree * deletenode(int value, Tree * t)
{
  // Deletes node from the tree
  // Return a pointer to the resulting tree
  Tree * x;
  Tree *tmp_cell;
  
  if (t==NULL) return NULL;
  
  if (value < t->element){
    t->left = deletenode(value, t->left);
  } 
  else if (value > t->element){
	t->right = deletenode(value, t->right);
  } 
  // the followings (four cases) are when we found the search value
  else if (t->left && t->right){	// case 1 (t:LR): t has both left and right children
    tmp_cell = find_min(t->right);
    t->element = tmp_cell->element;
    t->right = deletenode(t->element, t->right);
  }
  else{ 
    tmp_cell = t;
    if (t->left == NULL)			// case2-3 (t:xR, t:xx): t does not have left child, with/without right child node
      t = t->right;				       // t->right is also null, when t does not have children
    else if (t->right == NULL)		// case4 (t:Lx): t does not have right child, with left child node
      t = t->left;				   
    free(tmp_cell);
  }

  return t;
}


//printing tree in ascii

typedef struct asciinode_struct{
  struct asciinode_struct * left, * right;

  //length of the edge from this node to its children
  int edge_length;

  int height;

  int lablen;

  //-1=I am left, 0=I am root, 1=right
  int parent_dir;

  //max supported unit32 in dec, 10 digits max
  char label[11];
} asciinode;


#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

//adjust gap between left and right nodes
int gap = 3;

//used for printing next node in the same level,
//this is the x coordinate of the next char printed
int print_next;

int MIN (int X, int Y)
{
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)
{
  return ((X) > (Y)) ? (X) : (Y);
}

asciinode * build_ascii_tree_recursive(Tree * t)
{
  asciinode * node;

  if (t == NULL) return NULL;

  node = (asciinode *)malloc(sizeof(asciinode));
  node->left = build_ascii_tree_recursive(t->left);
  node->right = build_ascii_tree_recursive(t->right);

  if (node->left != NULL)
  {
    node->left->parent_dir = -1;
  }

  if (node->right != NULL)
  {
    node->right->parent_dir = 1;
  }

  sprintf(node->label, "%d", t->element);
  node->lablen = strlen(node->label);

  return node;
}


//Copy the tree into the ascii node structre
asciinode * build_ascii_tree(Tree * t)
{
  asciinode *node;
  if (t == NULL) return NULL;
  node = build_ascii_tree_recursive(t);
  node->parent_dir = 0;
  return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
  if (node == NULL) return;
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
  int i, isleft;
  if (node == NULL) return;
  isleft = (node->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
  if (node->left != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    lprofile[y+i] = MIN(lprofile[y+i], x-i);
    }
  }
  compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
  int i, notleft;
  if (node == NULL) return;
  notleft = (node->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
  if (node->right != NULL)
  {
	  for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
    {
	    rprofile[y+i] = MAX(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
  compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
  int h, hmin, i, delta;
  if (node == NULL) return;
  compute_edge_lengths(node->left);
  compute_edge_lengths(node->right);

  /* first fill in the edge_length of node */
  if (node->right == NULL && node->left == NULL)
  {
	  node->edge_length = 0;
  }
  else
  {
    if (node->left != NULL)
    {
	    for (i=0; i<node->left->height && i < MAX_HEIGHT; i++)
      {
		    rprofile[i] = -INFINITY;
	    }
	    compute_rprofile(node->left, 0, 0);
	    hmin = node->left->height;
    }
    else
    {
	    hmin = 0;
    }
	  if (node->right != NULL)
    {
	    for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
      {
		    lprofile[i] = INFINITY;
	    }
	    compute_lprofile(node->right, 0, 0);
	    hmin = MIN(node->right->height, hmin);
    }
    else
    {
	    hmin = 0;
    }
	  delta = 4;
	  for (i=0; i<hmin; i++)
    {
	    delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }

    //If the node has two children of height 1, then we allow the
    //two leaves to be within 1, instead of 2
	  if (((node->left != NULL && node->left->height == 1) ||
	      (node->right != NULL && node->right->height == 1))&&delta>4)
    {
      delta--;
    }

    node->edge_length = ((delta+1)/2) - 1;
  }

  //now fill in the height of node
  h = 1;
  if (node->left != NULL)
  {
	  h = MAX(node->left->height + node->edge_length + 1, h);
  }
  if (node->right != NULL)
  {
	  h = MAX(node->right->height + node->edge_length + 1, h);
  }
  node->height = h;
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
  int i, isleft;
  if (node == NULL) return;
  isleft = (node->parent_dir == -1);
  if (level == 0) 
  {
	  for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++) 
    {
	    printf(" ");
    }
	  print_next += i;
	  printf("%s", node->label);
	  print_next += node->lablen;
  } 
  else if (node->edge_length >= level) 
  {
	  if (node->left != NULL) 
    {
	    for (i=0; i<(x-print_next-(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("/");
	    print_next++;
    }
	  if (node->right != NULL) 
    {
	    for (i=0; i<(x-print_next+(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("\\");
	    print_next++;
    }
  } 
  else 
  {
	  print_level(node->left, 
                x-node->edge_length-1, 
                level-node->edge_length-1);
	  print_level(node->right, 
                x+node->edge_length+1, 
                level-node->edge_length-1);
  }
}

//prints ascii tree for given Tree structure
void print_ascii_tree(Tree * t) 
{
  asciinode *proot;
  int xmin, i;
  if (t == NULL) return;
  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);
  for (i=0; i<proot->height && i < MAX_HEIGHT; i++) 
  {
	  lprofile[i] = INFINITY;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) 
  {
	  xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++) 
  {
	  print_next = 0;
	  print_level(proot, -xmin, i);
	  printf("\n");
  }
  if (proot->height >= MAX_HEIGHT) 
  {
	  printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot); 
}

Tree* initialTree(Tree *root){
  int treeData[] = {20, 12, 30, 15, 40, 10, 25};
  int value;
  int i;

  // initial tree fron treeData[]
  int sizeOfTree = sizeof(treeData)/sizeof(treeData[0]);
  for(i = 0; i < sizeOfTree; i++){
    value = treeData[i];
	root = insert(value, root);
  }
  print_ascii_tree(root);
  return root;
}

// INITIALIZE STACK 
int MAXSIZE = 30;
int top = -1;
int loop1;

// STRUCTURE FOR STACK //       
struct DataStack{
    int stack;
    char indicate;
}stp[30];  
// STRUCTURE FOR STACK //       

// BOOL EMPTY //
int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
// BOOL EMPTY //

// BOOL FULL //   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}
// BOOL FULL //   

// SEE HISTORY //
int history() {
  for(loop1 = 0 ; loop1 <= top ; loop1++)
  {
      if(stp[loop1].indicate == 'I')
      {
          printf("%d Insert %d\n",loop1,stp[loop1].stack);
      }else if(stp[loop1].indicate == 'D'){
          printf("%d Delete %d\n",loop1,stp[loop1].stack);
      }else{
      }
  }
}
// SEE HISTORY //

// TRUNCATE STACK //
int undo() {
   int data;
   char datinc;

   if(!isempty()) {
      data = stp[top].stack;
      datinc == stp[top].indicate;
      top = top - 1;   
   } else {
      printf("Could not retrieve data, Stack is empty.\n");
   }
}
// TRUNCATE STACK //

// PUSH IN STACK //
int incoming(int data, char datinc) {

   if(!isfull()) {
      top = top + 1;   
      stp[top].stack = data;
      stp[top].indicate = datinc;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
  return 3;
}
// PUSH IN STACK //

// EOINT

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
      case 1: printf("Insert value: ");
		scanf("%d", &value);
		root = insert(value, root);
		print_ascii_tree(root);
		// ADD HISTORY //
    incoming(value,'I'); /* <-- 'I' is meaning insert is a bool */
    // ADD HISTORY //
		break;
      case 2: printf("Delete value: ");
		scanf("%d", &value);
		root = deletenode(value, root);
		print_ascii_tree(root);
		// DELETE HISTORY //
    incoming(value,'D'); /* <-- 'D' is meaning insert is a bool */
    // DELETE HISTORY //
		break;
      case 3: printf("Undo: ");
    // REVERT //
    if(stp[top].indicate == 'I')
    {
      root = deletenode(stp[top].stack, root);
      /* SHOW What deleted */
      printf("Deleted %d ",stp[top].stack, root);
      /* SHOW What deleted */
      undo();
    }else if(stp[top].indicate == 'D'){
      root = insert(stp[top].stack, root);
      /* SHOW What inserted */
      printf("Inserted %d ",stp[top].stack, root);
      /* SHOW What inserted */
      undo();
    }else{

    }
		print_ascii_tree(root);
		break;
      case 4: printf("History: \n");
    history();
		break;
      default:  break;
    }
  }while(select != 5);

  make_empty(root);
  return 0;
}
