#include "entrylist.hpp"
#include "exif/exifmanager.hpp"

EntryList::EntryList() {
}

void EntryList::setDir(const QString &path,
                       const QString &sort) {
    m_dir = QDir(path);
    m_sort = sort;
}

void EntryList::applyFilters() {
    if (m_sort == SORT_EXIF) {
        m_dir.setFilter(QDir::Files);
    } else {
        m_dir.setFilter(QDir::Files);
        m_dir.setSorting(QDir::Name);
    }

    m_dir.setNameFilters(VALID_IMAGE_TYPES);
}

void EntryList::run() {
    m_time.start();
    fillEntryList();
    emit writeLog(LOG_PROGRESS, TIME_ELAPSED.arg(m_time.elapsed()));
}

void EntryList::fillEntryList() {
    emit statusChanged(STATUS_ENTRY_LIST);
    applyFilters();
    int fileCount = m_dir.count();
    m_entryList.reserve(fileCount);

    QDirIterator it(m_dir);
    int current = 0;
    while(it.hasNext()) {
        int progress = int((float(current) / float(fileCount)) * 100);
        emit progressChanged(progress);
        m_entryList << it.next();
        current++;
    }
    if (m_sort == SORT_EXIF) {
        emit progressChanged(100);
        emit statusChanged(STATUS_SORTING);
        ExifManager().sortByDateTime(m_entryList);
    }
    emit statusChanged(STATUS_TOTAL);
}

void EntryList::clear() {
    m_entryList.clear();
}

EntryList::~EntryList() {
}
