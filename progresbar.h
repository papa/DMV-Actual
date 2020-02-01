#ifndef PROGRESBAR_H
#define PROGRESBAR_H

#include <QWidget>

class progresbar : public QWidget
{
    Q_OBJECT
public:
    explicit progresbar(QWidget *parent = nullptr);

signals:
    void promenjenaVrednos(int vrednost);

public slots:
    void postaviVrednos(int vrednos);
};

#endif // PROGRESBAR_H
