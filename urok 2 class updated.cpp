#include <iostream>
#include <vector>
#include <stack>
#include <random>
using namespace std;

class Task {
public:
    int id;
    string name;
    short priority;
    string status;

    Task(string name = "Unnamed task", short priority = 3, string status = "Default") {
        this->name = name;
        this->priority = priority;
        this->status = status;
    }

    void create_id(Task* object) {
        object->id = rand() % 10000;
    }

    int getId() {
        return id;
    }

    void show() {
        cout << id << ", " << name << ", " << status << ", " << priority << endl;
    }
};

class TaskM {
private:
    vector<Task>tasks;
public:
    void deleteTask(string name, int priority, string status) {
        auto iterator = find(tasks.begin(), tasks.end(), (name, priority, status));

        tasks.erase(iterator);
    }

    void addTask(string name = "Unnamed task", short priority = 3, string status = "Default") {
        tasks.push_back(Task(name, priority, status));
    }
};

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~Command() = default;
};

class AddTaskCom : public Command {
private:
    TaskM* manager;
    Task task;

public:
    AddTaskCom(TaskM* obj, Task task) {
        manager = obj;
        this->task = task;
    }
    void execute() override {
        manager->addTask(task.name, task.priority, task.status);
    }
    void undo() override {
        manager->deleteTask(task.name, task.priority, task.status);
    }
};

class ComManager {
private:
    stack<shared_ptr<Command>> undoS;
    stack<shared_ptr<Command>> redoS;
public:
    void executeCom(shared_ptr<Command>command) {
        command->execute();
        undoS.push(command);

        redoS.pop();
    }

    void undo(shared_ptr<Command> command) {
        if (undoS.empty()) {
            return;
        }
        auto lastCommand = undoS.top();

        undoS.pop();
    }

    void redo() {
        if (undoS.empty()) {
            return;
        }
        auto lastCommand = redoS.top();
        redoS.pop();
        lastCommand->execute();
        undoS.push(lastCommand);
    }

    void show() {
        cout << "undoS: ";
        for (int i = 0; i < undoS.size(); i++) {
            cout << undoS.top() << ", ";
            undoS.pop();
        }
        cout << "redoS: ";
        for (int i = 0; i < redoS.size(); i++) {
            cout << redoS.top() << ", ";
            redoS.pop();
        }
        cout << endl;
    }
};

int main()
{
    std::srand(std::time(0));
    TaskM space;
    space.addTask("Go for a walk", 5, "Not done");
    space.deleteTask("Go for a walk", 5, "Not done");

    ComManager manager;

    manager.executeCom(make_shared<AddTaskCom>(&space, "Go for a walk", 5, "Not done"));
    manager.executeCom(make_shared<AddTaskCom>(&space, "Go grab a coffee", 2, "Soon to be done"));
    manager.executeCom(make_shared<AddTaskCom>(&space, "Wake up", 1, "Done"));

    manager.show();

    manager.undo(make_shared<AddTaskCom>(&space, "task1"));

    manager.show();
}
