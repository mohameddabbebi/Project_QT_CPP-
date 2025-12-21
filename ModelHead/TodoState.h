#ifndef TODOSTATE_H
#define TODOSTATE_H

#include <QString>

enum class TodoState {
    Not_Ready,
    Ready_Todo,
    In_Progress,
    Done
};

/**
 * Convertit un état en QString pour l'affichage
 */
inline QString todoStateToString(TodoState state) {
    switch(state) {
        case TodoState::Not_Ready: return "NOT READY";
        case TodoState::Ready_Todo: return "READY TODO";
        case TodoState::In_Progress: return "IN PROGRESS";
        case TodoState::Done: return "DONE";
    }
    return "Inconnu";
}
inline TodoState stringToTodoState(const QString& str) {
    QString s = str.simplified().replace(" ", "_").toLower();

    if (s == "not_ready") return TodoState::Not_Ready;
    if (s == "ready_todo") return TodoState::Ready_Todo;
    if (s == "in_progress") return TodoState::In_Progress;
    if (s == "done") return TodoState::Done;

    return TodoState::Not_Ready;
}

#endif
