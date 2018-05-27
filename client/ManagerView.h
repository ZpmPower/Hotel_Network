#ifndef MANAGERVIEW_H
#define MANAGERVIEW_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class ManagerView;
}

class ManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerView(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~ManagerView();
    void onRead(const network::ResponseContext &response);

private:
    Ui::ManagerView *ui;
    std::shared_ptr<MessageManager> message_manager_;
};

#endif // MANAGERVIEW_H
