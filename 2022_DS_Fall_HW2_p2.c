#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define table_size 100

typedef struct _NODE {
  int key;
  int value;
  int degree;
  struct _NODE *left_sibling;
  struct _NODE *right_sibling;
  struct _NODE *parent;
  struct _NODE *child;
  bool mark;
  bool visited;
} NODE;

typedef struct fibanocci_heap {
  int n/*size*/;
  NODE *min;
} FIB_HEAP;

FIB_HEAP *make_fib_heap();
NODE *init_node();
void insertion(FIB_HEAP *H, NODE *new);
NODE *extract_min(FIB_HEAP *H);
void consolidate(FIB_HEAP *H);
NODE *fib_heap_merge(NODE *x, NODE *y);
void decrease_key(FIB_HEAP *H, NODE *node, int key);
void cut(FIB_HEAP *H, NODE *node_to_be_decrease, NODE *parent_node);
void cascading_cut(FIB_HEAP *heap, NODE *parent_node);
void Delete_Node(FIB_HEAP *heap, NODE *node);
void unlink_node(NODE * node_to_unlink);
NODE *find_node(FIB_HEAP *heap, NODE *now, int key);

FIB_HEAP *make_fib_heap() {
    FIB_HEAP *heap = (FIB_HEAP *)malloc(sizeof(FIB_HEAP));
    heap->n = 0;
    heap->min = NULL;
    return heap;
}

NODE *init_node() {
  NODE *node = (NODE *)malloc(sizeof(NODE));
  node->child = NULL;
  node->parent = NULL;
  node->left_sibling = node;
  node->right_sibling = node;
  node->degree = 0;
  node->mark = false;
  //node->visited = false;
  return node;
}

void insertion(FIB_HEAP *heap, NODE *node) {
  if (heap->min == NULL || node->key < heap->min->key) {
    heap->min = node;
  } 
  else {
    node->left_sibling->right_sibling = heap->min->right_sibling;
    heap->min->right_sibling->left_sibling = node->left_sibling;
    heap->min->right_sibling = node;
    node->left_sibling = heap->min;
  }
  heap->n++ ;
}

void consolidate(FIB_HEAP *heap) {
  int i, d;
  NODE *A[table_size], *x;
  NODE *now = heap->min;
  if(now == NULL) return;
  for (i = 0; i <= table_size; i++) {
    A[i] = NULL;
  }
  do {
    d = now->degree;
    while (A[d]) {
      x = A[d];
      if (now->key != x->key) {
        now = fib_heap_merge(now, x);
        A[d ++] = NULL;
      }
    }
    if(now->key < heap->min->key) heap->min = now;
    x = now;
    now = now->right_sibling;
  } while (now != heap->min);
}

NODE *fib_heap_merge(NODE *x, NODE *y) {
  if(x->key > y->key) return fib_heap_merge(y, x);
  unlink_node(y);
  y->left_sibling = y;
  y->right_sibling = y;
  y->parent = x;
  if (x->child == NULL) x->child = y;
  y->right_sibling = x->child;
  y->left_sibling = x->child->left_sibling;
  x->child->left_sibling->right_sibling = y;
  x->child->left_sibling = y;
  if (y->key < x->child->key) x->child = y;
  x->degree++;
  return x;
}

NODE *extract_min(FIB_HEAP *heap) {
  NODE *Min_to_delete = heap->min;
  Delete_Node(heap, Min_to_delete);
  return Min_to_delete;
}

void unlink_node(NODE * node_to_unlink){
  node_to_unlink->left_sibling->right_sibling = node_to_unlink->right_sibling;
  node_to_unlink->right_sibling->left_sibling = node_to_unlink->left_sibling;
}

void cut(FIB_HEAP *heap, NODE *node_to_be_decrease, NODE *parent_node) {
  if (node_to_be_decrease == node_to_be_decrease->right_sibling)
    parent_node->child = NULL;
  else if (node_to_be_decrease == parent_node->child)
    parent_node->child = node_to_be_decrease->right_sibling;
  parent_node->degree--;
  node_to_be_decrease->parent = NULL;
  node_to_be_decrease->mark = false;
  unlink_node(node_to_be_decrease);
  node_to_be_decrease->left_sibling = node_to_be_decrease;
  node_to_be_decrease->right_sibling = node_to_be_decrease;
  insertion(heap, node_to_be_decrease);
}
 
void cascading_cut(FIB_HEAP *heap, NODE *parent_node) {
  if(parent_node == NULL) return;
  NODE *aux = parent_node->parent;
  if (aux) {
    if (!parent_node->mark) {
      parent_node->mark = true;
    } else {
      cut(heap, parent_node, aux);
      cascading_cut(heap, aux);
    }
  }
}

void decrease_key(FIB_HEAP *heap, NODE *node_to_be_decrease, int new_key) {
  NODE *parent_node = node_to_be_decrease->parent;
  if (heap == NULL) return;
  node_to_be_decrease->key = new_key;
  if ((parent_node != NULL) && (node_to_be_decrease->key < parent_node->key)) {
    cut(heap, node_to_be_decrease, parent_node);
    cascading_cut(heap, parent_node);
  }
  if (node_to_be_decrease->key < heap->min->key) {
    heap->min = node_to_be_decrease;
  }
}

NODE *find_node(FIB_HEAP *heap, NODE *now, int key) {
  NODE *find = NULL;
  NODE *f_now = now;
  if (now == NULL && f_now->visited == true) return NULL;
  f_now->visited = true;
  if (f_now->key == key) find = f_now;
  if (find == NULL && f_now->child) find = find_node(heap, f_now->child, key);
  if (find == NULL && f_now->right_sibling) find = find_node(heap, f_now->right_sibling, key);

  f_now->visited = false;
  return find;
}

void Delete_Node(FIB_HEAP *heap, NODE *node) {
  if(node == NULL) return;
  if(node->child){
    NODE *now = node->child;
    do{
      now->parent = NULL;
      now = now->right_sibling;      
    }while (node != node->child);
    insertion(heap, node->child);
  }
  unlink_node(node);
  if(heap->min == node->right_sibling){
    heap->min = NULL;
  }
  else{
    heap->min = node->right_sibling;
    consolidate(heap);
  }
}

int main() {
  NODE *min_node, *extracted_min;
  FIB_HEAP *heap = make_fib_heap();;
  int key, val, dec_key;
  while (1) {
    char method[10];
    scanf("%s", method);
    switch (method[0]){
      case 'i':
          scanf("%d %d", &key, &val);
          NODE *new_node = init_node();
          new_node->key = key;
          new_node->value = val;
          insertion(heap, new_node);
          consolidate(heap);
          break;
      case 'd':
        if(method[2] == 'l'){
          scanf("%d %d", &key, &val);
          NODE *ToDelete = find_node(heap, heap->min, key);
          decrease_key(heap, ToDelete, -2147483647);
          extract_min(heap);
        }
        else{
          scanf("%d %d %d", &key, &val, &dec_key);
          NODE *ToDecrease = find_node(heap, heap->min, key);
          decrease_key(heap, ToDecrease, key - dec_key);
        }
        break;
      case 'e':
        extracted_min = extract_min(heap);
        if(extracted_min != NULL) printf("(%d)%d\n", extracted_min->key, extracted_min->value);
        break;    
      case 'q':
        return 0;   
    }
  }
}