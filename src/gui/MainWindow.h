#pragma once

#include "../headers/TaskManager.h"
#include <QHBoxLayout>
#include <QInputDialog>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);

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

  QListWidget *taskList_;
  QPushButton *addBtn_;
  QPushButton *removeBtn_;
  QPushButton *editBtn_;
  QPushButton *saveBtn_;
  QPushButton *loadBtn_;
};