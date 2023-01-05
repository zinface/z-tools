#include "desktopcustomecontent.h"

#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

DesktopCustomeContent::DesktopCustomeContent(QWidget *parent) : QTextEdit (parent)
{

    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

//    connect(this,&QTextEdit::textChanged, this, &DesktopCustomeContent::fitHeightToDocument);
}

void DesktopCustomeContent::fitHeightToDocument()
{
    this->document()->setTextWidth(this->viewport()->width());
    QSize document_size(this->document()->size().toSize());
    this->fitted_height = document_size.height();
    this->updateGeometry();
}

QSize DesktopCustomeContent::sizeHint() const
{
    QSize sizehint = QTextEdit::sizeHint();
    sizehint.setHeight(this->fitted_height);
    return sizehint;
}
