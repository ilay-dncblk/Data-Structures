#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

struct TreeNode {
    int plakaKodu;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int plakaKodu)
            : plakaKodu(plakaKodu), left(nullptr), right(nullptr) {}
};

class Tree {
private:
    TreeNode* root;
    string plakaKodlari[100][2];

public:
    Tree() : root(nullptr) {}

    void Insert(TreeNode*& node, int plakaKodu) {
        if (node == nullptr) {
            node = new TreeNode(plakaKodu);
        } else {
            TreeNode* currentNode = node;
            while (true) {
                if (plakaKodu < currentNode->plakaKodu) {
                    if (currentNode->left == nullptr) {
                        currentNode->left = new TreeNode(plakaKodu);
                        break;
                    } else {
                        currentNode = currentNode->left;
                    }
                } else {
                    if (currentNode->right == nullptr) {
                        currentNode->right = new TreeNode(plakaKodu);
                        break;
                    } else {
                        currentNode = currentNode->right;
                    }
                }
            }
        }
    }

    int GetTreeWidth(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        } else {
            int leftWidth = GetTreeWidth(node->left);
            int rightWidth = GetTreeWidth(node->right);
            int currentWidth = to_string(node->plakaKodu).length();
            if (leftWidth > rightWidth) {
                return (leftWidth > currentWidth) ? leftWidth : currentWidth;
            } else {
                return (rightWidth > currentWidth) ? rightWidth : currentWidth;
            }
        }
    }

    int GetDepth(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        } else {
            int leftDepth = GetDepth(node->left);
            int rightDepth = GetDepth(node->right);
            if (leftDepth > rightDepth) {
                return leftDepth + 1;
            } else {
                return rightDepth + 1;
            }
        }
    }

    bool IsBalanced(TreeNode* node) {
        if (node == nullptr) {
            return true;
        } else {
            int leftDepth = GetDepth(node->left);
            int rightDepth = GetDepth(node->right);
            int diff = abs(leftDepth - rightDepth);
            return (diff <= 1) && IsBalanced(node->left) && IsBalanced(node->right);
        }
    }

    TreeNode* FindNode(TreeNode* node, int plakaKodu) {
        if (node == nullptr || node->plakaKodu == plakaKodu) {
            return node;
        } else if (plakaKodu < node->plakaKodu) {
            return FindNode(node->left, plakaKodu);
        } else {
            return FindNode(node->right, plakaKodu);
        }
    }

    int GetLevelDifference(TreeNode* node1, TreeNode* node2) {
        int depth1 = GetDepth(node1);
        int depth2 = GetDepth(node2);
        return abs(depth1 - depth2);
    }

    void PreorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            cout << node->plakaKodu << "-";
            PreorderTraversal(node->left);
            PreorderTraversal(node->right);
        }
    }

    void InorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            InorderTraversal(node->left);
            cout << node->plakaKodu << "-";
            InorderTraversal(node->right);
        }
    }

    void PostorderTraversal(TreeNode* node) {
        if (node != nullptr) {
            PostorderTraversal(node->left);
            PostorderTraversal(node->right);
            cout << node->plakaKodu << "-";
        }
    }

    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Dosya acilamadi!" << endl;
            return;
        }

        string line;
        int index = 0;
        while (getline(file, line)) {
            size_t commaPos = line.find(",");
            if (commaPos != string::npos) {
                int plakaKodu = stoi(line.substr(0, commaPos));
                string sehir = line.substr(commaPos + 1);
                Insert(root, plakaKodu);
                plakaKodlari[plakaKodu][0] = to_string(plakaKodu);
                plakaKodlari[plakaKodu][1] = sehir;
                index++;
            }
        }

        file.close();
    }

    int getcol(int h) {
        if (h == 1)
            return 1;
        return getcol(h - 1) + getcol(h - 1) + 1;
    }

    void printTree(int** M, TreeNode* node, int col, int row, int height) {
        if (node == nullptr)
            return;

        int nodeWidth = GetTreeWidth(node);
        int space = nodeWidth / 2;

        M[row][col] = node->plakaKodu;

        printTree(M, node->left, col - space, row + 1, height - 1);
        printTree(M, node->right, col + space, row + 1, height - 1);
    }

    void TreePrinter() {
        int h = GetDepth(root);
        int col = getcol(h);
        int** M = new int*[h];
        for (int i = 0; i < h; i++) {
            M[i] = new int[col];
            for (int j = 0; j < col; j++) {
                M[i][j] = 0;
            }
        }
        printTree(M, root, col / 2, 0, h);
        for (int i = 0; i < h; i++) {
            bool isFirstElement = true;
            for (int j = 0; j < col; j++) {
                if (M[i][j] == 0)
                    cout << "   ";
                else {
                    if (!isFirstElement)
                        cout << " ";
                    cout << M[i][j];
                    isFirstElement = false;
                }
            }
            cout << endl;
        }
    }

    void start() {
        readFromFile("sehir_plaka_kodlari.txt");

        cout << "Agac Goruntusu:\n";
        TreePrinter();

        int depth = GetDepth(root) - 1;
        bool isBalanced = IsBalanced(root);
        cout << "\nDerinlik: " << depth << endl;
        cout << "Denge Durumu: " << (isBalanced ? "Dengeli" : "Dengesiz") << endl;

        int plakaKodu1, plakaKodu2;
        bool isValidInput = false;
        string input;
        regex pattern("\\b\\d+,\\s+\\d+\\b");
        cout << "1. ve 2. Sehir icin plaka kodunu giriniz: ";
        while (!isValidInput) {
            getline(cin, input);

            istringstream iss(input);
            if (iss >> plakaKodu1 && (iss.ignore(), regex_match(input, pattern)) && iss >> plakaKodu2) {
                isValidInput = true;
            }

            if (!isValidInput) {
                cout << "Hatali giris!";
                cin.clear();
            }
        }
        TreeNode* node1 = FindNode(root, plakaKodu1);
        TreeNode* node2 = FindNode(root, plakaKodu2);
        if (node1 != nullptr && node2 != nullptr) {
            cout << "Sehirler: " << plakaKodlari[plakaKodu1][1] << ", " << plakaKodlari[plakaKodu2][1] << endl;
            cout << "Agactaki Duzey Farki: " << GetLevelDifference(node1, node2) << "\n" << endl;
        } else {
            cout << endl << "Girilen plaka kodlarina ait sehir bulunamadi!" << "\n" << endl;
        }

        cout << "Preorder Gezinme:" << endl;
        PreorderTraversal(root);
        cout << endl;

        cout << "\nInorder Gezinme:" << endl;
        InorderTraversal(root);
        cout << endl;

        cout << "\nPostorder Gezinme:" << endl;
        PostorderTraversal(root);
        cout << endl;
    }
};

#endif
