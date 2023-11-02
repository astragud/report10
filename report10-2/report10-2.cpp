#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100
#define INF 1000

int parent[MAX_VERTICES]; // 부모 노드

void set_init(int n) // 초기화
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = -1;
    }
}

// curr가 속하는 집합을 반환한다.
int set_find(int curr)
{
    if (parent[curr] == -1)
    {
        return curr; // 루트
    }

    while (parent[curr] != -1)
    {
        curr = parent[curr];
    }
    return curr;
}

// 두개의 원소가 속한 집합을 합친다.
void set_union(int a, int b)
{
    int root1 = set_find(a); // 노드 a의 루트를 찾는다. 
    int root2 = set_find(b); // 노드 b의 루트를 찾는다. 
    if (root1 != root2) // 합한다. 
    {
        parent[root1] = root2;
    }
}

struct Edge // 간선을 나타내는 구조체
{
    int start, end, weight;
};

typedef struct GraphType
{
    int n; // 간선의 개수
    struct Edge edges[2 * MAX_VERTICES];
} GraphType;

// 그래프 초기화
void graph_init(GraphType* g)
{
    g->n = 0;
    for (int i = 0; i < 2 * MAX_VERTICES; i++)
    {
        g->edges[i].start = 0;
        g->edges[i].end = 0;
        g->edges[i].weight = 1000;
    }
}

// 간선 삽입 연산
void insert_edge(GraphType* g, int start, int end, int w)
{
    g->edges[g->n].start = start;
    g->edges[g->n].end = end;
    g->edges[g->n].weight = w;
    g->n++;
}

// 힙 구조체 정의
typedef struct HeapType
{
    struct Edge heap[MAX_VERTICES];
    int heap_size;
} HeapType;

// 최소 힙 초기화
void init_heap(HeapType* h)
{
    h->heap_size = 0;
}

// 최소 힙에 요소 삽입
void insert_min_heap(HeapType* h, struct Edge item)
{
    int i;
    i = ++(h->heap_size);
    while ((i != 1) && (item.weight < h->heap[i / 2].weight))
    {
        h->heap[i] = h->heap[i / 2];
        i /= 2;
    }
    h->heap[i] = item;
}

// 최소 힙에서 요소 삭제
struct Edge delete_min_heap(HeapType* h)
{
    int parent, child;
    struct Edge item, temp;
    item = h->heap[1];
    temp = h->heap[(h->heap_size)--];
    parent = 1;
    child = 2;

    while (child <= h->heap_size)
    {
        if ((child < h->heap_size) && (h->heap[child].weight) > h->heap[child + 1].weight)
        {
            child++;
        }

        if (temp.weight < h->heap[child].weight)
        {
            break;
        }

        h->heap[parent] = h->heap[child];
        parent = child;
        child *= 2;
    }

    h->heap[parent] = temp;
    return item;
}

// kruskal의 최소 비용 신장 트리 프로그램
void kruskal(GraphType* g)
{
    int edge_accepted = 0; // 현재까지 선택된 간선의 수
    int uset, vset; // 정점 u와 정점 v의 집합 번호
    struct Edge e;

    set_init(g->n); // 집합 초기화
    HeapType heap;
    init_heap(&heap);

    for (int i = 0; i < g->n; i++)
    {
        insert_min_heap(&heap, g->edges[i]);
    }

    printf("크루스칼 최소 신장 트리 알고리즘 \n");

    while (edge_accepted < (g->n - 1)) // 간선의 수 < (n-1)
    {
        e = delete_min_heap(&heap);
        uset = set_find(e.start); // 정점 u의 집합 번호
        vset = set_find(e.end); // 정점 v의 집합 번호

        if (uset != vset) // 서로 속한 집합이 다르면
        {
            printf("간선 (%d,%d) %d 선택\n", e.start, e.end, e.weight);
            edge_accepted++;
            set_union(uset, vset); // 두 개의 집합을 합친다.
        }
    }
}

int main()
{
    GraphType* g;
    g = (GraphType*)malloc(sizeof(GraphType));
    graph_init(g);

    insert_edge(g, 1, 6, 11);
    insert_edge(g, 1, 2, 3);
    insert_edge(g, 1, 7, 12);
    insert_edge(g, 6, 5, 9);
    insert_edge(g, 6, 2, 7);
    insert_edge(g, 7, 2, 8);
    insert_edge(g, 5, 2, 1);
    insert_edge(g, 7, 8, 13);
    insert_edge(g, 7, 3, 6);
    insert_edge(g, 3, 2, 5);
    insert_edge(g, 3, 8, 5);
    insert_edge(g, 2, 4, 4);
    insert_edge(g, 3, 4, 2);
    insert_edge(g, 8, 4, 14);
    insert_edge(g, 5, 4, 13);
    insert_edge(g, 5, 9, 18);
    insert_edge(g, 8, 10, 15);
    insert_edge(g, 4, 10, 16);
    insert_edge(g, 5, 10, 17);
    insert_edge(g, 9, 10, 10);

    kruskal(g);
    free(g);
    return 0;
}