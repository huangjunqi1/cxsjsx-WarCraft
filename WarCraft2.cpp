#include<cstdio>
#include<algorithm>
#include<vector>
#include<cstring>
#include<string>
using namespace std;
struct node{
	int type,obj,ord,cnt,res;
};
vector<node> ans [10010];
int Red[10],Blue[10],a[10],flag[10];
const int Nexta[5] = {2,0,3,4,1};
const int Nextb[5] = {1,2,4,0,3};
const char* arm[3] = {"sword","bomb","arrow"};
int main()
{
	int cas;
	scanf("%d",&cas);
	int Cas=0;
	while (Cas<cas)
	{
		Cas++;
		printf("Case:%d\n",Cas);
		int nowtime = 0,M;
		scanf("%d",&M);
		for (int i=0;i<5;i++) scanf("%d",&a[i]);
		for (int i=0;i<5;i++)
			Red[i]=Blue[i]=0;
		int M1 = M,nowa=2,cnta=0,t=0,Cnta=0;
		for (int i=0;i<=M;i++)
			ans[i].clear();
		memset(flag,0,sizeof(flag));
		while (cnta<5)
		{
			if (M1<a[nowa]) 
			{
				
				if (!flag[nowa]) cnta++;
				flag[nowa]=1;
				nowa=Nexta[nowa]; 
				continue;
			}
			M1-=a[nowa];
			ans[t++].push_back((node){0,nowa,++Cnta,++Red[nowa],M1});
			nowa=Nexta[nowa]; 
		}
		ans[t++].push_back((node){0,5,0,0,0});
		t = 0,nowa=3,cnta=0,t=0,Cnta=0;
		M1=M;
		memset(flag,0,sizeof(flag));
		while (cnta<5)
		{
			if (M1<a[nowa])
			{
				
				if (!flag[nowa]) cnta++;
				flag[nowa]=1;
				nowa = Nextb[nowa];
				continue;
			}
			M1-=a[nowa];
			ans[t++].push_back((node){1,nowa,++Cnta,++Blue[nowa],M1});
			nowa = Nextb[nowa];
		}
		ans[t++].push_back((node){1,5,0,0,0});
		for (int i=0;i<=M;i++)
			for (int j=0;j<ans[i].size();j++)
			{
				if (i<10) printf("00");
				else if (i<100) printf("0");
				printf("%d",i);
				if (ans[i][j].type == 0) printf(" red ");
				else printf(" blue ");
				if (ans[i][j].obj == 5)
				{
					puts("headquarter stops making warriors");
					continue;
				}
				if (ans[i][j].obj == 0) printf("dragon");
				if (ans[i][j].obj == 1) printf("ninja");
				if (ans[i][j].obj == 2) printf("iceman");
				if (ans[i][j].obj == 3) printf("lion");
				if (ans[i][j].obj == 4) printf("wolf");
				printf(" %d ",ans[i][j].ord);
				printf("born with strength ");
				printf("%d,",a[ans[i][j].obj]);
				printf("%d ",ans[i][j].cnt);
				if (ans[i][j].obj == 0) printf("dragon");
				if (ans[i][j].obj == 1) printf("ninja");
				if (ans[i][j].obj == 2) printf("iceman");
				if (ans[i][j].obj == 3) printf("lion");
				if (ans[i][j].obj == 4) printf("wolf");
				printf(" in ");
				
				if (ans[i][j].type == 0) printf("red");
				else printf("blue");
				puts(" headquarter");
				if (ans[i][j].obj == 0)
				{
					printf("It has a %s,and it's morale is %.2f\n",arm[ans[i][j].ord%3],(double)ans[i][j].res/a[ans[i][j].obj]);
				}
				if (ans[i][j].obj == 1)
				{
					printf("It has a %s and a %s\n",arm[ans[i][j].ord%3],arm[(ans[i][j].ord+1)%3]);
				}
				if (ans[i][j].obj == 2)
				{
					printf("It has a %s\n",arm[ans[i][j].ord%3]);
				}
				if (ans[i][j].obj == 3)
				{
					printf("It's loyalty is %d\n",ans[i][j].res);
				}
			}
	}
	return 0;
}