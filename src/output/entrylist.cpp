#include "entrylist.hpp"
#include "exif/exifmanager.hpp"

EntryList::EntryList(Logger *logger) :
  p_logger(logger) {
}

void EntryList::setDir(const QString &path,
                       const QString &sort) {
    m_dir = QDir(path);
    m_sort = sort;
}

void EntryList::applyFilters() {
    if (m_sort == SORT_EXIF) {
        m_dir.setFilter(QDir::Files);
    }
    else {
        m_dir.setFilter(QDir::Files);
        m_dir.setSorting(QDir::Name);
    }

    m_dir.setNameFilters(VALID_IMAGE_TYPES);
}

void EntryList::run() {
    m_time.start();
    fillEntryList();
    p_logger->log(TIME_ENTRYLIST.arg(m_time.elapsed()));
}

void EntryList::fillEntryList() {
    p_logger->status(STATUS_ENTRY_LIST);

    applyFilters();
    m_entryList.reserve(m_dir.count());

    QDirIterator it(m_dir);
    while(it.hasNext()) {
        m_entryList.append(it.next());
    }
    if (m_sort == SORT_EXIF) {
        p_logger->status(STATUS_SORTING);
        ExifManager(p_logger).sortByDateTime(m_entryList);
    }

    p_logger->status(STATUS_TOTAL);
}

void EntryList::clear() {
    m_entryList.clear();
}

EntryList::~EntryList() {
}
