#include <iostream>
#include <string>
#include <Windows.h>

enum Color
{
    BLACK,
    RED
};

struct Node
{
    Node* left;
    Node* right;
    Node* parent;
    Color color;
    int data;
};

Node nil = { nullptr, nullptr, nullptr, BLACK, 0 };
Node* NIL = &nil;
Node* root = NIL;

void menu()
{
    std::cout << "1 - insert an element or elements\n";
    std::cout << "2 - delete an element\n";
    std::cout << "3 - output the tree\n";
    std::cout << "4 - exit\n";
}

void rotateRight(Node* node)
{
    Node* temp = node->left;
    node->left = temp->right;

    if (temp->right != NIL) temp->right->parent = node;

    temp->parent = node->parent;
    if (node->parent == nullptr) root = temp;
    else if (node == node->parent->right) node->parent->right = temp;
    else node->parent->left = temp;

    temp->right = node;
    node->parent = temp;
}

void rotateLeft(Node* node)
{
    Node* temp = node->right;
    node->right = temp->left;

    if (temp->left != nullptr) temp->left->parent = node;

    if (temp != nullptr) temp->parent = node->parent;
    if ((node != nullptr) && (node->parent != nullptr))
        if (node == node->parent->left) node->parent->left = temp;
        else node->parent->right = temp;
    else root = temp;

    temp->left = node;
    if (node != nullptr) node->parent = temp;
}

void balanceTree(Node* node)
{
    while ((node != root) && (node->parent->color == RED))
    {
        if (node->parent == node->parent->parent->left)
        {
            Node* temp = node->parent->parent->right;
            if (temp->color == RED)
            {
                node->parent->color = BLACK;
                temp->color = BLACK;
                node->parent->parent->color = BLACK;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    rotateLeft(node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateRight(node->parent->parent);
            }
        }
        else
        {
            Node* temp = node->parent->parent->left;
            if (temp->color == RED)
            {
                node->parent->color = BLACK;
                temp->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

bool insertNode(int value)
{
    Node* temp = new Node();
    temp->parent = nullptr;
    temp->data = value;
    temp->left = NIL;
    temp->right = NIL;
    temp->color = RED;

    Node* previous = nullptr, * copyRoot = root;
    bool isSame = false;
    while ((!isSame) && (copyRoot != NIL))
    {
        previous = copyRoot;
        if (temp->data < copyRoot->data) copyRoot = copyRoot->left;
        else if (temp->data > copyRoot->data) copyRoot = copyRoot->right;
        else isSame = true;
    }

    if (isSame) return false;

    temp->parent = previous;
    if (previous == nullptr) root = temp;
    else if (temp->data > previous->data) previous->right = temp;
    else previous->left = temp;

    if (temp->parent == nullptr) temp->color = BLACK;
    else if ((temp->parent != nullptr) && (temp->parent->parent != nullptr)) balanceTree(temp);

    return true;
}

void balanceTreeDelete(Node* node)
{
    while ((node != root) && (node->color == BLACK))
    {
        if (node == node->parent->left)
        {
            Node* uncle = node->parent->right;
            if (uncle->color == RED)
            {
                uncle->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                uncle = node->parent->right;
            }
            if ((uncle->left->color == BLACK) && (uncle->right->color == BLACK))
            {
                uncle->color = RED;
                node = node->parent;
            }
            else
            {
                if (uncle->right->color == BLACK)
                {
                    uncle->left->color = BLACK;
                    uncle->color = RED;
                    rotateRight(uncle);
                    uncle = node->parent->right;
                }
                uncle->color = node->parent->color;
                node->parent->color = BLACK;
                uncle->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        }
        else
        {
            Node* uncle = node->parent->left;
            if (uncle->color == RED)
            {
                uncle->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                uncle = node->parent->left;
            }
            if ((uncle->right->color == BLACK) && (uncle->left->color == BLACK))
            {
                uncle->color = RED;
                node = node->parent;
            }
            else
            {
                if (uncle->left->color == BLACK)
                {
                    uncle->right->color = BLACK;
                    uncle->color = RED;
                    rotateLeft(uncle);
                    uncle = node->parent->left;
                }
                uncle->color = node->parent->color;
                node->parent->color = BLACK;
                uncle->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}

bool deleteNode(int value)
{
    Node* temp = root;
    while ((temp != nullptr) && (temp->data != value))
        if (temp->data > value) temp = temp->left;
        else if (temp->data < value) temp = temp->right;

    if ((temp == nullptr) ||
        (
            (temp != nullptr) &&
            (temp->left == nullptr) &&
            (temp->right == nullptr)
            )
        ) return false;
    else
    {
        Node* rightFromRight, * leftFromRight;
        if ((temp->left == NIL) || (temp->right == NIL)) leftFromRight = temp;
        else
        {
            leftFromRight = temp->right;
            while (leftFromRight->left != NIL) leftFromRight = leftFromRight->left;
        }

        if (leftFromRight->left != NIL) rightFromRight = leftFromRight->left;
        else rightFromRight = leftFromRight->right;

        rightFromRight->parent = leftFromRight->parent;

        if ((leftFromRight->parent) && (leftFromRight == leftFromRight->parent->left))
            leftFromRight->parent->left = rightFromRight;
        else if ((leftFromRight->parent) && (leftFromRight != leftFromRight->parent->left))
            leftFromRight->parent->right = rightFromRight;
        else root = rightFromRight;

        if (leftFromRight != temp) temp->data = leftFromRight->data;

        if (leftFromRight->color == BLACK) balanceTreeDelete(rightFromRight);
    }
    return true;
}

void output(Node* start, int shift)
{
    if ((start != nullptr) && ((start->left != nullptr) || (start->right != nullptr)))
    {
        HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
        output(start->right, shift + 5);
        std::cout.width(shift);
        if (start->color == BLACK)
        {
            SetConsoleTextAttribute(Console, 7);
            std::cout << start->data << '\n';
        }
        else
        {
            SetConsoleTextAttribute(Console, 12);
            std::cout << start->data << '\n';
        }
        output(start->left, shift + 5);
        SetConsoleTextAttribute(Console, 7);
    }
}

int main()
{
    std::cout << "Input a number of nodes ";
    while (true)
    {
        system("cls");
        std::cout << "Choose an option:\n";
        menu();
        int choise = 0, element = 0;
        std::cin >> choise;
        std::string line;
        bool checkInsert = true;
        switch (choise)
        {
        case 1:

            std::cin.ignore(32767, '\n');
            std::cout << "Input an element or elements ";
            std::getline(std::cin, line);
            if (line.empty()) std::cout << "Error. You input nothing\n";
            else
            {
                int i = 0;
                bool checkInput = true;
                while (line.size() > i)
                {
                    std::string number;
                    while ((line.size() > i) && (line[i] != ' '))
                    {
                        if (((line[i] >= '0') && (line[i] <= '9')) || (line[i] == '-'))
                            number.push_back(line[i]);
                        else checkInput = false;
                        i++;
                    }
                    i++;

                    if (checkInput)
                    {
                        int value = std::stoi(number);
                        if (!insertNode(value))
                            std::cout << "Tree already has " << value << '\n';
                    }
                    else
                    {
                        std::cout << "Error. You input wrong\n";
                        exit(0);
                    }

                    if (!checkInsert)
                    {
                        std::cout << "Error. You inserted the same value\n";
                        exit(0);
                    }
                }
            }
            break;

        case 2:

            std::cin.ignore(32767, '\n');
            std::cout << "Input an element ";
            std::getline(std::cin, line);
            if (line.empty()) std::cout << "Error. You input nothing\n";
            else
            {
                int i = 0;
                bool checkInput = true;
                while (line.size() > i)
                {
                    std::string number;
                    while ((line.size() > i) && (line[i] != ' '))
                    {
                        if (((line[i] >= '0') && (line[i] <= '9')) || (line[i] == '-'))
                            number.push_back(line[i]);
                        else checkInput = false;
                        i++;
                    }
                    i++;

                    if (checkInput)
                    {
                        int value = std::stoi(number);
                        if (!deleteNode(value))
                            std::cout << value << " is not a node of the tree\n";
                    }
                    else
                    {
                        std::cout << "Error. You input wrong\n";
                        exit(0);
                    }
                }
            }
            break;

        case 3:

            std::cin.ignore(32767, '\n');
            if (root != nullptr) output(root, 5);
            else std::cout << "The tree is empty\n";
            break;

        case 4:

            exit(0);

        default:

            std::cout << "There is no such command";
            break;
        }
        std::cin.get();
    }
}
