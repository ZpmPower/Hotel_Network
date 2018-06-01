#ifndef RECEPTIONISTVIEW_H
#define RECEPTIONISTVIEW_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class ReceptionistView;
}

class ReceptionistView : public QWidget
{
    Q_OBJECT

public:
    explicit ReceptionistView(std::shared_ptr<MessageManager> message_manager, uint32_t hotelid,  QWidget *parent = 0);
    ~ReceptionistView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_ReceptionistView_destroyed();

signals:
    void enableGb();
private:
    Ui::ReceptionistView *ui;
    uint32_t hotelID_;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
};

#endif // RECEPTIONISTVIEW_H
