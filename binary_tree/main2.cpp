#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class Employee {
public:
    int id;
    int salary;
    int department;
    Employee(int id, int salary, int department) : id(id), salary(salary), department(department) {}
};

class Node {
public:
    Employee* employee;
    Node* left;
    Node* right;
    Node(Employee* employee) : employee(employee), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node* root;

    void insertRecursive(Node*& node, Employee* employee) {
        if (!node) {
            node = new Node(employee);
            return;
        }
        if (employee->id < node->employee->id) {
            insertRecursive(node->left, employee);
        } else {
            insertRecursive(node->right, employee);
        }
    }

    void printPreOrder(Node* node) {
        if (node) {
            cout << node->employee->id << ";" << node->employee->salary << ";" << node->employee->department << endl;
            printPreOrder(node->left);
            printPreOrder(node->right);
        }
    }

    int getHeight(Node* node) {
        if (!node) return 0;
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        return max(leftHeight, rightHeight) + 1;
    }

    void updateRecursive(Node*& node, int id, int newSalary, int newDepartment) {
        if (!node) return;
        if (id == node->employee->id) {
            node->employee->salary = newSalary;
            node->employee->department = newDepartment;
        } else if (id < node->employee->id) {
            updateRecursive(node->left, id, newSalary, newDepartment);
        } else {
            updateRecursive(node->right, id, newSalary, newDepartment);
        }
    }

    Node* deleteRecursive(Node*& node, int id) {
        if (!node) return node;
        if (id < node->employee->id) {
            node->left = deleteRecursive(node->left, id);
        } else if (id > node->employee->id) {
            node->right = deleteRecursive(node->right, id);
        } else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = minValueNode(node->right);
            node->employee = temp->employee;
            node->right = deleteRecursive(node->right, temp->employee->id);
        }
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

public:
    BinaryTree() : root(nullptr) {}

    void insert(Employee* employee) {
        insertRecursive(root, employee);
    }

    void printToConsole() {
        printPreOrder(root);
    }

    int getHeight() {
        return getHeight(root);
    }

    void update(int id, int newSalary, int newDepartment) {
        updateRecursive(root, id, newSalary, newDepartment);
    }

    void remove(int id) {
        root = deleteRecursive(root, id);
    }
};
int main() {
    BinaryTree bt;

    // Read employee data from CSV file
    ifstream file("minidataset.csv");
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id, salary, department;
        char delimiter;
        ss >> id >> delimiter >> salary >> delimiter >> department;
        bt.insert(new Employee(id, salary, department));
    }
    file.close();

    // Read operations from another CSV file and perform them
    ifstream opFile("tree_op1.csv");
    while (getline(opFile, line)) {
        if (line == "PRINT") {
            bt.printToConsole();
        } else if (line == "HEIGHT") {
            cout << "H " << bt.getHeight() << endl;
        } else if (line.find("ADD") == 0) {
            stringstream ss(line.substr(4));
            int id, salary, department;
            char delimiter;
            ss >> id >> delimiter >> salary >> delimiter >> department;
            bt.insert(new Employee(id, salary, department));
        } else if (line.find("UPDATE") == 0) {
            stringstream ss(line.substr(7));
            int id, newSalary, newDepartment;
            char delimiter;
            ss >> id >> delimiter >> newSalary >> delimiter >> newDepartment;
            bt.update(id, newSalary, newDepartment);
        } else if (line.find("DELETE") == 0) {
            int id = stoi(line.substr(7));
            bt.remove(id);
        }
        // Implement other operations as required
    }
    opFile.close();

    return 0;
}