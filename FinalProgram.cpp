// All Operations.
#include <bits/stdc++.h>
using namespace std;

// FixedPointInteger Class.
class FloatingPointNumber{
    private:
    int left,right;				// Stores number of integer and fractional bits.
    int *integer,*floating;                   // Stores the actual integer and fractional data.
    int accumulator;                          // Stores the guard bit.
    
    public:
    FloatingPointNumber(int left,int right){
        this->left=left;
        this->right=right;
        integer=new int[this->left];
        floating=new int[this->right];
    }
    double minimum(){                        // Returns the minimum possible value that can be stored in the given Qleft,right format.
        return -1*pow(2,left-1);
    }
    double maximum(){                       // Returns the maximum possible value that can be stored in the given Qleft,right format.
        double ans=0;
        for(int i=1;i<=right;i++){
            ans+=(pow(2,-i));
        }
        for(int i=0;i<left-1;i++){
            ans+=pow(2,i);
        }
        return ans;
    }
    double stepSize(){                     // Returns the stepsize ( ie difference between two consecutive values that can be stored in the given Qleft,right format.
        int range=pow(2,left+right)-1;
        return (maximum()-minimum())/range;
    }
    void print(){                         // Prints all the values that can be represented in the given Qleft,right format.
        double mini=minimum(); double maxi=maximum(); double step=stepSize();
        for(double i=mini;i<=maxi;i+=step){
            cout<<i<<" ";
        }
        cout<<endl;
    }
    int getLeft(){                      // Returns the number of integer bits.
        return this->left;
    }
    int getRight(){                    // Returns the number of fractional bits.
        return this->right;
    }
    int* getInteger(){                // Returns the integral bits.
        return this->integer;
    }
    int* getFloating(){              // Returns the fractional bits.
        return this->floating;
    }
    int getaccumulator(){           // Returns the bit stored in accumulator/guard bit.
        return this->accumulator;
    }
    void setInteger(int *integer){ // Sets the integer bits.
        this->integer=integer;
    }
    void setFloating(int *floating){ // Sets the fractional bits.
        this->floating=floating;
    }
    void setAccumulator(int carry){ // Sets the value of accumulatot/guardbit.
        this->accumulator=carry;
    }
    double getDecimal(){           // Returns the decimal value stored in the Fixed-Point number.
        double ans=this->accumulator*pow(2,this->left)*-1;
        if(this->integer[0]==1){
            if(this->accumulator==0) ans+=(pow(2,this->left-1)*-1);
            else ans+=(pow(2,this->left-1));
        }
        for(int i=this->left-1;i>0;i--){
            ans+=(pow(2,this->left-1-i)*this->integer[i]);
        }
        int k=-1;
        for(int i=0;i<this->right;i++){
            ans+=(pow(2,k)*this->floating[i]); k--;
        }
        return ans;
    }
};
// Operations class: Addition, multiplication, division, log2, log10, sine, cosine, tan, a^x.
class Operations{
    public:
    FloatingPointNumber* num1,*num2;  
    
    Operations(FloatingPointNumber*num1, FloatingPointNumber*num2){
        this->num1=num1;
        this->num2=num2;
    }
    
    // Converts the fixed point number with Qoldl,oldr format to Qnewl,newr format. The value is retained.
    FloatingPointNumber* adjust(int oldl,int oldr,int newl,int newr,FloatingPointNumber*num){
        if(oldl==newl && oldr==newr){
            return num;
        }
        int* newinteger=new int[newl];
        int* newfloating=new int[newr];
        
        int* oldinteger=num->getInteger();
        int* oldfloating=num->getFloating();
        
        int i=oldl-1; int j=newl-1;
        while(i>=0 && j>=0){
            newinteger[j--]=oldinteger[i--];            // Copies the integer part.
        }
        while(j>=0){
            newinteger[j--]=1;                          // Sign extension.
        }
        
        i=0; j=0;
        while(i<oldr && j<newr){
            newfloating[j++]=oldfloating[i++];         // Copies the float part.
        }
        while(j<newr){
            newfloating[j++]=0;                       // Padding with zero does not change the value.
        }
        
        FloatingPointNumber* ans=new FloatingPointNumber(newl,newr);
        ans->setInteger(newinteger);
        ans->setFloating(newfloating);
        return ans;
    }
    
    // Performs the addition between two signed fixed point numbers.
    FloatingPointNumber* addition(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);
        FloatingPointNumber* ans=new FloatingPointNumber(newl,newr);       // Initializing the result.
        
        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);     // Adjusting fixed point number 1.
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);     // Adjusting fixed point number 2.
        
        int* newnum1integer=newnum1->getInteger();                               
        int* newnum1floating=newnum1->getFloating();
        
        int* newnum2integer=newnum2->getInteger();
        int* newnum2floating=newnum2->getFloating();
        
        int* ansinteger=new int[newl];
        int* ansfloating=new int[newr];
        
        for(int i=0;i<newl;i++) cout<<newnum1integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum1floating[i];
        cout<<endl;
        
        for(int i=0;i<newl;i++) cout<<newnum2integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum2floating[i];
        cout<<endl;

        
        int carry=0;
        for(int i=newr-1;i>=0;i--){                                                  // Addition operation starts from LSB bit of fractional part.
            int temp=newnum1floating[i]+newnum2floating[i]+carry;                          
            if(temp==3){
                carry=1;
                ansfloating[i]=1;
            }
            else if(temp==2){
                carry=1;
                ansfloating[i]=0;
            }
            else if(temp==1){
                carry=0;
                ansfloating[i]=1;
            }
            else{
                carry=0;
                ansfloating[i]=0;
            }
        }
        for(int i=newl-1;i>=0;i--){                                                   // Addition in the integer part.
            int temp=newnum1integer[i]+newnum2integer[i]+carry;
            if(temp==3){
                carry=1;
                ansinteger[i]=1;
            }
            else if(temp==2){
                carry=1;
                ansinteger[i]=0;
            }
            else if(temp==1){
                carry=0;
                ansinteger[i]=1;
            }
            else{
                carry=0;
                ansinteger[i]=0;
            }
        }
        ans->setInteger(ansinteger); 
        ans->setFloating(ansfloating);
        ans->setAccumulator(carry);
        
        return ans;
        
    }
    
    // Performs the multiplication of two fixed point numbers.
    FloatingPointNumber * multiply(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);

        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);              // Adjusting fixed point number 1.
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);              // Adjusting fixed point number 2.
        
        int* newnum1integer=newnum1->getInteger();
        int* newnum1floating=newnum1->getFloating();
        
        int* newnum2integer=newnum2->getInteger();
        int* newnum2floating=newnum2->getFloating();
        
        /*for(int i=0;i<newl;i++) cout<<newnum1integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum1floating[i];
        cout<<endl;
        
        for(int i=0;i<newl;i++) cout<<newnum2integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum2floating[i];
        cout<<endl;*/


        vector<int> v1((newl+newr)*2,0); vector<int>v2((newl+newr)*2,0);                  // Copying the entire number into a single vector.
        int j=newl+newr;
        for(int i=0;i<newl;i++) v1[j++]=newnum1integer[i];
        for(int i=0;i<newr;i++) v1[j++]=newnum1floating[i];
        j=newl+newr;
        for(int i=0;i<newl;i++) v2[j++]=newnum2integer[i];
        for(int i=0;i<newr;i++) v2[j++]=newnum2floating[i];

        vector<vector<int>> products;                                                    // Stores the partial products.
        
        int k=0;
        for(int i=2*(newl+newr)-1;i>newl+newr;i--){
            if(v2[i]==1){
                vector<int> partial((newl+newr)*2,0); 
                int j;
                for(j=v1.size()-1;j>=newl+newr;j--){                                    // When num2th i th bit is 1, copy num1 and sign extend it. becuase it is a signed number.
                    partial[j-k]=v1[j];
                }
                //Sign Extending.
                for(;j>=0;j--){
                    partial[j]=1;
                }
                products.push_back(partial);
            }
            k++;
        }
        vector<int> partial((newl+newr)*2,0);					// Last parital product is find by 2's complement of num1 preceded by a 1.
        for(int j=v1.size()-1;j>=newl+newr;j--){
            partial[j-k]=v1[j];
        }
        partial[0]=1;
        int temp=(newl+newr)*2-1;
        while(partial[temp]!=1){
            temp--;
        }
        temp--;
        while(temp>=0){
            partial[temp]=1-partial[temp]; temp--;
        }
        products.push_back(partial);
        
        vector<int>ans=products[0];
        int carry=0; k=1;
        for(int m=1;m<=products.size()-1;m++){			// Loops to add the partial products.
            if(k>=products.size()) break;
            for(int i=products[k].size()-1;i>=0;i--){
                int temp=ans[i]+products[k][i]+carry;
                if(temp==3){
                    carry=1;
                    ans[i]=1;
                }
                else if(temp==2){
                    carry=1;
                    ans[i]=0;
                }
                else if(temp==1){
                    carry=0;
                    ans[i]=1;
                }
                else{
                    carry=0;
                    ans[i]=0;
                }
            }
            k++;
        }
        

        FloatingPointNumber *finalAns =  new FloatingPointNumber(2*newl,2*newr);          // Copying back the result to the fixedpoint number.
        int* integerPart=new int[2*newl];
        int* floatPart=new int[2*newr];
        k=0; int m=0;
        while(k<2*newl) integerPart[m++]=ans[k++];
        m=0;
        while(k<2*(newl+newr)) floatPart[m++]=ans[k++];
        
        finalAns->setInteger(integerPart); 
        finalAns->setFloating(floatPart);
        finalAns->setAccumulator(carry);
        
        return finalAns;
    }
    // Multiplication of unsigned numbers.
    FloatingPointNumber * multiply1(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);

        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);
        
        int* newnum1integer=newnum1->getInteger();
        int* newnum1floating=newnum1->getFloating();
        
        int* newnum2integer=newnum2->getInteger();
        int* newnum2floating=newnum2->getFloating();
        

        vector<int> v1((newl+newr)*2,0); vector<int>v2((newl+newr)*2,0);
        int j=newl+newr;
        for(int i=0;i<newl;i++) v1[j++]=newnum1integer[i];
        for(int i=0;i<newr;i++) v1[j++]=newnum1floating[i];
        j=newl+newr;
        for(int i=0;i<newl;i++) v2[j++]=newnum2integer[i];
        for(int i=0;i<newr;i++) v2[j++]=newnum2floating[i];

        vector<vector<int>> products;
        
        int k=0;
        for(int i=2*(newl+newr)-1;i>=newl+newr;i--){
            if(v2[i]==1){
                vector<int> partial((newl+newr)*2,0); 
                int j;
                for(j=v1.size()-1;j>=newl+newr;j--){
                    partial[j-k]=v1[j];
                }
                products.push_back(partial);
            }
            k++;
        }

        vector<int>ans=products[0];
        int carry=0; k=1;
        for(int m=1;m<=products.size()-1;m++){
            if(k>=products.size()) break;
            for(int i=products[k].size()-1;i>=0;i--){
                int temp=ans[i]+products[k][i]+carry;
                if(temp==3){
                    carry=1;
                    ans[i]=1;
                }
                else if(temp==2){
                    carry=1;
                    ans[i]=0;
                }
                else if(temp==1){
                    carry=0;
                    ans[i]=1;
                }
                else{
                    carry=0;
                    ans[i]=0;
                }
            }
            k++;
        }
        

        FloatingPointNumber *finalAns =  new FloatingPointNumber(2*newl,2*newr);
        int* integerPart=new int[2*newl];
        int* floatPart=new int[2*newr];
        k=0; int m=0;
        while(k<2*newl) integerPart[m++]=ans[k++];
        m=0;
        while(k<2*(newl+newr)) floatPart[m++]=ans[k++];
        
        finalAns->setInteger(integerPart); 
        finalAns->setFloating(floatPart);
        finalAns->setAccumulator(0);
        
        return finalAns;
        
    }
    
    // Returns the power of a fixed point number. num^x.
    double power(FloatingPointNumber *num,int x){
        int l=num->getLeft(); int r=num->getRight();
        FloatingPointNumber *ans=new FloatingPointNumber(l,r);
        int ansinteger[l]; int ansfloat[r];
        for(int i=0;i<l;i++) ansinteger[i]=0;
        for(int i=0;i<r;i++) ansfloat[i]=0;
        ansinteger[l-1]=1;
        ans->setInteger(ansinteger); ans->setFloating(ansfloat);
        double powerans=num->getDecimal()*(0.0174533); double temp=num->getDecimal()*(0.0174533);
        for(int i=2;i<=x;i++){
            powerans*=temp;
        }
        /*for(int i=1;i<=x;i++){
            Operations *op = new Operations(num,ans);
            ans=op->multiply1();
        }*/
        return powerans;
    }
    
    // Returns the factorial of a number.
    int factorial(int x){
        int ans=1;
        for(int i=1;i<=x;i++) ans*=i;
        return ans;
    }
    
    // Returns the sine of a fixed point number using series expansion.
    double sine(FloatingPointNumber *num){
        double ans=num->getDecimal()*(0.0174533);
        bool b=false; int x=3;
        for(int i=1;i<=10;i++){
            if(!b){
                ans=ans-(this->power(num,x)/this->factorial(x));
            }
            else{
                ans=ans+(this->power(num,x)/this->factorial(x));
            }
            b=!b; x+=2;
        }
        return ans;
    }
    
    // Returns the cosine of a fixed point number using series expansion.
    double cosine(FloatingPointNumber *num){
        double ans=1;
        bool b=false; int x=2;
        for(int i=1;i<=10;i++){
            if(!b){
                ans=ans-(this->power(num,x)/this->factorial(x));
            }
            else{
                ans=ans+(this->power(num,x)/this->factorial(x));
            }
            b=!b; x+=2;
        }
        return ans;

    }
    
    // Returns the tan of a fixed point number using formula sin(x)/cos(x).
    double tan(FloatingPointNumber *num){
        return this->sine(num)/this->cosine(num);
    }
    
    // Returns the logartihm of a fixed point number to the base2.
    double log2(FloatingPointNumber *num){
        int decimal=num->getDecimal();
        int ans=0;
        while(decimal>0){
            ans++; decimal/=2;
        }
        return ans;
    }
    
    // Returns the logartihm of a fixed point number to the base10.
    double log10(FloatingPointNumber *num){
        int decimal=num->getDecimal();
        int ans=0;
        while(decimal>0){
            ans++; decimal/=10;
        }
        return ans;

    }
    
    // Returns the a^x, where a is constant and x is a fixed point number.
    double f(FloatingPointNumber *constant,FloatingPointNumber *real){
        double a=constant->getDecimal(); double x=real->getDecimal();
        return pow(a,x);
    }
    double division(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);

        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);
        
        int* newnum1integer=newnum1->getInteger();
        int* newnum1floating=newnum1->getFloating();
        
        int* newnum2integer=newnum2->getInteger();
        int* newnum2floating=newnum2->getFloating();
        
        return num1->getDecimal()/num2->getDecimal();
    }
    
};
int main() {
    FloatingPointNumber *n1=new FloatingPointNumber(3,3);
    FloatingPointNumber *n2=new FloatingPointNumber(3,3);
    
    int n1integer[3]={0,1,1}; int n1floating[3]={0,0,0};      // unsigned: 2.5  , Signed:-1.5
    int n2integer[3]={0,1,1}; int n2floating[3]={1,0,0}; //  unsigned: 4.5  , Signed:-3.5
    
    n1->setInteger(n1integer); n1->setFloating(n1floating);
    n2->setInteger(n2integer); n2->setFloating(n2floating);
    
    cout<<n1->getDecimal()<<" "<<n2->getDecimal()<<endl;
    
    Operations *op=new Operations(n1,n2);
    /*FloatingPointNumber * ans = op->multiply();
    
    int* ansInteger=ans->getInteger(); 
    int* ansFloating=ans->getFloating();
    
    ans->setAccumulator(0);
    cout<<ans->getaccumulator();
    for(int i=0;i<ans->getLeft();i++) cout<<ansInteger[i];
    cout<<".";
    for(int i=0;i<ans->getRight();i++) cout<<ansFloating[i];
    cout<<endl;
    cout<<ans->getDecimal();*/
    
    cout<<"SINE:: ";
    cout<<op->sine(n1)<<endl;
    cout<<"COSINE:: ";
    cout<<op->cosine(n1)<<endl;
    cout<<"TAN:: ";
    cout<<op->tan(n1)<<endl;
    cout<<"LOG2:: ";
    cout<<op->log2(n2)<<endl;
    cout<<"LOG10:: ";
    cout<<op->log10(n2)<<endl;
    cout<<"a^x:: ";
    cout<<op->f(n1,n2)<<endl;
    
    cout<<op->division();
}
