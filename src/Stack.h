template <class T,int stack_size> class StackFS {
private:
int mem[stack_size];
int stack_pointer;

public:
StackFS () : stack_pointer(-1)
{ }



void push(T val) {
   
       if (stack_pointer< stack_size) {
          stack_pointer++;
          mem[stack_pointer]=val;
       } 
    
}

T pop() {
     T val;
     if (stack_pointer>=0) {
         val=mem[stack_pointer];
         stack_pointer--;
     } else{
         val = -1;
     }
    
    return val;
}

bool empty() {

    return (stack_pointer=0);
}

int size() {
    if (stack_pointer + 1>= stack_size)
    return stack_size;
    else
    return stack_pointer+1;
    //return stack_pointer;
}	

};
