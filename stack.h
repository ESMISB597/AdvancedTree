
int MAXSIZE = 30;
int top = -1;
int loop1;
       
struct DataStack{
    int stack;
    char indicate;
}stp[30];  

int isempty() {

   if(top == -1)
      return 1;
   else
      return 0;
}
   
int isfull() {

   if(top == MAXSIZE)
      return 1;
   else
      return 0;
}

int peek() {
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

int pop() {
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

int push(int data, char datinc) {

   if(!isfull()) {
      top = top + 1;   
      stp[top].stack = data;
      stp[top].indicate = datinc;
   } else {
      printf("Could not insert data, Stack is full.\n");
   }
	return 3;
}

