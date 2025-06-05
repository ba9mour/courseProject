#pragma once

#include "Task.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

// ===============================================
// CLASS TaskManager: operating std::vector<Task>
// and CRUD methods + sorting.
// ===============================================
class TaskManager {
public:
  TaskManager() = default;

  // Add new Task
  void addTask(const Task &t) { tasks_.push_back(t); }

  // Delete task by index
  void removeTask(size_t index) {
    if (index >= tasks_.size()) {
      std::cerr << "Ошибка: неверный индекс задачи.\n";
      return;
    }
    tasks_.erase(tasks_.begin() + index);
  }

  void updateTask(size_t index, const Task &newTask) {
    if (index >= tasks_.size()) {
      std::cerr << "Ошибка: неверный индекс задачи.\n";
      return;
    }
    tasks_[index] = newTask;
  }

  const std::vector<Task> &getAllTasks() const { return tasks_; }

  template <typename Compare> void sortTasks(Compare cmp) {
    std::sort(tasks_.begin(), tasks_.end(), cmp);
  }

  // ================================================================
  // Save all tasks to JSON
  // ================================================================
  void saveToFile(const std::string &filename) const {
    json j_array = json::array();
    for (const auto &t : tasks_) {
      j_array.push_back(t);
    }

    std::ofstream ofs(filename);
    if (!ofs.is_open()) {
      std::cerr << "Ошибка: не удалось открыть файл для записи: " << filename
                << "\n";
      return;
    }
    ofs << j_array.dump(4);
    ofs.close();
  }

  // ================================================================
  // Loading tasks from JSOM
  // ================================================================
  void loadFromFile(const std::string &filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
      std::cerr << "Внимание: файл \"" << filename
                << "\" не найден. Будет создан при сохранении.\n";
      return;
    }

    json j_array;
    try {
      ifs >> j_array;
      if (!j_array.is_array()) {
        std::cerr << "Ошибка: формат файла неверный (ожидался массив JSON).\n";
        return;
      }
      tasks_.clear();
      for (const auto &elem : j_array) {
        tasks_.push_back(elem.get<Task>());
      }
    } catch (std::exception &e) {
      std::cerr << "Ошибка при парсинге JSON: " << e.what() << "\n";
    }
  }

private:
  std::vector<Task> tasks_;
};