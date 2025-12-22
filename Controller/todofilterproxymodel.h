#include <QSortFilterProxyModel>

class TodoFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TodoFilterProxyModel(QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

private:
    bool hasMatchingChild(const QModelIndex &parent) const;
    bool hasMatchingParent(const QModelIndex &child) const;
};
