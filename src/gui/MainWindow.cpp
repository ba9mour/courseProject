#include "MainWindow.h"

// Constructor: build interface and load tasks
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
// Central widget
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // Main vertical layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    central->setLayout(mainLayout);

    // Tasks list
    taskList_ = new QListWidget(this);
    mainLayout->addWidget(taskList_);

    // Horizontal layout for buttons
    QHBoxLayout* btnLayout = new QHBoxLayout;
    mainLayout->addLayout(btnLayout);

    // Buttons and Save/Delete
    addBtn_    = new QPushButton("Добавить", this);
    removeBtn_ = new QPushButton("Удалить",  this);
    editBtn_   = new QPushButton("Редактировать", this);
    saveBtn_   = new QPushButton("Сохранить", this);
    loadBtn_   = new QPushButton("Загрузить", this);

    btnLayout->addWidget(addBtn_);
    btnLayout->addWidget(removeBtn_);
    btnLayout->addWidget(editBtn_);
    btnLayout->addWidget(saveBtn_);
    btnLayout->addWidget(loadBtn_);

    // Connecting button-signals for slots
    connect(addBtn_,    &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(removeBtn_, &QPushButton::clicked, this, &MainWindow::onRemoveTask);
    connect(editBtn_,   &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(saveBtn_,   &QPushButton::clicked, this, &MainWindow::onSaveTasks);
    connect(loadBtn_,   &QPushButton::clicked, this, &MainWindow::onLoadTasks);

// loading tasks from file
    manager_.loadFromFile(defaultFilename_);
    refreshList();
}

void MainWindow::onAddTask() {
    bool ok;

    // title 
    QString title = QInputDialog::getText(
        this,
        "Новая задача",
        "Заголовок:",
        QLineEdit::Normal,
        "",
        &ok
    );
    if (!ok || title.isEmpty()) return;

    // description
    QString description = QInputDialog::getText(
        this,
        "Новая задача",
        "Описание:",
        QLineEdit::Normal,
        "",
        &ok
    );
    if (!ok) return;

    // priority
    int priority = QInputDialog::getInt(
        this,
        "Новая задача",
        "Приоритет (целое):",
        0,
        0,
        100,
        1,
        &ok
    );
    if (!ok) return;

    // deadline
    QString dueDate = QInputDialog::getText(
        this,
        "Новая задача",
        "Дедлайн (YYYY-MM-DD):",
        QLineEdit::Normal,
        "",
        &ok
    );
    if (!ok) return;

    // choose task status
    QStringList statuses{ "TODO", "IN_PROGRESS", "DONE" };
    QString statusStr = QInputDialog::getItem(
        this,
        "Новая задача",
        "Статус:",
        statuses,
        0,
        false,
        &ok
    );
    if (!ok) return;

    Task::Status status = Task::stringToStatus(statusStr.toStdString());

    // create and add task
    Task t(
        title.toStdString(),
        description.toStdString(),
        priority,
        dueDate.toStdString(),
        status
    );
    manager_.addTask(t);
    refreshList();
}

void MainWindow::onRemoveTask() {
    int idx = taskList_->currentRow();
    if (idx < 0) return;
    manager_.removeTask(static_cast<size_t>(idx));
    refreshList();
}

void MainWindow::onEditTask() {
    int idx = taskList_->currentRow();
    if (idx < 0) return;

    Task old = manager_.getAllTasks().at(idx);
    bool ok;

    // edit title
    QString title = QInputDialog::getText(
        this,
        "Редактировать задачу",
        "Заголовок:",
        QLineEdit::Normal,
        QString::fromStdString(old.getTitle()),
        &ok
    );
    if (!ok || title.isEmpty()) return;

    // edit description
    QString description = QInputDialog::getText(
        this,
        "Редактировать задачу",
        "Описание:",
        QLineEdit::Normal,
        QString::fromStdString(old.getDescription()),
        &ok
    );
    if (!ok) return;

    // edit priority
    int priority = QInputDialog::getInt(
        this,
        "Редактировать задачу",
        "Приоритет:",
        old.getPriority(),
        0,
        100,
        1,
        &ok
    );
    if (!ok) return;

    // edit deadline
    QString dueDate = QInputDialog::getText(
        this,
        "Редактировать задачу",
        "Дедлайн (YYYY-MM-DD):",
        QLineEdit::Normal,
        QString::fromStdString(old.getDueDate()),
        &ok
    );
    if (!ok) return;

    // edit status
    QStringList statuses{ "TODO", "IN_PROGRESS", "DONE" };
    int currentIndex = static_cast<int>(old.getStatus());
    QString statusStr = QInputDialog::getItem(
        this,
        "Редактировать задачу",
        "Статус:",
        statuses,
        currentIndex,
        false,
        &ok
    );
    if (!ok) return;

    Task::Status status = Task::stringToStatus(statusStr.toStdString());

    // new task
    Task newT(
        title.toStdString(),
        description.toStdString(),
        priority,
        dueDate.toStdString(),
        status
    );
    manager_.updateTask(static_cast<size_t>(idx), newT);
    refreshList();
}

void MainWindow::onSaveTasks() {
    manager_.saveToFile(defaultFilename_);
    QMessageBox::information(
        this,
        "Сохранено",
        "Задачи сохранены в " + QString::fromStdString(defaultFilename_)
    );
}

void MainWindow::onLoadTasks() {
    manager_.loadFromFile(defaultFilename_);
    refreshList();
    QMessageBox::information(
        this,
        "Загружено",
        "Задачи загружены из " + QString::fromStdString(defaultFilename_)
    );
}

void MainWindow::refreshList() {
    taskList_->clear();
    const auto& all = manager_.getAllTasks();
    for (size_t i = 0; i < all.size(); ++i) {
        const Task& t = all[i];
        QString line = QString("[%1] %2 (%3) — %4")
            .arg(t.getPriority())
            .arg(QString::fromStdString(t.getTitle()))
            .arg(QString::fromStdString(Task::statusToString(t.getStatus())))
            .arg(QString::fromStdString(t.getDueDate()));
        taskList_->addItem(line);
    }
}