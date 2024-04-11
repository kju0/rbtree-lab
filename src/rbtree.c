#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h>

rbtree *new_rbtree(void) {
  // TODO: rbtree와 nilnode 메모리 할당 및 초기화하는 함수
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  
  nilNode->color = RBTREE_BLACK;
  p->nil = nilNode;
  p->root = nilNode;
  
  return p;
}

void right_rotation(rbtree * tree, node_t * x){
  // TODO: 타겟 노드 x의 왼쪽 자식을 타겟 노드 x를 오른쪽 부모로 갖는 부모로 바꿔주는 함수

  node_t *y;

  // 1. target의 left으로 y 설정
  y = x->left;
  // 2. y의 오른쪽 서브트리를 target의 왼쪽 서브트리로 옮김
  x->left = y->right;
  // 3. y의 오른쪽 노드가 NIL이 아니라면, y의 오른쪽 노드 부모를 target으로 설정
  if (y->right != tree->nil) {
    y->right->parent = x;
  }
  // 4. y의 부모 노드를 target의 부모 노드로 설정
  y->parent = x->parent;
  // 5. target의 부모 노드가 nil이라면, 트리 구조체의 root를 y로 설정
  if (x->parent == tree->nil)
    tree->root = y;
  // 6. target이 target 부모 노드의 왼쪽이면, target 부모의 왼쪽을 y로 설정
  else if (x == x->parent->left)
    x->parent->left = y;
  // 7. target이 target 부모 노드의 오른쪽이면, target 부모의 오른쪽을 y로 설정
  else
    x->parent->right = y;
  // 8. target을 y의 오른쪽으로 설정
  y->right = x;
  // 9. target의 부모를 y로 설정
  x->parent = y;
}

void left_rotation(rbtree * tree, node_t * x){
  // TODO: 타겟 노드 x의 오른쪽 자식을 타겟 노드 x를 왼쪽 부모로 갖는 부모로 바꿔주는 함수
  node_t *y;

  y = x->right;
  x->right = y->left;
  
  if (y->left != tree->nil) 
    y->left->parent = x;
  
  y->parent = x->parent;
  
  if (x->parent == tree->nil) 
    tree->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else 
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}

void free_node(rbtree *t, node_t *x) {
  // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  // TODO: 삽입 후 삽입한 노드부터 root까지 탐색하며 
  // 레드블랙트리 규칙 준수 여부 검사 및 회전, 색칠 재조정하는 함수
  node_t *y;
  
  while (z->parent->color == RBTREE_RED) {
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    if (z->parent == z->parent->parent->left) {
      y = z->parent->parent->right;
      
      // CASE 1 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 2 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->right) {
          z = z->parent;
          left_rotation(t, z);
        }
        // CASE 3 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotation(t, z->parent->parent);
      }
    }
    // z의 부모가 조부모의 왼쪽 서브 트리일 경우
    else {
      y = z->parent->parent->left;

      // CASE 4 : 노드 z의 삼촌 y가 적색인 경우
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // CASE 5 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotation(t, z);
        }
        // CASE 6 : z의 삼촌 y가 흑색이며의 z가 오른쪽 자식인 경우
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotation(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: 이진탐색트리 삽입과 같이 새로운 노드를 삽입하는 함수
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  
  z->key = key;

  while (x != t->nil) {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right; 
  }

  z->parent = y;
  
  if (y == t->nil) {
    t->root = z;
  }
  else if (z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rbtree_insert_fixup(t, z);

  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: rbtree에서 key값을 갖는 노드가 있으면 해당 노드 리턴, 없으면 NULL 리턴
  node_t * current = t->root;

  while (current != t->nil) {
    if (current->key == key)
      return current;

    if (current->key < key) 
      current = current->right;
    else
      current = current->left;
  }

  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: RB tree 중 최소 값을 가진 node pointer 반환
  if (t->root == t->nil) {
    return NULL;
  }
  node_t * curr = t->root;

  while (curr->left != t->nil) {
    curr = curr->left;
  }
  return curr;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: 최대값을 가진 node pointer 반환
  if (t->root == t->nil) {
    return NULL;
  }

  node_t * curr = t->root;

  while (curr->right != t->nil) {
    curr = curr->right;
  }
  return curr;
}

void rbtree_transplant(rbtree *t, node_t * u, node_t * v) {
  // TODO: 노드 u의 부모의 자식으로 노드 v로 바꿔주는 함수
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x) {
  // TODO: 삭제한 노드 x 부터 레드블랙트리 규칙을 준수하지 않았는지 검사하고 재조정하는 함수
  while (x != t->root && x->color == RBTREE_BLACK) {
    // CASE 1 ~ 4 : LEFT CASE
    if (x == x->parent->left){
      node_t *w = x->parent->right;
      
      // CASE 1 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation(t, x->parent);
        w = x->parent->right;
      }

      // CASE 2 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 3 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else{ 
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation(t, w);
          w = x->parent->right;
        }

        // CASE 4 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation(t, x->parent);
        x = t->root;
      }
    }
    // CASE 5 ~ 8 : RIGHT CASE
    else {
      node_t *w = x->parent->left;

      // CASE 5 : x의 형제 w가 적색인 경우
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation(t, x->parent);
        w = x->parent->left;
      }

      // CASE 6 : x의 형제 w는 흑색이고 w의 두 지식이 모두 흑색인 경우
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      // CASE 7 : x의 형제 w는 흑색, w의 왼쪽 자식은 적색, w의 오른쪽 자신은 흑색인 경우
      else 
      {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation(t, w);
          w = x->parent->left;
        }

        // CASE 8 : x의 형제 w는 흑색이고 w의 오른쪽 자식은 적색인 경우
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: RB tree 내부의 ptr로 지정된 node를 삭제하고 메모리 반환하는 함수
  node_t *y;
  node_t *x;
  color_t y_origin_color;

  y = p;
  y_origin_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else {
    y = p->right;
    while(y->left != t->nil){
      y = y->left;
    }
    y_origin_color = y->color;
    x = y->right;

    if (y->parent == p) {
      x->parent = y;
    } 
    else {
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_origin_color == RBTREE_BLACK) {
    rbtree_delete_fixup(t, x);
  }

  free(p);

  return 0;
}

void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count) {
  // TODO: 키 순서대로 배열을 반환하기 위해 중위순회하는 함수
  if (curr == t->nil) {
    return;
  }
  
  subtree_to_array(t, curr->left, arr, n, count);
  if (*count < n) {
    arr[(*count)++] = curr->key;
  }
  else return;
  subtree_to_array(t, curr->right, arr, n, count);

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: key 순서대로 배열 만들어서 반환하는 함수
  if (t->root == t->nil) {
    return 0;
  }

  size_t cnt = 0;
  subtree_to_array(t, t->root, arr, n, &cnt); 
  
  return 0;
}