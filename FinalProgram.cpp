// All Operations.
#include <bits/stdc++.h>
using namespace std;

class FloatingPointNumber{
    private:
    int left,right;
    int *integer,*floating;
    int accumulator;
    
    public:
    FloatingPointNumber(int left,int right){
        this->left=left;
        this->right=right;
        integer=new int[this->left];
        floating=new int[this->right];
    }
    double minimum(){
        return -1*pow(2,left-1);
    }
    double maximum(){
        double ans=0;
        for(int i=1;i<=right;i++){
            ans+=(pow(2,-i));
        }
        for(int i=0;i<left-1;i++){
            ans+=pow(2,i);
        }
        return ans;
    }
    double stepSize(){
        int range=pow(2,left+right)-1;
        return (maximum()-minimum())/range;
    }
    void print(){
        double mini=minimum(); double maxi=maximum(); double step=stepSize();
        for(double i=mini;i<=maxi;i+=step){
            cout<<i<<" ";
        }
        cout<<endl;
    }
    int getLeft(){
        return this->left;
    }
    int getRight(){
        return this->right;
    }
    int* getInteger(){
        return this->integer;
    }
    int* getFloating(){
        return this->floating;
    }
    int getaccumulator(){
        return this->accumulator;
    }
    void setInteger(int *integer){
        this->integer=integer;
    }
    void setFloating(int *floating){
        this->floating=floating;
    }
    void setAccumulator(int carry){
        this->accumulator=carry;
    }
    double getDecimal(){
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
class Operations{
    public:
    FloatingPointNumber* num1,*num2;  
    
    Operations(FloatingPointNumber*num1, FloatingPointNumber*num2){
        this->num1=num1;
        this->num2=num2;
    }
    
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
            newinteger[j--]=oldinteger[i--];
        }
        while(j>=0){
            newinteger[j--]=1;
        }
        
        i=0; j=0;
        while(i<oldr && j<newr){
            newfloating[j++]=oldfloating[i++];
        }
        while(j<newr){
            newfloating[j++]=0;
        }
        
        FloatingPointNumber* ans=new FloatingPointNumber(newl,newr);
        ans->setInteger(newinteger);
        ans->setFloating(newfloating);
        return ans;
    }
    FloatingPointNumber* addition(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);
        FloatingPointNumber* ans=new FloatingPointNumber(newl,newr);
        
        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);
        
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
        for(int i=newr-1;i>=0;i--){
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
        for(int i=newl-1;i>=0;i--){
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
    
    FloatingPointNumber * multiply(){
        int l1=num1->getLeft(); int l2=num2->getLeft();
        int r1=num1->getRight(); int r2=num2->getRight();
        int newl=max(l1,l2); int newr=max(r1,r2);

        FloatingPointNumber* newnum1=this->adjust(l1,r1,newl,newr,num1);
        FloatingPointNumber* newnum2=this->adjust(l2,r2,newl,newr,num2);
        
        int* newnum1integer=newnum1->getInteger();
        int* newnum1floating=newnum1->getFloating();
        
        int* newnum2integer=newnum2->getInteger();
        int* newnum2floating=newnum2->getFloating();
        
        for(int i=0;i<newl;i++) cout<<newnum1integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum1floating[i];
        cout<<endl;
        
        for(int i=0;i<newl;i++) cout<<newnum2integer[i];
        cout<<".";
        for(int i=0;i<newr;i++) cout<<newnum2floating[i];
        cout<<endl;


        vector<int> v1((newl+newr)*2,0); vector<int>v2((newl+newr)*2,0);
        int j=newl+newr;
        for(int i=0;i<newl;i++) v1[j++]=newnum1integer[i];
        for(int i=0;i<newr;i++) v1[j++]=newnum1floating[i];
        j=newl+newr;
        for(int i=0;i<newl;i++) v2[j++]=newnum2integer[i];
        for(int i=0;i<newr;i++) v2[j++]=newnum2floating[i];

        vector<vector<int>> products;
        
        int k=0;
        for(int i=2*(newl+newr)-1;i>newl+newr;i--){
            if(v2[i]==1){
                vector<int> partial((newl+newr)*2,0); 
                int j;
                for(j=v1.size()-1;j>=newl+newr;j--){
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
        vector<int> partial((newl+newr)*2,0);
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
        
        for(int i=0;i<products.size();i++){
            for(int j=0;j<products[i].size();j++){
                cout<<products[i][j];
            }
            cout<<endl;
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
        finalAns->setAccumulator(carry);
        
        return finalAns;
        
    }
    
};
int main() {
    FloatingPointNumber *n1=new FloatingPointNumber(2,3);
    FloatingPointNumber *n2=new FloatingPointNumber(2,3);
    
    int n1integer[2]={1,1}; int n1floating[3]={0,0,1};      // unsigned: 2.5  , Signed:-1.5
    int n2integer[2]={1,0}; int n2floating[3]={0,1,0}; //  unsigned: 4.5  , Signed:-3.5
    
    n1->setInteger(n1integer); n1->setFloating(n1floating);
    n2->setInteger(n2integer); n2->setFloating(n2floating);
    
    cout<<n1->getDecimal()<<" "<<n2->getDecimal()<<endl;
    
    Operations *op=new Operations(n1,n2);
    FloatingPointNumber * ans = op->multiply();
    
    int* ansInteger=ans->getInteger(); 
    int* ansFloating=ans->getFloating();
    
    ans->setAccumulator(0);
    cout<<ans->getaccumulator();
    for(int i=0;i<ans->getLeft();i++) cout<<ansInteger[i];
    cout<<".";
    for(int i=0;i<ans->getRight();i++) cout<<ansFloating[i];
    cout<<endl;
    cout<<ans->getDecimal();
}
