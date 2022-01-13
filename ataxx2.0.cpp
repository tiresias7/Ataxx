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
	int a[11][11];//棋盘本身，周围围绕两圈-1不可落子位，[2][2]至[8][8]为正式棋盘 
	int b=1; //目前轮到玩家1还是2 
	int x[3]={0,2,2};//x[1]和x[2]分别是目前玩家1和玩家2占据的格子数目，及时更新 
	int ord=0; //权值 
	int lasti=0,lastj=0,lastx=0,lasty=0;
	void init(){//初始化棋盘 
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
	void print(){//打印棋盘 
		cout<<"  ①②③④⑤⑥⑦";
		cout<<endl;
		for(int i=2;i<=8;++i){
			switch(i-1){
				case 1: cout<<"①"; break;
				case 2: cout<<"②"; break;
				case 3: cout<<"③"; break;
				case 4: cout<<"④"; break;
				case 5: cout<<"⑤"; break;
				case 6: cout<<"⑥"; break;
				case 7: cout<<"⑦"; break;
			}
			for(int j=2;j<=8;++j)
				if(a[i][j]==0) cout<<"  ";
				else if(a[i][j]==1) cout<<"●";
				else cout<<"○";
			cout<<endl;
		}
	}
	
	int gg(){//判断游戏是否结束 返回赢家 
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
	void ggprint(){//判断游戏是否结束并输出 
		int g=gg();
		if((g==1&&botfirst)||(g==2&&!botfirst)) cout<<"能被赐予这样的胜利，我很荣幸。"<<endl;
		else if((g==2&&botfirst)||(g==1&&!botfirst)) cout<<"智慧的生物，胜利属于你！"<<endl;
		else cout<<"战斗尚未结束！"<<endl;
	}
	bool check(int i,int j){//判断一个位置是否在棋盘内 
		return a[i][j]!=-1;
	}
	bool checknon(int i,int j){//判断一个位置是否为空 
		return a[i][j]==0;
	}
	bool checkopp(int i,int j){//判断一个位置是否被对手占领 
		return a[i][j]==3-b;
	}
	bool checkmy(int i,int j){//判断一个位置是否被我方占领 
		return a[i][j]==b;
	}
	void move(int i,int j,int ni,int nj,bool type,bool cross){//将[i][j]位置的棋移动到[ni][nj],若为跳棋则type=true,若走斜角cross=true 
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
		cout<<"无可读取存档！"<<endl; 
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
		cout<<"读档成功！"<<endl;
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

bool pausemenu(){//返回是否悔棋 
	system("cls");
	cout<<"暂停"<<endl;
	cout<<"1.继续"<<endl;
	cout<<"2.存档"<<endl;
	cout<<"3.悔棋"<<endl;
	cout<<"4.退出"<<endl;
	while(true){
		char c=getch();
		bool b=true;
		switch(c){
			case '1':
			case 27 : return false; break;
			case '2':{
				cout<<"存档中..."<<endl;
				Sleep(sleeptime);
				save();
				cout<<"存档成功！"<<endl;
				Sleep(sleeptime);
				return false; 
				break;
			}
			case '3':{
				if(step<2){
					cout<<"还未开战已生悔意？"<<endl; 
					Sleep(sleeptime);
					return false;
				}
				else{
					cout<<"姑且饶你一次！"<<endl;
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

void battle(){//人先下，电脑后下 
//	system("cls");

	int inp[4];
	int cnt=0;
	board A=history[step];
	while(true){
		bool entering=true;
		while(entering){
			system("cls");
//			cout<<"上一局bot的招式是！"<<endl;
//			if(first) cout<<"四大皆空！"<<endl;
//			else cout<<"从("<<history[step].lasti-2<<","<<history[step].lastj-2<<")到("<<history[step].lastx-2<<","<<history[step].lasty-2<<")！"<<endl;
//			cout<<"目前的局势是："<<endl;
			A.print(); 
			cout<<"请出招！"<<endl;
			cout<<"从(a,b)到(x,y)！依次输入四个整数(1-7)吧！"<<endl;
			cout<<"a:";
			if(cnt>0) cout<<inp[0]<<" b:";
			if(cnt>1) cout<<inp[1]<<" x:";
			if(cnt>2) cout<<inp[2]<<" y:";
			if(cnt>3) cout<<inp[3];
			
//			cin.sync();
//			cin.clear();
//			cin.sync();//清空输入流 
			char c;
			while(true){
				if(kbhit()) c=getch();
				else break;
			}//处理等待时的用户输入 
			while(true){
				c=getch();
				bool b=true;//是否为有效输入 
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
					case 83:{//backspace或者delete 好像不同电脑上的delete键可能不同，我就做了我电脑上的 
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
					case 32:{//换行 回车 空格
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
		cout<<endl<<"招式检索中..."<<endl;
		Sleep(sleeptime);
		if(A.checkmy(inp[0]+1,inp[1]+1)&&A.checknon(inp[2]+1,inp[3]+1)&&abs(inp[2]-inp[0])<=2&&abs(inp[3]-inp[1])<=2){
			cout<<"招式合法！"<<endl;
			Sleep(sleeptime); 
			break;
		}
		else{
			cout<<"歪门邪道！呔！休得逃过我的法眼！"<<endl;
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
	
	cout<<"轮到bot出招！"<<endl; 
	play(A,0,-inf,inf);
	
	cout<<"从("<<history[step].lasti-1<<","<<history[step].lastj-1<<")到("<<history[step].lastx-1<<","<<history[step].lasty-1<<")！"<<endl;

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
	cout<<"轮到bot出招！"<<endl; 
	play(history[0],0,-inf,inf);
	cout<<"从("<<history[step].lasti-1<<","<<history[step].lastj-1<<")到("<<history[step].lastx-1<<","<<history[step].lasty-1<<")！"<<endl;
	Sleep(sleeptime*2);
	battle();
}

void choosewhofirst(){
	system("cls");
	cout<<"尊敬的玩家，先手还是后手？"<<endl;
	cout<<"1.先手"<<endl;
	cout<<"2.后手"<<endl;
	cout<<"3.随机"<<endl;
	cout<<"4.请不要歧视无手玩家"<<endl;
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
					cout<<"随机结果是！"<<endl; 
					cout<<"您先手！"<<endl;
					Sleep(sleeptime);
					bot2();
					break;
				}
				else{
					cout<<"随机结果是！"<<endl; 
					cout<<"您后手！"<<endl;
					Sleep(sleeptime);
					bot1();
					break;
				}
			}
			case '4':{
				cout<<"那就让你一手！"<<endl;
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
	system("mode con cols=50 lines=15");//调整窗口大小 
	step=0;
	while(true){
		system("cls");
		cout<<"菜单"<<endl;
		cout<<"1.新游戏"<<endl;
		cout<<"2.继续游戏"<<endl;
		cout<<"3.退出游戏"<<endl;
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
						cout<<"何人篡改！胆大包天！存档已销毁！"<<endl;
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
