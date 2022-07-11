#include "FreqBST.h"

// This destructs the BST by deallocating the root, and therefore the BST
FreqBST::~FreqBST() {
    root = nullptr;
}

// Takes a single word and adds it, and it's score to the BST
// If the word is already in the BST, it updates its score and frequency
void FreqBST::addScore(string word, int score) {
    WordNode *curr = root;
    WordNode *parent = nullptr;
    bool found = false;

    while (!found && curr != nullptr) {
        parent = curr;
        if (word < curr->word)
            curr = curr->left;
        else if (word > curr->word)
            curr = curr->right;
        else
            found = true;
    }

    if (!found) {
        curr = new WordNode;
        if (parent == nullptr) {
            root = curr;
            curr->word = word;
            curr->frequency = 1;
            curr->totalScore = score;
        }
        else if (word < parent->word) {
            parent->left = curr;
            curr->word = word;
            curr->frequency = 1;
            curr->totalScore = score;
        }
        else {
            parent->right = curr;
            curr->word = word;
            curr->frequency = 1;
            curr->totalScore = score;
        }
    }

    else {
        curr->frequency += 1;
        curr->totalScore += score;
    }
        
}

// This removes a word from the BST and updates the BST accordingly
void FreqBST::remove(string word) {
    WordNode *curr = root;
    WordNode *succ = nullptr;
    WordNode *parent = root;
    bool found = false;

    while (!found && curr != nullptr) {
        if (word < curr->word) {
            parent = curr;
            curr = curr->left;
        }

        else if (word > curr->word) {
            parent = curr;
            curr = curr->right;
        }

        else
            found = true;
    }
    
    if (!found)
        return;
    
    if (curr->left != nullptr && curr->right != nullptr) {
        succ = curr->right;
        parent = curr;
        while (succ->left != nullptr) {
            parent = succ;
            succ = succ->left;
        }
        
        curr->word = succ->word;
        curr->totalScore = succ->totalScore;
        curr->frequency = succ->frequency;
        curr = succ;
    }
    
    WordNode *subtree = curr->left;
    if (subtree == nullptr)
        subtree = curr->right;
    
    if (parent == nullptr)
        root = subtree;
    else if (parent->left == curr)
        parent->left = subtree;
    else
        parent->right = subtree;
    
    delete curr;
}

// This returns the average score of the word put into the parameter
// by finding the word in the BST
double FreqBST::getAverageScore(string word) const {
    WordNode *curr = root;
    WordNode *parent = nullptr;
    bool found = false;
    
    while (!found && curr != 0) {
        parent = curr;
        if (word < curr->word)
            curr = curr->left;
        else if (word > curr->word)
            curr = curr->right;
        else
            found = true;
    }
    
    if (!found)
        return 2;
    
    else
        return(curr->totalScore/curr->frequency);

}

// This prints the BST in preorder, taking a node as a parameter (usually the root)
void FreqBST::printPreorder(WordNode *curr) const {
    if (curr == nullptr)
        return;
    
    cout << curr->word << " ";
    printPreorder(curr->left);
    printPreorder(curr->right);
    
}

// This prints the BST in inorder, taking a node as a parameter (usually the root)
void FreqBST::printInorder(WordNode *curr) const {
    if (curr == nullptr)
        return;
    
    printInorder(curr->left);
    cout << curr->word << " ";
    printInorder(curr->right);
}

// This prints the BST in postorder, taking a node as a parameter (usually the root)
void FreqBST::printPostorder(WordNode *curr) const {
    if (curr == nullptr)
        return;
    
    printPostorder(curr->left);
    printPostorder(curr->right);
    cout << curr->word << " ";
}

// This prints the BST in inorder, printing each word, its frequency and its total score
// It takes a node as a parameter, usually the root
void FreqBST::printFrequencyTable(WordNode *curr) const {
    if (curr == nullptr)
        return;
    
    printFrequencyTable(curr->left);
    cout << "Word: " << curr->word << " ";
    cout << "Frequency: " << curr->frequency << " ";
    cout << "Total Score: " << curr->totalScore << endl;
    printFrequencyTable(curr->right);
}

// This counts and returns the words in the BST through recursion using a node (root) as a parameter
int FreqBST::countWords(WordNode *curr) const {
    if (curr == nullptr)
        return 0;
    
    else {
        int leftWords = countWords(curr->left);
        int rightWords = countWords(curr->right);
        return(leftWords + rightWords + 1);
    }
}

// This returns the height of the BST through recursion using a node (root) as a parameter
int FreqBST::height(WordNode *curr) const {
    if (curr == nullptr)
        return 0;
    
    else {
        int leftHeight = height(curr->left);
        int rightHeight = height(curr->right);
        
        if (leftHeight > rightHeight)
            return(leftHeight + 1);
        else
            return(rightHeight + 1);
    }
}

// This accesses the root, a private node
WordNode FreqBST::accessRoot() {
    return *root;
}
