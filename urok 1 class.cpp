#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class TaskM {
private:
    vector<string>tasks;
public:
    void deleteTask(string task) {
        auto iterator = find(tasks.begin(), tasks.end(), task);

        tasks.erase(iterator);
    }

    void addTask(string task) {
        tasks.push_back(task);
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
    string task;

public:
    AddTaskCom(TaskM* obj, const string task) {
        manager = obj;
        this->task = task;
    }
    void execute() override {
        manager->addTask(task);
    }
    void undo() override {
        manager->deleteTask(task);
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
    }

    void undo(shared_ptr<Command> command) {
        if (undoS.empty()) {
            return;
        }
        auto lastCommand = undoS.top();

        undoS.pop();
    }

    void show() {
        cout << "undoS: ";
        stack<shared_ptr<Command>> undoSCopy = undoS;
        for (int i = 0; i < undoSCopy.size(); i++) {
            cout << undoSCopy.top() << ", ";
            undoSCopy.pop();
        }
        cout << "redoS: ";
        stack<shared_ptr<Command>> redoSCopy = redoS;
        for (int i = 0; i < redoSCopy.size(); i++) {
            cout << redoSCopy.top() << ", ";
            redoSCopy.pop();
        }
        cout << endl;
    }
};

int main()
{
    TaskM space;
    space.addTask("task1");
    space.deleteTask("task1");

    ComManager manager;

    manager.executeCom(make_shared<AddTaskCom>(&space, "task1"));
    manager.executeCom(make_shared<AddTaskCom>(&space, "task2"));
    manager.executeCom(make_shared<AddTaskCom>(&space, "task3"));

    manager.show();

    manager.undo(make_shared<AddTaskCom>(&space, "task1"));

    manager.show();
}
