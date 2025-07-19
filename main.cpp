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

    std::string finder = "\\s*(" + find_str + ")\\s*"; // для регулярного выражения
    QRegularExpression reg(QString::fromStdString(finder));
    std::smatch match;

    QDir cur_dir = QDir::current();
    QFileInfoList entries = cur_dir.entryInfoList(QDir::Files);
    bool success = false; //для счетчика переименований
    int count = 0;// счетчик переименований

    for (QFileInfo& entry : entries) {
        QRegularExpressionMatch match = reg.match(entry.fileName());
        if (match.hasMatch()) {
            QString old_name = entry.absoluteFilePath();
            QString new_name = entry.fileName().remove(match.captured(0));
            success = QFile::rename(old_name, new_name);
        }
        count = success ? (count + 1) : count;
        success = false;
    }
    std::cout << "Переименовано " << count << " файл(ов)";
    return 0;
}
