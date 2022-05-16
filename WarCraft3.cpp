#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<string>
#include<set>
#define pb push_back
using namespace std;
int a[10],att[10];
int M,N,K,T;
int conqA,conqB;
const char* arm[3] = {"sword","bomb","arrow"};
const char* warrior[5] = {"dragon","ninja","iceman","lion","wolf"};
const char* city[3] = {"","red","blue"};
void PrintTime(int ti)
{
	int h = ti/60,m = ti%60;
	if (h<=9) printf("00%d:",h);
	else if (h<=99) printf("0%d:",h);
	else printf("%d:",h);
	if (m == 0) printf("00 ");
	else if (m<=9) printf("0%d ",m);
	else printf("%d ",m);
}
template<class T>
void Movep(vector<T>* ps,int pos,int ti)
{
	if (ps[pos].size()) 
	{
		ps[pos][0].Hurtself();
		ps[pos][0].faith-=K;
		ps[pos+1].pb(ps[pos][0]);
		ps[pos].clear();
		if (pos+1==N+1) 
		{
			conqA=1;
			//PrintTime(ti);
			//printf("red %s %d reached blue headquarter with %d elements and force %d\n",warrior[ps[pos][0].obj],ps[pos][0].ord,ps[pos][0].heal,att[ps[pos][0].obj]);
		}
	}
}
template<class T>
void Moven(vector<T>* ps,int pos,int ti)
{
	if (ps[pos].size())
	{
		ps[pos][0].Hurtself();
		ps[pos][0].faith-=K;
		ps[pos-1].pb(ps[pos][0]);
		ps[pos].clear();
		if (pos-1==0) 
		{
			conqB=1;
			//PrintTime(ti);
			//printf("blue %s %d reached red headquarter with %d elements and force %d\n",warrior[ps[pos][0].obj],ps[pos][0].ord,ps[pos][0].heal,att[ps[pos][0].obj]);
		}
	}
}
template<class T>
void PrintArm(vector<T> ps,int ti)
{
	if (!ps.size()) return;
	PrintTime(ti);
	set<int>::iterator i = ps[0].Arm.begin();
	int cnt1=0,cnt2=0,cnt3=0;
	for (;i!=ps[0].Arm.end();i++)
	{
		if (*i==0) cnt1++;
		if (*i==1) cnt2++;
		if (*i==2||*i==3) cnt3++;
	}
	printf("%s %s %d has %d sword %d bomb %d arrow and %d elements\n",city[ps[0].type],warrior[ps[0].obj],ps[0].ord,cnt1,cnt2,cnt3,ps[0].heal);
}

class CCreature{
	public:
	bool exis;
    int type,obj,ord,res,heal,faith;	
    multiset<int> Arm;
    CCreature (int ty , int ob , int Ord , int Res)
    {
        type = ty;
        obj = ob;
        ord = Ord;
        res = Res;
		faith = Res;
		heal = a[ob];
		exis = true;
        Arm.clear();
    }
    ~CCreature()
    {
    	Arm.clear();
	}
	void createArm(int armid)
	{
		Arm.insert(armid);
	}
	void MovePrint(int p,int ti)
	{
		if (type == 0 ) return;
		if (p==0||p==N+1) 
		{
			PrintTime(ti);
			printf("%s %s %d reached %s headquarter with %d elements and force %d\n",city[type],warrior[obj],ord,city[((type-1)^1)+1],heal,att[obj]);
			PrintTime(ti);
			printf("%s headquarter was taken\n",city[3-type]);
		}
		else
		{
			PrintTime(ti);
			if (type == 1) printf("red ");else printf("blue ");
			printf("%s %d marched to city %d with %d elements and force %d\n",warrior[obj],ord,p,heal,att[obj]);
		}
	/*	if (j==N)
		{
			
		}*/
	}
	void Hurt(int n)
	{
		heal-=n;
	}
	virtual void Hurtself()
	{

	}
	//virtual void RunLion(int K,int ti){}
	friend bool operator < (const CCreature & A,const CCreature & B)
	{
		return A.ord<B.ord;
	}
	virtual void Bombeffect(int n)
	{
		heal-=n;
	}
	virtual void Cheer(int ti,int p)
	{

	}
};
class CDragon : public CCreature{
 	public:
		CDragon (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
		}
		virtual void Cheer(int ti,int p)
		{
		//	chee[p].push_back(*this);
			PrintTime(ti);
			printf("%s dragon %d yelled in city %d\n",city[type],ord,p);
		}
		
};
class CNinja : public CCreature{
	public:
		CNinja (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
			createArm((Ord+1)%3);
		}
		virtual void Bombeffect(int n)
		{

		}
};
class CIceman: public CCreature{
	public:
		CIceman (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
		}
		virtual void Hurtself()
		{
			heal-=heal/10;
		}
};
class CLion : public CCreature{
    public: 
    	CLion (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)
        {
			createArm(Ord%3);
			printf("Its loyalty is %d\n",Res);
        }
		void RunLion(int K,int ti)
		{
			if (faith <=0 && exis)
			{
				exis = false;
				//PrintLion[pos].push_back()
				PrintTime(ti);
				printf("%s lion %d ran away\n",city[type],ord);
			}
		}
};
class CWolf : public CCreature{
	public:
		CWolf (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res) {}
		void stealarm(CCreature & ccreature,int ti,int p)
		{
			if (ccreature.Arm.begin()==ccreature.Arm.end()) return;
			int armid = *(ccreature.Arm.begin()),cnt=Arm.size();
			int xx = 0;
			multiset<int>::iterator ii = ccreature.Arm.begin();
			for (;ii!=ccreature.Arm.end()&&((armid==2&&*ii==3)||(armid==*ii))&&cnt<10;cnt++,ii++,xx++) Arm.insert(*ii);
			ccreature.Arm.erase(ccreature.Arm.begin(),ii);	
			if (xx)
			{
				PrintTime(ti);
				if (type == 1) printf("red ");else printf("blue ");
				if (armid == 3) armid=2;
				printf("wolf %d took %d %s from %s %s %d in city %d\n",ord,xx,arm[armid],city[ccreature.type],warrior[ccreature.obj],ccreature.ord,p);
			}
		}
		
};
//vector<CCreature> ans [10010];
const int Nexta[5] = {2,0,3,4,1};
const int Nextb[5] = {1,2,4,0,3};
//vector<CCreature> CC;
vector<CDragon> BlueDragon[25],RedDragon[25];
vector<CNinja> BlueNinja[25],RedNinja[25];
vector<CIceman> BlueIceman[25],RedIceman[25];
vector<CLion> BlueLion[25],RedLion[25];
vector<CWolf> BlueWolf[25],RedWolf[25];
//typedef set<CCreature>::iterator si;
void Attack(CCreature* A,CCreature* B,int armid)
{
	if (armid == 0) B->Hurt(att[A->obj]*2/10);
	if (armid == 1)
	{
		B->Hurt(att[A->obj]*4/10);
		A->Bombeffect(att[A->obj]*4/10/2);
	}
	if (armid == 2 || armid == 3)
	{
		B->Hurt(att[A->obj]*3/10);
	}
}
void solo(CCreature* A,CCreature* B,int p)
{
	multiset<int> S1,S2;
	S1.clear(),S2.clear();
	for (multiset<int>::iterator ii = A->Arm.begin();ii!=A->Arm.end();ii++)
	{
		if (*ii==1||*ii==0) S1.insert(*ii);
		if (*ii==2) S1.insert(3);
		if (*ii==3) S1.insert(2);
	}
	for (multiset<int>::iterator ii = B->Arm.begin();ii!=B->Arm.end();ii++)
	{
		if (*ii==1||*ii==0) S2.insert(*ii);
		if (*ii==2) S2.insert(3);
		if (*ii==3) S2.insert(2);
	}
	multiset<int>::iterator nowa = S1.begin(),nowb = S2.begin();
	while (1)
	{
		//if (A->type==2&&A->obj==1) puts("hahaha");
		if (nowa == S1.end()) nowa = S1.begin();
		if (nowb == S2.end()) nowb = S2.begin();
		if (nowa!=S1.end()) 
		{
			Attack(A,B,*nowa);
			multiset<int>::iterator aaa = nowa;
			nowa++;
			int armid = *aaa;
			if (armid==1) S1.erase(aaa);
			if (armid==3) S1.erase(aaa),S1.insert(2);
			if (armid==2) S1.erase(aaa);
		}
		if ( B->heal<=0 || A->heal<=0) break; 
		//if (A->type==2&&A->obj==1) puts("hahaha");
		if (nowb!=S2.end()) 
		{
			Attack(B,A,*nowb);
			multiset<int>::iterator aaa = nowb;
			nowb++;
			int armid = *aaa;
			if (armid==1) S2.erase(aaa);
			if (armid==3) S2.erase(aaa),S2.insert(2);
			if (armid==2) S2.erase(aaa);
		}
		if (A->heal <= 0 || B->heal<=0) break;
		if (S1.begin()==S1.end()&&S2.begin()==S2.end()) break;
		int c1 = 0 , c2=0;
		for (multiset<int>::iterator ii = S1.begin();ii!=S1.end();ii++)
		{
			int armid = *ii,tmp=1;
			if (armid == 0) tmp = att[A->obj]*2/10;
			//if (armid == 1 && A->obj == 1) tmp = att[A->obj]*4/10;
			if (tmp) c1++;
		} 
		for (multiset<int>::iterator ii = S2.begin();ii!=S2.end();ii++)
		{
			int armid = *ii,tmp=1;
			if (armid == 0) tmp = att[B->obj]*2/10;
			//if (armid == 1 && B->obj == 1) tmp = att[B->obj]*4/10;
			if (tmp) c2++;
		} 
		if (!c1&&!c2) break;
	}
	if (A->heal>0&&B->heal>0) 
	{
		A->Arm.clear();B->Arm.clear();
		multiset<int>::iterator ii=S1.begin();
		for (;ii!=S1.end();ii++)
		{
			if (*ii==0||*ii==1) A->Arm.insert(*ii);
			if (*ii==2) A->Arm.insert(3);
			if (*ii==3) A->Arm.insert(2);
		 } 
		ii=S2.begin();
		for (;ii!=S2.end();ii++)
		{
			if (*ii==0||*ii==1) B->Arm.insert(*ii);
			if (*ii==2) B->Arm.insert(3);
			if (*ii==3) B->Arm.insert(2);
		 } 
		return;
	}
	if (A->heal<=0&&B->heal<=0) {(A->exis) = (B->exis) = 0;return;}
	if (A->heal>0)
	{
		A->Arm.clear();
		B->exis = 0;
		multiset<int>::iterator i = S1.begin();
		for (;i!=S1.end();i++)
		{
			if (*i==0||*i==1) A->Arm.insert(*i);
			if (*i==3) A->Arm.insert(2);
			if (*i==2) A->Arm.insert(3);
		}
		S1.clear();
		i = S2.begin();
		for (;i!=S2.end();i++)
		{
			if (*i==0||*i==1) S1.insert(*i);
			if (*i==3) S1.insert(2);
			if (*i==2) S1.insert(3);
		}
		int cnt=A->Arm.size();
		S2.clear();
		i = S1.begin();
		for (;i!=S1.end()&&cnt<10;cnt++,i++) A->Arm.insert(*i);
		//if (A->type==2&&A->obj==1)
		//	printf("%d %d %d %d\n",*i,cnt,S1.size(),A->Arm.size());
		printf("%s %s %d killed %s %s %d in city %d remaining %d elements\n",city[A->type],warrior[A->obj],A->ord,city[B->type],warrior[B->obj],B->ord,p,A->heal);
	}
	else
	{
		B->Arm.clear();
		A->exis = 0;
		multiset<int>::iterator i = S2.begin();
		for (;i!=S2.end();i++)
		{
			if (*i==0||*i==1) B->Arm.insert(*i);
			if (*i==3) B->Arm.insert(2);
			if (*i==2) B->Arm.insert(3);
		}
		S2.clear();
		i = S1.begin();
		for (;i!=S1.end();i++)
		{
			if (*i==0||*i==1) S2.insert(*i);
			if (*i==3) S2.insert(2);
			if (*i==2) S2.insert(3);
		}
		int cnt=B->Arm.size();
		S1.clear();
		i = S2.begin();
		for (;i!=S2.end()&&cnt<10;cnt++,i++) B->Arm.insert(*i);
		printf("%s %s %d killed %s %s %d in city %d remaining %d elements\n",city[B->type],warrior[B->obj],B->ord,city[A->type],warrior[A->obj],A->ord,p,B->heal);
	}
}
int main()
{
	//freopen("datapub.in","r",stdin);
	//freopen("datapub.ans","w",stdout);
	int cas;
	scanf("%d",&cas);
	int Cas=0;
	while (Cas<cas)
	{
		Cas++;
		printf("Case %d:\n",Cas);	
		scanf("%d%d%d%d",&M,&N,&K,&T);
		for (int i=0;i<5;i++) scanf("%d",&a[i]);
		for (int i=0;i<5;i++) scanf("%d",&att[i]);
		for (int i=0;i<=N+1;i++)
		{
			RedDragon[i].clear(),RedIceman[i].clear(),RedNinja[i].clear(),RedLion[i].clear(),RedWolf[i].clear();
			BlueDragon[i].clear(),BlueIceman[i].clear(),BlueNinja[i].clear(),BlueLion[i].clear(),BlueWolf[i].clear();
		}
		//for (int i=0;i<5;i++) Red[i]=Blue[i]=0;
		int Cnta=0,Cntb=0,flaga = 1,flagb = 1;
		conqA=conqB=0;
		//CC.clear();
		int rednow = 2,bluenow = 3;
		int redrest = M,bluerest = M;
		for (int i=0;i<=T;i+=60)
		{
			if (redrest >= a[rednow] && flaga) 
			{
				redrest -=a [rednow];
				PrintTime(i);
				printf("red %s %d born\n",warrior[rednow],++Cnta);
				if (rednow == 0) RedDragon[0].push_back(CDragon(1,rednow,Cnta,redrest));
				if (rednow == 1) RedNinja[0].pb(CNinja(1,rednow,Cnta,redrest));
				if (rednow == 2) RedIceman[0].pb(CIceman(1,rednow,Cnta,redrest));
				if (rednow == 3) RedLion[0].pb(CLion(1,rednow,Cnta,redrest));
				if (rednow == 4) RedWolf[0].pb(CWolf(1,rednow,Cnta,redrest));
				//redrest-=a[bluenow];
				rednow = Nexta[rednow];
			}
			else flaga = 0;
			
			if (bluerest >= a[bluenow]&&flagb)
			{
				bluerest -=a [bluenow];
				PrintTime(i);
				printf("blue %s %d born\n",warrior[bluenow],++Cntb);
				if (bluenow == 0) BlueDragon[N+1].pb(CDragon(2,bluenow,Cntb,bluerest));
				if (bluenow == 1) BlueNinja[N+1].pb(CNinja(2,bluenow,Cntb,bluerest));
				if (bluenow == 2) BlueIceman[N+1].pb(CIceman(2,bluenow,Cntb,bluerest));
				if (bluenow == 3) BlueLion[N+1].pb(CLion(2,bluenow,Cntb,bluerest));
				if (bluenow == 4) BlueWolf[N+1].pb(CWolf(2,bluenow,Cntb,bluerest));
				//bluerest-=a[bluenow];
				bluenow = Nextb[bluenow];
			}
			else flagb=0;
			
			if (i+5<=T)
			{
				for (int j = 0; j<=N+1; j++)
				{
					for (int ii=0;ii<RedLion[j].size();ii++)
					{
						RedLion[j][ii].RunLion(K,i+5);
						if (!RedLion[j][ii].exis)
						{
							RedLion[j].clear();
							break;
						}
					}
					for (int jj=0;jj<BlueLion[j].size();jj++)
					{
						BlueLion[j][jj].RunLion(K,i+5);
						if (!BlueLion[j][jj].exis)
						{
							BlueLion[j].clear();
							break;
						}
					}
				}
			}
			if (i+10<=T)
			{
				for (int j = N; j >= 0; j--)
				{
					Movep(RedDragon,j,i+10);
					Movep(RedIceman,j,i+10);
					Movep(RedLion,j,i+10);
					Movep(RedNinja,j,i+10);
					Movep(RedWolf,j,i+10);
				}
				for (int j = 1; j <= N+1;j++)
				{
					Moven(BlueDragon,j,i+10);
					Moven(BlueIceman,j,i+10);
					Moven(BlueLion,j,i+10);
					Moven(BlueNinja,j,i+10);
					Moven(BlueWolf,j,i+10);
				}
				for (int j = 0;j<=N+1;j++)
				{
					CCreature Redhere = CCreature(0,0,0,0);
					CCreature Bluehere = CCreature(0,0,0,0);
				//	Redhere = Null;
				//	Bluehere = Null;
					if (RedDragon[j].size()) Redhere = RedDragon[j][0];
					if (RedIceman[j].size()) Redhere = RedIceman[j][0];
					if (RedLion[j].size()) Redhere = RedLion[j][0];
					if (RedNinja[j].size()) Redhere = RedNinja[j][0];
					if (RedWolf[j].size()) Redhere = RedWolf[j][0];
					if (BlueDragon[j].size()) Bluehere = BlueDragon[j][0];
					if (BlueIceman[j].size()) Bluehere = BlueIceman[j][0];
					if (BlueLion[j].size()) Bluehere = BlueLion[j][0];
					if (BlueNinja[j].size()) Bluehere = BlueNinja[j][0];
					if (BlueWolf[j].size()) Bluehere = BlueWolf[j][0];
					Redhere.MovePrint(j,i+10);
					Bluehere.MovePrint(j,i+10);
				}
				if (conqA||conqB) break;
			}
			if (i+35<=T)
			{
				for (int j=1;j<=N;j++)
				{
					if (RedWolf[j].size()&&BlueWolf[j].size()) continue;
					if (RedWolf[j].size())
					{
						if (BlueDragon[j].size()) RedWolf[j][0].stealarm(BlueDragon[j][0],i+35,j);
						if (BlueIceman[j].size()) RedWolf[j][0].stealarm(BlueIceman[j][0],i+35,j);
						if (BlueLion[j].size()) RedWolf[j][0].stealarm(BlueLion[j][0],i+35,j);
						if (BlueNinja[j].size()) RedWolf[j][0].stealarm(BlueNinja[j][0],i+35,j);
					}
					if (BlueWolf[j].size())
					{
						if (RedDragon[j].size()) BlueWolf[j][0].stealarm(RedDragon[j][0],i+35,j);
						if (RedIceman[j].size()) BlueWolf[j][0].stealarm(RedIceman[j][0],i+35,j);
						if (RedLion[j].size()) BlueWolf[j][0].stealarm(RedLion[j][0],i+35,j);
						if (RedNinja[j].size()) BlueWolf[j][0].stealarm(RedNinja[j][0],i+35,j);
					}
				}
			}
			if (i+40<=T)
			{
				for (int j=1;j<=N;j++)
				{
				//	chee[j].clear();
					CCreature* Redhere = NULL; //a1 = CCreature(0,0,0,0);
					CCreature* Bluehere = NULL; //a2 = NULL;
					if (RedDragon[j].size()) Redhere = &RedDragon[j][0];
					if (RedIceman[j].size()) Redhere = &RedIceman[j][0];
					if (RedLion[j].size()) Redhere = &RedLion[j][0];
					if (RedNinja[j].size()) Redhere = &RedNinja[j][0];
					if (RedWolf[j].size()) Redhere = &RedWolf[j][0];
					if (BlueDragon[j].size()) Bluehere = &BlueDragon[j][0];
					if (BlueIceman[j].size()) Bluehere = &BlueIceman[j][0];
					if (BlueLion[j].size()) Bluehere = &BlueLion[j][0];
					if (BlueNinja[j].size()) Bluehere = &BlueNinja[j][0];
					if (BlueWolf[j].size()) Bluehere = &BlueWolf[j][0];
					if (Redhere&&Bluehere)
					{
						PrintTime(i+40);
						if (j&1) solo(Redhere,Bluehere,j);
						else solo(Bluehere,Redhere,j);
						if (Redhere->heal<=0&&Bluehere->heal<=0)
						{
							printf("both red %s %d and blue %s %d died in city %d\n",warrior[Redhere->obj],Redhere->ord,warrior[Bluehere->obj],Bluehere->ord,j);
						}
						if (Redhere->heal>0&&Bluehere->heal>0)
						{
							printf("both red %s %d and blue %s %d were alive in city %d\n",warrior[Redhere->obj],Redhere->ord,warrior[Bluehere->obj],Bluehere->ord,j);
						}
						if (Redhere->heal<=0) 
						{
							RedDragon[j].clear();
							RedIceman[j].clear();
							RedLion[j].clear();
							RedNinja[j].clear();
							RedWolf[j].clear();
						}
						else
						{
							Redhere->Cheer(i+40,j);
						}
						if (Bluehere->heal<=0)
						{
							BlueDragon[j].clear();
							BlueIceman[j].clear();
							BlueLion[j].clear();
							BlueWolf[j].clear();
							BlueNinja[j].clear();
						}
						else
						{
							Bluehere->Cheer(i+40,j);
						}
					}
				}
			}
			if (i+50<=T)
			{
				PrintTime(i+50);
				printf("%d elements in red headquarter\n",redrest);
				PrintTime(i+50);
				printf("%d elements in blue headquarter\n",bluerest);
			}
			if (i+55<=T)
			{
				for (int j = 0; j<=N+1; j++ )
				{
					PrintArm(RedDragon[j],i+55);
					PrintArm(RedIceman[j],i+55);
					PrintArm(RedLion[j],i+55);
					PrintArm(RedNinja[j],i+55);
					PrintArm(RedWolf[j],i+55);
					PrintArm(BlueDragon[j],i+55);
					PrintArm(BlueIceman[j],i+55);
					PrintArm(BlueLion[j],i+55);
					PrintArm(BlueNinja[j],i+55);
					PrintArm(BlueWolf[j],i+55);
				}
			}
		}

	}
	return 0;
}