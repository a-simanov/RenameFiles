#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <iostream>
#include <regex>
#include <QRegularExpression>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::string find_str;
    std::cin >> find_str;
    std::string finder = "\\s*(" + find_str + ")\\s*";
    QRegularExpression reg(QString::fromStdString(finder));
    std::smatch match;
    QDir cur_dir = QDir::current();
    qDebug() << cur_dir;
    QFileInfoList entries = cur_dir.entryInfoList(QDir::Files);
    bool succses = false;
    int count = 0;
    for (QFileInfo& entry : entries) {
        QRegularExpressionMatch match = reg.match(entry.fileName());
        if (match.hasMatch()) {
            QString old_name = entry.absoluteFilePath();
            qDebug() << old_name;
            QString new_name = entry.fileName().remove(match.captured(0));
            qDebug() << new_name;
            succses = QFile::rename(old_name, new_name);
        }
        count = succses ? (count + 1) : count;
        succses = false;
    }
    std::cout << "Переименовано " << count << " файл(ов)";
    return 0;
}
