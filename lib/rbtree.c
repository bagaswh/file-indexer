#include "util.h"

enum rbtree_node_color {
	RED,
	BLACK
};

/* begin rbtree */
struct rbtree_node {
	struct rbtree_node *p;
	struct rbtree_node *left;
	struct rbtree_node *right;
	enum rbtree_node_color color;
	u8 key[];
};

/* end rbtree */