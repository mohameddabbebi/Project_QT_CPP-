#include "TodoFilterProxyModel.h"
#include <QAbstractItemModel>

TodoFilterProxyModel::TodoFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool TodoFilterProxyModel::filterAcceptsRow(int sourceRow,
                                            const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!index.isValid())
        return false;


    QString text = sourceModel()->data(index).toString();
    if (text.contains(filterRegularExpression())) {

        return true;
    }


    if (hasMatchingParent(sourceParent))
        return true;


    if (hasMatchingChild(index))
        return true;

    return false;
}


bool TodoFilterProxyModel::hasMatchingParent(const QModelIndex &child) const
{
    QModelIndex parent = child;


    while (parent.isValid()) {
        QString text = sourceModel()->data(parent).toString();


        if (text.contains(filterRegularExpression()))
            return true;

        parent = parent.parent();
    }

    return false;
}


bool TodoFilterProxyModel::hasMatchingChild(const QModelIndex &parent) const
{
    int rows = sourceModel()->rowCount(parent);

    for (int i = 0; i < rows; ++i) {
        QModelIndex child = sourceModel()->index(i, 0, parent);
        if (!child.isValid())
            continue;

        QString text = sourceModel()->data(child).toString();


        if (text.contains(filterRegularExpression()))
            return true;


        if (hasMatchingChild(child))
            return true;
    }

    return false;
}
