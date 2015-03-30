//ͷ�ļ�
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define LEN   sizeof(struct Node)   


//���嶯̬�������ݽṹ
struct Node
{
	char   data;   
	struct Node *next;      
};   

/*��������*/
void output(struct Node *);//ɨ����������
void scaner();            //�ʷ�����
void getbc();             //��ch���ǿհ��ַ�����ͣ����getch()ֱ������Ĳ��ǿհ��ַ�Ϊֹ
void getch();             //�ӻ���������һ�ַ�
void concat();            //��ch�е��ַ����ӵ�token�ĺ���
int letter(char ch);      //�ж�ch�е��Ƿ�����ĸ
int digit(char ch);       //�ж�ch�е��Ƿ�������
int reserve();            //�ж�token�е��ַ����Ƿ��ǹؼ��ֻ��Ǳ�ʶ��
void retract();           //ָ�����һ���ַ�
struct Node *  ReturnPoint(struct Node *p);  //��������ָ�루�����ط��ű����ָ�룩
void error();             //������
void count();             //����ĸ�����ӷ�����ͳ��

struct Node *head,*p;     
char ch; //ȫ���ַ���������ŵ�ǰ�ַ�
char *key[]={"auto","break","case","char","const","continue","default","do","double","else",
			 "enum","extern","float","for","goto","include","if","int","long","main",
			 "pause","printf","register","return","scanf","short","signed","static","sizeof","struct",
			 "switch","system","typedef","union","unsigned","void","volatile","while","Bool","Complex",
			 "Imaginary","inline","restrict"};//�ؼ��ֱ� ��43��
char token[20];//�ַ����飬��Ź��ɵ��ʵ��ַ���
int num=0; 
int Er_Line=1;//���д��еļ���

//ɨ������������������
void output(struct Node *head)                              
{   
	if(!head)   {    printf("error.\n");   exit(1);    }   
    p=head->next;//ָ����C���Դ����һ���ַ�
	while(p->next!=NULL) //ѭ��ֱ�����д�ӡ����
    {   
		printf("%c",p->data);  
          p=p->next;   //��ͷ��β��������
    }   
	printf("\n"); //��ʾ���һ���س�
}

//�ʷ�������������
void scaner()     
{            
	int c,j=0;  
	for(j=0;j<20;j++) 
        token[j]='\0';    //��token���
	getch();             //����һ���ַ�
	getbc();             //����һ������
    if(letter(ch))//�����ַ�
    {   
        while((letter(ch)) || (digit(ch)))//����ĸ��ͷ����ĸ���ִ�
		{   
			concat();   //�ַ�����token������
			getch();    //ָ���������һ���ַ�
		}   
       retract();         //�����ֳ���ĸ����������ַ�ʱ��ָ�����һ��
       c=reserve();       //��ĸ���ִ��뱣����ƥ��
	   if(c!=50) 
			printf("(%s,-)\n",key[c]);    //��������ּǺ�
	   else
       {
			printf("(%s,%x)\n",token,ReturnPoint(p));
       }       
     }   
     else if(digit(ch)) //�������ֿ�ͷ�Ĵ�
     {   
		while(digit(ch))//���ִ������η���token������
       {   
			concat(); //�ַ�����token������
		    getch();  //ָ���������һ���ַ�
       }   
       retract();   //�����ֳ���ĸ����������ַ�ʱ��ָ�����һ��
	   printf("(num,%d)\n",atoi(token));  //������ֵļǺż�������
     }   
	else
	{             //������ĸ����������������
		count();
		switch(ch)
		{        
			case'+':   getch();               //��Ҫ��ǰɨ��
			if(ch=='+')
				printf("(++,-)\n"); 
			else if(ch=='=')
				printf("(+=,-)\n"); 
			else 
           {
				retract();          //ָ�����һ��
				printf("(+,-)\n");          
           }
			break;

            case'-':   getch();              //��Ҫ��ǰɨ��
			if(ch=='-')
				printf("(--,-)\n");      
			else if(ch=='=')
				printf("(-=,-)\n");      
			else if(ch=='>')
				printf("(->,-)\n");      
			else
            {
				retract();         //ָ�����һ��
				printf("(-,-)\n");       
            }  
			break;

			case'*':   getch();  
			if(ch=='=')
				printf("(*=,-)\n");      
			else
           {
				retract();         //ָ�����һ��
				printf("(*,-)\n");          
            }
			break;

           case'/':   getch();              //��Ҫ��ǰɨ��
		   if(ch=='/')
          {
			getch();        
			printf("ע��Ϊ��");        
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
			printf("ע��Ϊ��");  
			while(ch!='*')
           {       
				printf("%c",ch);  
				getch(); 
           }
			printf("\n");  
			getch();                  //���˵�ע�ͽ�����
          }
		 else if(ch=='=')
        {
			printf("(/=,-)\n");      
			break;
         }
		else
        {
			retract();          //ָ�����һ��
			printf("(/,-)\n");          
        }
		break;

		case'%':   getch();
		if(ch=='=')
			printf("(%=,-)\n");      
		else
       {
			retract();          //ָ�����һ��
			printf("(%,-)\n");          
		}        
		break;

        case'<':   getch();              //��Ҫ��ǰɨ��
		if(ch=='=')   
			printf("(relop,LE)\n"); //��ϵ�����<=
		else
       {
			retract();            //ָ�����һ��
			printf("(relop,LT)\n");    //��ϵ�����<
       }
		break;

       case'>':   getch();             //��Ҫ��ǰɨ��
	   if(ch=='=') 
			printf("(relop,GE)\n");//��ϵ�����
	   else
      {
           retract();           //ָ�����һ��
		   printf("(relop,GT)\n");   //��ϵ�����
      }
	   break;

        case'=':   getch();             //��Ҫ��ǰɨ��
		if(ch=='=')
			printf("(relop,EQ)\n");//��ϵ�����
		else
       {
            retract();          //ָ�����һ��
			printf("(assign-op,-)\n");//��ֵ�����
       }
		break;

		case'!':   getch();
		if(ch=='=')
			printf("(relop,NE)\n");//��ϵ�����
		else
	   {
			retract();          //ָ�����һ��
			printf("(logic,NOT)\n");//�߼������
       }
		break;

		case'&':   getch();
		if(ch=='&')
			printf("(logic,AND)\n");//�߼������
		else
       {
			retract();          //ָ�����һ��
			printf("(&,-)\n");      
       }
		break;

		case'|':   getch();
		if(ch=='|')
			printf("(logic,OR)\n");//�߼������
		else
       {
            retract();          //ָ�����һ��
			printf("(|,-)\n");      
       }
		break;

		case'\n':  printf("(enter-op,-)\n");break;
		
       /*������*/
		case':':  getch();             //��Ҫ��ǰɨ��
		if(ch=='=')
			printf("(assign-op,-)\n");//��ϵ�����
		else
       {
            retract();          //ָ�����һ��
			 printf("(:,-)\n");//��ֵ�����
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

//�ӻ���������һ�ַ������Ķ���
void getch()            
{  
	ch=p->data;   
     p=p->next;
}  

//��ch���ǿհ��ַ�����ͣ����getch()ֱ������Ĳ��ǿհ��ַ�Ϊֹ�����Ķ���
void getbc()     
{ 
	while(ch==' ')   
	getch();   
}      

int letter(char ch)      {   return   isalpha((int)ch);}   //ǿ��ת�������ÿ⺯�����ж�ch�е��Ƿ�����ĸ
int digit(char ch)       {   return   isdigit((int)ch);}   //ǿ��ת�������ÿ⺯�����ж�ch�е��Ƿ�������

//��ch���ַ����ӵ�token����ĺ�������
void concat()          
{   
	 unsigned int i;   
     i=strlen(token);   //���ַ������ȿ⺯������
	token[i]=ch;   
	token[i+1]='\0';   
}

//ָ�����һ���ַ������Ķ���
void retract()      
{   
	struct Node *Q;   
    Q=head->next;   
	while(Q->next!=p)   
	{	Q=Q->next;}   
    p=Q;             //ָ�����һ������
}   

//�ж�token�е��ַ����Ƿ��ǹؼ��ֻ��Ǳ�ʶ�������Ķ���
int reserve()      
{   
	int k=0;   
	for(k=0;k<=42;k++)
		if(strcmp(key[k],token)==0)  //�����ַ����ȽϿ⺯��
          return k; //�ǹؼ���
    return 50;  //���ǹؼ���
}  

struct Node *  ReturnPoint(struct Node *p)      
{  return p;  }  //��������ָ�루�����ط��ű����ָ�룩��������

void error()   {   printf("�����ڵ�%d������1������ʶ����ַ���\n",Er_Line);   }  //������������

//����ĸ�����ַ�����ͳ�ƺ�������
void count()   {    num++;    }            

//������
int  main()   
{
	int line=0,h=0,word=0;
     head=(struct Node *)malloc(LEN);   //����ͷ�ڵ�洢�ռ�
	if(!head)   {    printf("error.\n");   exit(1);    }   
	head->next=NULL;   
	head->data=' ';   
    p=head;   
	printf("���������꣬������һ������@\n");   
	printf("����Ὣ���Ĵ����������һ����Ϊ���ա�\n���������:\n");   //��ʾ��ʾ��Ϣ
    while(1)   //��������Ϊ��λ����Դ���룬ֱ��@�����׳���Դ�������������������ѭ��
    {   
		 int i=0;   
         char temp[255];//ÿ�г��Ȳ�����256���ַ�
         gets(temp);   //����Ϊ��λ����Դ����
         if(temp[0]=='@')   break;//������ĵ�һ���ַ�Ϊ$ʱ��ʾ����Դ�������
		line++;  //Դ��������ͳ�Ʊ���
		p->next=(struct Node *)malloc(LEN);   
		if(!(head->next))  {   printf("error.\n");    exit(1);  }   
        p=p->next;   
		while((temp[i]!='\0') && (i<256))     //������Ĵ�������Ϊ��λ���뻺����
        {   
			p->data=temp[i];   
			p->next=(struct Node *)malloc(LEN);   
			if(!(p->next)) 
			{    
				printf("error.\n");      exit(1);  
			}     
				p=p->next;
				i++;  //�ж�ÿ���ַ��������������б߽�ľֲ�����
				++h;  //���س����ȫ���ַ�����ͳ�Ʊ���
		}   
			p->data='\n';   
			p->next=NULL;         //β���
	} 
		printf("������Ĵ������£�\n");
		output(head);             //ɨ�軺������������
		printf("�ʷ�����������£�\n");
		p=head->next;             //pָ������ָ��ͷ�ַ����ڽ��
		while(p->next!=NULL) 
		{  
			if(p->data=='\n')
				Er_Line++; 
			word++;
			scaner();            //�ʷ�����
		}
		printf("Դ������%d�С�\n",line);
		printf("��ע���⣬Դ������%d�����ʡ�\n",word-num);
		printf("Դ������%d���ַ���\n",h);//�����ʾ��ʾ��Ϣ

		system("pause");   
		return 0;   
}   