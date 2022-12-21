#include <stdio.h>
#include <stdlib.h>

#define MAX 1000000
#define debug(x) printf("debug: %d", x)

struct Edge
{
    int u, v, w;
};

struct Edge_list
{
    struct Edge data[351115];
    int n;
};

struct Edge edge;
struct Edge_list elist;
struct Edge_list spanlist;

int VertexN;
int edge_cnt= 0;
void kruskalAlgorithm();
int Find(int vertexno);
void applyUnion(int c1, int c2);
void sort();
void print();

int belongs[1000005];

int cmp(const void *a, const void *b)
{
      struct Edge c = *(struct Edge *)a;
      struct Edge d = *(struct Edge *)b;
      if(c.w < d.w) {return -1;}
      else if (c.w == d.w) {return 0;}
      else return 1;
}

void kruskalAlgorithm(){
    int i, j, cno1, cno2;

    qsort(elist.data, edge_cnt, sizeof(elist.data[0]), cmp);

    for(i = 0 ; i < VertexN ; i ++){
        belongs[i] = i;
    }

    spanlist.n = 0;

    for(i = 0 ; i < edge_cnt ; i ++){
        cno1 = Find(elist.data[i].u);
        cno2 = Find(elist.data[i].v);

        if(cno1 != cno2){
            spanlist.data[spanlist.n] = elist.data[i];
            spanlist.n ++;
            applyUnion(cno1, cno2);
        }
    }
}

int Find(int vertexno){
    return vertexno == belongs[vertexno] ? vertexno : Find(belongs[vertexno]);
}

void applyUnion(int c1, int c2){
    belongs[Find(c1)] = Find(c2);
}

void print(){
    long long i, cost = 0;

    for(i = 0 ; i < spanlist.n ; i ++){
        cost += spanlist.data[i].w;
    }
    printf("\n%lld", cost);
}

int main(){
    int i, j, total_cost;
    scanf("%d %d", &VertexN, &edge_cnt);
    int cnt = 0;

    for(int k = 0 ; k < edge_cnt; k ++){
        int tmp;
        scanf("%d %d %d", &i, &j, &tmp);
        elist.data[cnt].u = i;
        elist.data[cnt].v = j;
        elist.data[cnt].u = j;
        elist.data[cnt].v = i;
        elist.data[cnt].w = tmp;
        elist.data[cnt++].w = tmp;
    }

    kruskalAlgorithm();
    print();
}