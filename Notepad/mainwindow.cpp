#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui -> textEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}



/**
 * ------------------------------------------------------------ NEW ------------------------------------------------------------
 * @brief MainWindow::on_actionNew_triggered
 */
void MainWindow::on_actionNew_triggered()
{
    //take the current file name and clear it?
    currentFile.clear();

    //clear the text inside the text edit by setting it to an empty string
    ui -> textEdit -> setText((QString()));
}



/**
 * ----------------------------------------------------------- OPEN --------------------------------------------------------------
 * @brief MainWindow::on_actionOpen_triggered
 */
void MainWindow::on_actionOpen_triggered()
{
    /*
     * NOTES:
     * QString stores a string of QChars.
     *
     * QFileDialog provides a dialog that allows users to select files.
     *
     * getOpenFileName returns a file selected by the user
     *
     * a modal dialog is a window that forces the user to interact with it before going back to
     * the parent application.
     *
     *
     * SUMMARY:
     * this line forces the user to select a file, and then stores the file's name in the
     * "filename" variable.
    */
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");


    // constructs a new file object that represents the file with the given name
    QFile file(filename);

    // sets the filename in mainwindow.h as the selected file
    currentFile = filename;

    /*
     * NOTES:
     * QFile.open returns a boolean. When a file is opened using this function, calling
     * close() will only flush the file unless AutoCloseHandle is specified.
     *
     * QIODevice -> QFileDevice -> QFile
     *
     * ReadOnly is an OpenMode, which is an enum that descibes the mode in which a device is open
     *
     * Text mode is when reading, the end of line terminators are translated tp '\n'. when writing
     * the end of line characters are translated to the local encoding.
     *
     * opens the file using the readOnly mode or text mode, and returns a boolean if it was successful
     * don't understand why you'd do readOnly OR text?
    */
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        // if the file failed to open, display a modal dialog that it failed
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }

    // if the file opened, set the window's name to the file's name
    setWindowTitle(filename);

    // ???
    // convenient interface for reading and writing text
    //
    // contructs a QTextStream that operates on fileHandle... internally, a QFile is created
    // to handle the FILE pointer
    //
    // this constructor works directly with the common file based input output streams
    //
    // so basically we can read and edit a file via this object
    QTextStream in(&file);

    // reads the entire content of the stream, returns it as a QString
    QString text = in.readAll();

    // set the text in the window to the text in the chosen file
    ui -> textEdit -> setText(text);

    file.close();
}



/**
 * ------------------------------------------------------------ SAVE -------------------------------------------------------------
 * @brief MainWindow::on_actionSave_triggered
 */
void MainWindow::on_actionSave_triggered()
{
    // makes a file object that represents our current file
    QFile file(currentFile);

    // if it wasn't able to open the file in write mode...
    if(!file.open(QFile::WriteOnly | QFile::Text)) {

        // display a modal dialog that it failed to save
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }

    // create an object that will allow us to write to the file
    QTextStream out(&file);

    // copy the text that's currently in the window
    QString text = ui -> textEdit -> toPlainText();

    // write the text to the file
    out << text;

    // close the file
    file.close();
}




/**
 * ------------------------------------------------------------ SAVE AS -------------------------------------------------------------
 * @brief MainWindow::on_actionSave_As_triggered
 */
void MainWindow::on_actionSave_As_triggered()
{
    // prompt user to place a name, save it in filename
    QString filename = QFileDialog::getSaveFileName(this, "Save as");

    // constructs a new file object that represents the file with the given name... how does the actual file get saved?
    QFile file(filename);

    // if it wasn't able to open the file in write mode...
    if(!file.open(QFile::WriteOnly | QFile::Text)) {

        // display a modal dialog that it failed to save
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }

    // set the currentFile string in the header file to the saved file name
    currentFile = filename;

    // set the window title as the saved file name
    setWindowTitle(filename);

    // create an object that will allow us to write to the file
    QTextStream out(&file);

    // copy the text that's currently in the window
    QString text = ui -> textEdit -> toPlainText();

    // write the text to the file
    out << text;

    // close the file
    file.close();
}



/**
 * ------------------------------------------------------------ PRINT -------------------------------------------------------------
 * @brief MainWindow::on_actionPrint_triggered
 */
void MainWindow::on_actionPrint_triggered()
{
    // allows us to interact with any printer on our network
    QPrinter printer;

    // ??
    printer.setPrinterName("Printer Name");

    // QPrintDialog provides a dialog for specifying the printer's configuration
    //
    // allow user to select the printer that they want to use. Passes a reference to the printer
    QPrintDialog pDialog(&printer, this);

    // NOTES:
    // exec() reimplements QAbstractPrintDialog exec().. which is called to pop up a dialog?
    // returns an int?
    //
    // QAbstractPrintDialog exec() reimplements QDialog exec() which shows a modal dialog and
    // returns a DialogCode
    //
    // Rejected is one of the DialogCod enums returned by a modal dialog... 0
    //
    // why do we execute using the PrintDialog and not the actual QPrinter?? Maybe the PrintDialog
    // tries to connect to the QPrinter and if it can't, it sends back the enum?
    //
    // SUMMARY:
    // So the QPrintDialog will send back either the success enum or rejected enum and we're
    // checking to see which enum it sent back
    //
    // verify that we were able to connect to the printer
    if(pDialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot access printer.");
        return;
    }

    // send the text in our textbox to the chosen printer
    ui -> textEdit -> print(&printer);
}



/**
 * ------------------------------------------------------------ EXIT -------------------------------------------------------------
 * @brief MainWindow::on_actionExit_triggered
 */
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}



/**
 * ------------------------------------------------------------ COPY -------------------------------------------------------------
 * @brief MainWindow::on_actionCopy_triggered
 */
void MainWindow::on_actionCopy_triggered()
{
    // comes from the QTextEdit class. Copies any selected text to the clipboard
    ui -> textEdit -> copy();
}



/**
 * ------------------------------------------------------------ PASTE -------------------------------------------------------------
 * @brief MainWindow::on_actionPaste_triggered
 */
void MainWindow::on_actionPaste_triggered()
{
    // comes from the QTextEdit class. Pastes text from the clipboard to the text edit at the
    // current cursor position
    ui -> textEdit -> paste();
}



/**
 * ------------------------------------------------------------ CUT -------------------------------------------------------------
 * @brief MainWindow::on_actionCut_triggered
 */
void MainWindow::on_actionCut_triggered()
{
    // QTextEdit class. Copies selected text to the clipboard and deletes it from the text edit
    ui -> textEdit -> cut();
}



/**
 * ------------------------------------------------------------ UNDO -------------------------------------------------------------
 * @brief MainWindow::on_actionUndo_triggered
 */
void MainWindow::on_actionUndo_triggered()
{
    // QTextEdit class. Undoes the last operation
    ui -> textEdit -> undo();
}



/**
 * ------------------------------------------------------------ REDO -------------------------------------------------------------
 * @brief MainWindow::on_actionRedo_triggered
 */
void MainWindow::on_actionRedo_triggered()
{
    // QTextEdit class. Redoes the last operation
    ui -> textEdit -> redo();
}
