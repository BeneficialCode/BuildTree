// BuildTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
#define GVDLL
#include <graphviz/gvc.h>

struct Node {
    char data;
    struct Node* lt, * rt;
};

// 前序遍历
void preorder(struct Node* root) {
    if (root) {
        printf("%c", root->data);
        preorder(root->lt);
        preorder(root->rt);
    }
}

// 中序遍历
void midorder(struct Node* root) {
    if (root) {
        midorder(root->lt);
        printf("%c", root->data);
        midorder(root->rt);
    }
}

// 后序遍历
void postorder(struct Node* root) {
    if (root) {
        postorder(root->lt);
        postorder(root->rt);
        printf("%c", root->data);
    }
}

// 层序遍历
void levelorder(struct Node* root) {
    if (root == nullptr)
        return;

    std::queue<Node*> queue;
    struct Node* cur;

    queue.push(root);

    while (!queue.empty()) {
        // while q is not empty
        // dequeue
        cur = queue.front();
        queue.pop();

        printf("%c", cur->data);

        // push children to quueue
        if (cur->lt != nullptr)
            queue.push(cur->lt);
        if (cur->rt != nullptr)
            queue.push(cur->rt);
    }
}

Agnode_t* draw_tree_node(Agraph_t* g, Agnode_t* parent, char* name, char* label, int color) {
    Agnode_t* node = agnode(g, name, 1);
    
    if (parent != nullptr) {
        Agedge_t* edge = agedge(g, parent, node, nullptr, 1);
        // 隐藏边
        if(name == nullptr)
            agsafeset(edge, "style", "invis", "");
    }
   /* if (strcmp("hide", name) == 0) {
        agsafeset(node, "style", "invis", "");
    }*/
    if (name == nullptr) {
        // 隐藏节点
        agsafeset(node, "style", "invis", "");
    }
    if (label != nullptr) {
        agsafeset(node, "label", label, "");
    }
    
    return node;
}


std::string get_node_name(char c) {
    char node_name[20];
    sprintf_s(node_name, "%c", c);
    return node_name;
}

void draw_tree(Agraph_t* g,Agnode_t* parent,Node* parent_node, Node* node) {
    if (node != nullptr) {
        std::string node_name = get_node_name(node->data);
        parent = draw_tree_node(g, parent, 
            const_cast<char*>(node_name.c_str()), nullptr, 0);

        if (node->lt != nullptr) {
            draw_tree(g, parent, node, node->lt);
        }
        else {
            draw_tree_node(g, parent, NULL, NULL, 0);
        }

        if (node->rt != nullptr) {
            draw_tree(g, parent, node, node->rt);
        }
        else {
            draw_tree_node(g, parent, NULL, NULL, 0);
        }

        draw_tree_node(g, parent, "hide", NULL, 0);
    }
}

// 由先序序列和中序序列建立二叉树
struct Node* create_by_pre_and_mid(int n, char* pre, char* mid) {
    struct Node* root;
    int i;
    if (n == 0)
        return nullptr;

    root = (struct Node*)malloc(sizeof(Node));
    root->data = pre[0];

    for (i = 0; i < n; i++) {
        // 寻找左右子树的元素
        if (pre[0] == mid[i])
            break;
    }
    // 建立左子树
    root->lt = create_by_pre_and_mid(i, pre + 1, mid);
    // 建立右子树
    root->rt = create_by_pre_and_mid(n - i - 1, pre + i + 1, mid + i + 1);

    return root;
}

// 由后序和中序序列建立二叉树
struct Node* create_by_post_and_mid(int n, char* post, char* mid) {
    struct Node* root;
    int i;

    if (n == 0)
        return nullptr;

    root = (struct Node*)malloc(sizeof(struct Node));
    root->data = post[n - 1];

    for (i = 0; i < n; i++) {
        // 寻找左右子树的元素
        if (post[n - 1] == mid[i])
            break;
    }

    root->lt = create_by_post_and_mid(i, post, mid);
    root->rt = create_by_post_and_mid(n - i - 1, post + i, mid + i + 1);

    return root;
}

// 由层序序列和中序序列建立二叉树
struct Node* create_by_level_and_mid(int n, char* level, char* mid)
{
    struct Node* root;
    char left[20];
    char right[20];
    int i, j, k;
    int l, r;
    int lcnt, rcnt;

    lcnt = 0;
    rcnt = 0;

    if (n == 0)
        return NULL;

    root = (struct Node*)malloc(sizeof(struct Node));
    root->data = level[0];

    // 找到根节点
    for (i = 0; i < n; i++)
        if (level[0] == mid[i])
            break;

    // 划分左右子树
    for (k = 0; k < n; k++)
    {
        for (l = 0; l < i; l++)
        {
            if (mid[l] == level[k])
            {
                left[lcnt++] = level[k];
            }
        }
        for (r = 0; r < n - i - 1; r++)
        {
            if (mid[r + i + 1] == level[k])
            {
                right[rcnt++] = level[k];
            }
        }
    }

    root->lt = create_by_level_and_mid(lcnt, left, mid);
    root->rt = create_by_level_and_mid(rcnt, right, mid + i + 1);

    return root;
}
/**
 *
 *           A
 *          / \
 *         B   C
 *        / \ /  \
 *       D  E G   H
 *         /       \
 *        F         I
 **/
int main(void)
{
    struct Node* root = NULL;
    int len;
    char* post = "DFEBGIHCA";
    char* pre = "ABDEFCGHI";
    char* mid = "DBFEAGCHI";
    char* level = "ABCDEGHFI";

    len = strlen(mid);

    //root = create_by_pre_and_mid(len, pre, mid);
    //root = create_by_post_and_mid(len, post, mid);
    root = create_by_level_and_mid(len, level, mid);

    printf("\r\npreorder:\r\n");
    preorder(root);

    printf("\r\nmidorder:\r\n");
    midorder(root);

    printf("\r\npostorder:\r\n");
    postorder(root);

    printf("\r\nlevelorder:\r\n");
    levelorder(root);

    printf("\r\n");

    GVC_t* gvc = nullptr;
    gvc = gvContext();

    Agraph_t* g = agopen("g", Agdirected, 0);

    draw_tree(g, nullptr, nullptr, root);

    gvLayout(gvc, g, "dot");

    gvRenderFilename(gvc, g, "pdf", "tree.pdf");

    gvFreeLayout(gvc, g);
    agclose(g);

    gvFreeContext(gvc);

    system("pause");
    return 0;
}
