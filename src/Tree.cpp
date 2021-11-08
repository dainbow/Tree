#include "Tree.h"
#include "Graph.h"

int main() {
    TreeCtor(tree);

    tree.root->data  = 12;
    tree.root->left  = MakeNewNode(228);
    tree.root->right = MakeNewNode(337);

    tree.root->left->left = MakeNewNode(100-7);
    tree.root->left->right = MakeNewNode(322);

    tree.root->right->left = MakeNewNode(1337);
    tree.root->right->left->left = MakeNewNode(1);
    tree.root->right->left->right = MakeNewNode(2);

    tree.root->right->right = MakeNewNode(1488);
    tree.root->right->right->left = MakeNewNode(5);
    tree.root->right->right->right = MakeNewNode(6);

    MakeTreeGraph(tree.root, G_STANDART_NAME);
    TreeDtor(tree.root);

    printf("OK\n");
}

void TreeCtor_(Tree* tree) {
    assert(tree != nullptr);

    tree->treeDepth = 0;
    tree->root = (Node*)calloc(1, sizeof(tree->root[0]));
}

void TreeDtor(Node* root) {
    assert(root != nullptr);

    if (root->left != nullptr)  TreeDtor(root->left);
    if (root->right != nullptr) TreeDtor(root->right);

    root->data  = -666;
    root->left  = (Node*)13;
    root->right = (Node*)13;
}

Node* MakeNewNode(TreeElem data, Node* left, Node* right) {
    assert((left == nullptr) == (right == nullptr));

    Node* newNode = (Node*)calloc(1, sizeof(newNode[0]));
    *newNode = {data, left, right};

    return newNode;
}

void PrintTreeNodes(Node* root, FILE* output) {
    assert(root != nullptr);

    static int64_t rankArray[MAX_RECURSION_DEPTH] = {};
    static int32_t curRecursionDepth = 1;
    static int32_t curNodeNumber = 0;
    static Node* pointerAnchor = root;

    curNodeNumber++;    

    if (rankArray[curRecursionDepth / 2] == 0) {
        rankArray[curRecursionDepth / 2] = root - pointerAnchor;
    }
    else {
        fprintf(output, "\t{rank = same; %lld; %lld}\n", rankArray[curRecursionDepth / 2], root - pointerAnchor);
    }

    #if (_ENABLE_PTR_ADRESSES_VIEW_ == 1)
        fprintf(output, "\tsubgraph cluster%d {\n", curNodeNumber);
        fprintf(output, "\t\tstyle = filled;\n"
                        "\t\tcolor = lightgreen;\n"
                        "\t\t%lld;\n"
                        "\t\tlabel = \"%lld\";\n", root - pointerAnchor, root - pointerAnchor);
        fprintf(output, "\t}\n");
    #endif

    if ((root->left != nullptr) && (root->right != nullptr))  {
        fprintf(output, 
            "\t%lld[shape = record, style = rounded, color = red, label = \"{" TREE_TYPE "|{<l%d>" LEFT_BRANCH "|<r%d>" RIGHT_BRANCH "}}\"];\n", 
            root - pointerAnchor, root->data, 
            curNodeNumber, LEFT_BRANCH_VALUE, 
            curNodeNumber, RIGHT_BRANCH_VALUE);

        curRecursionDepth += 2;

        fprintf(output, "\t\t%lld: <l%d> -> %lld;\n", 
                root - pointerAnchor, curNodeNumber, root->left  - pointerAnchor);
        fprintf(output, "\t\t%lld: <r%d> -> %lld;\n", 
                root - pointerAnchor, curNodeNumber, root->right - pointerAnchor);

        PrintTreeNodes(root->left, output);
        PrintTreeNodes(root->right, output);
    }
    else {
        fprintf(output, 
            "\t%lld[shape = record, style = rounded, color = red, label = \"{" TREE_TYPE "|{0 | 0}}\"];\n", 
            root - pointerAnchor, root->data);

        curRecursionDepth--;
    }
}

bool VerifyTree(Node* root) {
    assert(root != nullptr);

    if ((root->left == nullptr) != (root->right == nullptr)) {
        return 1;
    }

    if (root->left  != nullptr) return VerifyTree(root->left);
    if (root->right != nullptr) return VerifyTree(root->right);
    return 0;
}
