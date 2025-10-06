#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <iostream>
#include <regex>
#include <QRegularExpression>
#include <QDebug>

void RenameFile(QFileInfo info, QRegularExpression& reg, bool& success, int& count) {
    if (info.isDir()) {
        QDir dir_tmp = QDir(info.filePath());
        QFileInfoList ent = dir_tmp.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for (QFileInfo& e : ent) {
            RenameFile(e, reg, success, count);
        }
    } else if (info.isFile()) {
        QRegularExpressionMatch match = reg.match(info.fileName());
        if (match.hasMatch()) {
            QString old_name = info.absoluteFilePath();
            QString new_name = info.absoluteFilePath().remove(match.captured(0));
            success = QFile::rename(old_name, new_name);
        }
        count = success ? (count + 1) : count;
        success = false;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);    
    std::string find_str;
    std::cin >> find_str;

    std::string finder = "\\s*(" + find_str + ")\\s*"; // для регулярного выражения
    QRegularExpression reg(QString::fromStdString(finder));
    std::smatch match;
    QDir cur_dir = QDir::current();
    QFileInfoList entries = cur_dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    bool success = false; //для счетчика переименований
    int count = 0;// счетчик переименований
    for (QFileInfo& entry : entries) {
        RenameFile(entry, reg, success, count);
    }
    std::cout << "Переименовано " << count << " файл(ов)";
    return 0;
}
