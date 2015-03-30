//头文件
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define LEN   sizeof(struct Node)   


//定义动态链表数据结构
struct Node
{
	char   data;   
	struct Node *next;      
};   

/*函数声明*/
void output(struct Node *);//扫描链表缓冲区
void scaner();            //词法分析
void getbc();             //若ch中是空白字符，则不停调用getch()直到读入的不是空白字符为止
void getch();             //从缓冲区读入一字符
void concat();            //将ch中的字符连接到token的后面
int letter(char ch);      //判断ch中的是否是字母
int digit(char ch);       //判断ch中的是否是数字
int reserve();            //判断token中的字符串是否是关键字或是标识符
void retract();           //指针回退一个字符
struct Node *  ReturnPoint(struct Node *p);  //返回链表指针（即返回符号表入口指针）
void error();             //错误处理
void count();             //非字母数字子符数的统计

struct Node *head,*p;     
char ch; //全局字符变量，存放当前字符
char *key[]={"auto","break","case","char","const","continue","default","do","double","else",
			 "enum","extern","float","for","goto","include","if","int","long","main",
			 "pause","printf","register","return","scanf","short","signed","static","sizeof","struct",
			 "switch","system","typedef","union","unsigned","void","volatile","while","Bool","Complex",
			 "Imaginary","inline","restrict"};//关键字表 共43个
char token[20];//字符数组，存放构成单词的字符串
int num=0; 
int Er_Line=1;//对有错行的计数

//扫描链表缓冲区函数定义
void output(struct Node *head)                              
{   
	if(!head)   {    printf("error.\n");   exit(1);    }   
    p=head->next;//指向本行C语言代码第一个字符
	while(p->next!=NULL) //循环直到本行打印结束
    {   
		printf("%c",p->data);  
          p=p->next;   //从头至尾遍历链表
    }   
	printf("\n"); //显示最后一个回车
}

//词法分析函数定义
void scaner()     
{            
	int c,j=0;  
	for(j=0;j<20;j++) 
        token[j]='\0';    //将token清空
	getch();             //读入一个字符
	getbc();             //读入一个单词
    if(letter(ch))//处理字符
    {   
        while((letter(ch)) || (digit(ch)))//以字母开头的字母数字串
		{   
			concat();   //字符放入token数组中
			getch();    //指针再往后读一个字符
		}   
       retract();         //当出现除字母、数字外的字符时，指针回退一步
       c=reserve();       //字母数字串与保留字匹配
	   if(c!=50) 
			printf("(%s,-)\n",key[c]);    //输出保留字记号
	   else
       {
			printf("(%s,%x)\n",token,ReturnPoint(p));
       }       
     }   
     else if(digit(ch)) //处理数字开头的串
     {   
		while(digit(ch))//数字串则依次放入token数组中
       {   
			concat(); //字符放入token数组中
		    getch();  //指针再往后读一个字符
       }   
       retract();   //当出现除字母、数字外的字符时，指针回退一步
	   printf("(num,%d)\n",atoi(token));  //输出数字的记号及其属性
     }   
	else
	{             //处理字母、数字外的特殊符号
		count();
		switch(ch)
		{        
			case'+':   getch();               //需要超前扫描
			if(ch=='+')
				printf("(++,-)\n"); 
			else if(ch=='=')
				printf("(+=,-)\n"); 
			else 
           {
				retract();          //指针回退一步
				printf("(+,-)\n");          
           }
			break;

            case'-':   getch();              //需要超前扫描
			if(ch=='-')
				printf("(--,-)\n");      
			else if(ch=='=')
				printf("(-=,-)\n");      
			else if(ch=='>')
				printf("(->,-)\n");      
			else
            {
				retract();         //指针回退一步
				printf("(-,-)\n");       
            }  
			break;

			case'*':   getch();  
			if(ch=='=')
				printf("(*=,-)\n");      
			else
           {
				retract();         //指针回退一步
				printf("(*,-)\n");          
            }
			break;

           case'/':   getch();              //需要超前扫描
		   if(ch=='/')
          {
			getch();        
			printf("注释为：");        
			while(ch!='\n')
		   {
				printf("%c",ch);
				getch();
           }
			printf("\n");             
		  }
		  else if(ch=='*')
         {
			getch(); 
			printf("注释为：");  
			while(ch!='*')
           {       
				printf("%c",ch);  
				getch(); 
           }
			printf("\n");  
			getch();                  //过滤掉注释结束符
          }
		 else if(ch=='=')
        {
			printf("(/=,-)\n");      
			break;
         }
		else
        {
			retract();          //指针回退一步
			printf("(/,-)\n");          
        }
		break;

		case'%':   getch();
		if(ch=='=')
			printf("(%=,-)\n");      
		else
       {
			retract();          //指针回退一步
			printf("(%,-)\n");          
		}        
		break;

        case'<':   getch();              //需要超前扫描
		if(ch=='=')   
			printf("(relop,LE)\n"); //关系运算符<=
		else
       {
			retract();            //指针回退一步
			printf("(relop,LT)\n");    //关系运算符<
       }
		break;

       case'>':   getch();             //需要超前扫描
	   if(ch=='=') 
			printf("(relop,GE)\n");//关系运算符
	   else
      {
           retract();           //指针回退一步
		   printf("(relop,GT)\n");   //关系运算符
      }
	   break;

        case'=':   getch();             //需要超前扫描
		if(ch=='=')
			printf("(relop,EQ)\n");//关系运算符
		else
       {
            retract();          //指针回退一步
			printf("(assign-op,-)\n");//赋值运算符
       }
		break;

		case'!':   getch();
		if(ch=='=')
			printf("(relop,NE)\n");//关系运算符
		else
	   {
			retract();          //指针回退一步
			printf("(logic,NOT)\n");//逻辑运算符
       }
		break;

		case'&':   getch();
		if(ch=='&')
			printf("(logic,AND)\n");//逻辑运算符
		else
       {
			retract();          //指针回退一步
			printf("(&,-)\n");      
       }
		break;

		case'|':   getch();
		if(ch=='|')
			printf("(logic,OR)\n");//逻辑运算符
		else
       {
            retract();          //指针回退一步
			printf("(|,-)\n");      
       }
		break;

		case'\n':  printf("(enter-op,-)\n");break;
		
       /*标点符号*/
		case':':  getch();             //需要超前扫描
		if(ch=='=')
			printf("(assign-op,-)\n");//关系运算符
		else
       {
            retract();          //指针回退一步
			 printf("(:,-)\n");//赋值运算符
       }
		break;
		case';':   printf("(;,-)\n");break; 
		case'{':   printf("({,-)\n");break;   
		case'}':   printf("(},-)\n");break;   
		case'(':   printf("((,-)\n");break;   
		case')':   printf("(),-)\n");break; 
		case'[':   printf("([,-)\n");break;
		case']':   printf("(],-)\n");break;  
		case'.':   printf("(.,-)\n");break;
		case',':   printf("(,,-)\n");break;  
		case'?':   printf("(?,-)\n");break;
  
		default:   error();          break;   
       }//end switch
      }//end else  
  }   

//从缓冲区读入一字符函数的定义
void getch()            
{  
	ch=p->data;   
     p=p->next;
}  

//若ch中是空白字符，则不停调用getch()直到读入的不是空白字符为止函数的定义
void getbc()     
{ 
	while(ch==' ')   
	getch();   
}      

int letter(char ch)      {   return   isalpha((int)ch);}   //强制转换后利用库函数，判断ch中的是否是字母
int digit(char ch)       {   return   isdigit((int)ch);}   //强制转换后利用库函数，判断ch中的是否是数字

//将ch中字符连接到token后面的函数定义
void concat()          
{   
	 unsigned int i;   
     i=strlen(token);   //求字符串长度库函数调用
	token[i]=ch;   
	token[i+1]='\0';   
}

//指针回退一个字符函数的定义
void retract()      
{   
	struct Node *Q;   
    Q=head->next;   
	while(Q->next!=p)   
	{	Q=Q->next;}   
    p=Q;             //指针回退一步操作
}   

//判断token中的字符串是否是关键字或是标识符函数的定义
int reserve()      
{   
	int k=0;   
	for(k=0;k<=42;k++)
		if(strcmp(key[k],token)==0)  //调用字符串比较库函数
          return k; //是关键字
    return 50;  //不是关键字
}  

struct Node *  ReturnPoint(struct Node *p)      
{  return p;  }  //返回链表指针（即返回符号表入口指针）函数定义

void error()   {   printf("程序在第%d行遇到1个不能识别的字符！\n",Er_Line);   }  //错误处理函数定义

//非字母数字字符数的统计函数定义
void count()   {    num++;    }            

//主函数
int  main()   
{
	int line=0,h=0,word=0;
     head=(struct Node *)malloc(LEN);   //分配头节点存储空间
	if(!head)   {    printf("error.\n");   exit(1);    }   
	head->next=NULL;   
	head->data=' ';   
    p=head;   
	printf("若代码输完，请另起一行输入@\n");   
	printf("程序会将您的代码重新输出一遍作为参照。\n请输入代码:\n");   //显示提示信息
    while(1)   //不断以行为单位读入源代码，直到@在行首出现源程序输入结束，跳出本循环
    {   
		 int i=0;   
         char temp[255];//每行长度不超过256个字符
         gets(temp);   //以行为单位读入源程序
         if(temp[0]=='@')   break;//当输入的第一个字符为$时表示输入源代码结束
		line++;  //源程序行数统计变量
		p->next=(struct Node *)malloc(LEN);   
		if(!(head->next))  {   printf("error.\n");    exit(1);  }   
        p=p->next;   
		while((temp[i]!='\0') && (i<256))     //将输入的代码以行为单位存入缓冲区
        {   
			p->data=temp[i];   
			p->next=(struct Node *)malloc(LEN);   
			if(!(p->next)) 
			{    
				printf("error.\n");      exit(1);  
			}     
				p=p->next;
				i++;  //判断每行字符结束并不超过行边界的局部变量
				++h;  //除回车外的全文字符总数统计变量
		}   
			p->data='\n';   
			p->next=NULL;         //尾结点
	} 
		printf("您输入的代码如下：\n");
		output(head);             //扫描缓冲区，输出结果
		printf("词法分析结果如下：\n");
		p=head->next;             //p指针重新指向头字符所在结点
		while(p->next!=NULL) 
		{  
			if(p->data=='\n')
				Er_Line++; 
			word++;
			scaner();            //词法分析
		}
		printf("源程序共有%d行。\n",line);
		printf("除注释外，源程序共有%d个单词。\n",word-num);
		printf("源程序共有%d个字符。\n",h);//输出显示提示信息

		system("pause");   
		return 0;   
}   