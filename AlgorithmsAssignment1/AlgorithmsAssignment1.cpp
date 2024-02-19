
using namespace std;
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

const string filename = "product_data.txt";

struct Product {
public:
    int id;
    string name;
    string category;
    float price;

    Product(int _id, string _name, string _category, float _price) {
        id = _id;
        name = _name;
        category = _category;
        price = _price;
    }
};

class Node {
public: // this *should* be fine
    Product* product;
    Node* next;

    Node(Product* _product) {
        product = _product;
        next = nullptr;
    };
    Node(Product* _product, Node* _next) {
        product = _product;
        next = _next;
    };
};

class LinkedList {
public:
    Node* head;

    LinkedList() {
        head = nullptr;
    }

    LinkedList(Node* _head) {
        head = _head;
    }
    
    LinkedList(vector<Product> products) {
        head = nullptr;

        for (Product product : products) { // foreach basically
            Node* newNode = new Node(new Product(product)); // new node
            InsertNode(newNode); // insert node
        }
    }

    void InsertNode(Node* _node, int _pos)
    {
        if (_pos == 0) { // run if the position is at the beginning
            if (head != nullptr)
                _node->next = head;

            head = _node;
            return;
        }

        Node* _temp = head;
        int i = 0;
        // run through the list and pointers and find item at position
        while (_temp != nullptr && i < _pos - 1) {
            _temp = _temp->next;
            i++;
        }

        if (_temp == nullptr) { // out of range exception
            cout << "Item at position " << _pos << " not found.";
            return;
        }

        _node->next = _temp->next;
        _temp->next = _node;
    }

    void InsertNode(Node* _node) // append
    {
        if (head == nullptr) {
            head = _node;
            return;
        }

        Node* _temp = head;
        while (_temp->next != nullptr) {
            _temp = _temp->next;
        }

        _temp->next = _node;
    }

    void DeleteNode(Node* _node)
    {
        if (_node == head)
        {
            head = head->next;
            delete _node;
            return;
        }

        Node* _temp = head;
        while (_temp != nullptr && _temp->next != _node) {
            _temp = _temp->next;
        }

        if (_temp == nullptr) { // exception
            cout << "Item not found.";
            return;
        }

        _temp->next = _node->next;
        delete _node;
    }

    template<typename T> // copypaste cringe i'm not writing the same thing four times
    const Node* SearchFor(const T& _key, string _param) {
        Node* _temp = head;
        transform(_param.begin(), _param.end(), _param.begin(), ::tolower);
        auto key = ConvertKey(_key, _param);

        while (_temp != nullptr)
        {
            // SWITCH STATEMENTS DON'T WORK WITH STRINGS????? YANDEV MOMENT
            
            if (_param == "id") {
                if (_temp->product->id == key) return _temp;
            }
            else if (_param == "name") {
                if (_temp->product->name == key) return _temp;
            }
            else if (_param == "price") {
                if (_temp->product->price == key) return _temp;
            }
            else {
                cout << "Type of parameter not found."; // throw exception
                return nullptr;
            }

            _temp = _temp->next;
        }

        return nullptr;
    }

    template<typename T>
    const vector<Node*> SearchCategory(string _key, string _param) { // returns matching category
        Node* _temp = head;
        vector<Node*> matchCategory;

        transform(_param.begin(), _param.end(), _param.begin(), ::tolower);
        auto key = ConvertKey(_key, _param);

        while (_temp != nullptr)
        {
            if (_temp->product->category == key)
                matchCategory.push_back(_temp);

            _temp = _temp->next;
        }

        return matchCategory;
    }

    template<typename T>
    void UpdateProduct(Node* _node, string _key, string _param) {
        transform(_param.begin(), _param.end(), _param.begin(), ::tolower);
        auto key = ConvertKey(_key, _param);

        if (_param == "id") {
            _node->product->id = key;
        }
        else if (_param == "name") {
            _node->product->name = key;
        }
        else if (_param == "price") {
            _node->product->price = key;
        }
        else if (_param == "category") {
            _node->product->category = key;
        }
        else {
            cout << "Type of parameter not found."; // throw exception
            return;
        }
    }

    template<typename T>
    T ConvertKey(string _key, string _param) {
        if (_param == "id") {
            return stoi(_key);
        }
        else if (_param == "price") {
            return stof(_key);
        }

        return _key;
    }

    // SORTING ALGORITHM GOES HERE
    void BubbleSortPrice() {
        if (head == nullptr || head->next == nullptr) {
            return;
        }

        auto startAt = chrono::steady_clock::now();

        bool swap = false;
        Node* _now = nullptr;
        Node* _max = nullptr;

        do {
            swap = false;
            Node* _now = head;

            while (_now->next != _max) { // PAIN AND SUFFERING
                if (_now == nullptr || _now->next == nullptr)
                    break;

                if (_now->product->price > _now->next->product->price) {
                    SwapNodes(_now);

                    swap = true;
                }
                
                if (_now->next != nullptr)
                    _now = _now->next;
                else
                    break;
            }
            // _max = _now; // i give up on this tbh
        } while (swap);

        auto endAt = chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(endAt - startAt);

        cout << "Bubble sort finished in " << duration.count() << "microseconds.";
    }

    void SwapNodes(Node* _now)
    {
        if (FindPrevious(_now) == nullptr) {
            head = _now->next;
        }
        else {
            FindPrevious(_now)->next = _now->next;
        }

        Node* temp = _now->next->next;
        _now->next->next = _now;
        _now->next = temp;
    }

    // swapping two nodes makes a previous node skip over the swapped node so this helps a bit
    Node* FindPrevious(Node* _node)
    {
        if (head == nullptr || head == _node)
        {
            return nullptr;
        }

        Node* _temp = head;
        while (_temp->next != nullptr && _temp->next != _node) {
            _temp = _temp->next;
        }

        if (_temp->next == nullptr) {
            return nullptr;
        }

        // cout << "Previous node found with ID: " << _temp->product->id << "\n";

        return _temp;
    }

    // other stuff
    void PrintList() {
        Node* _temp = head;
        while (_temp != nullptr)
        {
            cout << "ID: " << _temp->product->id << ","
                << "Name: " << _temp->product->name << ","
                << "Category: " << _temp->product->category << ","
                << "Price: " << _temp->product->price << "\n";

            _temp = _temp->next;
        }
    }
};

class FileReadWrite {  // HELP
public:
    static vector<Product> ReadFile(string filename) {
        vector<Product> products;
        string line;

        ifstream file(filename); // open

        if (!file.is_open()) { // if the file can't be opened
            std::cout << "Failed to open file: " << filename;
            return products;
        }

        while (getline(file, line)) {
            istringstream iss(line); // iss: istringstream
            char c = ','; // delimiter
            string id, name, category, price;

            // parses the line also checks if it can be parsed at all
            if (getline(iss, id, c) &&
                getline(iss, name, c) &&
                getline(iss, category, c) &&
                getline(iss, price)) 
            {
                Product newProduct(stoi(id), name, category, stof(price));
                products.push_back(newProduct);
            }
            else {
                cout << "Could not parse line" << line;
            }
        }
        file.close(); // close
        return products;
    }
    static void WriteFile(LinkedList* list, string filename) {
        ofstream file(filename); // open

        if (!file.is_open()) { // if the file can't be opened
            std::cout << "Failed to open file: " << filename;
            return;
        }

        Node* _temp = list->head;
        char c = ',';

        while (_temp != nullptr) { // writes to file
            file << _temp->product->id << c
                << _temp->product->name << c
                << _temp->product->category << c
                << _temp->product->price << "\n";

            _temp = _temp->next;
        }

        file.close(); // close
    }
};

int main()
{
    FileReadWrite* rw = new FileReadWrite();
    LinkedList* itemList = new LinkedList(rw->ReadFile(filename));

    itemList->PrintList();

    cout << "\n Now sorting by price: \n";
    itemList->BubbleSortPrice();
    itemList->PrintList();
    // rw->WriteFile(itemList, filename);

    // delete objects
    delete rw;
    delete itemList;

    return 0;
}

