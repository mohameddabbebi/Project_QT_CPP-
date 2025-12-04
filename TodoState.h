/**
 * @file TodoState.h
 * @brief Énumération des états possibles d'une tâche
 * @author [Votre Nom]
 */

#ifndef TODOSTATE_H
#define TODOSTATE_H

#include <QString>

/**
 * @enum TodoState
 * @brief États possibles d'une tâche
 */
enum class TodoState {
    Not_Ready,      ///< Pas prête (dépendances non satisfaites)
    Ready_Todo,     ///< Prête à être commencée
    In_Progress,    ///< En cours de réalisation
    Done            ///< Terminée
};

/**
 * @brief Convertit un état en QString pour l'affichage
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

/**
 * @brief Retourne la couleur associée à un état
 */
/*inline QColor todoStateColor(TodoState state) {
    switch(state) {
        case TodoState::Not_Ready: return QColor(255, 100, 100);    // Rouge clair
        case TodoState::Ready_Todo: return QColor(255, 200, 0);     // Jaune
        case TodoState::In_Progress: return QColor(100, 150, 255);  // Bleu
        case TodoState::Done: return QColor(100, 200, 100);         // Vert
    }
    return Qt::gray;
}
*/
#endif // TODOSTATE_H
