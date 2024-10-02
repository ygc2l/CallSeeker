#include <QMimeData>
#include "lineedit.h"

LineEdit::LineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

void LineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void LineEdit::dropEvent(QDropEvent *event)
{
    const QMimeData* mine_data = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mine_data->hasUrls())
    {
      QStringList filepaths;
      QList<QUrl> urlList = mine_data->urls();

      // extract local paths of files
      for (int i = 0; i < urlList.size(); i++)
      {
          std::string filename = urlList.at(i).toLocalFile().toStdString();
          std::string::size_type idx;

          idx = filename.rfind('.');

          if(idx != std::string::npos)
          {
              std::string ext = filename.substr(idx+1);
              std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

              if (ext == "wav")
              {
                  filepaths.append(urlList.at(i).toLocalFile());
              }
          }
      }

      event->acceptProposedAction();
      this->setText(filepaths.join(", "));
      this->filepaths = filepaths;
    }
}
