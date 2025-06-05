#pragma once

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include "../headers/TaskManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAddTask();
    void onRemoveTask();
    void onEditTask();
    void onSaveTasks();
    void onLoadTasks();

private:
    void refreshList();

    TaskManager manager_;
    const std::string defaultFilename_ = "tasks.json";

    QListWidget* taskList_;
    QPushButton* addBtn_;
    QPushButton* removeBtn_;
    QPushButton* editBtn_;
    QPushButton* saveBtn_;
    QPushButton* loadBtn_;
};