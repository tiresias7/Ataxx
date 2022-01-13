#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>
using namespace std;

const int inf=10000;
const int sleeptime=1000;
const int p1=1e8+7,p2=1e9+7;
const int dx[24]={1,-1,1,-1,0,0,1,-1,2,2,-2,-2,2,2,2,-2,-2,-2,-1,0,1,-1,0,1};
const int dy[24]={1,-1,-1,1,1,-1,0,0,2,-2,2,-2,-1,0,1,-1,0,1,2,2,2,-2,-2,-2};
const int w1=10,w2=4,w3=7,w4=4,w5=1;

int botfirst;
struct board{
	int a[11][11];//���̱�����ΧΧ����Ȧ-1��������λ��[2][2]��[8][8]Ϊ��ʽ���� 
	int b=1; //Ŀǰ�ֵ����1����2 
	int x[3]={0,2,2};//x[1]��x[2]�ֱ���Ŀǰ���1�����2ռ�ݵĸ�����Ŀ����ʱ���� 
	int ord=0; //Ȩֵ 
	int lasti=0,lastj=0,lastx=0,lasty=0;
	void init(){//��ʼ������ 
		memset(a,0,sizeof(a));
		for(int i=0;i<11;++i){
			a[0][i]=-1; a[1][i]=-1;
			a[9][i]=-1; a[10][i]=-1;
			a[i][0]=-1; a[i][1]=-1;
			a[i][9]=-1; a[i][10]=-1;
		}
		a[2][2]=1;
		a[2][8]=2;
		a[8][2]=2;
		a[8][8]=1;
		b=1;
		x[1]=2;
		x[2]=2;
	}
	void print(){//��ӡ���� 
		cout<<"  �٢ڢۢܢݢޢ�";
		cout<<endl;
		for(int i=2;i<=8;++i){
			switch(i-1){
				case 1: cout<<"��"; break;
				case 2: cout<<"��"; break;
				case 3: cout<<"��"; break;
				case 4: cout<<"��"; break;
				case 5: cout<<"��"; break;
				case 6: cout<<"��"; break;
				case 7: cout<<"��"; break;
			}
			for(int j=2;j<=8;++j)
				if(a[i][j]==0) cout<<"  ";
				else if(a[i][j]==1) cout<<"��";
				else cout<<"��";
			cout<<endl;
		}
	}
	
	int gg(){//�ж���Ϸ�Ƿ���� ����Ӯ�� 
		if(x[1]+x[2]==49)
			if(x[1]>x[2]) return 1;
			else return 2;
		if(x[2]==0) return 1;
		if(x[1]==0) return 2;
		for(int i=2;i<=8;++i)
			for(int j=2;j<=8;++j)
				if(checkmy(i,j))
					for(int k=0;k<24;++k){
						int ni=i+dx[k],nj=j+dy[k];
						if(checknon(ni,nj)) return 0;
					}
		return 3-b; 
	}
	void ggprint(){//�ж���Ϸ�Ƿ��������� 
		int g=gg();
		if((g==1&&botfirst)||(g==2&&!botfirst)) cout<<"�ܱ�����������ʤ�����Һ����ҡ�"<<endl;
		else if((g==2&&botfirst)||(g==1&&!botfirst)) cout<<"�ǻ۵����ʤ�������㣡"<<endl;
		else cout<<"ս����δ������"<<endl;
	}
	bool check(int i,int j){//�ж�һ��λ���Ƿ��������� 
		return a[i][j]!=-1;
	}
	bool checknon(int i,int j){//�ж�һ��λ���Ƿ�Ϊ�� 
		return a[i][j]==0;
	}
	bool checkopp(int i,int j){//�ж�һ��λ���Ƿ񱻶���ռ�� 
		return a[i][j]==3-b;
	}
	bool checkmy(int i,int j){//�ж�һ��λ���Ƿ��ҷ�ռ�� 
		return a[i][j]==b;
	}
	void move(int i,int j,int ni,int nj,bool type,bool cross){//��[i][j]λ�õ����ƶ���[ni][nj],��Ϊ������type=true,����б��cross=true 
		int ret=0;
		if(cross) ret+=w5;
		if(type) a[i][j]=0;
		else ret+=w3,x[b]++;
		a[ni][nj]=b;
		
		for(int k=0;k<8;++k){
			int nx=ni+dx[k],ny=nj+dy[k];
			if(checkopp(nx,ny)){
				a[nx][ny]=b;
				x[b]++;
				x[3-b]--;
				ret+=w1;
			}
			else if(checkmy(nx,ny)) ret+=w2;
			
			nx=i+dx[k],ny=j+dy[k];
			if(checkmy(nx,ny)&&type) ret-=w4;
		} 
		b=3-b;
		ord=ret;
		lasti=i;
		lastj=j;
		lastx=ni;
		lasty=nj;
	}
	void movexy(int i,int j,int ni,int nj){
		move(i+1,j+1,ni+1,nj+1,!(abs(ni-i)<=1&&abs(nj-j)<=1),abs(ni-i)==1&&abs(nj-j)==1);
	}
	int eva(){
		int g=gg();
		if(g==b) return (49-x[3-b])-x[3-b];
		if(g==3-b) return x[b]-(49-x[b]);
		return x[b]-x[3-b];
	}
};

int step=0;
board history[500];

int code(board A){
	long long ret=A.b*3+botfirst;
	for(int i=2;i<=8;++i)
		for(int j=2;j<=8;++j)
			ret=(ret*3+A.a[i][j])%p1;
	return (ret*p2)%p1;
}

void save(){
//	freopen("save.board","w",stdout);
	ofstream of;
	of.open("save.board");
	board A=history[step];
	of<<A.b<<endl;
	of<<botfirst<<endl;
	of<<A.x[1]<<' '<<A.x[2]<<endl;
	for(int i=2;i<=8;++i){
		for(int j=2;j<=8;++j){
			of<<A.a[i][j]<<' ';
		}
		of<<endl;
	}
	of<<code(A);
//	freopen("CON","r",stdin);
//	freopen("CON","w",stdout);
}

int load(){ 
	ifstream ifs("save.board",ios::in);
	if(!ifs){
		cout<<"�޿ɶ�ȡ�浵��"<<endl; 
		Sleep(sleeptime); 
		return 0;
	}
	board A;
	A.init();
	ifs>>A.b;
	ifs>>botfirst;
	ifs>>A.x[1]>>A.x[2];
	for(int i=2;i<=8;++i)
		for(int j=2;j<=8;++j)
			ifs>>A.a[i][j];
	int codex;
	ifs>>codex;
	if(code(A)==codex){
		history[++step]=A;
		cout<<"�����ɹ���"<<endl;
		Sleep(sleeptime);
		//freopen("CON","r",stdin);
		return 1;
	}
	else{
		//system("del save.board");
		//freopen("CON","r",stdin);
		return 2;
	}
}

int play(board A,int dep,int alpha,int beta){
	if(A.gg()) return A.eva();
	if(dep==4) return A.eva();
	
	board choice;
	board cand[100];
	int cnt=0;
	for(int i=2;i<=8;++i)
		for(int j=2;j<=8;++j){
			if(!A.checkmy(i,j)) continue;
			for(int k=0;k<24;++k){
				int ni=i+dx[k],nj=j+dy[k];
				if(!A.checknon(ni,nj)) continue;
				board B=A;
				B.move(i,j,ni,nj,k>=8,k<4);
				if(cnt<100) cand[cnt++]=B;
				
//				int val=-play(B,dep+1,-beta,-alpha);
//				if(val>=beta) return beta;
//		        if(val>alpha) alpha=val,choice=B;
			}
		}
	
	for(int i=0;i<cnt;++i)
		for(int j=i+1;j<cnt;++j)
			if(cand[i].ord<cand[j].ord) swap(cand[i],cand[j]);
	for(int i=0;i<cnt;++i){
		int val=-play(cand[i],dep+1,-beta,-alpha);
		if(val>=beta) return beta;
		if(val>alpha) alpha=val,choice=cand[i];
	}
	
	if(dep==0){
		history[++step]=choice;
	}
	return alpha;
}

bool pausemenu(){//�����Ƿ���� 
	system("cls");
	cout<<"��ͣ"<<endl;
	cout<<"1.����"<<endl;
	cout<<"2.�浵"<<endl;
	cout<<"3.����"<<endl;
	cout<<"4.�˳�"<<endl;
	while(true){
		char c=getch();
		bool b=true;
		switch(c){
			case '1':
			case 27 : return false; break;
			case '2':{
				cout<<"�浵��..."<<endl;
				Sleep(sleeptime);
				save();
				cout<<"�浵�ɹ���"<<endl;
				Sleep(sleeptime);
				return false; 
				break;
			}
			case '3':{
				if(step<2){
					cout<<"��δ��ս�������⣿"<<endl; 
					Sleep(sleeptime);
					return false;
				}
				else{
					cout<<"��������һ�Σ�"<<endl;
					step-=2; 
					Sleep(sleeptime);
					return true;
				}
				break;
			}
			case '4': exit(0); break;
			default : b=false; break;
		}
		if(b) break;
	}
}

void battle(){//�����£����Ժ��� 
//	system("cls");

	int inp[4];
	int cnt=0;
	board A=history[step];
	while(true){
		bool entering=true;
		while(entering){
			system("cls");
//			cout<<"��һ��bot����ʽ�ǣ�"<<endl;
//			if(first) cout<<"�Ĵ�Կգ�"<<endl;
//			else cout<<"��("<<history[step].lasti-2<<","<<history[step].lastj-2<<")��("<<history[step].lastx-2<<","<<history[step].lasty-2<<")��"<<endl;
//			cout<<"Ŀǰ�ľ����ǣ�"<<endl;
			A.print(); 
			cout<<"����У�"<<endl;
			cout<<"��(a,b)��(x,y)�����������ĸ�����(1-7)�ɣ�"<<endl;
			cout<<"a:";
			if(cnt>0) cout<<inp[0]<<" b:";
			if(cnt>1) cout<<inp[1]<<" x:";
			if(cnt>2) cout<<inp[2]<<" y:";
			if(cnt>3) cout<<inp[3];
			
//			cin.sync();
//			cin.clear();
//			cin.sync();//��������� 
			char c;
			while(true){
				if(kbhit()) c=getch();
				else break;
			}//����ȴ�ʱ���û����� 
			while(true){
				c=getch();
				bool b=true;//�Ƿ�Ϊ��Ч���� 
				switch(c){
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':{
						if(cnt!=4){
							cout<<c;
							inp[cnt++]=c-'0';
							}
						else b=false;
							break;
					}
					case 8:
					case 83:{//backspace����delete ����ͬ�����ϵ�delete�����ܲ�ͬ���Ҿ������ҵ����ϵ� 
						if(cnt>0) cnt--;
						break;
					}
					case 27:{//esc
						if(pausemenu()){
							A=history[step];
							cnt=0;
						}
						break;
					}
					case 10:
					case 13:
					case 32:{//���� �س� �ո�
						if(cnt==4) entering=false; 
						else b=false;
						break;
					}
					default :b=false; break;
				}
				//cout<<(int)c<<endl; 
				if(b) break;
			} 
		}
		cout<<endl<<"��ʽ������..."<<endl;
		Sleep(sleeptime);
		if(A.checkmy(inp[0]+1,inp[1]+1)&&A.checknon(inp[2]+1,inp[3]+1)&&abs(inp[2]-inp[0])<=2&&abs(inp[3]-inp[1])<=2){
			cout<<"��ʽ�Ϸ���"<<endl;
			Sleep(sleeptime); 
			break;
		}
		else{
			cout<<"����а����߾���ݵ��ӹ��ҵķ��ۣ�"<<endl;
			Sleep(sleeptime);
			cnt=0;
		}	
	}
	
	
	A.movexy(inp[0],inp[1],inp[2],inp[3]);
	history[++step]=A;
	system("cls");
	A.print();
	A.ggprint();
	if(A.gg()){
		system("pause");
		return;
	}
	
	cout<<"�ֵ�bot���У�"<<endl; 
	play(A,0,-inf,inf);
	
	cout<<"��("<<history[step].lasti-1<<","<<history[step].lastj-1<<")��("<<history[step].lastx-1<<","<<history[step].lasty-1<<")��"<<endl;

	Sleep(sleeptime*2);
	system("cls");
	history[step].print();
	history[step].ggprint();
	if(history[step].gg()){
		system("pause");
		return;
	}
	
	battle();
}

void bot2(){
	botfirst=0;
	history[0].init();
	battle();
}

void bot1(){
	botfirst=1;
	history[0].init();
	system("cls");
	history[0].print();
	cout<<"�ֵ�bot���У�"<<endl; 
	play(history[0],0,-inf,inf);
	cout<<"��("<<history[step].lasti-1<<","<<history[step].lastj-1<<")��("<<history[step].lastx-1<<","<<history[step].lasty-1<<")��"<<endl;
	Sleep(sleeptime*2);
	battle();
}

void choosewhofirst(){
	system("cls");
	cout<<"�𾴵���ң����ֻ��Ǻ��֣�"<<endl;
	cout<<"1.����"<<endl;
	cout<<"2.����"<<endl;
	cout<<"3.���"<<endl;
	cout<<"4.�벻Ҫ�����������"<<endl;
	while(true){
		char c=getch();
		bool b=true;
		switch(c){
			case '1':{
				bot2();
				break;
			}
			case '2':{
				bot1();
				break;
			}
			case '3':{
				if(rand()%2){
					cout<<"�������ǣ�"<<endl; 
					cout<<"�����֣�"<<endl;
					Sleep(sleeptime);
					bot2();
					break;
				}
				else{
					cout<<"�������ǣ�"<<endl; 
					cout<<"�����֣�"<<endl;
					Sleep(sleeptime);
					bot1();
					break;
				}
			}
			case '4':{
				cout<<"�Ǿ�����һ�֣�"<<endl;
				Sleep(sleeptime);
				bot2();
				break;
			}
			default : b=false; break;
		}
		if(b) break;
	} 
}

void startmenu(){
	srand(time(0));
	system("title Ataxx");
	system("mode con cols=50 lines=15");//�������ڴ�С 
	step=0;
	while(true){
		system("cls");
		cout<<"�˵�"<<endl;
		cout<<"1.����Ϸ"<<endl;
		cout<<"2.������Ϸ"<<endl;
		cout<<"3.�˳���Ϸ"<<endl;
		char c=getch();
		bool b=true;
		switch(c){
			case '1': choosewhofirst(); break;
			case '2': {
				int l=load(); 
				if(l==1){
					battle();
				}
				else{
					b=false;
					if(l==2){
						system("del save.board");
						cout<<"���˴۸ģ�������죡�浵�����٣�"<<endl;
						Sleep(sleeptime);
					}
				}
				break;
			}
			case '3': exit(0); break;
			case 27 : break;
			default : b=false;
		}
		if(b) break;
	} 
}

int main(){
	startmenu();
}
