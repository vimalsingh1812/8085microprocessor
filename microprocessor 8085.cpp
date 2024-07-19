#include<iostream>
#include<stdint.h>
#include<string.h>
#include<map>
#include<math.h>
#include<sstream>
using namespace std ;
class p8085
{
    private:
      map<string,string>address ;
      string code ;       // complete code
      string  inst ;      // small instruction
      string A="0", B="0",C="0",D="0",E="0",H="0",L="0" ;     //uint8_t
      long double ppval ;    // program pointer value
      static uint16_t  spointer ;   // stack pointer
      bool flag[5] ={false,false,false,false,false};
    //                SF    ZF    ACF   PF    CF
      
    public:
//***************************************************************CONSTRUCTOR************************************************************
     p8085(int a)
    {
        ppval = a ;
        A = "0" ;
    }

//*******************************************************************CONVERSION*****************************************************
   string decimaltohex(int n)
    {
        string b ="";
        while(n>0)
        {
            int a = n%16 ;
            n=n/16 ;
            string s = to_string(a);
            if(s>="10" && s<="15")
            {
                if(s == "10") s = "A" ;   
                else if(s == "11") s ="B" ;
                else if(s == "12") s ="C" ;
                else if(s == "13") s ="D" ;
                else if(s == "14") s ="E" ;
                else if(s == "15") s ="F" ;

            }
            b.append(s);
        }
        int nu = b.length() ;
        for(int i=0,j=nu-1;i<nu/2;i++,j--)
        {
            char temp = b[i];
            b[i] = b[j] ;
            b[j] = temp ;
        }
        return b ;
    }

    int hextodecimal(string subs)
    {
        int sum=0 , n = subs.length(),i=0;
        for(int j=n-1;j>=0;j--)
        {
           
            if(subs[j] == '0'){ i++ ; continue ;}
            else if(subs[j] == '1') sum+=(pow(16,i++)*1);
            else if(subs[j] == '2') sum+=(pow(16,i++)*2);
            else if(subs[j] == '3') sum+=(pow(16,i++)*3);
            else if(subs[j] == '4') sum+=(pow(16,i++)*4);
            else if(subs[j] == '5') sum+=(pow(16,i++)*5);
            else if(subs[j] == '6') sum+=(pow(16,i++)*6);
            else if(subs[j] == '7') sum+=(pow(16,i++)*7);
            else if(subs[j] == '8') sum+=(pow(16,i++)*8);
            else if(subs[j] == '9') sum+=(pow(16,i++)*9);
            else if(subs[j] == 'A') sum+=(pow(16,i++)*10);
            else if(subs[j] == 'B') sum+=(pow(16,i++)*11);
            else if(subs[j] == 'C') sum+=(pow(16,i++)*12);
            else if(subs[j] == 'D') sum+=(pow(16,i++)*13);
            else if(subs[j] == 'E') sum+=(pow(16,i++)*14);
            else if(subs[j] == 'F') sum+=(pow(16,i++)*15);
         
        }
        return sum ;
    }

    int BinToDecimal(string a)
    {
        int val1 ,n = a.length(),j=0,sum=0;
        for(int i=n-1 ; i>=0 ; i-- )
        {
            if(a[i] == '0')     sum+=(0*pow(2,j++)) ;
            else  sum+=(1*pow(2,j++)) ;
        }
        return sum ;

    }

    string hextobinary(string str)
    {
        cout<<str<<" ";
        string ans ="" ;
        int a = hextodecimal(str) ,i=0,c;
        while(a>=0 && i < 8)
        {
            c = a%2 ;
            a/=2 ;
            str = to_string(c) ;
            ans.append(str) ;
            i++ ;
        }
        int n = ans.length() ;
        for(int i=0,j=n-1;i<n/2;i++,j--)
        {
            char temp = ans[i] ;
            ans[i] = ans[j] ;
            ans[j] = temp ;

        }
        cout<<ans<<endl ;

        return ans ;
        
    }




//*************************************************************VALIDITY******************************************************


    bool checkreg(char reg)
    {
        if(reg == 'A'||reg == 'B'||reg == 'C'||reg=='D'||reg=='E'||reg=='H'||reg=='L')  return true ; 
        else  return false ;
        
    }

    bool checkValueforAddress(string sstr)
    {
        if(sstr.length()>4 ) return false ;
        else{
            for(int i=0 ; i<4 ; i++)
            {
                if((sstr[i]>='0' && sstr[i]<='9')||(sstr[i]>='A' && sstr[i]<='F')) continue ;
             
                else return false ;
            }
            return true ; 
        }
    }


    bool checkvalueinMVI(string sstr)     // substring
    {
        if(sstr.length()>2 ) return false ;
        else{
            for(int i=0 ; i<sstr.length() ; i++)
            {
                if((sstr[i]>='0' && sstr[i]<='9')||(sstr[i]>='A' && sstr[i]<='F')) continue;
             
                else return false ;
            }
            return true ; 
        }
    }
//***************************************************SET VALUE********************************************************************************
    void setval(string i) 
    {  code = i ;}
        void setRegVal(char reg,string val)   // for giving value to the register
    {
        if(reg=='A') A=val ;
        if(reg=='B') B=val;
        else if(reg=='C') C=val;
        else if(reg=='D') D=val;
        else if(reg=='E') E=val;
        else if(reg=='H') H=val;
        else if(reg=='L') L=val;
    }
    void UpdateFlagValue(string lastans, string PuranaAns)    //lastans = latest accumulator value, puranans =last value of accumulator
    {
        cout<<lastans<<" "<<PuranaAns<<" "<<endl ;
        
         if(hextodecimal(lastans) == 0)   flag[1] = 1 ;      //   zero flag
         else flag[1] = 0 ;
         
        string val = hextobinary(lastans) ;       //  sign flag
        if(val[0] == '1')  flag[0] = 1 ;

        int count1 = 0,pos1=0,j=0 ;
        for(int i=0;i<8;i++)         //parity flag
        {
            if(val[i]=='1') count1++ ;
            if(count1 == 1 && j==0)
            {
                 pos1 = i ;
                 j++ ;  }
        }
        if(count1 % 2 == 0 ) flag[3] = 1 ;      //if(count1 % 2 == 0 && count1 > 1) flag[3] = 1  replacable

        string val2 = hextobinary(PuranaAns) ;
        int i ;
        for(i=0;i<val2.length();i++)
        {
            if(val2[i]=='1') break ;
        }
        int pos2 = i ,f = hextodecimal(lastans);
        if(pos1!=pos2 ||f > 255 ) flag[4]= 1 ;


    }
    void UpdateAuxillaryFlag(string FirstVal , string SecondVal)
    {
        string bin1 = hextobinary(FirstVal) ;           // converting first value to binary
        string bin2 = hextobinary(SecondVal) ;          // converting second value to binary
        string last4 = bin1.substr(4,4);                 // taking last four digit
        string last4_b2 = bin2.substr(4,4);
        int a = BinToDecimal(last4) ;
        int b = BinToDecimal(last4_b2) ;
        int c = a+b ;
        if(c>15) flag[2] = 1 ;
    }


//*********************************************************INSTRUCTIONS*******************************************************************
    void commands() 
    {
        int i=0 ;
        while (code[i]!=32)   i++ ;
        inst = code.substr(0,i);
        int n = code.length() ;

        if(inst == "MVI")   // checking we have found substring or not
        { 
                     char reg = code[4] ;
                     string num ;
                    if(checkreg(reg))
                    {
                        int i = 0 ;
                        while(code[n-1]!=32) { i++ ;  n--;}
                        num = code.substr(n,i);        //substr(position,steps)  error handling for single  digit number done
                        num = decimaltohex(stoi(num));
                        
                        if(checkvalueinMVI(num))  setRegVal(reg,num);             // stoi is used for string to integer
                        else{ cout<<"Enter the valid value"<<endl;
                        exit(1); }
                    }

                    else{ cout<<"invalid register"<<endl ;
                          exit(1) ;}
                
                    // string a = to_string(ppval) ;
                    // string b = to_string(A) ;
                    // address[a] = b ;                                                for sequence
                    ppval+=2 ;
                
        }
        else if(inst == "LDA")
        {
            int i=0 ;
            string val ;
            while(code[n-1]!= 32){ n-- ; i++; }    // for finding the last number 
            string AddressStr = code.substr(n,i);   // taking address value of the code
            cout<<"Insert address value " ;
            cin>>val ;
            A = val ;
            ppval+=3 ;
            address[AddressStr] = val ;
               
            // UpdateFlagValue(val);    // need to check value for this

        }
        else if(inst == "MOV")
        {
            if(checkreg(code[4]))
               {
                if(checkreg(code[6]))
               {
                string a = code.substr(n-1,1) ;
                if(a == "A") setRegVal(code[4],A) ;
                else if(a =="B") setRegVal(code[4],B) ;
                else if(a =="C") setRegVal(code[4],C);
                else if(a =="D") setRegVal(code[4],D);
                else if(a =="E") setRegVal(code[4],E);
                else if(a =="H") setRegVal(code[4],H);
                else if(a =="L") setRegVal(code[4],L);
                ppval+=1;
                }
            else
               {
                cout<<"not valid MOV command "<<endl ;
                exit(1) ;
               } 
            }
        }
        else if(inst == "ADD")
        {

            if(checkreg(code[n-1]))
            {
                int a = hextodecimal(A) ;
                string pre = A ;            // previous value of the accumulator
                string b = code.substr(n-1,1);
                int c ;
                if(checkreg(code[n-1]))
                {
                    if(b=="A") {c = hextodecimal(A);          UpdateAuxillaryFlag(pre,A);}                           //0xA
                    else if(b=="B") { c = hextodecimal(B);      UpdateAuxillaryFlag(pre,B);}                         //0xB
                    else if(b=="C") { c = hextodecimal(C);       UpdateAuxillaryFlag(pre,C);}
                    else if(b=="D") { c = hextodecimal(D);       UpdateAuxillaryFlag(pre,D);}
                    else if(b=="E") { c = hextodecimal(E);       UpdateAuxillaryFlag(pre,E);}
                    else if(b=="H") { c = hextodecimal(H);      UpdateAuxillaryFlag(pre,H);}
                    else if(b=="L") { c = hextodecimal(L);       UpdateAuxillaryFlag(pre,L);}

                }
                int d = a+c ;
                b = decimaltohex(d) ;
                setRegVal('A',b) ;
                ppval+=1 ;
                UpdateFlagValue(A,pre) ;   // for signed flag, zero flag and parity flag
                
                
            }            
        }
        else if(inst == "SUB")                // recheck for the subtraction
        {
            int a = hextodecimal(A) ;
            string pre = A ;
            string b = code.substr(n-1,1);
                int c ;
                if(checkreg(code[n-1]))
                {
                    if(b=="A") c = hextodecimal(A);    //    0xA
                    if(b=="B") c = hextodecimal(B);    //    0xB
                    if(b=="C") c = hextodecimal(C);
                    if(b=="D") c = hextodecimal(D);
                    if(b=="E") c = hextodecimal(E);
                    if(b=="H") c = hextodecimal(H);
                    if(b=="L") c = hextodecimal(L);
                }
                int d ;
                if(a>=c)  d = a-c ;
                else{
                      d = c-a ;
                      d = 256-d ;
                }

                b = decimaltohex(d) ;
                setRegVal('A',b) ;
                ppval+=1 ;
                UpdateFlagValue(A,pre) ;   // for signed flag, zero flag and parity flag

        }
        else if(inst == "STA")
        {
            while(code[n-1]!=32){n--; i++;}
            string adrs = code.substr(n,i) ;
            if(checkValueforAddress(adrs))
            {
                address[adrs] = A;
            }
            else cout<<"insert the valid address"<<endl ; 
            ppval+=3 ;
            // function for checking value at a address
        }
        else if(inst == "INR")
        {
            char reg = code[n-1] ;
            string b;
            b+=reg ;
            int c ;
            if(checkreg(reg))
            {
                    if(b=="A") c = hextodecimal(A);              // 0xA
                    else if(b=="B") c = hextodecimal(B);         // 0xB
                    else if(b=="C") c = hextodecimal(C);        
                    else if(b=="D") c = hextodecimal(D);
                    else if(b=="E") c = hextodecimal(E);
                    else if(b=="H") c = hextodecimal(H);
                    else if(b=="L") c = hextodecimal(L);   
            }
            else   cout<<"enter the valid resistor"<<endl ;
            c+=1 ;
            b = decimaltohex(c) ;
            setRegVal(reg,b) ;
            ppval+=1 ;
            
        }
    }
//***************************************************FOR DISPLAYING**************************************************************
    void getvalue()
    {
       
        cout<<"program pointer = "<<ppval<<endl ;
        cout<<"A="<<A<<" B="<<B<<" C="<<C<<" D="<<D<<" E="<<E<<" H="<<H<<" L="<<L<<endl ;

    }
    void displayflag()
    {
        cout<<"SF = "<<flag[0]<<endl ;
        cout<<"ZF = "<<flag[1]<<endl ;
        cout<<"ACF = "<<flag[2]<<endl ;
        cout<<"PF = "<<flag[3]<<endl ;
        cout<<"CF = "<<flag[4]<<endl ;

    }
    class flag{
    // int a = 0x02;
    // int b = 0x03;   
    // int sum = a + b;
    // int carry = sum > 0xFF ? 1 : 0;
    // int zero = sum == 0 ? 1 : 0;
    // int sign = sum & 0x80 ? 1 : 0;
    // int parity = 0;
    // for (int i = 0; i < 8; i++) {
    // parity ^= ((sum >> i) & 1); }
    // int auxcarry = ((a & 0x0F) + (b & 0x0F)) > 0x0F ? 1 : 0;

    };
    
    void Pppval()
    {
        cout<<ppval<<"  ";
    }

   
    class stackpointer{} ;
    

};

int main()
{
    int n=3000 ;
    // cout<<"Insert the address ";
    // cin>>n;
    p8085 obj(n) ;
    // p8085 obj ;
    string ins,sub ;
    cout<<"enter the commands and press HLT for stop "<<endl ;


    do{

    getline(cin,ins) ;
    obj.setval(ins) ;
    obj.commands() ;
    sub = ins.substr(0,3) ;
    }while(sub!="HLT") ;
    obj.displayflag() ;
    obj.getvalue() ;


    return 0;
}
