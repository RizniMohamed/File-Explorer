#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QtAndroidExtras/QtAndroid>
#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Dir
    dirModel = new QFileSystemModel(this);
    dirModel->setRootPath("");
    dirModel->setReadOnly(false);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    ui->treeView->setModel(dirModel);


    //styling mainwindow buttons
    QString btnStyle = "QPushButton{ background-color:black; border:1px solid red;border-radius: 3px;padding: 2px;text-align:center;}"
                       "QPushButton:hover{background-color:red; color:black;}";
    ui->btnNewFolder->setStyleSheet( btnStyle);
    ui->btnRemove->setStyleSheet(btnStyle);

    preLoad();

    for(int i=1 ; i < dirModel->columnCount();i++){
        ui->treeView->hideColumn(i);
    }

    //file
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath("");
    fileModel->setReadOnly(false);
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    ui->listView->setModel(fileModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::preLoad(){
#ifdef  Q_OS_ANDROID


    auto  result = QtAndroid::checkPermission(QString("android.permission.WRITE_EXTERNAL_STORAGE"));
    auto  result2 = QtAndroid::checkPermission(QString("android.permission.READ_EXTERNAL_STORAGE"));
    if((result == QtAndroid::PermissionResult::Denied) || (result2 == QtAndroid::PermissionResult::Denied)){
        QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.WRITE_EXTERNAL_STORAGE","android.permission.READ_EXTERNAL_STORAGE"}));
        if((resultHash["android.permission.WRITE_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied) || (resultHash["android.permission.READ_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied))
            exit(0);
        else{
            const QStringList mPathList = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
            QString mPath = QString("%1").arg(mPathList.first());

            QString a = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

            qDebug() << mPathList;
            qDebug() << mPath;
            dirModel = new QFileSystemModel (this);
            dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
            dirModel->setRootPath(a);
            ui->treeView->setModel(dirModel);

            for(int i=1 ; i < dirModel->columnCount();i++){
                ui->treeView->hideColumn(i);
            }
        }
    }

    QString a = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    const QStringList mPathList = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    QString mPath = QString("%1").arg(mPathList.first());
    qDebug() << mPathList;
    qDebug() << mPath;
    dirModel = new QFileSystemModel (this);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    dirModel->setRootPath(a);
    ui->treeView->setModel(dirModel);

    for(int i=1 ; i < dirModel->columnCount();i++){
        ui->treeView->hideColumn(i);
    }

    // QPoint b = ui->treeView->childAt();
    ui->treeView->collapseAll();

#endif

}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    Directories=true;
    QString path = dirModel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(fileModel->setRootPath(path));
}

void MainWindow::on_btnNewFolder_clicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    if(!(index.isValid())) return;

    this->setStyleSheet("QInputDialog QPushButton{width:100px; color:red; border:1px solid red; border-radius: 3px;padding: 2px;text-align:center;}"
                        "QInputDialog QLabel{color:red; }"
                        "QInputDialog QPushButton:hover{background-color:red; color:black; transition:10s; }"
                        "QInputDialog {background-color:black; color:red;}"

                        "MainWindow {background-color:black; color:red;}"
                        "MainWindow QLabel{color:red;}"
                        "QPushButton{width:100px; color:red; border:1px solid red; border-radius: 3px;padding: 2px;text-align:center;}"
                        "QInputDialog QPushButton:hover{background-color:red; color:black; transition:10s; }"
                        );
    QString name = QInputDialog::getText(this,"New Folder","Enter Folder Name");

    if(name.isEmpty()) return;
    dirModel->mkdir(index,name);
}

void MainWindow::on_btnRemove_clicked()
{
    QModelIndex Dindex = ui->treeView->currentIndex();
    QModelIndex Findex = ui->listView->currentIndex();

    QMessageBox msgBox ;
    msgBox.setStyleSheet("QMessageBox {background-color:black;}"
                         "QMessageBox QLabel{color:red; }"
                         "QMessageBox QPushButton{width:100px; color:red; border:1px solid red; border-radius: 3px;padding: 2px;text-align:center;}"
                         "QMessageBox QPushButton:hover{background-color:red; color:black; transition:10s; }"
                         );
    msgBox.setText("Directory [ " + ui->treeView->model()->data(Dindex).toString() + " ] will permenently delete, Are you sure?");
    msgBox.addButton(msgBox.Yes);
    msgBox.addButton(msgBox.No);
    msgBox.setIcon(msgBox.Warning);
    msgBox.setWindowTitle("Remove");
    msgBox.setWindowIcon(QIcon("D:/Software/Softwares Bro/Software Mechatronics/Matlab - R2016a/Setup/help/includes/product/images/nextgen/doc_center/ie6_ico_category.gif"));

    if(!(Dindex.isValid())) return;
    if(dirModel->fileInfo(Dindex).isDir()){
        if(Directories){
            if( msgBox.exec() == QMessageBox::Yes)
                fileModel->remove(Dindex);
        }else{
            msgBox.setText("File [ " + ui->treeView->model()->data(Dindex).toString() + "\\" + ui->listView->model()->data(Findex).toString() + " ] will permenently delete, Are you sure?");
            if(msgBox.exec() == QMessageBox::Yes)
                fileModel->remove(Findex);
        }
    }
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{

//    QAndroidJniObject *javaFilePath =  new QAndroidJniObject();
//    javaFilePath->callObjectMethod("FilesCPDemo","onCreate");
//    QString filePath = javaFilePath->toString();

    QAndroidJniObject asd = QAndroidJniObject::callStaticObjectMethod("org/qtproject/example/FilesCPDemo","show","(String)","Rizni");
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileModel->filePath(index)));
    qDebug() << asd.toString();
    Directories=false;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    //#ifdef O_OS_WIN
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileModel->filePath(index)));
    // #endif


}
