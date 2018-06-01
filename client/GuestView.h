#ifndef GUESTVIEW_H
#define GUESTVIEW_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class GuestView;
}

class GuestView : public QWidget
{
    Q_OBJECT

public:
    explicit GuestView(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~GuestView();    
    void onRead(const network::ResponseContext &response);

private:
    Ui::GuestView *ui;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
};

#endif // GUESTVIEW_H
