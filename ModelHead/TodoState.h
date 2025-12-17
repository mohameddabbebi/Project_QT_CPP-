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
    if (str == "Not Ready" || str == "Not_Ready")
        return TodoState::Not_Ready;
    if (str == "Ready" || str == "Ready_Todo")
        return TodoState::Ready_Todo;
    if (str == "In Progress" || str == "In_Progress")
        return TodoState::In_Progress;
    if (str == "Done")
        return TodoState::Done;

    return TodoState::Not_Ready; // Default
}

#endif
