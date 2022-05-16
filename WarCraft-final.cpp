#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector>
#include<set>
#define pb push_back
using namespace std;
int Att[10],a[10],flag[100],lastw[100],winn[100],ele[100],rdwin,blwin,M,N,R,K,T;
int tmp[100]; 
const char* arm[3] = {"sword","bomb","arrow"};
const char* warrior[5] = {"dragon","ninja","iceman","lion","wolf"};
const char* city[3] = {"","red","blue"};
const char* printmp[2] = {"",","};
const int Nexta[5] = {2,0,3,4,1};
const int Nextb[5] = {1,2,4,0,3};
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
class CCreature{
	public:
	bool exis;
    int type,obj,ord,res,heal,faith,att;	
    int sword,arrow,bomb;
	CCreature(){}
    CCreature (int ty , int ob , int Ord , int Res)
    {
        type = ty;
        obj = ob;
        ord = Ord;
        res = Res;
		faith = Res;
		heal = a[ob];
        att = Att[ob];
		exis = true;
        sword = arrow = bomb = 0;
    }
    ~CCreature()
    {
	}
	void createArm(int armid)
	{
		if (armid == 0) sword = att/5;
        if (armid == 2) arrow = 3;
        if (armid == 1) bomb = 1;
	}
	void Hurt(int n)
	{
		heal-=n;
	}
	virtual void Hurtself()
	{

	}
	virtual void RunLion(int K,int ti){}
	friend bool operator < (const CCreature & A,const CCreature & B)
	{
		return A.ord<B.ord;
    }
    virtual void dragonmorale(int fl){}
	virtual void Cheer(int ti,int p){}
    virtual void stealarm(CCreature* B){}
    virtual void nowin(){}
};
class CDragon : public CCreature{
 	public:
 		CDragon(){}
        double morale;
		CDragon (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
            morale = (double)Res/(double)heal;
            printf("Its morale is %.2f\n",morale);
		}
        virtual void dragonmorale(int fl)
        {
            if (fl) morale+=0.2;
            else morale-=0.2;
        }
        virtual void nowin()
        {
            dragonmorale(0);
        }
		virtual void Cheer(int ti,int p)
		{
            if (morale>0.8)
            {
		    	PrintTime(ti);
		    	printf("%s dragon %d yelled in city %d\n",city[type],ord,p);
            }
		}
		
};
class CNinja : public CCreature{
	public:
		CNinja(){}
		CNinja (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
			createArm((Ord+1)%3);
		}
};
class CIceman: public CCreature{
	public:
		int ff;
		CIceman(){}
		CIceman (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)  
		{
			createArm(Ord%3);
			ff = 0;
		}
		virtual void Hurtself()
		{
			ff ^= 1;
			if (ff==0)
			{
				heal-=9;
           		heal = max(heal,1);
            	att += 20;
			}
		}
};
class CLion : public CCreature{
    public: 
    	CLion(){}
    	CLion (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res)
        {
			//createArm(Ord%3);
			printf("Its loyalty is %d\n",Res);
        }
        virtual void nowin()
        {
            faith -= K;
        }
		void RunLion(int K,int ti)
		{
			if (faith <=0 && exis)
			{
				exis = false;
				PrintTime(ti);
				printf("%s lion %d ran away\n",city[type],ord);
			}
		}
};
class CWolf : public CCreature{
	public:
		CWolf(){}
		CWolf (int ty , int ob , int Ord , int Res):CCreature(ty,ob,Ord,Res) {}
		virtual void stealarm(CCreature* B)
        {
            if (!sword) sword = B->sword;
            if (!arrow) arrow = B->arrow;
            if (!bomb) bomb = B->bomb;
        }
};
CDragon RdDragon[1010],BlDragon[1010];
CNinja RdNinja[1010],BlNinja[1010];
CIceman RdIceman[1010],BlIceman[1010];
CLion RdLion[1010],BlLion[1010];
CWolf RdWolf[1010],BlWolf[1010]; 
vector<CCreature*> rd[50],bl[50];
void usearrow(int ti,int c1,int c2,CCreature* A,CCreature* B)
{
    if (A->arrow==0) return;
    A->arrow--;
    B->heal-=R;
    if (B->heal>0) 
    {
        PrintTime(ti);
        printf("%s %s %d shot\n",city[A->type],warrior[A->obj],A->ord);
    }
    else
    {
        PrintTime(ti);
        printf("%s %s %d shot and killed %s %s %d\n",city[A->type],warrior[A->obj],A->ord,city[B->type],warrior[B->obj],B->ord);
    }
}
void Usebomb(int p,int ti,CCreature* A,CCreature* B)
{
    PrintTime(ti);
    printf("%s %s %d used a bomb and killed %s %s %d\n",city[A->type],warrior[A->obj],A->ord,city[B->type],warrior[B->obj],B->ord);
}
void usebomb(int p,int ti)
{
    if (!bl[p][0]->bomb&&!rd[p][0]->bomb) return;
    int rdnow = rd[p][0]->heal,blnow = bl[p][0]->heal,fl=0;
    if (rdnow<=0||blnow<=0) return;
    if (flag[p]==1||(flag[p]==0&&(p&1))) 
    {
        blnow -= rd[p][0]->att+rd[p][0]->sword;
        if (blnow>0&&bl[p][0]->obj!=1) rdnow -= bl[p][0]->att/2+bl[p][0]->sword;
        if (blnow<=0&&bl[p][0]->bomb) Usebomb(p,ti,bl[p][0],rd[p][0]),fl=1;
        if (rdnow<=0&&rd[p][0]->bomb) Usebomb(p,ti,rd[p][0],bl[p][0]),fl=1;
    }
    else
    {
        rdnow -= bl[p][0]->att+bl[p][0]->sword;
        if (rdnow>0&&rd[p][0]->obj!=1) blnow -= rd[p][0]->att/2+rd[p][0]->sword;
        if (rdnow<=0&&rd[p][0]->bomb) Usebomb(p,ti,rd[p][0],bl[p][0]),fl=1;
        if (blnow<=0&&bl[p][0]->bomb) Usebomb(p,ti,bl[p][0],rd[p][0]),fl=1;
    }
    if (fl)
    {
        rd[p].clear(),bl[p].clear();
        winn[p] = -1;
    }
}
void bury(int p,int ti,CCreature* A,CCreature* B,int preb)
{
    //A->sword=A->sword*4/5;
    A->dragonmorale(1);
    A->stealarm(B);
    if (B->obj==3)
        A->heal+=preb;
}
void positive(int p,int ti,CCreature* A,CCreature* B)
{
    PrintTime(ti);
    printf("%s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",city[A->type],warrior[A->obj],A->ord,city[B->type],warrior[B->obj],B->ord,p,A->heal,A->att);
}
void negtive(int p,int ti,CCreature* A,CCreature* B)
{
    PrintTime(ti);
    printf("%s %s %d fought back against %s %s %d in city %d\n",city[A->type],warrior[A->obj],A->ord,city[B->type],warrior[B->obj],B->ord,p);
}
void Printdie(int p,int ti,CCreature* A)
{
    PrintTime(ti);
    printf("%s %s %d was killed in city %d\n",city[A->type],warrior[A->obj],A->ord,p);
}
int solo(int p,int ti)
{
    int winnn = 0,tmp = 0;
    int rdnow = rd[p][0]->heal,blnow = bl[p][0]->heal;
    if (rdnow<=0&&blnow<=0) 
    {
    	winn[p]=-1;
    	return 0;
	}
    if (rdnow<=0||blnow<=0) return 0;
    if (flag[p]==1||(flag[p]==0&&(p&1))) 
    {
        positive(p,ti,rd[p][0],bl[p][0]);
        bl[p][0]->heal -= rd[p][0]->att+rd[p][0]->sword;
        rd[p][0]->sword = rd[p][0]->sword*4/5;
        if (bl[p][0]->heal<=0) winnn = 1,tmp = blnow,Printdie(p,ti,bl[p][0]);
        else if (bl[p][0]->obj!=1)
        {
            negtive(p,ti,bl[p][0],rd[p][0]);
            rd[p][0]->heal -= bl[p][0]->att/2+bl[p][0]->sword;
            bl[p][0]->sword = bl[p][0]->sword*4/5;
            if (rd[p][0]->heal<=0) winnn = 2 , tmp = rdnow,Printdie(p,ti,rd[p][0]);
        }
    }
    else
    {
        positive(p,ti,bl[p][0],rd[p][0]);
		//if (p==4) printf("xxxxxx%d\n",rd[p][0]->heal);
        rd[p][0]->heal -= bl[p][0]->att+bl[p][0]->sword;
        bl[p][0]->sword = bl[p][0]->sword*4/5;
        if (rd[p][0]->heal<=0) winnn = 2 , tmp = rdnow,Printdie(p,ti,rd[p][0]);
        else if (rd[p][0]->obj!=1)
        {
            negtive(p,ti,rd[p][0],bl[p][0]);
            bl[p][0]->heal -= rd[p][0]->att/2+rd[p][0]->sword;
            rd[p][0]->sword = rd[p][0]->sword*4/5;
            if (bl[p][0]->heal<=0) winnn = 1,tmp = blnow,Printdie(p,ti,bl[p][0]);
        }    
    }
    winn[p] = winnn;
    return tmp;
}
void moverd(int p,int ti)
{
    rd[p][0]->Hurtself();
    rd[p+1].push_back(rd[p][0]);
    rd[p].clear();
}
void movebl(int p,int ti)
{
    bl[p][0]->Hurtself();
    bl[p-1].push_back(bl[p][0]);
    bl[p].clear();
}
void PrintMove(int p,int ti,CCreature* A)
{
    PrintTime(ti);
    if (p == 0)
    {
        printf("blue %s %d reached red headquarter with %d elements and force %d\n",warrior[A->obj],A->ord,A->heal,A->att);
        if (blwin==2) 
        {
            PrintTime(ti);
            puts("red headquarter was taken");
        }
        return;
    }
    if (p == N+1)
    {
        printf("red %s %d reached blue headquarter with %d elements and force %d\n",warrior[A->obj],A->ord,A->heal,A->att);
        if (rdwin==2)
        {
            PrintTime(ti);
            puts("blue headquarter was taken");
        }
        return;
    }
    printf("%s %s %d marched to city %d with %d elements and force %d\n",city[A->type],warrior[A->obj],A->ord,p,A->heal,A->att);
}
void Printearn(int p,int ti,CCreature* A,int element)
{
    if (!element) return;
    PrintTime(ti);
    printf("%s %s %d earned %d elements for his headquarter\n",city[A->type],warrior[A->obj],A->ord,element);
}
int main()
{
    int cas;
    scanf("%d",&cas);
    int Cas = 0;
    while (cas--)
    {
        printf("Case %d:\n",++Cas);
        scanf("%d%d%d%d%d",&M,&N,&R,&K,&T);
        for (int i=0;i<5;i++) scanf("%d",&a[i]);
        for (int i=0;i<5;i++) scanf("%d",&Att[i]);
        for (int i=0;i<=N+1;i++)
            rd[i].clear(),bl[i].clear(),lastw[i]=0;
        for (int i=0;i<=N+1;i++) ele[i]=0,flag[i]=0;
        int Cnta=0,Cntb=0;
        rdwin = blwin = 0;
		//conqA=conqB=0;
		//CC.clear();
		int rednow = 2,bluenow = 3;
		int redrest = M,bluerest = M;
        int rddragon=0,rdninja=0,rdiceman=0,rdlion=0,rdwolf=0;
        int bldragon=0,blninja=0,bliceman=0,bllion=0,blwolf=0;
        for (int h=0;h<=T;h+=60)
        {
            if (redrest >= a[rednow]) 
			{
				redrest -=a [rednow];
				PrintTime(h);
				printf("red %s %d born\n",warrior[rednow],++Cnta);
                if (rednow == 0) RdDragon[++rddragon]=CDragon(1,rednow,Cnta,redrest),rd[0].pb(&RdDragon[rddragon]);
                if (rednow == 1) RdNinja[++rdninja]=CNinja(1,rednow,Cnta,redrest),rd[0].pb(&RdNinja[rdninja]);
				if (rednow == 2) RdIceman[++rdiceman]=CIceman(1,rednow,Cnta,redrest),rd[0].pb(&RdIceman[rdiceman]);
                if (rednow == 3) RdLion[++rdlion]=CLion(1,rednow,Cnta,redrest),rd[0].pb(&RdLion[rdlion]);
                if (rednow == 4) RdWolf[++rdwolf]=CWolf(1,rednow,Cnta,redrest),rd[0].pb(&RdWolf[rdwolf]);
                rednow = Nexta[rednow];
			}
            if (bluerest >= a[bluenow]) 
			{
				bluerest -=a [bluenow];
				PrintTime(h);
				printf("blue %s %d born\n",warrior[bluenow],++Cntb);
                if (bluenow == 0) BlDragon[++bldragon]=CDragon(2,bluenow,Cntb,bluerest),bl[N+1].pb(&BlDragon[bldragon]);
                if (bluenow == 1) BlNinja[++blninja]=CNinja(2,bluenow,Cntb,bluerest),bl[N+1].pb(&BlNinja[blninja]);
				if (bluenow == 2) BlIceman[++bliceman]=CIceman(2,bluenow,Cntb,bluerest),bl[N+1].pb(&BlIceman[bliceman]);
                if (bluenow == 3) BlLion[++bllion]=CLion(2,bluenow,Cntb,bluerest),bl[N+1].pb(&BlLion[bllion]);
                if (bluenow == 4) BlWolf[++blwolf]=CWolf(2,bluenow,Cntb,bluerest),bl[N+1].pb(&BlWolf[blwolf]);
                bluenow = Nextb[bluenow];
			}
			if ( h+5 <= T )
        	{
            for (int i=0;i<=N+1;i++)
            {
                if (rd[i].size())
                {
                    rd[i][0]->RunLion(i,h+5);
                    if (!rd[i][0]->exis) rd[i].clear();
                }
                if (bl[i].size())
                {
                    bl[i][0]->RunLion(i,h+5);
                    if (!bl[i][0]->exis) bl[i].clear();
                }
            }
        	}
        	if (h+10<=T)
        	{
            for (int i=N;i>=0;i--)
                if (rd[i].size()) moverd(i,h+10);
            for (int i=1;i<=N+1;i++)
                if (bl[i].size()) movebl(i,h+10);
            if (bl[0].size()>blwin) blwin=bl[0].size(),PrintMove(0,h+10,bl[0][blwin-1]);
            for (int i=1;i<=N;i++)
            {
                if (rd[i].size()) PrintMove(i,h+10,rd[i][0]);
                if (bl[i].size()) PrintMove(i,h+10,bl[i][0]);
            }
            if (rd[N+1].size()>rdwin) rdwin=rd[N+1].size(),PrintMove(N+1,h+10,rd[N+1][rdwin-1]);
        }
        if (blwin==2||rdwin==2) break;
        if (h+20<=T)
            for (int i=0;i<=N+1;i++) ele[i]+=10;
        if (h+30<=T)
        {
            for (int i=1;i<=N;i++)
            {
                if (rd[i].size()&&(bl[i].size()==0)) Printearn(i,h+30,rd[i][0],ele[i]),redrest+=ele[i],ele[i]=0;
                if (bl[i].size()&&(rd[i].size()==0)) Printearn(i,h+30,bl[i][0],ele[i]),bluerest+=ele[i],ele[i]=0;
            }
        }
        if (h+35<=T)
        {
            for (int i=1;i<=N;i++)
            {
                if (i<N&&rd[i].size()&&bl[i+1].size()) usearrow(h+35,i,i+1,rd[i][0],bl[i+1][0]);
                if (i>1&&bl[i].size()&&rd[i-1].size()) usearrow(h+35,i,i-1,bl[i][0],rd[i-1][0]);
            }
            for (int i=1;i<=N;i++)
            {
                winn[i] = 0;
                if (rd[i].size()&&rd[i][0]->heal<=0) winn[i] = 2;
                if (bl[i].size()&&bl[i][0]->heal<=0) winn[i] = 1;
                if (rd[i].size()&&rd[i][0]->heal<=0&&bl[i].size()&&bl[i][0]<=0) winn[i] = -1;
            }
        }
        if (h+38<=T)
        {
            for (int i=1;i<=N;i++)
                if (rd[i].size()&&bl[i].size()) usebomb(i,h+38);
        }
        if (h+40<=T)
        {
        	int rdpre = redrest;
        	int blpre = bluerest;
        	for (int i=1;i<=N;i++)
        	{
        		int tmp = 0;
        		if (rd[i].size()&&bl[i].size()) tmp = solo(i,h+40);
        		else winn[i]=-1;
        		if (winn[i]==-1) continue;
        		if (winn[i]==0)
        		{
        			rd[i][0]->nowin();
        			bl[i][0]->nowin();
                    if (flag[i]==1||(flag[i]==0&&i%2==1)) rd[i][0]->Cheer(h+40,i);
                    if (flag[i]==2||(flag[i]==0&&i%2==0)) bl[i][0]->Cheer(h+40,i);
				}
				if (winn[i]==1)
				{
					bury(i,h+40,rd[i][0],bl[i][0],tmp);
                    if (flag[i]==1||(flag[i]==0&&i%2==1)) rd[i][0]->Cheer(h+40,i);
                    bl[i].clear();
				}
				if (winn[i] == 2)
                {
                    bury(i,h+40,bl[i][0],rd[i][0],tmp);
                    if (flag[i]==2||(flag[i]==0&&i%2==0)) bl[i][0]->Cheer(h+40,i);
                    rd[i].clear();
                }
                if (winn[i]==1) Printearn(i,h+40,rd[i][0],ele[i]),redrest+=ele[i],ele[i]=0;
                if (winn[i]==2) Printearn(i,h+40,bl[i][0],ele[i]),bluerest+=ele[i],ele[i]=0;
                if (winn[i]!=0&&winn[i]!=-1&&winn[i]==lastw[i])
                {
                    if (winn[i]!=flag[i])
                    {
                        PrintTime(h+40);
                        printf("%s flag raised in city %d\n",city[winn[i]],i);
                        flag[i]=winn[i];
                    }
                }
                lastw[i]=winn[i];
			}
			for (int i=1;i<=N;i++)
				if (winn[i]==1&&rdpre>=8) rdpre-=8,redrest-=8,rd[i][0]->heal+=8;
			for (int i=N;i;i--)
				if (winn[i]==2&&blpre>=8) blpre-=8,bluerest-=8,bl[i][0]->heal+=8;
    		for (int i=1;i<=N;i++)
    		{
    			if (rd[i].size()&&rd[i][0]->heal<=0) rd[i].clear();
    			if (bl[i].size()&&bl[i][0]->heal<=0) bl[i].clear();
			}
        }
        if (h+50<=T)
        {
            PrintTime(h+50);
            printf("%d elements in red headquarter\n",redrest);
            PrintTime(h+50);
            printf("%d elements in blue headquarter\n",bluerest);        
        }
        if (h+55<=T)
        {
            for (int i=0;i<=N+1;i++)
            {
                if (!rd[i].size()) continue;
                PrintTime(h+55);
                printf("red %s %d has ",warrior[rd[i][0]->obj],rd[i][0]->ord);
                int fl=0;
                if (rd[i][0]->arrow) printf("arrow(%d)",rd[i][0]->arrow),fl=1;
                if (rd[i][0]->bomb) printf("%sbomb",printmp[fl]),fl=1;
                if (rd[i][0]->sword) printf("%ssword(%d)",printmp[fl],rd[i][0]->sword),fl=1;
                if (!fl) printf("no weapon");
                puts("");
            }
            for (int i=0;i<=N+1;i++)
            {
                if (!bl[i].size()) continue;
                PrintTime(h+55);
                printf("blue %s %d has ",warrior[bl[i][0]->obj],bl[i][0]->ord);
                int fl=0;
                if (bl[i][0]->arrow) printf("arrow(%d)",bl[i][0]->arrow),fl=1;
                if (bl[i][0]->bomb) printf("%sbomb",printmp[fl]),fl=1;
                if (bl[i][0]->sword) printf("%ssword(%d)",printmp[fl],bl[i][0]->sword),fl=1;
                if (!fl) printf("no weapon");
                puts("");
            }
		}
        }
    }
    return 0;
}