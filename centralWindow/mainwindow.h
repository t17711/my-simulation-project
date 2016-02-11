#ifndef MAINWINDOW_H // if window not defined
#define MAINWINDOW_H // define
// make sure it is included once and only once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;

class MainWindow : public QMainWindow
{
 Q_OBJECT  // qt macro included if there is signal or slots.
    // helps convert signal and slots into c++ code

public:
 MainWindow(); // constructor

protected:
 void closeEvent(QCloseEvent *event);

private slots:   // when signal happens these slots happen
 // signal happen when widget does stuff
 // private cause nobody else should use this
 void newFile(); //when new file selected
 void open();  // select open file calls this
 bool save();
 bool saveAs();
 void find();
 void goToCell();
 void sort();
 void about();
 void openRecentFile();
 void updateStatusBar();
 void spreadsheetModified();

private:  // private that has nothong to do with slots

 void createActions();
 void createMenus();
 void createContextMenu();
 void createToolBars();
 void createStatusBar();
 void readSettings();
 void writeSettings();
 bool okToContinue();
 bool loadFile(const QString &fileName);
 bool saveFile(const QString &fileName);
 void setCurrentFile(const QString &fileName);
 void updateRecentFileActions();
 QString strippedName(const QString &fullFileName);

 Spreadsheet *spreadsheet;
  FindDialog *findDialog;
  QLabel *locationLabel;
  QLabel *formulaLabel;
  QStringList recentFiles;
  QString curFile;
  enum { MaxRecentFiles = 5 };
  QAction *recentFileActions[MaxRecentFiles];
  QAction *separatorAction;
  QMenu *fileMenu;
  QMenu *editMenu;

  QToolBar *fileToolBar;
  QToolBar *editToolBar;
  QAction *newAction;
  QAction *openAction;

  QAction *aboutQtAction;
 };
#endif
