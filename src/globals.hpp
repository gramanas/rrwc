#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <QString>

///////////////////////////
// Global values in rrwc //
///////////////////////////

// sort modes
static const QString SORT_EXIF = "Exif date";
static const QString SORT_FILENAME = "Filename";

// output values
static const QString OTP_FOLDER = "outputFolder";
static const QString OTP_RESIZE = "resize";
static const QString OTP_LENGTH = "length";
static const QString OTP_HEIGHT = "height";
static const QString OTP_RENAME = "rename";
static const QString OTP_RENAME_TEXT = "renameText";
static const QString OTP_COUNTER_START = "counterStart";
static const QString OTP_COUNTER_STEP = "counterStep";
static const QString OTP_COUNTER_DIGITS = "counterDigits";
static const QString OTP_WATERMARK = "watermark";
static const QString OTP_WATERMARK_TEXT = "watermarkText";
static const QString OTP_OPACITY = "opacity";
static const QString OTP_THREADS = "threads";
static const QString OTP_STRIP_METADATA = "stripMetadata";

// logs
static const QString LOG_PROGRESS = "__progress__";
static const QString LOG_ERROR = "__error__";
static const QString LOG_CLEAR = "__clear__";

// errors
static const QString ERR_OTP = "Error in output [%1]: ";
static const QString ERR_IMAGE = "In image \"%1\": ";
static const QString ERR_NO_INPUT_FILES = "Error: The input folder does not contain any JPEG files.";
static const QString ERR_WATERMARK_TEXT = "Watermark file \"%1\" does not exist.";
static const QString ERR_SMALL_WATERMARK = "Watermark is too small to fit this resize setting. Rows: %1 Columns: %2";
static const QString ERR_WATERMARK_FIT = "Watermark is too small to fit this image.";
static const QString ERR_NO_EXIF = "No exif information found.";
static const QString ERR_FAIL = "Output: [%1]\t File: \"%2\" failed.";
static const QString ERR_WRONG_OUTPUT = "Output folder is not a directory, or you don't have the right permissions.";

// progress
static const QString FILE_PROGRESS = "Output: [%1]\t Thread: [%2]\t File: \"%3\"";

// buttons
static const QString BUT_LOG = "Logs";
static const QString BUT_OUTPUTS = "Outputs";

#endif // GLOBALS_HPP
