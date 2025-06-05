#pragma once

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "Task.h"

using json = nlohmann::json;

// ===============================================
// CLASS TaskManager: обёртка над std::vector<Task>
// и методы CRUD + шаблонный метод сортировки.
// ===============================================
class TaskManager {
public:
    TaskManager() = default;

    // Добавить новую задачу
    void addTask(const Task& t) {
        tasks_.push_back(t);
    }

    // Удалить задачу по индексу (если индекс не валидный — выводим ошибку)
    void removeTask(size_t index) {
        if (index >= tasks_.size()) {
            std::cerr << "Ошибка: неверный индекс задачи.\n";
            return;
        }
        tasks_.erase(tasks_.begin() + index);
    }

    // Обновить задачу: заменяем на новую (по индексу)
    void updateTask(size_t index, const Task& newTask) {
        if (index >= tasks_.size()) {
            std::cerr << "Ошибка: неверный индекс задачи.\n";
            return;
        }
        tasks_[index] = newTask;
    }

    // Получить ссылку на все задачи (для вывода, например)
    const std::vector<Task>& getAllTasks() const {
        return tasks_;
    }

    // Шаблонный метод сортировки: передаём любую функцию сравнения
    // Пример вызова: sortTasks([](auto& a, auto& b){ return a.getPriority() < b.getPriority(); });
    template<typename Compare>
    void sortTasks(Compare cmp) {
        std::sort(tasks_.begin(), tasks_.end(), cmp);
    }

    // ================================================================
    // Сохранить все задачи в файл JSON (tasks.json). При ошибке — сообщаем пользователю.
    // ================================================================
    void saveToFile(const std::string& filename) const {
        json j_array = json::array();
        for (const auto& t : tasks_) {
            j_array.push_back(t);
        }

        std::ofstream ofs(filename);
        if (!ofs.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для записи: " << filename << "\n";
            return;
        }
        ofs << j_array.dump(4); // dump с отступом 4 пробела для читабельности
        ofs.close();
    }

    // ================================================================
    // Загрузить задачи из JSON-файла. Существующие задачи удаляются.
    // ================================================================
    void loadFromFile(const std::string& filename) {
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            // Если файл не существует, просто оставляем пустой список без ошибки
            std::cerr << "Внимание: файл \"" << filename << "\" не найден. Будет создан при сохранении.\n";
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
            for (const auto& elem : j_array) {
                tasks_.push_back(elem.get<Task>());
            }
        } catch (std::exception& e) {
            std::cerr << "Ошибка при парсинге JSON: " << e.what() << "\n";
        }
    }

private:
    std::vector<Task> tasks_;
};