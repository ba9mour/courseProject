#pragma once

#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;

// ======================================
// TASK CLASS
// ======================================

class Task {
public:
    enum class Status {
        TODO,
        IN_PROGRESS,
        DONE
    };

    // constructor
    Task() = default;
    Task(std::string title,
         std::string description,
         int priority,
         std::string dueDate,
         Status status = Status::TODO)
        : title_(std::move(title))
        , description_(std::move(description))
        , priority_(priority)
        , dueDate_(std::move(dueDate))
        , status_(status)
    {}

    // getters
    const std::string& getTitle() const { return title_; }
    const std::string& getDescription() const { return description_; }
    int getPriority() const { return priority_; }
    const std::string& getDueDate() const { return dueDate_; }
    Status getStatus() const { return status_; }

    // setters
    void setTitle(const std::string& t) { title_ = t; }
    void setDescription(const std::string& d) { description_ = d; }
    void setPriority(int p) { priority_ = p; }
    void setDueDate(const std::string& d) { dueDate_ = d; }
    void setStatus(Status s) { status_ = s; }

    // status-to-string
    static std::string statusToString(Status s) {
        switch (s) {
            case Status::TODO: return "TODO";
            case Status::IN_PROGRESS: return "IN_PROGRESS";
            case Status::DONE: return "DONE";
            default: return "UNKNOWN";
        }
    }

    // parse string to status
    static Status stringToStatus(const std::string& str) {
        if (str == "TODO") return Status::TODO;
        if (str == "IN_PROGRESS") return Status::IN_PROGRESS;
        if (str == "DONE") return Status::DONE;
        // default - TODO
        return Status::TODO;
    }

private:
    std::string title_;
    std::string description_;
    int priority_ = 0;
    std::string dueDate_; //  YYYY-MM-DD
    Status status_ = Status::TODO;

    // friend json funcs for private access
    friend void to_json(json& j, const Task& t);
    friend void from_json(const json& j, Task& t);
};

// ============================================================
// Convertation Task <-> nlohmann::json
// ============================================================
inline void to_json(json& j, const Task& t) {
    j = json{
        {"title",       t.title_},
        {"description", t.description_},
        {"priority",    t.priority_},
        {"dueDate",     t.dueDate_},
        {"status",      Task::statusToString(t.status_)}
    };
}

inline void from_json(const json& j, Task& t) {
    t.title_       = j.at("title").get<std::string>();
    t.description_ = j.at("description").get<std::string>();
    t.priority_    = j.at("priority").get<int>();
    t.dueDate_     = j.at("dueDate").get<std::string>();
    std::string s  = j.at("status").get<std::string>();
    t.status_      = Task::stringToStatus(s);
}