#include <iostream>
#include <map>
#include <vector>
#include <tuple>
using namespace std;

enum class TaskStatus {
    NEW, // новая
    IN_PROGRESS, // в разработке
    TESTING, // на тестировании
    DONE // завершена
};

using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return teamTasks.at(person);
    }

    void AddNewTask(const string& person) {
        TasksInfo& tmp = teamTasks[person];
        tmp[TaskStatus::NEW]++;
    }

    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
            const string& person, int task_count) {
        TasksInfo updated;
        TasksInfo constant;
        TasksInfo& pTasks = teamTasks[person];
        for (int i = 0; i < task_count; ++i) {
            if (pTasks[TaskStatus::NEW] > 0) {
                updated[TaskStatus::IN_PROGRESS]++;
                pTasks[TaskStatus::NEW]--;
            } else if (pTasks[TaskStatus::IN_PROGRESS] > 0) {
                updated[TaskStatus::TESTING]++;
                pTasks[TaskStatus::IN_PROGRESS]--;
            } else if (pTasks[TaskStatus::TESTING] > 0) {
                updated[TaskStatus::DONE]++;
                pTasks[TaskStatus::TESTING]--;
            } else {
                break;
            }
        }
        for (auto item : pTasks) {
            if ( item.second != 0 && item.first != TaskStatus::DONE ) {
                constant[item.first] = item.second;
            } else if( item.second == 0 ) {
                pTasks.erase(item.first);
            }
        }

        for (auto item : updated) {
            pTasks[item.first] += item.second;
        }
        for (auto item : pTasks) {
            if (item.second == 0) {
                pTasks.erase(item.first);
            }
        }
        return make_tuple(updated, constant);
    }

private:
    map<string, TasksInfo> teamTasks;
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
            ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
            ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
            ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice");
    }
    TasksInfo updated;
    TasksInfo untoched;
    
    tie( updated, untoched ) = tasks.PerformPersonTasks("Alice", 5);
    tie( updated, untoched ) = tasks.PerformPersonTasks("Alice", 5);
    tie( updated, untoched ) = tasks.PerformPersonTasks("Alice", 1);
    PrintTasksInfo( tasks.GetPersonTasksInfo( "Alice" ));
    for (int i = 0; i < 5; ++i) {
        tasks.AddNewTask("Alice");
    }
    PrintTasksInfo( tasks.GetPersonTasksInfo( "Alice" ));
    tie( updated, untoched ) = tasks.PerformPersonTasks("Alice", 2);
    PrintTasksInfo( untoched );
    return 0;
}